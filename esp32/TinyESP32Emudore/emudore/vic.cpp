// Mod by ackerman (convert C, refactor, optimice)
//
// emudore, Commodore 64 emulator
// Copyright (c) 2016, Mario Ballano <mballano@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "gbConfig.h"
#include "gbGlobals.h"
#include "vic.h"
#include "Arduino.h"

// ctor and emulate()  ///////////////////////////////////////////////////////

//Vic::Vic()
void Vic_constructor()
{
//JJ  kRefreshRate = 1 / 50.125; // ~50Hz (PAL) Inicializacion JJ
          
  //raster
  VIC_raster_irq_ = VIC_raster_c_ = 0; 
  VIC_irq_enabled_ = VIC_irq_status_ = 0;
  VIC_next_raster_at_ = VIC_kLineCycles;
  //sprites
  for(int i = 0 ; i<8 ; i++)
  {
    VIC_mx_[i] = VIC_my_[i] = VIC_sprite_colors_[i] = 0;
  }
  VIC_msbx_ = VIC_sprite_double_height_ = VIC_sprite_double_width_ = 0;
  VIC_sprite_enabled_ = VIC_sprite_priority_ = VIC_sprite_multicolor_ = 0;       
  VIC_sprite_shared_colors_[0] = VIC_sprite_shared_colors_[1] = 0;
  //colors
  VIC_border_color_ = 0;
  VIC_bgcolor_[0] = VIC_bgcolor_[1] = VIC_bgcolor_[2] = VIC_bgcolor_[3] = 0;
  //control regs
  VIC_cr1_ = VIC_cr2_ = 0;
  //frame counter
  VIC_frame_c_ = 0;
  //default memory pointers
  VIC_screen_mem_ = MEMORY_kBaseAddrScreen;
  VIC_char_mem_   = MEMORY_kBaseAddrChars;
  VIC_bitmap_mem_ = MEMORY_kBaseAddrBitmap;
  //bit 0 is unused
  VIC_mem_pointers_ = (1 << 0);
  //current graphic mode
  VIC_graphic_mode_ = kCharMode;
  
  Serial.printf("Vic_constructor\r\n");
  //fflush(stdout);  
}

//bool Vic::VIC_emulate()
bool VIC_emulate()
{
 unsigned short int screen_y;
 unsigned short int rstr;
 // 
 // if there are unacknowledged interrupts 
 // raise an interrupt again
 //
 if((VIC_read_register(0x19) & 0x80) != 0)
 {
  //VIC_cpu_->CPU_irq(); //No quiero objetos
  if(!CPU_idf_)
  {
   CPU_irq();
  }
 }

 //are we at the next raster line?
 //if (VIC_cpu_->CPU_cycles() >= VIC_next_raster_at_) //No quiero objetos
 //if (CPU_cycles_get() >= VIC_next_raster_at_)
 if (CPU_cycles_ >= VIC_next_raster_at_)
 {
  rstr = (VIC_raster_c_ | ((VIC_cr1_&0x80) << 1)); //rstr = VIC_raster_counter_get();
  //check raster IRQs
  //if (VIC_raster_irq_enabled() &&
  //    rstr == VIC_raster_irq_)
  if (ISSET_BIT(VIC_irq_enabled_,0) && rstr == VIC_raster_irq_)
  {
   //set interrupt origin (raster)
   VIC_irq_status_ |= (1<<0);
   //raise interrupt
   //VIC_cpu_->CPU_irq(); //No quiero objetos
   if(!CPU_idf_)
   {
    CPU_irq();
   }
  }
  if ((rstr >= VIC_kFirstVisibleLine) && (rstr < VIC_kLastVisibleLine))
  {
   if (!((gb_cpu_fast==1)&&((gb_fps_cur & gb_fps_fast_mask) != 0)))
   {     
    //draw border
    screen_y = rstr - VIC_kFirstVisibleLine;
    //io_->IO_screen_draw_border(screen_y,border_color_); //no quiero objetos
    
    //IO_screen_draw_border(screen_y,VIC_border_color_);
    IO_screen_draw_rect(0,screen_y,IO_cols_,VIC_border_color_); //llamada directa en lugar IO_screen_draw_border
   }
   
   
   //draw raster on current graphic mode
   switch(VIC_graphic_mode_)
   {
    case kCharMode:
    case kMCCharMode: 
     if (!((gb_cpu_fast==1)&&((gb_fps_cur & gb_fps_fast_mask) != 0)))
     {
      VIC_draw_raster_char_mode(); 
     }
     break;
    case kBitmapMode:
    case kMCBitmapMode:
     if (!((gb_cpu_fast==1)&&((gb_fps_cur & gb_fps_fast_mask) != 0)))
     {
      VIC_draw_raster_bitmap_mode();
     }
     break;
    default:
     //D("unsupported graphic mode: %d\n",graphic_mode_);
     Serial.printf("unsupported graphic mode: %d\r\n",VIC_graphic_mode_);
     //fflush(stdout);
     return false;
   }
   //draw sprites
   if(VIC_sprite_enabled_ != 0)
   {//Optimizado, antes se llamaba siempre 22720 veces
    if (!((gb_cpu_fast==1)&&((gb_fps_cur & gb_fps_fast_mask) != 0)))
    {
     VIC_draw_raster_sprites();
    }
   }
  }
  // next raster
  if(VIC_is_bad_line())
  {
   VIC_next_raster_at_+= VIC_kBadLineCycles;
  }
  else
  {
   VIC_next_raster_at_+= VIC_kLineCycles;
  }
  //update raster
  VIC_raster_counter_set(++rstr);
  if (rstr >= VIC_kScreenLines)
  {
   //io_->IO_screen_refresh();
   IO_screen_refresh(); //no quiero objetos
   VIC_frame_c_++;
   VIC_raster_counter_set(0);
      
   gb_fps_cur++;
      
   //gb_fps_time_cur= SDL_GetTicks();
   //unsigned int auxTime= (gb_fps_time_cur - gb_fps_time_last);
   //if (auxTime >= 999)
   //{
   // gb_fps_time_last= gb_fps_time_cur;
   // #ifdef use_lib_log_fps
   //  printf("FPS %d\n",gb_fps_cur);
   //  fflush(stdout);
   // #endif 
   // //fprintf(gb_fichLog,"%ld %ld %ld FPS %d\n",gb_fps_time_last,gb_fps_time_cur,auxTime,gb_fps_cur);
   // //fflush(gb_fichLog); 
   // gb_fps_cur=0;
   // //242  244 232 223 fps   32 fps cuando SDL_Delay(20) 32 MB de ram
   //}
  }
 }
 return true;
}

