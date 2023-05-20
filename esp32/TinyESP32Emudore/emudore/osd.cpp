#include "gbConfig.h"
#include "osd.h"
//#include "dataFlash/gbrom.h"
//#include "dataFlash/gbpjr.h"
//#include "dataFlash/gbcom.h"
//#include "dataFlash/gbdsk.h"
//#include "dataFlash/gbsnarare.h"
#include "gbGlobals.h"
#include "dataFlash/gbprg.h"
//#include "render.h"
//#include "cpu.h"
//#include "ports.h"
#include "sid.h"
#include <string.h>
#include "Arduino.h"


#define ID_COLOR_BLACK 0 //Negro
#define ID_COLOR_WHITE 15 //Blanco
#define ID_COLOR_VIOLETA 0 //Violeta

#define KEY_ENTER 0x5A
#define KEY_CURSOR_LEFT 0x6B
#define KEY_CURSOR_DOWN 0x72
#define KEY_CURSOR_RIGHT 0x74
#define KEY_CURSOR_UP 0x75
#define KEY_ESC 0x76

#define BLACK   0
#define BLUE    4
#define RED     1
#define MAGENTA 5
#define GREEN   2
#define CYAN    6
#define YELLOW  3

//#define WHITE   255
#define WHITE   15



//extern int gb_screen_xIni;
//extern int gb_screen_yIni;
//extern unsigned char gb_cache_zxcolor[8];
//static SDL_Surface * gb_osd_sdl_surface;
//static SDL_Event * gb_osd_sdl_event;
//static SDL_Color * gb_osd_sdl_palette;
unsigned char gb_show_osd_main_menu=0;

//extern SDL_Surface * gb_screen;
//extern SDL_Event gb_event;

//unsigned int gbSDLVideoDelay = 20;


//void jj_fast_putpixel(unsigned short x,unsigned short int y,unsigned char c)
//{
// gb_buffer_vga[y][x]=c;
//}



