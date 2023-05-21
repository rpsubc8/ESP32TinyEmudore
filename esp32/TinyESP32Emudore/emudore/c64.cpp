// Mod by ackerman (convert C, refactor, optimice)
//
//  emudore, Commodore 64 emulator
//  Copyright (c) 2016, Mario Ballano <mballano@gmail.com>
// 
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//  http://www.apache.org/licenses/LICENSE-2.0
// 
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "gbConfig.h" 
#include "c64.h"
#include "gbGlobals.h"
#include "Arduino.h"
#include "dataFlash/gbprg.h"

//C64::C64()
void C64_constructor()
{
  Serial.printf("Constructor C64\r\n");
  //fflush(stdout);
  // create chips
  //sid_  = new Sid(); //Lo instancio primero para que el resto pueda escribir
  jj_SID_Init(); //C lo seteo
  //C64_cpu_  = new Cpu(); //No objetos
  //C64_mem_  = new Memory(); //No quiero objetos
  MEMORY_constructor();
  
  //cia1_ = new Cia1(); //No quiero objetos
  CIA1_constructor();
  
  //cia2_ = new Cia2(); //No quiero objetos
  CIA2_constructor();
  
  //C64_vic_  = new Vic();
  Vic_constructor(); //No quiero objetos
  //io_   = new IO(); //no quiero objetos
  IO_constructor();
  // init cpu
  //cpu_->memory(mem_); //No quiero objetos
  //cpu_->reset();
  //C64_cpu_->CPU_memory(C64_mem_); //No objetos
  //C64_cpu_->CPU_reset();
  //CPU_memory(C64_mem_); //No objetos
  CPU_reset();  
  
  // init vic-ii
  //C64_vic_->VIC_memory(C64_mem_);
  //VIC_memory(C64_mem_); //No quiero objetos
  
  //C64_vic_->VIC_cpu(C64_cpu_); //No objetos

  //vic_->io(io_); //no quiero objetos
  // init cia1
  //cia1_->CIA1_cpu(cpu_); //No quiero objetos
  //CIA1_cpu(C64_cpu_); //No objetos
  
  //cia1_->io(io_); //no quiero objetos
  // init cia2
  //cia2_->CIA2_cpu(cpu_); //No quiero objetos
  //CIA2_cpu(C64_cpu_); //No objetos
  
  // init io
  //io_->IO_cpu(cpu_); //no quiero objetos
  //IO_cpu(C64_cpu_); //no quiero objetos
  
  
  // DMA
  //C64_mem_->MEMORY_vic(C64_vic_);
  //MEMORY_vic(C64_vic_); //No quiero objetos
  
  //mem_->MEMORY_cia1(cia1_); //No quiero objetos
  //mem_->MEMORY_cia2(cia2_); //No quiero objetos
 // r2 support
//#ifdef DEBUGGER_SUPPORT
//  debugger_ = new Debugger();
//  debugger_->memory(mem_);
//  debugger_->cpu(cpu_);
//#endif   

 //gb_ptr_frame= io_->IO_frame_; //Capturamos el puntero a frame
 //gb_cols= io_->IO_cols_; //Fuera objetos
// gb_ptr_frame= IO_frame_; //Fuera objetos //No lo necesitamos
 gb_cols= IO_cols_;
}

//C64::~C64()
void C64_destroy()
{
  //delete C64_cpu_; //No objetos
  //delete C64_mem_; 
  MEMORY_destroy(); //No objetos
  
  //delete cia1_; //No quiero objetos
  //delete cia2_; //No quiero objetos
  //delete C64_vic_; //No quiero objetos
//  delete sid_; //Ya no hay C++, es C
  //delete io_; //no quiero objetos
  IO_destroy();
//#ifdef DEBUGGER_SUPPORT
//  delete debugger_;
//#endif   
}