// DMA register access  //////////////////////////////////////////////////////

//unsigned char Vic::VIC_read_register(unsigned char r)
unsigned char VIC_read_register(unsigned char r)
{
 unsigned char retval;
 switch(r)
 {
  // get X coord of sprite n
  case 0x0:
  case 0x2:
  case 0x4:
  case 0x6:
  case 0x8:
  case 0xc:
  case 0xe:
    retval = VIC_mx_[r >> 1];
    break;
  // get Y coord of sprite n
  case 0x1:
  case 0x3:
  case 0x5:
  case 0x7:
  case 0x9:
  case 0xb:
  case 0xd:
  case 0xf:
    retval = VIC_my_[r >> 1];
    break;
  // MSBs of sprites X coordinates
  case 0x10:
    retval = VIC_msbx_;
    break;
  // control register 1
  case 0x11:
    retval = VIC_cr1_;
    break;
  // raster counter
  case 0x12:
    retval = VIC_raster_c_;
    break;
  // sprite enable register
  case 0x15:
    retval = VIC_sprite_enabled_;
    break;
  // control register 2
  case 0x16:
    retval = VIC_cr2_;
    break;
  // sprite double height
  case 0x17:
    retval = VIC_sprite_double_height_;
    break;
  // memory pointers
  case 0x18:
    retval = VIC_mem_pointers_;
    break;
  // 
  // interrupt status register 
  // IRQ|  - |  - |  - | ILP|IMMC|IMBC|IRST|
  //
  case 0x19:
    retval = (0xf & VIC_irq_status_);
    if(retval!=0) retval |= 0x80; // IRQ bit 
    retval |= 0x70; // non-connected bits (always set)
    break;
  // 
  // interrupt enable register 
  //   - |  - |  - |  - | ELP|EMMC|EMBC|ERST|
  //
  case 0x1a:
    retval = (0xf0 | VIC_irq_enabled_);
    break;
  // sprite priority register
  case 0x1b:
    retval = VIC_sprite_priority_;
    break;
  // sprite multicolor mode
  case 0x1c:
    retval = VIC_sprite_multicolor_;
    break;
  // sprite double width
  case 0x1d:
    retval = VIC_sprite_double_width_;
    break;
  // border color
  case 0x20:
    retval = VIC_border_color_;
    break;
  // background colors
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
    retval = VIC_bgcolor_[r-0x21];
    break;
  // sprite colors
  case 0x25:
  case 0x26:
    retval = VIC_sprite_shared_colors_[r-0x25];
    break;
  case 0x27:
  case 0x28:
  case 0x29:
  case 0x2a:
  case 0x2b:
  case 0x2c:
  case 0x2d:
  case 0x2e:
    retval = VIC_sprite_colors_[r-0x27];
    break;
  // unused
  case 0x2f:
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x38:
  case 0x39:
  case 0x3a:
  case 0x3b:
  case 0x3c:
  case 0x3d:
  case 0x3e:
  case 0x3f:
  default:
    retval = 0xff;
    break;
 }
 return retval;
}