#ifdef use_lib_keyboard_uart
 void do_keyboard_uart()
 {
  unsigned int contBuf=0;
  if (gb_rlen_uart>0)
  {
   
   memset(IO_keyboard_matrix_,0xFF,sizeof(IO_keyboard_matrix_)); //Borro teclas

   while (contBuf < gb_rlen_uart)
   {
    switch (gb_buf_uart[contBuf])
    {
     case 0x01: //F2   01 62 4F 51      
      if ((contBuf+3) < gb_rlen_uart)
      {
       gb_show_osd_main_menu= 1;
       contBuf+= 3;
      }
      break;

     case 0x09: //TAB saco menu tambien
      gb_show_osd_main_menu= 1;      
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER 
      //keyboard[6]&= 0xFE; //ENTER
      IO_keyboard_matrix_[0]= 0xFD;
      break;

     case 0x2B: 
      //keyboard[3]&= 0xFE; //El + como ALT_GR
      break;

     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: 
          //keyboard[4]&= 0xF7; //kempston arriba //SHIFT + 7
          IO_keyboard_matrix_[0]&= 0x7F;
          IO_keyboard_matrix_[6]&= 0xBF;
          //arriba 1B 5B 41
          break;
         case 0x42: 
          //keyboard[4]&= 0xEF;  //kempston abajo //SHIFT + 6
          break; //abajo 1B 5B 42
         case 0x43: 
          //keyboard[4]&= 0xFB; //derecha 1B 5B 43 //SHIFT + 8
          break;
         case 0x44: 
          //keyboard[3]&= 0xEF; //izquierda 1B 5B 44 //SHIFT + 5
          IO_keyboard_matrix_[0]&= 0xFB;
          IO_keyboard_matrix_[7]&= 0xFD;          
          break; 
        }
       }
      }
      break;       

     //row0
     //case 0x2D: keyboard[0]&= 0xFE; break; //SHIFT LEFT -
     case 0x7A: case 0x5A: IO_keyboard_matrix_[1]&= 0xEF; break; //Z
     case 0x78: case 0x58: IO_keyboard_matrix_[2]&= 0x7F; break; //X
     case 0x63: case 0x43: IO_keyboard_matrix_[2]&= 0xEF; break; //C
     case 0x76: case 0x56: IO_keyboard_matrix_[3]&= 0x7F; break; //V

     //row 1
     case 0x61: case 0x41: IO_keyboard_matrix_[1]&= 0xFB; break; //A
     case 0x73: case 0x53: IO_keyboard_matrix_[1]&= 0xDF; break; //S
     case 0x64: case 0x44: IO_keyboard_matrix_[2]&= 0xFB; break; //D
     case 0x66: case 0x46: IO_keyboard_matrix_[2]&= 0xDF; break; //F
     case 0x67: case 0x47: IO_keyboard_matrix_[3]&= 0xFB; break; //G

     //row 2
     case 0x71: case 0x51: IO_keyboard_matrix_[7]&= 0xBF; break; //Q
     case 0x77: case 0x57: IO_keyboard_matrix_[1]&= 0xFD; break; //W
     case 0x65: case 0x45: IO_keyboard_matrix_[1]&= 0xBF; break; //E
     case 0x72: case 0x52: IO_keyboard_matrix_[2]&= 0xFD; break; //R
     case 0x74: case 0x54: IO_keyboard_matrix_[2]&= 0xBF; break; //T

     //row 3
     case 0x31: IO_keyboard_matrix_[7]&= 0xFE; break; //1
     case 0x32: IO_keyboard_matrix_[7]&= 0xF7; break; //2
     case 0x33: IO_keyboard_matrix_[1]&= 0xFE; break; //3
     case 0x34: IO_keyboard_matrix_[1]&= 0xF7; break; //4
     case 0x35: IO_keyboard_matrix_[2]&= 0xFE; break; //5       

     //row 4
     case 0x30: IO_keyboard_matrix_[4]&= 0xF7; break; //0
     case 0x39: IO_keyboard_matrix_[4]&= 0xFE; break; //9
     case 0x38: IO_keyboard_matrix_[3]&= 0xF7; break; //8
     case 0x37: IO_keyboard_matrix_[3]&= 0xFE; break; //7
     case 0x36: IO_keyboard_matrix_[2]&= 0xF7; break; //6

     //row 5
     case 0x70: case 0x50: IO_keyboard_matrix_[5]&= 0xFD; break; //P
     case 0x6F: case 0x4F: IO_keyboard_matrix_[4]&= 0xBF; break; //O
     case 0x69: case 0x49: IO_keyboard_matrix_[4]&= 0xFD; break; //I
     case 0x75: case 0x55: IO_keyboard_matrix_[3]&= 0xBF; break; //U     
     case 0x79: case 0x59: IO_keyboard_matrix_[3]&= 0xFD; break; //Y   

     //row 6
     //if (!keymap[PS2_KC_ENTER] || !keymap[PS2_KC_KP_ENTER]) { keyboard[6]&= 0xFE; } //ENTER
     case 0x6C: case 0x4C: IO_keyboard_matrix_[5]&= 0xFB; break; //L
     case 0x6B: case 0x4B: IO_keyboard_matrix_[4]&= 0xDF; break; //K
     case 0x6A: case 0x4A: IO_keyboard_matrix_[4]&= 0xFB; break; //J
     case 0x68: case 0x48: IO_keyboard_matrix_[3]&= 0xDF; break; //H       

     //row 7
     case 0x20: IO_keyboard_matrix_[7]&= 0xEF; break; //SPACE
     //case 0x2E: keyboard[7]&= 0xFD; break; //.
     case 0x6D: case 0x4D: IO_keyboard_matrix_[4]&= 0xEF; break; //M
     case 0x6E: case 0x4E: IO_keyboard_matrix_[4]&= 0x7F; break; //N
     case 0x62: case 0x42: IO_keyboard_matrix_[3]&= 0xEF; break; //B
     

    }//fin switch gb_buf_uart
    contBuf++;
   }//fin while contBuf   
  }//fin if gb_rlen_uart
 }

 void keyboard_uart_poll()
 {
  if(Serial.available() > 0)
  {
   gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);

   //Serial.print("OSD I received: ");
   gb_buf_uart[gb_rlen_uart]='\0';
   #ifdef use_lib_log_keyboard_uart
    Serial.printf("UART key tot:%d\r\nASCII:%s\r\n",gb_rlen_uart,gb_buf_uart);        
    for (unsigned short int i=0;i<gb_rlen_uart;i++)
    {
     Serial.printf("%02X ",gb_buf_uart[i]);
    }
    Serial.printf("\r\n"); 
   #endif 
  }
  else
  {
   gb_rlen_uart=0;
   gb_buf_uart[0]='\0';
  }  
 }

 unsigned char checkKey_uart(unsigned char scancode)
 {
  unsigned int contBuf=0;
  unsigned char toReturn= 0;
  //if(Serial.available() > 0)
  //{
  // gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);
  //
  // Serial.print("OSD I received: ");
  // gb_buf_uart[gb_rlen_uart]='\0';
  // Serial.printf("OSD Tot:%d\nASCII:%s\n",gb_rlen_uart,gb_buf_uart);     
  // Serial.printf("\n"); 
   
   //for(contBuf= 0; contBuf < gb_rlen_uart; contBuf++)
   //Serial.printf("OSD check tot:%d\n",gb_rlen_uart);
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {          
         case 0x41: toReturn = (scancode == KEY_CURSOR_UP) ? 1 : 0; break; //arriba 1B 5B 41
         case 0x42: toReturn = (scancode == KEY_CURSOR_DOWN) ? 1 : 0; break; //abajo 1B 5B 42
         case 0x43: toReturn = (scancode == KEY_CURSOR_RIGHT) ? 1 : 0; break; //derecha 1B 5B 43
         case 0x44: toReturn = (scancode == KEY_CURSOR_LEFT) ? 1 : 0; break; //izquierda 1B 5B 44                 
        }
       }
      }
      else
      {               
       toReturn = (scancode == KEY_ESC) ? 1 : 0; //ESC                     
      }
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
      //if ((contBuf+1) < gb_rlen_uart)
      //{
      // contBuf++;
      // if (gb_buf_uart[contBuf] == 0x0A)
      // {
      //  toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      //  //contBuf++;
      // }
      //}      
       toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER       
      break;

    }
    contBuf++;
   }
  //}
  //Serial.printf("\n");
  return toReturn;
 } 
