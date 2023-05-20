//Port by ESP32 by ackerman
//Mod by ackerman (convert C, refactor, optimice)
//License: WTFPL
//Load PRG
//VGA MODE400x300
//VGA Tiny library bitluni 0.3.3 mod convert C (Ricardo Massaro)
//Arduino IDE 1.8.11 Espressif System 1.0.6
//
// emudore, Commodore 64 emulator
// Copyright (c) 2016, Mario Ballano <mballano@gmail.com>

#include "gbConfig.h"
#include <stdio.h> 
#include <iostream>
#include <string>
#include <algorithm>
#include "gbGlobals.h"
#include "osd.h"

#include "c64.h"
#include "loader.h"
#include "dataFlash/gbprg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <SDL/SDL.h>
//#include <windows.h>
#include "cpu.h"
#include "gbrom.h"

#include "hardware.h"
#include "driver/timer.h"
#include "soc/timer_group_struct.h"


#include "gbsdlfont6x8.h"
const unsigned char *gb_sdl_font;

#include <Arduino.h>

#ifdef use_lib_tinybitluni_fast
 #include "vga_6bit.h"
 // VGA output pins
 //#define PIN_RED_LOW    21
 //#define PIN_RED_HIGH   22
 //#define PIN_GREEN_LOW  18
 //#define PIN_GREEN_HIGH 19
 //#define PIN_BLUE_LOW   4
 //#define PIN_BLUE_HIGH  5
 //#define PIN_HSYNC      23
 //#define PIN_VSYNC      15

 #ifdef use_lib_vga8colors
  //DAC 3 bits 8 colores
  // 3 bit pins  
  static const unsigned char pin_config[] = {  
   PIN_RED_HIGH,
   PIN_GREEN_HIGH,  
   PIN_BLUE_HIGH,
   255,
   255,
   255,
   PIN_HSYNC,
   PIN_VSYNC
  };
 #else 
  static const unsigned char pin_config[] = {
   PIN_RED_LOW,
   PIN_RED_HIGH,
   PIN_GREEN_LOW,
   PIN_GREEN_HIGH,
   PIN_BLUE_LOW,
   PIN_BLUE_HIGH,
   PIN_HSYNC,
   PIN_VSYNC
  };
 #endif 

#endif

unsigned short int gb_load_id_prg=0;

unsigned char gb_sync_bits;
unsigned char gb_memory64KB[0x10000];//64 KB en SRAM0


const int SAMPLE_RATE=44100;
unsigned int gb_sid_time_cur=0;
unsigned int gb_sid_time_last=0;
volatile unsigned char gbVolMixer_now_poll[3]={0,0,0}; //poll 20 ms 50 fps
volatile unsigned short int gbFrecMixer_now_poll[3]={0,0,0};
volatile unsigned char gbVolMixer_now[3]={0,0,0};
volatile unsigned short int gbFrecMixer_now[3]={0,0,0};
volatile int gb_frecuencia01=0;
volatile int gb_dutty_ch01=0;
volatile int gb_volumen01=0;
volatile int gb_duracion01=0;

volatile int gb_frecuencia02=0;
volatile int gb_dutty_ch02=0;
volatile int gb_volumen02=0;
volatile int gb_duracion02=0;

volatile int gb_frecuencia03=0;
volatile int gb_volumen03=0;

 volatile unsigned int gb_ex_contPulse[3]; 



unsigned int gb_cpu_time_cur=0;
unsigned int gb_cpu_time_ini=0;
unsigned int gb_cpu_time_last=0;

unsigned char gb_cpu_emu_execute=1; //START STOP emulacion CPU

//IO_H BEGIN
unsigned char IO_keyboard_matrix_[8];
unsigned short int IO_rows_;
unsigned short int IO_cols_;
//unsigned char *IO_frame_; //No lo necesitamos
bool IO_retval_;
//Cpu * IO_cpu_; //No objetos
//IO_H END