//void Vic::VIC_write_register(unsigned char r, unsigned char v)
void VIC_write_register(unsigned char r, unsigned char v)
{
 switch(r)
 {
  // store X coord of sprite n
  case 0x0:
  case 0x2:
  case 0x4:
  case 0x6:
  case 0x8:
  case 0xc:
  case 0xe:
    VIC_mx_[r >> 1] = v;
    break;
  // store Y coord of sprite n
  case 0x1:
  case 0x3:
  case 0x5:
  case 0x7:
  case 0x9:
  case 0xb:
  case 0xd:
  case 0xf:
    VIC_my_[r >> 1] = v;
    break;
  // MSBs of X coordinates
  case 0x10:
    VIC_msbx_ = v;
    break;
  // control register 1
  case 0x11:
    VIC_cr1_ = (v&0x7f);
    VIC_raster_irq_ &= 0xff;
    VIC_raster_irq_ |= (v&0x80) << 1;
    VIC_set_graphic_mode();
    break;
  // raster irq
  case 0x12:
    VIC_raster_irq_ = v | (VIC_raster_irq_ & (1 << 8));
    break;
  // sprite enable register
  case 0x15:
    VIC_sprite_enabled_ = v;
    break;
  // control register 2
  case 0x16:
    VIC_cr2_ = v;
    VIC_set_graphic_mode();
    break;
  // sprite double height
  case 0x17:
    VIC_sprite_double_height_ = v;
    break;
  // memory pointers
  case 0x18:
    // bits ----xxx- 
    VIC_char_mem_   = (v&0xe) << 10;
    // bits xxxx----
    VIC_screen_mem_ = (v&0xf0) << 6;
    // bit  ----x---
    VIC_bitmap_mem_ = (v&0x8) << 10;
    // save reg value (last bit always set)
    VIC_mem_pointers_ = v | (1 << 0);
    break;
  // interrupt request register
  case 0x19:
    // acknowledge interrupts by mask
    VIC_irq_status_ &= ~(v&0xf);
    break;
  // interrupt enable register
  case 0x1a:
    VIC_irq_enabled_= v;
    break;
  // sprite priority register
  case 0x1b:
    VIC_sprite_priority_ = v;
    break;
  // sprite multicolor mode
  case 0x1c:
    VIC_sprite_multicolor_ = v;
    break;
  // sprite double width
  case 0x1d:
    VIC_sprite_double_width_ = v;
    break;
  // border color
  case 0x20:
    VIC_border_color_ = v;
    break;
  // background colors
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
    VIC_bgcolor_[r-0x21] = v;
    break;
  // sprite colors
  case 0x25:
  case 0x26:
    VIC_sprite_shared_colors_[r-0x25] = v; //revisar offset son 2 index maximo
    break;
  case 0x27:
  case 0x28:
  case 0x29:
  case 0x2a:
  case 0x2b:
  case 0x2c:
  case 0x2d:
  case 0x2e:
    VIC_sprite_colors_[r-0x27] = v; //revisar offset son 2 index maximo
    break;
  // unused
  case 0x2f:
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x38:
  case 0x39:
  case 0x3a:
  case 0x3b:
  case 0x3c:
  case 0x3d:
  case 0x3e:
  case 0x3f:
  default:
    break;
 }
}       

// graphics //////////////////////////////////////////////////////////////////

//
// @brief set current graphic mode 
//
//void Vic::VIC_set_graphic_mode()
void VIC_set_graphic_mode()
{
  bool ecm = ((VIC_cr1_ & (1<<6))!= 0);
  bool bmm = ((VIC_cr1_ & (1<<5))!= 0);
  bool mcm = ((VIC_cr2_ & (1<<4))!= 0);

  if(!ecm && !bmm && !mcm)
    VIC_graphic_mode_ = kCharMode;  
  else if(!ecm && !bmm && mcm)
    VIC_graphic_mode_ = kMCCharMode;
  else if(!ecm && bmm && !mcm)
    VIC_graphic_mode_ = kBitmapMode;
  else if(!ecm && bmm && mcm)
    VIC_graphic_mode_ = kMCBitmapMode;
  else if(ecm && !bmm && !mcm)
    VIC_graphic_mode_ = kExtBgMode;
  else 
    VIC_graphic_mode_ = kIllegalMode;
}