#endif 



//***************************************
void SDLDrawVGA()
{
/* No lo necesitamos    
 unsigned int auxLine = screen->pitch;
 Uint8 *p = (Uint8 *)screen->pixels + auxLine;
 for (int y=0;y<284;y++)
 {
  p = (Uint8 *)screen->pixels + (auxLine*y);
  for (int x=0;x<403;x++)
  {    
   *p = gb_buffer_vga[y][x];   
   p++;
  }
 }
 
 SDL_Flip (screen);
 //SDL_Delay (20);
 */
}



//**********************************
//SDL_Surface * SDLOSDGetSurface()
//{
// return gb_osd_sdl_surface;
//}


//void SDLputpixel(SDL_Surface *surface, int x, int y, unsigned char pixel)
//{
//// if (x >= (surface->w) || y >= (surface->h))
////  return;
// if (x > 320 || x<0 || y<0 || y >= 192)
//  return;
// Uint8* p = (Uint8*)surface->pixels + (y * surface->pitch) + x;
// *p= pixel;
//}

//*************************************************************************************
void SDLprintCharOSD(char car,int x,int y,unsigned char color,unsigned char backcolor)
{ 
// unsigned char aux = gb_sdl_font_6x8[(car-64)];
 int auxId = car << 3; //*8
 unsigned char aux;
 //unsigned char auxBit,auxColor;
 unsigned char auxColor;
 for (unsigned char j=0;j<8;j++)
 {
  //aux = gb_sdl_font_8x8[auxId + j];
  aux = gb_sdl_font[auxId + j];
  for (int i=0;i<8;i++)
  {
   auxColor= ((aux>>i) & 0x01);
   //SDLputpixel(surface,x+(6-i),y+j,(auxColor==1)?color:backcolor);
   //gb_buffer_vga[(y+j)][(x+(6-i))]= (auxColor==1)?color:backcolor; //jj_fast_putpixel(x+(6-i),y+j,(auxColor==1)?color:backcolor);
   gb_buffer_vga[(y+j)][(x+(6-i))^2]= (auxColor==1)?gb_color_vga[color]:gb_color_vga[backcolor]; //jj_fast_putpixel(x+(6-i),y+j,(auxColor==1)?color:backcolor);
  }
 }
}

//*****************************************************************************************
void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor)
{
//SDL_Surface *surface,
// gb_sdl_font_6x8
 int auxLen= strlen(cad);
 if (auxLen>50)
  auxLen=50;
 for (int i=0;i<auxLen;i++)
 {
  SDLprintCharOSD(cad[i],x,y,color,backcolor);
  x+=7;
 }
}

//**********************************
//void SDLOSDClear()
//{
// SDLClear(gb_osd_sdl_surface);
//}

/*No lo necesito
void SDLClear(SDL_Surface *auxSurface)
{
 //for (int y=0; y<(auxSurface->w); y++)
 // for (int x=0; x<(auxSurface->h); x++)
 //  SDLputpixel(auxSurface,x,y,1);
 for (int y=0; y<200; y++){
  for (int x=0; x<320; x++){
   gb_buffer_vga[y][x]=0; //jj_fast_putpixel(x,y,0);
   //SDLputpixel(auxSurface,x,y,3); 
  }
 }
}
*/

/*No lo necesito
//Asignamos el Surface global
//void SDLAssignSDLSurfaceOSD(SDL_Surface *surface,SDL_Event * ptrEvent,SDL_Color *ptrPalette)
void SDLAssignSDLSurfaceOSD(SDL_Surface *surface,SDL_Event * ptrEvent)
{
 gb_osd_sdl_surface = surface;
 gb_osd_sdl_event = ptrEvent;
 //gb_osd_sdl_palette = ptrPalette;
}
*/

#define max_gb_reset_menu 2
const char * gb_reset_menu[max_gb_reset_menu]={
 "Soft",
 "Hard"
};


#define max_gb_osd_cpu_delay 6
const char * gb_osd_cpu_delay[max_gb_osd_cpu_delay]={
 "0",
 "20",
 "100",
 "200",
 "500",
 "1000"
};