//LOADER_H BEGIN
bool LOADER_booted_up_;
std::ifstream LOADER_is_;
unsigned char LOADER_format_; //No quiero enums
//Cpu *LOADER_cpu_; //No objetos
//Memory *LOADER_mem_; //No objetos
//C64 *LOADER_c64_; //No objetos
//LOADER_H END

//CIA1_H BEGIN
//Cpu *CIA1_cpu_; //No objetos
short int CIA1_timer_a_latch_;
short int CIA1_timer_b_latch_;
short int CIA1_timer_a_counter_;
short int CIA1_timer_b_counter_;    
bool CIA1_timer_a_enabled_;
bool CIA1_timer_b_enabled_;
bool CIA1_timer_a_irq_enabled_;
bool CIA1_timer_b_irq_enabled_;
bool CIA1_timer_a_irq_triggered_;
bool CIA1_timer_b_irq_triggered_;
unsigned char CIA1_timer_a_run_mode_;
unsigned char CIA1_timer_b_run_mode_;
unsigned char CIA1_timer_a_input_mode_;
unsigned char CIA1_timer_b_input_mode_;    
unsigned int CIA1_prev_cpu_cycles_; 
unsigned char CIA1_pra_, CIA1_prb_;
//CIA1_H END

//CIA2_H BEGIN
//Cpu *CIA2_cpu_; //No objetos
short int CIA2_timer_a_latch_;
short int CIA2_timer_b_latch_;
short int CIA2_timer_a_counter_;
short int CIA2_timer_b_counter_;
bool CIA2_timer_a_enabled_;
bool CIA2_timer_b_enabled_;
bool CIA2_timer_a_irq_enabled_;
bool CIA2_timer_b_irq_enabled_;
bool CIA2_timer_a_irq_triggered_;
bool CIA2_timer_b_irq_triggered_;
unsigned char CIA2_timer_a_run_mode_;
unsigned char CIA2_timer_b_run_mode_;
unsigned char CIA2_timer_a_input_mode_;
unsigned char CIA2_timer_b_input_mode_;
unsigned int CIA2_prev_cpu_cycles_;
unsigned char CIA2_pra_, CIA2_prb_;        
//CIA2_H END  


//CPU_H BEGIN
unsigned short int CPU_pc_;
unsigned char CPU_sp_, CPU_a_, CPU_x_, CPU_y_;
bool CPU_cf_,CPU_zf_,CPU_idf_,CPU_dmf_,CPU_bcf_,CPU_of_,CPU_nf_;
//memory and clock
//Memory *CPU_mem_; //No objetos
unsigned int CPU_cycles_;
unsigned char CPU_retval=1; //TRUE
//CPU_H END

//VIC_H BEGIN
//Memory *VIC_mem_; //No objetos
unsigned char VIC_mx_[8];
unsigned char VIC_my_[8];
unsigned char VIC_msbx_;
unsigned char VIC_sprite_enabled_;
unsigned char VIC_sprite_priority_;
unsigned char VIC_sprite_multicolor_;
unsigned char VIC_sprite_double_width_;
unsigned char VIC_sprite_double_height_;
unsigned char VIC_sprite_shared_colors_[2];
unsigned char VIC_sprite_colors_[8];
//background and border colors
unsigned char VIC_border_color_;
unsigned char VIC_bgcolor_[4];
//cpu sync
unsigned int VIC_next_raster_at_;
//frame counter
unsigned int VIC_frame_c_;
//control registers
unsigned char VIC_cr1_;
unsigned char VIC_cr2_;
//raster
unsigned char VIC_raster_c_;
int VIC_raster_irq_;
//interrupt control
unsigned char VIC_irq_status_;
unsigned char VIC_irq_enabled_;
//screen, character memory and bitmap addresses
unsigned short int VIC_screen_mem_;
unsigned short int VIC_char_mem_;
unsigned short int VIC_bitmap_mem_;
unsigned char VIC_mem_pointers_;

unsigned char VIC_graphic_mode_;
//VIC_H END

//MEMORY_H BEGIN
unsigned char *MEMORY_mem_ram_;
//unsigned char *MEMORY_mem_rom_;
unsigned char MEMORY_banks_[7];
//MEMORY_H END 