//
// @brief retrieves a character from screen Memory
//
//unsigned char Vic::VIC_get_screen_char(int column, int row)
unsigned char VIC_get_screen_char(unsigned short int column, unsigned short int row)
{
 unsigned short int addr = VIC_screen_mem_ + (row * VIC_kGCols) + column;
 //return VIC_mem_->MEMORY_vic_read_byte(addr);
 return MEMORY_vic_read_byte(addr); //No objetos
}

//
// @brief retrives color RAM for given screen coords
//
//unsigned char Vic::VIC_get_char_color(int column, int row)
inline unsigned char VIC_get_char_color(unsigned short int column, unsigned short int row)
{
 unsigned short int addr = MEMORY_kAddrColorRAM + (row * VIC_kGCols) + column;
 //return (VIC_mem_->MEMORY_read_byte_no_io(addr) & 0x0f);
 return (MEMORY_mem_ram_[addr] & 0x0f); //return (MEMORY_read_byte_no_io(addr) & 0x0f); //No objetos
}

//
// @brief retrieves pixel data from character memory
//
//unsigned char Vic::VIC_get_char_data(int chr, int line)
unsigned char VIC_get_char_data(unsigned short int chr, unsigned short int line)
{
 //unsigned short int addr = VIC_char_mem_ + (chr * 8) + line;
 //return VIC_mem_->MEMORY_vic_read_byte(addr);

 unsigned short int addr = VIC_char_mem_ + (chr<<3) + line;
 return MEMORY_vic_read_byte(addr); //No objetos
}

//
// @brief retrieves pixel data from bitmap memory
//
//unsigned char Vic::VIC_get_bitmap_data(int column, int row, int line)
unsigned char VIC_get_bitmap_data(unsigned short int column, unsigned short int row, unsigned short int line)
{
 //unsigned short int addr = VIC_bitmap_mem_ + (row * VIC_kGCols + column) * 8 + line;
 //return VIC_mem_->MEMORY_vic_read_byte(addr);
 
 unsigned short int addr = VIC_bitmap_mem_ + ((row * VIC_kGCols + column)<<3) + line;
 return MEMORY_vic_read_byte(addr); //No objetos
}

//
// @brief get sprite pointer 
//
// n is sprite # (0-7)
//
//unsigned short int Vic::VIC_get_sprite_ptr(int n)
unsigned short int VIC_get_sprite_ptr(unsigned char n)
{
  unsigned short int ptraddr = VIC_screen_mem_ + VIC_kSpritePtrsOffset + n;
  //return VIC_kSpriteSize * VIC_mem_->MEMORY_vic_read_byte(ptraddr);
  return VIC_kSpriteSize * MEMORY_vic_read_byte(ptraddr); //No objetos
}
 
// raster drawing  ///////////////////////////////////////////////////////////

//void Vic::VIC_draw_char(int x, int y, unsigned char data, unsigned char color)
void VIC_draw_char(unsigned short int x, unsigned short int y, unsigned char data, unsigned char color)
{
  unsigned short int xoffs;
  for(unsigned char i=0 ; i < 8 ; i++)
  {
    xoffs = x + 8 - i + (VIC_cr2_&0x7); //xoffs = x + 8 - i + VIC_horizontal_scroll();
    // don't draw outside (due to horizontal scroll)
    if(xoffs > (VIC_kGFirstCol + VIC_kGResX))
      continue;
    // draw pixel
    if(ISSET_BIT(data,i))
    {
      //io_->IO_screen_update_pixel(xoffs, y, color);
      //IO_screen_update_pixel(xoffs, y, color); //no quiero objetos
      gb_buffer_vga[y][xoffs^2] = gb_color_vga[color];
    }
  }
}

//void Vic::VIC_draw_mcchar(int x, int y, unsigned char data, unsigned char color)
void VIC_draw_mcchar(unsigned short int x, unsigned short int y, unsigned char data, unsigned char color)
{
 unsigned char c; //color
 unsigned char cs; //color source
 unsigned short int xoffs;
  
 for(unsigned char i=0 ; i < 4 ; i++)
 {    
  //unsigned char cs = ((data >> i*2) & 0x3);
  cs = ((data >> (i<<1)) & 0x3);
  switch(cs)
  {
   case 0: c = VIC_bgcolor_[0]; break;
   case 1: c = VIC_bgcolor_[1]; break;
   case 2: c = VIC_bgcolor_[2]; break;
   case 3: c = color; break;
  }
    //int xoffs = x + 8 - i * 2 + VIC_horizontal_scroll();
   xoffs = x + 8 - (i<<1) + (VIC_cr2_&0x7); //xoffs = x + 8 - (i<<1) + VIC_horizontal_scroll();
    //io_->IO_screen_update_pixel(xoffs, y, c);
//    IO_screen_update_pixel(xoffs, y, c); //no quiero objetos
    //io_->IO_screen_update_pixel(xoffs + 1, y, c);
//    IO_screen_update_pixel(xoffs + 1, y, c);//no quiero objetos
    
   gb_buffer_vga[y][xoffs^2] = gb_color_vga[c];
   gb_buffer_vga[y][(xoffs + 1)^2] = gb_color_vga[c];
 }
 //printf("VIC_draw_mcchar\n");
}