#define max_gb_osd_cpu_title 2
const char * gb_osd_cpu_title[max_gb_osd_cpu_title]={
 "AUTO",
 "FAST"
};


#define max_gb_osd_screen 1
const char * gb_osd_screen[max_gb_osd_screen]={
 "Pixels Left"//,
 //"Pixels Top",
 //"Color 8",
 //"Mono Blue 8",
 //"Mono Green 8",
 //"Mono Red 8",
 //"Mono Grey 8"
};

#define max_gb_osd_screen_values 5
const char * gb_osd_screen_values[max_gb_osd_screen_values]={
 "0",
 "2",
 "4", 
 "8", 
 "16"
};


//#define max_gb_main_menu 19
#define max_gb_main_menu 4
const char * gb_main_menu[max_gb_main_menu]={
 "Load PRG",
 "Select D64",
 "Reset",
 "CPU"
};


#define max_gb_speed_sound_menu 7
const char * gb_speed_sound_menu[max_gb_speed_sound_menu]={
 "0",
 "1",
 "2",
 "4",
 "8",
 "16",
 "32"
};

#define max_gb_value_binary_menu 2
const char * gb_value_binary_menu[max_gb_value_binary_menu]={
 "0",
 "1"
};


#define max_gb_speed_videoaudio_options_menu 4
const char * gb_speed_videoaudio_options_menu[max_gb_speed_videoaudio_options_menu]={
 "Audio poll",
 "Video delay",
 "Skip Frame",
 "Keyboard poll"
};

#define max_gb_speed_menu 5
const char * gb_speed_menu[max_gb_speed_menu]={
 "Normal",
 "2x",
 "4x",
 "8x",
 "16x"
};


//****************************
//void SDLEsperaTeclado()
//{
// /*No lo necesito 
// SDL_WaitEvent(gb_osd_sdl_event);
// */
//}

#define gb_pos_x_menu 50
#define gb_pos_y_menu 20
#define gb_osd_max_rows 10

void OSDMenuRowsDisplayScroll(const char **ptrValue,unsigned char currentId,unsigned char aMax)
{//Dibuja varias lineas
 for (int i=0;i<gb_osd_max_rows;i++)
  SDLprintText("                    ",gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),0,0);
 
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  if (currentId >= aMax)
   break;
  //SDLprintText(gb_osd_sdl_surface,ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?CYAN:WHITE),((i==0)?BLUE:BLACK),1);
  SDLprintText(ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_BLACK:ID_COLOR_WHITE),((i==0)?ID_COLOR_WHITE:ID_COLOR_BLACK));
  currentId++;
 } 
}


void SDLClear()
{
 unsigned int a= 0 | gb_sync_bits;
 a= (a<<24)|(a<<16)|(a<<8)|a;
 for (int y=0; y<300; y++)
 {
  for (int x=0; x<100; x++) //400 div 4
  {   
   gb_buffer_vga32[y][x]=a;
  }
 } 
}

//Maximo 256 elementos
unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax,short int aSel)
{  
 unsigned char aReturn=0;
 unsigned char salir=0;

 unsigned int curTime_keyboard;
 unsigned int curTime_keyboard_before;
 
 #ifdef use_lib_keyboard_uart
  unsigned int curTime_keyboard_uart;
  unsigned int curTime_keyboard_before_uart;
  curTime_keyboard_uart = curTime_keyboard_before_uart= millis();
 #endif

 curTime_keyboard= curTime_keyboard_before= millis();

 SDLClear();
 
 //SDLprintText("ESP32 Emudore by Ackerman",gb_pos_x_menu-(4<<3),gb_pos_y_menu-16,WHITE,BLACK,1);
 SDLprintText("ESP32 Emudore by Ackerman",gb_pos_x_menu-(4<<3),gb_pos_y_menu-16,ID_COLOR_WHITE,ID_COLOR_BLACK);
 for (int i=0;i<14;i++) 
  SDLprintCharOSD(' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 SDLprintText(cadTitle,gb_pos_x_menu,gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);

 aReturn = (aSel!=-1)?aSel:0;
 OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax); 

 while (salir == 0)
 {
  //case SDLK_UP:
  curTime_keyboard = millis();
  if ((curTime_keyboard - curTime_keyboard_before) >= 20)
  {
   curTime_keyboard_before= curTime_keyboard;

   #ifdef use_lib_keyboard_uart
    curTime_keyboard_uart= curTime_keyboard;
    if ((curTime_keyboard_uart - curTime_keyboard_before_uart) >= gb_current_ms_poll_keyboard_uart)
    {
     curTime_keyboard_before_uart = curTime_keyboard_uart;
     keyboard_uart_poll();
    
     if (checkKey_uart(KEY_CURSOR_LEFT)==1)
     {
      if (aReturn>10) aReturn-=10;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_CURSOR_RIGHT)==1)
     {
      if (aReturn<(aMax-10)) aReturn+=10;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
     }  
     if (checkKey_uart(KEY_CURSOR_UP)==1)
     {
      if (aReturn>0) aReturn--;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_CURSOR_DOWN)==1)
     {
      if (aReturn < (aMax-1)) aReturn++;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_ENTER)==1)
     {
      salir= 1;
     }
     if (checkKey_uart(KEY_ESC))
     {
      salir=1; aReturn= 255;    
     }
    }
   #endif


   //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
   //default: break;             
  }
 } 
 gb_show_osd_main_menu= 0;
 return aReturn;


 
}