//C64_H BEGIN
//C64_H END


unsigned char gb_cpu_fast=0;
volatile unsigned int gbVolMixer_prev[3]={0,0,0};
volatile unsigned int gbFrecMixer_prev[3]={0,0,0};

unsigned char gb_sid_volume[3]={0,0,0};
unsigned char gb_sid_freqLo[3]={0,0,0};
unsigned char gb_sid_freqHi[3]={0,0,0};
unsigned int gb_sid_frequency[3]={0,0,0};


unsigned char ** gb_buffer_vga;
unsigned int **gb_buffer_vga32;
unsigned char gb_quit=0;
unsigned char gb_reset=0;

unsigned int gb_vga_time_last=0;
unsigned int gb_vga_time_cur=0;
unsigned short int gb_cont_rgb=0;
unsigned int gb_keyboard_time_cur=0;
unsigned int gb_keyboard_time_last=0;
unsigned int gb_vsync_cur=0;
unsigned int gb_vsync_last=0; 

unsigned int gb_fps_cur=0;
unsigned int gb_fps_last=0;
unsigned int gb_fps_min=999999;
unsigned int gb_fps_max=0;
unsigned int gb_fps_med=0;
unsigned int gb_fps_time_cur=0;
unsigned int gb_fps_time_last=0;

const unsigned char *gb_basicRomC64;
const unsigned char *gb_charRomC64;
const unsigned char *gb_kernalRomC64;
unsigned char gb_load_ptr=0;
char gb_load_ptr_name[256];

unsigned int gb_cols;
unsigned char gb_inicia_espera=0;
unsigned int gb_time_espera_ini=0;
unsigned int gb_time_tiene_que_esperar=0;


unsigned char gb_color_vga[256];


void LeerFicherosROMS(void);
void sound_cycle(void);
void SetupMemory(void);
//void Setup(void);
void PreparaColorVGA(void);


void PreparaColorVGA()
{
 #ifdef use_lib_tinybitluni_fast
  #ifdef use_lib_vga8colors
   for (unsigned char i=0;i<8;i++)
   {  
    gb_color_vga[i] = (gb_color_vga[i] & 0x07) | gb_sync_bits;  
   }  
  #else
   for (unsigned char i=0;i<64;i++)
   {  
    gb_color_vga[i] = gb_color_vga[i] | gb_sync_bits;  
   }
  #endif 
 #else
  //#ifdef use_lib_tinybitluni_fast  
  // for (unsigned char i=0;i<8;i++)
  // {  
  //  gb_color_vga[i] =  (gb_color_vga[i] & 0x07) | gb_sync_bits;
  //  //gb_color_vga[i] = (gb_color_vga[i] & vga.RGBAXMask) | vga.SBits;  
  // }
  //#endif
 #endif 
}



//**********************************
void LeerFicherosROMS()
{ 
 //LeerFichero("basic.901226-01.bin",gb_basicRomC64);
 //LeerFichero("characters.901225-01.bin",gb_charRomC64);
 //LeerFichero("kernal.901227-03.bin",gb_kernalRomC64);
 gb_basicRomC64= gb_rom_basic_901226_01;
 gb_charRomC64= gb_rom_chararacteres_901225_01;
 gb_kernalRomC64= gb_rom_kernal_901227_03;
}


bool loader_cb()
{
 Serial.printf("loader_cb\r\n");
 //fflush(stdout); 
      
  //JJ if(!loader->emulate())
  //JJ   c64->callback(nullptr);
  
  //loader->emulate(); //JJ Hack 
  
  return true;
}

void SetRGBSDL(unsigned char r,unsigned char g,unsigned char b)
{
 unsigned char auxR,auxG,auxB;
 unsigned char aux;
 auxR= (r>>6)&0x03; //2 bits Paleta 64 colores TTGO VGA ESP32
 auxG= (g>>6)&0x03;
 auxB= (b>>6)&0x03;

 aux= (auxB<<4)|(auxG<<2)|(auxR); 

 gb_color_vga[gb_cont_rgb]= aux;

 gb_cont_rgb++;
 if (gb_cont_rgb>255)
  gb_cont_rgb=255;   
}