//void Vic::VIC_draw_raster_char_mode()
void VIC_draw_raster_char_mode()
{
  unsigned short int x, line, row, char_row;
  unsigned char c, data, color;
  unsigned short int rstr = (VIC_raster_c_ | ((VIC_cr1_&0x80) << 1)); //VIC_raster_counter_get();
  unsigned short int y = rstr - VIC_kFirstVisibleLine;
  if((rstr >= VIC_kGFirstLine) && (rstr < VIC_kGLastLine) && !VIC_is_screen_off())  
  {
    // draw background
    //io_->IO_screen_draw_rect(kGFirstCol,y,kGResX,bgcolor_[0]);
    IO_screen_draw_rect(VIC_kGFirstCol,y,VIC_kGResX,VIC_bgcolor_[0]);//no quiero objetos
    // draw characters
    for(unsigned short int column=0; column < VIC_kGCols ; column++)
    {
      // check 38 cols mode
      if(!ISSET_BIT(VIC_cr2_,3))
      {
        if (column == 0) continue; 
        if (column == VIC_kGCols -1 ) continue; 
      }
      x = VIC_kGFirstCol + (column<<3); //x = VIC_kGFirstCol + column * 8;
      line = rstr - VIC_kGFirstLine;
      row = (line>>3); //row = line/8;
      char_row = (line & 0x07); //char_row = line % 8;
      // retrieve screen character
      c = VIC_get_screen_char(column,row);
      // retrieve character bitmap data
      data = VIC_get_char_data(c,char_row);
      // retrieve color data
      color  = VIC_get_char_color(column,row);
      // draw character
      if(VIC_graphic_mode_ == kMCCharMode && ISSET_BIT(color,3))
      {
        VIC_draw_mcchar(x,y,data,(color&0x7));
      }
      else
      {
        VIC_draw_char(x,y,data,color);
      }
    }
  }
}

//void Vic::VIC_draw_bitmap(int x, int y, unsigned char data, unsigned char color)
void VIC_draw_bitmap(unsigned short int x, unsigned short int y, unsigned char data, unsigned char color)
{//No toca ningun registro de emulacion.Solo dibuja
 unsigned short int xoffs;
 unsigned char forec = (color >> 4) & 0xf;
 unsigned char bgc = color & 0xf;
 
 for(unsigned char i=0 ; i < 8 ; i++)
 {
  xoffs = x + 8 - i + (VIC_cr2_&0x7); //xoffs = x + 8 - i + VIC_horizontal_scroll();
  // don't draw outside (due to horizontal scroll)
  if(xoffs > (VIC_kGFirstCol + VIC_kGResX)){
   continue;
  }
  // draw pixel
//  if(ISSET_BIT(data,i))
//  {
//   //io_->IO_screen_update_pixel(xoffs, y, forec);
//   //IO_screen_update_pixel(xoffs, y, forec);//no quiero objetos
//   gb_buffer_vga[y][xoffs] = forec;
//  }
//  else
//  {
//   //io_->IO_screen_update_pixel(xoffs, y, bgc);
//   //IO_screen_update_pixel(xoffs, y, bgc);//no quiero objetos
//   gb_buffer_vga[y][xoffs] = bgc;
//  }

  gb_buffer_vga[y][xoffs^2] = ((ISSET_BIT(data,i)) == 1) ? gb_color_vga[forec] : gb_color_vga[bgc];
 }
}