//Sobra
     /* SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((contY+1)<<3),WHITE,BLACK,1);
      if (aReturn>0)
      {
       aReturn--;
       contY--;
      }
      if ((aReturn % gb_osd_max_rows == 0))
      {
       OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
       contY=gb_osd_max_rows;
      } 
      else
       SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((contY+1)<<3),CYAN,BLUE,1);
       */


/*      SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((contY+1)<<3),WHITE,BLACK,1);
      if (aReturn < (aMax-1))
      {
       aReturn++;
       contY++;
      }
      if ((aReturn!=0)&&(aReturn % gb_osd_max_rows == 0))
      {
       OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
       contY=0;
      }
      else 
       SDLprintText(gb_osd_sdl_surface,ptrValue[aReturn],gb_pos_x_menu,gb_pos_y_menu+((contY+1)<<3),CYAN,BLUE,1);
       */


//Menu DSK
//void ShowTinyDSKMenu()
//{
// unsigned char aSelNum;     
// aSelNum = ShowTinyMenu("DSK",gb_list_dsk_title,max_list_dsk);

 //gb_cartfilename= (char *)gb_list_rom_title[aSelNum];
// gb_force_load_dsk= 1;
// if (aSelNum > (max_list_dsk-1))
//  aSelNum= max_list_dsk-1;
// gb_id_cur_dsk= aSelNum;
 //running= 0;
//}

void ShowTinyCPUMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("CPU",gb_osd_cpu_title,max_gb_osd_cpu_title,-1);
 //0 AUTO 
 //1 FAST
 switch (aSelNum)
 {
  case 0: gb_cpu_fast=0; break;
  case 1: gb_cpu_fast=1; break;
 }
}

//Menu resetear
void ShowTinyResetMenu()
{
 unsigned char aSelNum;
 aSelNum= ShowTinyMenu("Reset",gb_reset_menu,max_gb_reset_menu,-1);   
 if (aSelNum==255)
 {
   return;
 }
 
 if (aSelNum == 1)
 {
  ESP.restart();
 }
 else
 {
   //gb_load_new_pfile=1;
   //gb_soft_reset=1;
   gb_reset= 1;
 } 
}


void ShowTinyCOMMenu()
{
 unsigned char aSelNum;     
 aSelNum = ShowTinyMenu("PRG",gb_list_prg_title,max_list_prg,-1);
 
 if (aSelNum!= 255)
 {
  gb_load_ptr= 1;
  gb_load_id_prg= aSelNum;
 }
 //sprintf(gb_load_ptr_name,"%s.prg",gb_list_prg_title[aSelNum]); 
 
 
//
// //gb_cartfilename= (char *)gb_list_rom_title[aSelNum];
// gb_force_load_com= 1;
// gb_id_cur_com= aSelNum;
 //running= 0;
}

//*******************************
//void ShowTinyLoadPJRMenu()
//{
// unsigned char aSelNum;     
//// aSelNum = ShowTinyMenu("PJR",gb_list_pjr_title,max_list_pjr);

// gb_force_load_pjr= 1;
// gb_id_cur_pjr= aSelNum;
//}

//*******************************
//void ShowTinyCPUDelay()
//{
// unsigned char aSelNum;     
// aSelNum = ShowTinyMenu("Delay CPU",gb_osd_cpu_delay,max_gb_osd_cpu_delay);
// switch (aSelNum)
// {
//  case 0: gb_cpu_delay= 0; break;
//  case 1: gb_cpu_delay= 20; break;
//  case 2: gb_cpu_delay= 100; break;
//  case 3: gb_cpu_delay= 200; break;
//  case 4: gb_cpu_delay= 500; break;
//  case 5: gb_cpu_delay= 1000; break;
// }
//}

//Menu ROM
//void ShowTinyMachineMenu()
//{
// unsigned char aSelNum;
// aSelNum = ShowTinyMenu("Machine",gb_machine_menu,max_gb_machine_menu);
// if (aSelNum<max_gb_machine_menu)
// {
//  gb_model_machine= aSelNum;
// }


// loadroms2FlashModel();
 //vTaskDelay(2);
// resetz80();
// resetcrtc();
// SDL_Delay(2);
//}