void PreparaPaleta()
{
 Serial.printf("PreparaPaleta\r\n"); 

 gb_cont_rgb=0;

 memset(gb_color_vga,0,sizeof(gb_color_vga));

 SetRGBSDL(0x00, 0x00, 0x00);
 SetRGBSDL(0xff, 0xff, 0xff);
 SetRGBSDL(0xab, 0x31, 0x26);
 SetRGBSDL(0x66, 0xda, 0xff);
 SetRGBSDL(0xbb, 0x3f, 0xb8);
 SetRGBSDL(0x55, 0xce, 0x58);
 SetRGBSDL(0x1d, 0x0e, 0x97);
 SetRGBSDL(0xea, 0xf5, 0x7c);
 SetRGBSDL(0xb9, 0x74, 0x18);
 SetRGBSDL(0x78, 0x53, 0x00);
 SetRGBSDL(0xdd, 0x93, 0x87);
 SetRGBSDL(0x5b, 0x5b, 0x5b);
 SetRGBSDL(0x8b, 0x8b, 0x8b);
 SetRGBSDL(0xb0, 0xf4, 0xac);
 SetRGBSDL(0xaa, 0x9d, 0xef);
 SetRGBSDL(0xb8, 0xb8, 0xb8); 
}


void SetupMemory()
{
  //MEMORY_mem_ram_ = (unsigned char *)malloc(MEMORY_kMemSize);
  MEMORY_mem_ram_ = gb_memory64KB;

  //MEMORY_mem_rom_ = (unsigned char *)malloc(MEMORY_kMemSize);
  
  IO_cols_= 403;
  IO_rows_= 284;
//  IO_frame_  = (unsigned char *)malloc(IO_cols_ * IO_rows_); //No lo necesitamos
}





#ifdef use_lib_keyboard_uart
 char gb_buf_uart[BUFFER_SIZE_UART];
 unsigned char gb_rlen_uart=0;
#endif





void setup()
{
 #if defined(use_lib_log_serial) || defined(use_lib_keyboard_uart)
  Serial.begin(115200);         
  Serial.printf("HEAP BEGIN %d\r\n", ESP.getFreeHeap()); 
 #endif
 #ifdef use_lib_keyboard_uart
  Serial.setTimeout(use_lib_keyboard_uart_timeout);
 #endif
 
 gb_sdl_font= gb_sdl_font_6x8;

 Serial.printf("Inicio C64 ESP32\r\n");  
 SetupMemory(); //Memoria RAM emulador    
 LeerFicherosROMS();
 C64_constructor(); //No objetos
 PreparaPaleta();
 
 PreparaColorVGA();

 #ifdef use_lib_log_serial  
  Serial.printf("Antes de video\r\n");
 #endif 
 vga_init(pin_config,VgaMode_vga_mode_400x300,false); //400x300 
 gb_sync_bits= vga_get_sync_bits();
 gb_buffer_vga = vga_get_framebuffer();
 gb_buffer_vga32=(unsigned int **)gb_buffer_vga; 
 #ifdef use_lib_log_serial
  Serial.printf("Fast Tiny Bitluni\r\n");
  Serial.printf("vsync_inv_bit:0x%02X hsync_inv_bit:0x%02X\r\n",vga_get_vsync_inv_bit(),vga_get_hsync_inv_bit());
  Serial.printf("Sync bits:0x%02X\r\n",gb_sync_bits);
 #endif  
 #ifdef use_lib_log_serial  
  Serial.printf("Antes de video\r\n");
 #endif 

 #ifdef use_lib_log_serial  
  Serial.printf("END SETUP %d\r\n", ESP.getFreeHeap()); 
 #endif   
}

void loop() 
{  
 if (gb_quit==0)
 {
  Serial.printf("Reset global emulator\r\n");
   //LeerFicherosROMS();
   C64_constructor();   
   gb_reset= 0;
   C64_start(); //No objetos  
 } 
}