//void Vic::VIC_draw_mcbitmap(int x, int y, unsigned char data, unsigned char scolor, unsigned char rcolor)
void VIC_draw_mcbitmap(unsigned short int x, unsigned short int y, unsigned char data, unsigned char scolor, unsigned char rcolor)
{//No toca ningun registro de emulacion.Solo dibuja
 unsigned char c; //color
 unsigned char cs; // color source
 unsigned short int xoffs;

 for(unsigned char i=0 ; i < 4 ; i++)
 {    
  //cs = ((data >> i*2) & 0x3);
  cs = ((data >> (i<<1)) & 0x3);
  switch(cs)
  {
   case 0: c = VIC_bgcolor_[0]; break;
   case 1: c = (scolor >> 4) & 0xf; break;
   case 2: c = scolor & 0xf; break;
   case 3: c = rcolor; break;
  }
  //xoffs = x + 8 - i * 2 + VIC_horizontal_scroll();
  xoffs = x + 8 - (i<<1) + (VIC_cr2_&0x7); //xoffs = x + 8 - (i<<1) + VIC_horizontal_scroll();
  //io_->IO_screen_update_pixel(xoffs, y, c);
//  IO_screen_update_pixel(xoffs, y, c);//no quiero objetos
  //io_->IO_screen_update_pixel(xoffs + 1, y, c);
//  IO_screen_update_pixel(xoffs + 1, y, c);//no quiero objetos
  
  gb_buffer_vga[y][xoffs^2] = gb_color_vga[c];
  gb_buffer_vga[y][(xoffs+1)^2] = gb_color_vga[c];
 }  
}
 
//void Vic::VIC_draw_raster_bitmap_mode()
void VIC_draw_raster_bitmap_mode()
{//No toca ningun registro de emulacion, solo dibuja
 unsigned short int x, line, row, bitmap_row;
 unsigned char data, scolor, rcolor;
 unsigned short int rstr = (VIC_raster_c_ | ((VIC_cr1_&0x80) << 1)); //VIC_raster_counter_get();
 unsigned short int y = rstr - VIC_kFirstVisibleLine;

 if((rstr >= VIC_kGFirstLine) && (rstr < VIC_kGLastLine) && !VIC_is_screen_off()) 
 {
  // draw background
  //io_->IO_screen_draw_rect(kGFirstCol,y,kGResX,bgcolor_[0]);
  IO_screen_draw_rect(VIC_kGFirstCol,y,VIC_kGResX,VIC_bgcolor_[0]);//no quiero objetos
  // draw bitmaps
  for(unsigned short int column=0; column < VIC_kGCols ; column++)
  {
   //x = VIC_kGFirstCol + column * 8;
   x = VIC_kGFirstCol + (column<<3);
   line = rstr - VIC_kGFirstLine;
   //row = line/8;
   row = (line>>3);
   //bitmap_row = line % 8;
   bitmap_row = (line & 0x07);
   // retrieve bitmap data
   data = VIC_get_bitmap_data(column,row,bitmap_row);
   // retrieve color data
   scolor = VIC_get_screen_char(column,row);
   rcolor = VIC_get_char_color(column,row); 
   // draw bitmap
   if(VIC_graphic_mode_ == kBitmapMode)
   {
    VIC_draw_bitmap(x,y,data,scolor);
   }
   else
   {
    VIC_draw_mcbitmap(x,y,data,scolor,rcolor);
   }
  }//fin for column
 }//fin if rstr
}

//void Vic::VIC_draw_mcsprite(int x, int y, int sprite, int row)
void VIC_draw_mcsprite(unsigned short int x, unsigned short int y, unsigned char sprite, unsigned short int row)
{
 unsigned char c; //color
 unsigned char cs; // color source
 unsigned char data;
 unsigned short int xOfs;
 unsigned short int addr = VIC_get_sprite_ptr(sprite);

 for (unsigned char i=0; i < 3 ; i++)
 {
  //unsigned char data = VIC_mem_->MEMORY_vic_read_byte(addr + row * 3 + i);
  data = MEMORY_vic_read_byte(addr + row * 3 + i); //No objetos
  for (unsigned char j=0; j < 4; j++)
  {      
   c = 0;      
   //cs = ((data >> j*2) & 0x3);
   cs = ((data >> (j<<1)) & 0x3);
   switch(cs)
   {
    // transparent
    case 0: break;
    case 1: c = VIC_sprite_shared_colors_[0]; break;
    case 2: c = VIC_sprite_colors_[sprite]; break;
    case 3: c = VIC_sprite_shared_colors_[1]; break;
   }
   // draw if not transparent
   if(cs != 0)
   {
    //io_->IO_screen_update_pixel((x + i*8 + 8 - j * 2), y, c);
//  IO_screen_update_pixel((x + i*8 + 8 - j * 2), y, c);//no quiero objetos
    //io_->IO_screen_update_pixel((x + i*8 + 8 - j * 2 + 1), y, c);
//  IO_screen_update_pixel((x + i*8 + 8 - j * 2 + 1), y, c);//no quiero objetos
  
    xOfs= (x + (i<<3) + 8 - (j<<1));
    gb_buffer_vga[y][xOfs^2] = gb_color_vga[c];
    gb_buffer_vga[y][(xOfs + 1)^2] = gb_color_vga[c];
   }
  }
 }      
}