//************************
//void ShowTinyPaleta()
//{
     /*
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("Palette",gb_paleta_menu,max_gb_paleta_menu);
 switch (aSelNum)
 {
  case 0: gb_invert_color= ((~gb_invert_color)&0x01); break;
  case 1: InitPaletaCGA(); break;
  case 2: InitPaletaCGA2(); break;
  case 3: InitPaletaCGAgray(); break;
  case 4: InitPaletaPCJR(); break;
  case 5: InitPaletaGauntletPortada(); break; //Portada Gauntlet
  case 6: InitPaletaGauntletGame(); break; //Paleta Gauntlet Juego
 }
 */
//}


//Force Video mode
//void ShowTinyForceVideoMenu()
//{
     /*
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("Force Video Mode",gb_video_menu,max_gb_video_menu);
 switch (aSelNum)
 {
  case 0: 
   gb_force_set_cga = 1; //Video5
   break;
  case 1:
    //InitPaletaCGA160x200x16(); //Force 160x200x16
    gb_force_set_cga = 1; //160x100 Text
    gb_font_8x8= 0;
    gb_portramTiny[fast_tiny_port_0x3D8]=9;
    gb_portramTiny[fast_tiny_port_0x3D4]=9;
    vidmode=0x82;
    gb_force_160x100 = 1;
    break;
  case 2:
    gb_force_set_cga = 1; //No Force 160x100 Text
    gb_font_8x8= 0;
    gb_portramTiny[fast_tiny_port_0x3D8]=0;
    gb_portramTiny[fast_tiny_port_0x3D4]=0;
    vidmode=0x82;
    gb_force_160x100=0;
    break;
           
  case 3:
    InitPaletaCGA160x200x16(); //Force 160x200x16
    vidmode= 0x08;
    break;       
    
  case 4:
    vidmode= 0x04; //No Force 160x200x16
    break;                         

  case 5:
    InitPaletaCGA160x200x16(); //Misma paleta 320x200x16
    vidmode= 0x09; //Force 320x200x16
    break;                             

  case 6:
    vidmode= 0x04; //No Force 320x200x16
    break;                                 
    
  case 7: gb_force_160x100_8088mph=1; break;
  case 8: gb_force_160x100_8088mph=0; break;
 }
 */
//}


//Menu TAPE
/*void ShowTinyTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 load_tape2Flash(aSelNum);
}

//Play sound tape
void ShowTinySelectTAPEMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K TAPE",gb_list_tapes_48k_title,max_list_tape_48);
 Z80EmuSelectTape(aSelNum);
// gb_play_tape_current = aSelNum;
// gb_play_tape_on_now = 1;
}

//Menu SCREEN
void ShowTinySCRMenu()
{
 unsigned char aSelNum;
 aSelNum = ShowTinyMenu("48K SCREEN",gb_list_scr_48k_title,max_list_scr_48);
 load_scr2Flash(aSelNum);     
}
*/

//Menu velocidad emulador
//void ShowTinySpeedMenu()
//{
     /*
 unsigned char aSelNum,aSelNumSpeed;
 aSelNum = ShowTinyMenu("SPEED VIDEO AUDIO",gb_speed_videoaudio_options_menu,max_gb_speed_videoaudio_options_menu);
 if (aSelNum == 255)
  return;
 switch (aSelNum)
 {
  case 0: aSelNumSpeed= ShowTinyMenu("AUDIO Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
//   gb_current_ms_poll_sound= (aSelNumSpeed<<1); //Multiplico x2
   break;
  case 1: aSelNumSpeed= ShowTinyMenu("Video DELAY ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
//   gb_current_delay_emulate_ms = (aSelNumSpeed<<1);
   break;
//  case 2: gb_current_frame_crt_skip = ShowTinyMenu("Skip Frame",gb_value_binary_menu,max_gb_value_binary_menu);
//   break;
  case 3: aSelNumSpeed= ShowTinyMenu("Keyboard Poll ms",gb_speed_sound_menu,max_gb_speed_sound_menu);
   if (aSelNumSpeed == 255)
    return;
//   gb_current_ms_poll_keyboard= (aSelNumSpeed<<1);
   break;   
  default: break;
 }
 */
//}


//***********************************************
//void OSD_PreparaPaleta64para8Colores()
//{//8 tonalidades de color en Modo 64 colores
/* for(int i=0;i<64;i++)
 {
  gb_osd_sdl_palette[i].r=((i>>4)&0x03)*85;
  gb_osd_sdl_palette[i].g=((i>>2)&0x03)*85;
  gb_osd_sdl_palette[i].b=(i & 0x03)*85;
 } */ 