//void C64::C64_leeme_prg_hack(const char *cadFich)
//void C64_leeme_prg_hack(const char *cadFich)
void C64_leeme_prg_hack(unsigned short int id)
{
  unsigned short int kBasicTxtTab = 0x002b;
  unsigned short int kBasicPrgStart = 0x0801;
  unsigned short int kBasicVarTab = 0x002d;
  unsigned short int kBasicAryTab = 0x002F;
  unsigned short int kBasicStrEnd = 0x0031;
  //unsigned char b; //No se usa
  unsigned short int pbuf, addr;
  unsigned char a0,a1;
  unsigned int cont=0;
  
  //fprintf(gb_fichLog,"Apertura hack prg %s\n",cadFich);
  //fflush(gb_fichLog);     

  a0= gb_list_prg_data[id][cont++];
  a1= gb_list_prg_data[id][cont++];
  addr= ((unsigned short int)a1<<8)|a0;
  pbuf = addr = addr;
  unsigned int lon= gb_list_prg_size[id];
  Serial.printf("Loader::leeme_prg_hack prg %s len:%d addr:%04X\r\n",gb_list_prg_title[id],lon,addr);
  memcpy(&MEMORY_mem_ram_[pbuf++], &gb_list_prg_data[id][cont], lon);
  cont= cont + (lon-2);
  pbuf= pbuf +(lon-2);

  if(addr == kBasicPrgStart) //if(addr == 0x0801)   
  {
   Serial.printf("kBasicPrgStart Teclea RUN\r\n");
   MEMORY_write_word_no_io(kBasicTxtTab,kBasicPrgStart); //No objetos
   MEMORY_write_word_no_io(kBasicVarTab,pbuf);
   MEMORY_write_word_no_io(kBasicAryTab,pbuf);
   MEMORY_write_word_no_io(kBasicStrEnd,pbuf);          
  }
  else
  {
   Serial.printf("ML\r\n");
   //fflush(stdout);        
   //cpu_->pc(addr); //no quiero objetos
   //C64_cpu_->CPU_pc(addr); //No objetos
   CPU_pc_= addr; //CPU_pc_set(addr)
  }


/*
  FILE * fich = fopen(cadFich,"rb");
  if (fich!=NULL)
  {
   fseek(fich, 0L, SEEK_END);
   unsigned int lon= ftell(fich);
  
   fseek(fich, 0L, SEEK_SET);
   fread(&a0,1,1,fich);
   cont++;
   fread(&a1,1,1,fich);
   cont++;
   addr= ((unsigned short int)a1<<8)|a0;
          
   pbuf = addr = addr;
  
   Serial.printf("Loader::leeme_prg_hack prg %s len:%d addr:%lu\r\n",cadFich,lon,addr);
   //fflush(stdout);     
  
   while (cont<lon)
   {
    fread(&b,1,1,fich);
    //C64_mem_->MEMORY_write_byte_no_io(pbuf++,b);
    MEMORY_mem_ram_[pbuf++]= b; //MEMORY_write_byte_no_io(pbuf++,b); //No objetos
    cont++;
    //fprintf(gb_fichLog,"%02X",b);
    //if ((cont%16)==0)
    // fprintf(gb_fichLog,"\n");
   }   

   if(addr == kBasicPrgStart) //if(addr == 0x0801)   
   {
    printf("kBasicPrgStart Teclea RUN\n");
    //key_event_queue_.push
      // make BASIC happy
      //C64_mem_->MEMORY_write_word_no_io(kBasicTxtTab,kBasicPrgStart);
      //C64_mem_->MEMORY_write_word_no_io(kBasicVarTab,pbuf);
      //C64_mem_->MEMORY_write_word_no_io(kBasicAryTab,pbuf);
      //C64_mem_->MEMORY_write_word_no_io(kBasicStrEnd,pbuf);
      MEMORY_write_word_no_io(kBasicTxtTab,kBasicPrgStart); //No objetos
      MEMORY_write_word_no_io(kBasicVarTab,pbuf);
      MEMORY_write_word_no_io(kBasicAryTab,pbuf);
      MEMORY_write_word_no_io(kBasicStrEnd,pbuf);      
      // exec RUN
//JJ      for(char &c: std::string("RUN\n"))
//JJ      {
//JJ        io_->IO::type_character(c);
//JJ      }

    }
    // ML
    else
    {
      Serial.printf("ML\r\n");
      //fflush(stdout);        
      //cpu_->pc(addr); //no quiero objetos
      //C64_cpu_->CPU_pc(addr); //No objetos
      CPU_pc_= addr; //CPU_pc_set(addr)
    }
   

  
  fclose(fich);
 }
 */

}