//void Vic::VIC_draw_sprite(int x, int y, int sprite, int row)
void VIC_draw_sprite(unsigned short int x, unsigned short int y, unsigned char sprite, unsigned short int row)
{
 //int swid = VIC_is_double_width_sprite(sprite) ? 2 : 1;

 unsigned char data;
 unsigned char swid = VIC_is_double_width_sprite(sprite) ? 2 : 1;
 unsigned short int addr = VIC_get_sprite_ptr(sprite);
 unsigned short int new_x;
 unsigned char color;
 unsigned short int side_border_offset, top_border_offset, btm_border_offset;

 for(unsigned char w=0; w < swid ; w++ )
 {
  for (unsigned char i=0; i < 3 ; i++)
  {
   //unsigned char data = VIC_mem_->MEMORY_vic_read_byte(addr + row * 3 + i);
   data = MEMORY_vic_read_byte(addr + row * 3 + i); //No objetos
   for (unsigned char j=0; j < 8; j++)
   {
    if(ISSET_BIT(data,j))
    {
     //new_x = (x+w + (i*8*swid) + (8*swid) - (j*swid)) ;
     new_x = (x+w + ((i<<3)*swid) + (8*swid) - (j*swid)) ;
     color = VIC_sprite_colors_[sprite];
     side_border_offset = 0;
     top_border_offset  = 0;
     btm_border_offset  = 0;
     // check 38 cols mode
     if(!ISSET_BIT(VIC_cr2_,3)){
      side_border_offset = 8;
     }
     // check 24 line mode
     if(!ISSET_BIT(VIC_cr1_,3))
     {
      top_border_offset = 2;
      btm_border_offset = 4;
     }
     // check bounds
     if(new_x <= VIC_kGFirstCol+side_border_offset ||
        y < VIC_kGFirstCol + top_border_offset ||
        new_x > VIC_kGResX+VIC_kGFirstCol-side_border_offset ||
        y >= VIC_kGResY+VIC_kGFirstCol - btm_border_offset)
     {   
      color = VIC_border_color_;
     }
     // update pixel
     //io_->IO_screen_update_pixel(new_x, y, color);
//   IO_screen_update_pixel(new_x, y, color);//no quiero objetos
     gb_buffer_vga[y][new_x^2] = gb_color_vga[color];
    }//fin ISSET_BIT
   }
  }//fin unsigned char i
 }//fin unsigned char w
}

//void Vic::VIC_draw_raster_sprites()
void VIC_draw_raster_sprites()
{//22720  llamadas segundo
 unsigned short int rstr, y, sp_y, height, row, x;
  
 //#ifdef use_lib_cont_call
 // gb_cont_call++;
 //#endif          
 
 if(VIC_sprite_enabled_ != 0)
 {
  rstr = (VIC_raster_c_ | ((VIC_cr1_&0x80) << 1)); //VIC_raster_counter_get();
  y = rstr - VIC_kFirstVisibleLine;
  sp_y = rstr - VIC_kSpritesFirstLine;
  // loop over sprites reverse order
  for(signed char n=7; n >= 0 ; n--)
  {
   height = ISSET_BIT(VIC_sprite_double_height_,n) ? (VIC_kSpriteHeight<<1) : VIC_kSpriteHeight; //height = VIC_is_double_height_sprite(n) ? VIC_kSpriteHeight * 2 : VIC_kSpriteHeight;
   // check if the sprite is visible
   if(VIC_is_sprite_enabled(n) && 
      sp_y >= VIC_my_[n] && 
      sp_y < VIC_my_[n] + height)
   {
    row = sp_y - VIC_my_[n];
    x = VIC_kSpritesFirstCol + VIC_sprite_x(n);
    if(VIC_is_double_height_sprite(n))
    {
     row = (sp_y - VIC_my_[n])/2;
    }
    if(VIC_is_multicolor_sprite(n))
    {
     VIC_draw_mcsprite(x,y,n,row);
    }
    else
    {
     VIC_draw_sprite(x,y,n,row);
    }
   }//fin VIC_is_sprite_enabled
  }//fin signed char n
 }//fin VIC_sprite_enabled_
}

// helpers ///////////////////////////////////////////////////////////////////

//void Vic::VIC_raster_counter(int v)
//void VIC_raster_counter(int v)
void VIC_raster_counter_set(unsigned short int v)
{
 VIC_raster_c_ = (unsigned char)(v&0xff);
 VIC_cr1_ &= 0x7f;
 VIC_cr1_ |= ((v >> 1)&0x80);
}