/* SDL_Color * auxPal= SDLCRTCGetPalette();
 for (int j=0;j<4;j++)
 {
  for (int i=0;i<64;i++)
  {
   auxPal[i+(j*64)].b = ((i>>4)&0x03)*85;
   auxPal[i+(j*64)].g = ((i>>2)&0x03)*85;
   auxPal[i+(j*64)].r = (i & 0x03)*85;
  }
 }*/
//}

//Test de 64 colores
//void OSD_Test64Colores()
//{
/* SDL_Color * auxPal = SDLCRTCGetPalette();
 // OSD_PreparaPaleta64para8Colores();
 //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, auxPal, 0, 256);
 for (int i=0;i<64;i++)
 {
  SDLprintText(gb_osd_sdl_surface," ",(i*2),30,i,i,1);  
 }
 SDL_Flip(gb_osd_sdl_surface);
 SDL_Delay(4000);
 //SDL_PollEvent(gb_osd_sdl_event); 
 //SDLEsperaTeclado();*/
//}


//***********************************************
//void OSD_PreparaPaleta8Colores()
//{
/*  SDL_Color * auxPal= SDLCRTCGetPalette();
  for (int i=0;i<256;i+=8)
  {
    auxPal[0+i].b = 0; auxPal[0+i].g = 0; auxPal[0+i].r= 0;
	auxPal[1+i].b = 0; auxPal[1+i].g = 0; auxPal[1+i].r= 255;
	auxPal[2+i].b = 0; auxPal[2+i].g = 255; auxPal[2+i].r= 0;
    auxPal[3+i].b = 0; auxPal[3+i].g = 255; auxPal[3+i].r= 255;
	auxPal[4+i].b = 255; auxPal[4+i].g = 0; auxPal[4+i].r= 0;
    auxPal[5+i].b = 255; auxPal[5+i].g = 0; auxPal[5+i].r= 255;
    auxPal[6+i].b = 255; auxPal[6+i].g = 255; auxPal[6+i].r= 0;
    auxPal[7+i].b = 255; auxPal[7+i].g = 255; auxPal[7+i].r= 255;
  }*/
//}

//Test de 8 colores
//void OSD_Test8Colores()
//{
/* SDL_Color * auxPal= SDLCRTCGetPalette();     
 //OSD_PreparaPaleta8Colores();
 //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, auxPal, 0, 256);
 for (int i=0;i<8;i++)
 {
  SDLprintText(gb_osd_sdl_surface," ",(i*2),30,i,i,1);  
 }
 SDL_Flip(gb_osd_sdl_surface);
 SDL_Delay(4000);
 //SDL_PollEvent(gb_osd_sdl_event); 
 //SDLEsperaTeclado();*/
//}

//Ajustar pantalla
/*
void ShowTinyScreenAdjustMenu()
{
 unsigned char aSelNum, auxCol; 
 aSelNum= ShowTinyMenu("Screen Adjust",gb_osd_screen,max_gb_osd_screen,-1);
 switch (aSelNum)
 {
  case 2:
   //OSD_PreparaPaleta8Colores();
   OSD_PreparaPaleta64para8Colores();
   //memcpy(gb_cache_zxcolor,gb_const_colornormal8,8);
   #ifdef use_lib_use_bright
    //memcpy(gb_cache_zxcolor_bright,gb_const_colornormal8_bright,8);
   #endif
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 3:
   OSD_PreparaPaleta64para8Colores();
   //memcpy(gb_cache_zxcolor,gb_const_monoBlue8,8);
   #ifdef use_lib_use_bright
    //memcpy(gb_cache_zxcolor_bright,gb_const_monoBlue8_bright,8);
   #endif 
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   //for (int k=0;k<256;k++)
   // printf("%d %d %d\n",gb_osd_sdl_palette[k].r,gb_osd_sdl_palette[k].g,gb_osd_sdl_palette[k].b);
   //for (int k=0;k<8;k++)
   // printf("%d ",gb_cache_zxcolor[k]);
   //printf("\n");    
   return;
   break;
  case 4:
   OSD_PreparaPaleta64para8Colores();   
   //memcpy(gb_cache_zxcolor,gb_const_monoGreen8,8);
   #ifdef use_lib_use_bright
    //memcpy(gb_cache_zxcolor_bright,gb_const_monoGreen8_bright,8);
   #endif 
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;
  case 5:
   OSD_PreparaPaleta64para8Colores();   
   //memcpy(gb_cache_zxcolor,gb_const_monoRed8,8);
   #ifdef use_lib_use_bright
    //memcpy(gb_cache_zxcolor_bright,gb_const_monoRed8_bright,8);
   #endif 
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
  case 6:
   OSD_PreparaPaleta64para8Colores();   
   //memcpy(gb_cache_zxcolor,gb_const_grey8,8);
   #ifdef use_lib_use_bright
    //memcpy(gb_cache_zxcolor_bright,gb_const_grey8_bright,8);
   #endif 
   //SDL_SetPalette(gb_osd_sdl_surface, SDL_LOGPAL|SDL_PHYSPAL, gb_osd_sdl_palette, 0, 256);
   return;
   break;          
 }
 auxCol= ShowTinyMenu("Pixels",gb_osd_screen_values,max_gb_osd_screen_values,-1);
 auxCol = auxCol<<1; //x2
// gb_screen_xOffset = auxCol;
// switch (aSelNum)
// {
//  case 0: //gb_screen_xIni = -(auxCol<<3); 
//   break;
//  case 1: //gb_screen_yIni = (auxCol<=3)? (-auxCol):(-3); 
//   break;  
//  default: break;
// }
}
*/