void ShowFPSCall()
{       
 #ifdef use_lib_log_fps
  Serial.printf("FPS %d\r\n",gb_fps_cur);
  
  #ifdef use_lib_cont_call
   Serial.printf(" %d \r\n",gb_cont_call);
   //fflush(stdout);
   gb_cont_call=0; 
  #endif       
  
  //Serial.printf("\r\n");
  //fflush(stdout);
 #endif 
 //fprintf(gb_fichLog,"%ld %ld %ld FPS %d\n",gb_fps_time_last,gb_fps_time_cur,auxTime,gb_fps_cur);
 //fflush(gb_fichLog); 
 gb_fps_cur=0;
 //242  244 232 223 fps   32 fps cuando SDL_Delay(20) 32 MB de ram    
}


//void C64::C64_start()
void C64_start()
{     
  unsigned int auxTime;
  // main emulator loop
  while(true)
  {
       
   //Medir FPS BEGIN
   gb_fps_time_cur= millis();
   auxTime= (gb_fps_time_cur - gb_fps_time_last);
   if (auxTime >= 999)
   {
    gb_fps_time_last= gb_fps_time_cur;
    ShowFPSCall();    
   }             
   //Medir FPS END
   
   //Volcado vga_dump_poll VGA BEGIN No se necesita
   //gb_vga_time_cur= millis();
   //auxTime= (gb_vga_time_cur - gb_vga_time_last);
   //if (auxTime >= 19)
   //{
   // gb_vga_time_last= gb_vga_time_cur;
   // vga_dump_poll();
   //}   
   //Volcado vga_dump_poll VGA END
   
             
             
   if (gb_inicia_espera==1)
   {
                           
    //vga_dump_poll begin
    //gb_vga_time_cur= SDL_GetTicks();
    //unsigned int auxTime= (gb_vga_time_cur - gb_vga_time_last);
    //if (auxTime >= 19)
    //{
    // gb_vga_time_last= gb_vga_time_cur;
    // vga_dump_poll();
    //}
    //vga_dump_poll end

    //CPU espera BEGIN                               
    unsigned int time_espera_cur= millis();
    auxTime= time_espera_cur - gb_time_espera_ini;
    //unsigned char espera= ((auxTime <= 19)?1:0);
    unsigned char espera= ((auxTime <= gb_time_tiene_que_esperar)?1:0);    
    //printf("vsync %u %u %u\n",time_espera_cur,gb_inicia_espera,auxTime);
    //fflush(stdout);    
    if ((espera==1)&&(gb_cpu_fast==0))
    {     
     //printf("continue\n");
     //fflush(stdout);
     continue;
     //gb_cpu_emu_execute= 0;
    }
    else
    {
     gb_inicia_espera= 0;
     //gb_cpu_emu_execute= 1;
     //printf("gb_inicia_espera\n");
    }
   }//fin gb_inicia_espera 
   //CPU espera END
             
   if (gb_quit==1)
   {
    //io_->IO_retval_= false;
    IO_retval_= false; //Fuera objetos
   }
   
   if (gb_reset==1)
   {
    Serial.printf("Estoy en gb_reset C64_start\r\n");
    //fflush(stdout);
    //gb_reset=0;    
    //C64_mem_->MEMORY_JJ_reset();
    MEMORY_JJ_reset(); //No objetos
    //C64_cpu_->CPU_reset(); //No objetos
    CPU_reset();
    jj_sid_mute();
    break; //Salimos  del while y volvemos a cargar nodo desde el main
   }
             
    if (gb_load_ptr == 1)
    {//Hack JJ
     gb_load_ptr= 0;
     //cpu_->pc(2049);
     
     //leeme_prg_hack("monopole.prg");     
     //C64_leeme_prg_hack(gb_load_ptr_name);
     C64_leeme_prg_hack(gb_load_id_prg);
     
     //leeme_prg_hack("guerrero.prg"); //no va
     //leeme_prg_hack("v:\\multi.prg");
     //leeme_prg_hack("v:\\BMP1.prg");
     //leeme_prg_hack("v:\\iLKke.prg");
     //leeme_prg_hack("v:\\onstellthefuhrer.prg");
     //leeme_prg_hack("v:\\WickedSoftware.prg");
     //leeme_prg_hack("v:\\3DPinball.prg");
    }
             
//#ifdef DEBUGGER_SUPPORT
//    if(!debugger_->emulate())
//      break;
//#endif

    // CIA1
    //if(!cia1_->CIA1_emulate())
    //if(!CIA1_emulate()) //No quiero objetos
    //  break;
    //if (gb_cpu_emu_execute == 1)
    {
     if ((CIA1_timer_a_enabled_==true)||(CIA1_timer_b_enabled_==true))
     {//Solo entra si tiene timers activos     
      CIA1_emulate(); //no requiere controlar return
     }
     else
     {
      CIA1_prev_cpu_cycles_ = CPU_cycles_;
     }
    }
    
    // CIA2
    //if(!cia2_->CIA2_emulate()) //No quiero objetos
    //if(!CIA2_emulate())
    //  break;

    //if (gb_cpu_emu_execute == 1)
    {
     if ((CIA2_timer_a_enabled_==true)||(CIA2_timer_b_enabled_==true))
     {//Solo entra si tiene timers activos
      CIA2_emulate(); //no requiere controlar return
     }
     else
     {
      CIA2_prev_cpu_cycles_ = CPU_cycles_;
     }
    }
    
    // CPU 
    //if(!C64_cpu_->CPU_emulate()) //No objetos
    //if (gb_cpu_emu_execute == 1)
    //{
     //if(!CPU_emulate())
     CPU_emulate();
     if (CPU_retval==0)
       break;
    //}  
    // VIC-II
    //if(!C64_vic_->VIC_emulate()) //no quiero objetos
    //if (gb_cpu_emu_execute == 1)
    {
     if(!VIC_emulate())
       break;
    }
    // IO
    //if(!io_->IO_emulate())
    //  break;
    //if(!IO_emulate())
    if (!IO_retval_)
    {//no quiero objetos
      break;    
    }
    // callback
    //JJ if(callback_ && !callback_())
    //JJ  break;
   
    //SID BEGIN 
    gb_sid_time_cur = millis();
    auxTime= (gb_sid_time_cur - gb_sid_time_last);
    if (auxTime>=19)
    {
     gb_sid_time_last= gb_sid_time_cur;
     sid_poll();    
    }
    //SID END

    //vga_dump_poll();
    //vga_dump_poll begin
    //gb_vga_time_cur= SDL_GetTicks();
    //auxTime= (gb_vga_time_cur - gb_vga_time_last);
    //if (auxTime >= 19)
    //{
    // gb_vga_time_last= gb_vga_time_cur;
    // vga_dump_poll();
    //}    
    //vga_dump_poll end
  }
  
  //vga_dump_poll();
  //vga_dump_poll begin
  //gb_vga_time_cur= SDL_GetTicks();
  //unsigned int auxTime= (gb_vga_time_cur - gb_vga_time_last);
  //if (auxTime >= 19)
  //{
  // gb_vga_time_last= gb_vga_time_cur;
  // vga_dump_poll();
  //}  
  //vga_dump_poll end
  
}