//int Vic::VIC_raster_counter()
//int VIC_raster_counter()
//unsigned short int VIC_raster_counter_get()
//{//Llega a 311
// //unsigned short int aux= (VIC_raster_c_ | ((VIC_cr1_&0x80) << 1));   
// // if  (aux> 254)
// //  printf("VIC_raster_counter %d\n",kk);
//    
// return (VIC_raster_c_ | ((VIC_cr1_&0x80) << 1));
//}

//
// @brief screen on/off
//
// Bit #4 of cr1 :
//
// 0 = Screen off, complete screen is covered by border. 
// 1 = Screen on, normal screen contents are visible.
//
//bool Vic::VIC_is_screen_off()
inline bool VIC_is_screen_off()
{
 return ((VIC_cr1_&(1<<4)) == 0);
}

//
// @brief checks for bad line condition
//
// According to Christian Bauer's paper:
// 
// A Bad Line Condition is given at any arbitrary clock cycle, 
// if at the negative edge of 0 at the beginning of the cycle 
// RASTER >= $30 and RASTER <= $f7 and the lower three bits 
// of RASTER are equal to YSCROLL and if the DEN bit was set 
// during an arbitrary cycle of raster line $30.
//
//
//bool Vic::VIC_is_bad_line()
inline bool VIC_is_bad_line()
{
 unsigned short int rstr = (VIC_raster_c_ | ((VIC_cr1_&0x80) << 1)); //VIC_raster_counter_get();
 //return (rstr >= 0x30 &&
 //        rstr <= 0xf7 &&
 //        (rstr & 0x7) == (VIC_vertical_scroll() & 0x7));
 return (rstr >= 0x30 &&
         rstr <= 0xf7 &&
         (rstr & 0x7) == ((VIC_cr1_&0x7) & 0x7));         
}

//
// @brief check if raster irq is enabled 
//
//bool Vic::VIC_raster_irq_enabled()
//inline bool VIC_raster_irq_enabled()
//{
// return ISSET_BIT(VIC_irq_enabled_,0);
//}

// 
// @brief get vertical raster scroll
//
//unsigned char Vic::VIC_vertical_scroll()
//inline unsigned char VIC_vertical_scroll()
//{
// return(VIC_cr1_&0x7);
//} 
 
// 
// @brief get horizontal raster scroll
//
//unsigned char Vic::VIC_horizontal_scroll()
//inline unsigned char VIC_horizontal_scroll()
//{
//  return(VIC_cr2_&0x7);
//}

//
// @brief check if sprite #n is enabled
//
//bool Vic::VIC_is_sprite_enabled(int n)
inline bool VIC_is_sprite_enabled(unsigned char n)
{
 return ISSET_BIT(VIC_sprite_enabled_,n);
}

//
//@brief check if sprite #n is in background
//
//bool Vic::VIC_is_background_sprite(int n)
inline bool VIC_is_background_sprite(unsigned char n)
{
 return ISSET_BIT(VIC_sprite_priority_,n);
}
 
//
// @brief check if sprite #n is double width
//
//bool Vic::VIC_is_double_width_sprite(int n)
inline bool VIC_is_double_width_sprite(unsigned char n)
{
 return ISSET_BIT(VIC_sprite_double_width_,n);
}

//
// @brief check if sprite #n is double height
//
//bool Vic::VIC_is_double_height_sprite(int n)
inline bool VIC_is_double_height_sprite(unsigned char n)
{
 return ISSET_BIT(VIC_sprite_double_height_,n);
}

//
// @brief check if sprite #n is multicolor
//
//bool Vic::VIC_is_multicolor_sprite(int n)
inline bool VIC_is_multicolor_sprite(unsigned char n)
{
 return ISSET_BIT(VIC_sprite_multicolor_,n);
}

//
// @brief returns sprite x coordinate 
//
//int Vic::VIC_sprite_x(int n)
unsigned short int VIC_sprite_x(unsigned char n)
{
 unsigned short int x = VIC_mx_[n];
 if(ISSET_BIT(VIC_msbx_,n)){
  x |= 1 << 8;
 }
 return x;
}



//void Vic::VIC_memory(Memory *v)
//void VIC_memory(Memory *v) //No quiero objetos
//{
// VIC_mem_ = v;
//};

//void Vic::VIC_cpu(Cpu *v) //No objetos
//{
// VIC_cpu_ = v;
//};

//unsigned int Vic::VIC_frames()
unsigned int VIC_frames()
{
 return VIC_frame_c_;
};