//#ifdef use_lib_amx_mouse
// void ShowTinyMouseMenu()
// {
//  unsigned char aSelNum;
//  aSelNum = ShowTinyMenu("Mouse Buttons",gb_osd_mouse_menu,max_gb_osd_mouse_menu);
//  switch (aSelNum)
//  {
 //  case 0: gb_force_left_handed= 0; break; //diestro
//   case 1: gb_force_left_handed= 1; break; //zurdo
//  }
//  SDL_Delay(2);
// }
//#endif

//*******************************************
void SDLActivarOSDMainMenu()
{     
 gb_show_osd_main_menu= 1;   
}

//Very small tiny osd
void do_tinyOSD() 
{
 unsigned char aSelNum;
 int auxVol;
 int auxFrec;
 Serial.printf("OSD %d\r\n",gb_show_osd_main_menu);
 //fflush(stdout);
 
 if (gb_show_osd_main_menu == 1)
 {
  //#ifdef use_lib_video_2bpp_direct_buffer
  // gb_videoMode_prev= vidmode;
  // vidmode= 199; //Modo especial para dibujar solo OSD sin buffer video
  //#endif 
                           
  auxVol= gb_volumen01;
  auxFrec= gb_frecuencia01;
  gb_volumen01= gb_frecuencia01=0;
  
  jj_sound_mute();
  
  aSelNum = ShowTinyMenu("MAIN MENU",gb_main_menu,max_gb_main_menu,-1);
  switch (aSelNum)
  {
   case 0: //ShowTinyMachineMenu(); 
    //gb_video_mode=0;
    ShowTinyCOMMenu();
    gb_show_osd_main_menu=0;
    break;
   case 1: //ShowTinyD64Menu(); 
    //gb_video_mode=2;
    //ShowTinyDSKMenu();
    gb_show_osd_main_menu=0;        
    break;
   case 2:
//    gb_reset= 1; 
//    write86(0x472,0x34);
//    write86(0x473,0x12);
//    SetRegCS(0xF000);
//    SetRegIP(0xFFF0);    
    //gb_reset= 1;
     ShowTinyResetMenu();
    break;
   case 3: 
    ShowTinyCPUMenu();
    gb_show_osd_main_menu=0; 
    break;
   case 4: 
    gb_quit= 1;
    gb_show_osd_main_menu=0;         
    break;
   case 5: //ShowTinyScreenAdjustMenu(); 
//    gb_font_8x8= 0; //font 4x8
//    InitPaletaCGA160x100();
    break;
   case 6: 
//    gb_font_8x8= 1; //font 8x8
//    InitPaletaCGA160x100();
    break;
   case 7:
//    gb_ram_truco_low= 0x80; //640 KB
//    gb_ram_truco_high= 0x02;
 //   bootstrapPoll();            
    break;   
   case 8: //OSD_Test8Colores(); 
    break;
   case 9: //OSD_Test64Colores(); 
    break;   
   
   case 10: //ShowTinyPaleta(); 
    break;
   
   case 11: 
    //running= 0; 
    //io_->retval_= false;
    gb_quit= 1;
    gb_show_osd_main_menu=0; 
    break;
   case 12: gb_show_osd_main_menu=0; break;
   case 13:
    gb_show_osd_main_menu=0;
    break;
   case 14:
    gb_show_osd_main_menu=0;
    break;
   case 15:
    gb_show_osd_main_menu=0;
    break;
   case 16:
    //ShowTinyForceVideoMenu();
    gb_show_osd_main_menu=0;
    break;
   case 17: //Force Machine
    //ShowTinyMachineMenu();
    gb_show_osd_main_menu=0;                
    break;
    
   case 18: //Load PJR cartdridge
    //ShowTinyLoadPJRMenu();
    gb_show_osd_main_menu=0;
    break;
   
   case 19:
    //ShowTinyCPUDelay();
    gb_show_osd_main_menu=0;
    break; 
                
   default: break;
  }
  
  gb_volumen01= auxVol;
  gb_frecuencia01= auxFrec;
  
  //#ifdef use_lib_video_2bpp_direct_buffer
  // vidmode= gb_videoMode_prev; //restablecemos el video
  //#endif   
  
  jj_sound_no_mute();
 }
}