//
//  @brief emscripten's main loop
//
//void C64::C64_emscripten_loop()
//void C64_emscripten_loop()
//{
//  //unsigned int frame = C64_vic_->VIC_frames();
//  unsigned int frame = VIC_frames(); //No quiero objetos
//  //while(frame == C64_vic_->VIC_frames())
//  while(frame == VIC_frames())
//  {
//    // CIA1
//    //cia1_->CIA1_emulate();
//    CIA1_emulate(); //No quiero objetos
//    // CIA2
//    //cia2_->CIA2_emulate(); //No quiero objetos
//    CIA2_emulate();
//    // CPU
//    //C64_cpu_->CPU_emulate(); //No objetos
//    CPU_emulate();
//    // VIC-II
//    //C64_vic_->VIC_emulate();
//    VIC_emulate(); //No quiero objetos
//    // IO 
//    //io_->IO_emulate(); //no quiero objetos
//    IO_emulate();
//    // callback
//    //JJ if(callback_) callback_();
//  }
//}
 
//
// @brief runs Klaus Dormann's 6502 test suite 
//
// https://github.com/Klaus2m5/6502_65C02_functional_tests
//
//void C64::C64_test_cpu()
/*Sobra no se necesita
void C64_test_cpu()
{
  unsigned short int pc=0;
  // unmap C64 ROMs
  //mem_->write_byte(Memory::kAddrMemoryLayout, 0);
  //C64_mem_->MEMORY_write_byte(MEMORY_kAddrMemoryLayout, 0);
  MEMORY_write_byte(MEMORY_kAddrMemoryLayout, 0); //No objetos
  // load tests into RAM
  //JJ mem_->load_ram("tests/6502_functional_test.bin",0x400);
  //C64_mem_->MEMORY_load_ram("6502_functional_test.bin",0x400);
  MEMORY_load_ram("6502_functional_test.bin",0x400); //No objetos
  //C64_cpu_->CPU_pc(0x400); //No objetos
  CPU_pc_= 0x400; //CPU_pc_set(0x400)
  while(true)
  {
    //if(pc == C64_cpu_->CPU_pc()) //No objetos
    //if(pc == CPU_pc_get())
    if(pc == CPU_pc_)
    {
      //D("infinite loop at %x\n",pc);
      Serial.printf("infinite loop at %x\r\n",pc);
      //fflush(stdout);
      break;
    }
    else
    {
     //if(CPU_pc_get() == 0x3463) //No objetos else if(C64_cpu_->CPU_pc() == 0x3463)
     if(CPU_pc_ == 0x3463) //No objetos else if(C64_cpu_->CPU_pc() == 0x3463)
     {
      //D("test passed!\n");
      printf("test passed!\n");
      break;
     }
    }
    //pc = C64_cpu_->CPU_pc();
    pc = CPU_pc_; //pc = CPU_pc_get() //No objetos
    //if(!C64_cpu_->CPU_emulate()) //No objetos
    //if(!CPU_emulate())
    CPU_emulate();
    if (CPU_retval==0)
      break;
  }
}
*/



//Cpu * C64::C64_cpu() //No objetos
//{
// return C64_cpu_;
//};

//Memory * C64::C64_memory() //No quiero objetos
//{
// return C64_mem_;
//};


//***********************
void sid_poll()
{
 //gb_sid_time_cur = SDL_GetTicks();
 //unsigned int auxTime= (gb_sid_time_cur - gb_sid_time_last);
 //if (auxTime>=19){
 // gb_sid_time_last= gb_sid_time_cur;
  
  for (unsigned char i=0;i<3;i++)
  {
   if (gbFrecMixer_now_poll[i] != gbFrecMixer_prev[i])
   {//Cambia la frecuencia
    gbFrecMixer_prev[i]= gbFrecMixer_now[i];
   
    gbVolMixer_now[i]= gbVolMixer_now_poll[i];
    gbFrecMixer_now[i]= gbFrecMixer_now_poll[i];
    
    gb_ex_contPulse[i]= (gbFrecMixer_now[i]>0) ? (unsigned int)((SAMPLE_RATE / gbFrecMixer_now[i]))>>1 : 0;
        
   }   
  }
  
  
  //for (unsigned char i=0;i<3;i++){
  // gbVolMixer_now[i]= gbVolMixer_now_poll[i];
  // gbFrecMixer_now[i]= gbFrecMixer_now_poll[i];
  //}
  
  //for (unsigned int i=0;i<3;i++){
  // gb_ex_contPulse[i]= (gbFrecMixer_now[i]>0) ? (unsigned int)((44100 / gbFrecMixer_now[i]))>>1 : 0;
  //}  
        
  
  #ifdef use_lib_log_sid
   unsigned char auxVol= MEMORY_read_byte(0xD418);
   Serial.printf("SID v:%02X %d %d %d Vol(D418):0x%02X\r\n",gbVolMixer_now_poll[0],gbFrecMixer_now_poll[0],gbFrecMixer_now_poll[1],gbFrecMixer_now_poll[2],auxVol);
   //fflush(stdout);
  #endif 
// }
}

//*************************************************
void vga_dump_poll()
{
/* No lo necesitamos  
  Uint8 *p;
  //gb_vga_time_cur= SDL_GetTicks();
  //unsigned int auxTime= (gb_vga_time_cur - gb_vga_time_last);
  //if (auxTime >= 19)
  //{
  // gb_vga_time_last= gb_vga_time_cur;
    
    
   p = (Uint8 *)screen->pixels;
   unsigned int auxLine = screen->pitch;
   //unsigned int auxDest= 0;
   for (unsigned int y=0;y<284;y++)
   {    
    //memcpy(p,&frame_[y * cols_],403);
    
    //memcpy(p,&gb_ptr_frame[auxDest],403);
    memcpy(p,gb_buffer_vga[y],403);
    
    //auxDest+= gb_cols;
    //*p+= (y*auxLine);
    p+= auxLine;
   }
     
   SDL_Flip (screen);
   //SDL_Delay (1);
  //}      
  
 //#ifdef use_lib_cont_call
 // printf("%d\n",gb_cont_call);
 // fflush(stdout);
 // gb_cont_call=0; 
 //#endif 
 */
}


