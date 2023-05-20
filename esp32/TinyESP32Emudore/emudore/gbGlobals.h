#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H

 #include "gbConfig.h"
 #include "stdio.h"
 //#include <SDL/SDL.h>
 #include "cpu.h"
 #include "io.h"
 #include "c64.h"
 
 extern unsigned short int gb_load_id_prg;

 #ifdef use_lib_keyboard_uart
  #define BUFFER_SIZE_UART 50
  extern char gb_buf_uart[BUFFER_SIZE_UART];
  extern unsigned char gb_rlen_uart;
  extern unsigned int gb_curTime_keyboard_before_uart;
  extern unsigned int gb_curTime_keyboard_uart;
 #endif

 extern unsigned char gb_memory64KB[0x10000];//64 KB en SRAM0

 extern const unsigned char *gb_sdl_font;

 extern unsigned char gb_sync_bits;
 extern unsigned char gb_color_vga[256];

 extern unsigned char ** gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32;
 
 extern unsigned char gb_show_osd_main_menu;
 extern unsigned char gb_quit;
 extern unsigned char gb_reset;
 
 //extern unsigned char gb_volumen01;
 //extern unsigned short int gb_frecuencia01;
 
 //extern SDL_Surface *screen;
 //extern SDL_Event gb_event; 
 extern unsigned int gb_vga_time_last;
 extern unsigned int gb_vga_time_cur;
 extern unsigned short int gb_cont_rgb;
 //extern SDL_Color gb_colors[256]; 
 extern unsigned int gb_keyboard_time_cur;
 extern unsigned int gb_keyboard_time_last;
 
 //extern unsigned char gb_basicRomC64[0x4000]; //8192
 //extern unsigned char gb_charRomC64[0x4000]; //4096
 //extern unsigned char gb_kernalRomC64[0x4000]; //8192
 extern const unsigned char *gb_basicRomC64;
 extern const unsigned char *gb_charRomC64;
 extern const unsigned char *gb_kernalRomC64;
 
 //extern FILE *gb_fichLog; 
 
 //extern Loader *gb_ptr_loader; //ptr global al loader
 extern unsigned char gb_load_ptr;
 extern char gb_load_ptr_name[256];
 
 
 extern unsigned int gb_fps_cur;
 extern unsigned int gb_fps_last;
 extern unsigned int gb_fps_min;
 extern unsigned int gb_fps_max;
 extern unsigned int gb_fps_med;
 extern unsigned int gb_fps_time_cur;
 extern unsigned int gb_fps_time_last;
 
 extern unsigned int gb_vsync_cur;
 extern unsigned int gb_vsync_last;




 //SID BEGIN
 extern unsigned int gb_sid_time_cur;
 extern unsigned int gb_sid_time_last;
 extern volatile unsigned char gbVolMixer_now_poll[3]; //poll 20 ms 50 fps
 extern volatile unsigned short int gbFrecMixer_now_poll[3];
 
 extern volatile unsigned char gbVolMixer_now[3];
 extern volatile unsigned short int gbFrecMixer_now[3];
 
 extern unsigned char gb_sid_volume[3];
 extern unsigned char gb_sid_freqLo[3];
 extern unsigned char gb_sid_freqHi[3];
 extern unsigned int gb_sid_frequency[3]; 
 
 extern volatile int gb_frecuencia01;
 extern volatile int gb_dutty_ch01;
 extern volatile int gb_volumen01;
 extern volatile int gb_duracion01;

 extern volatile int gb_frecuencia02;
 extern volatile int gb_dutty_ch02;
 extern volatile int gb_volumen02;
 extern volatile int gb_duracion02;

 extern volatile int gb_frecuencia03;
 extern volatile int gb_volumen03;

 extern volatile int gb_frecuencia04;
 extern volatile int gb_volumen04;

 extern volatile int gb_frecuencia05;
 extern volatile int gb_volumen05;
 
 
 
 extern volatile unsigned char nes_gbFrecMixer_now[3];

 extern volatile unsigned int gbVolMixer_prev[3];
 extern volatile unsigned int gbFrecMixer_prev[3];
 
 extern volatile unsigned int gb_ex_contPulse[3]; 
 extern const int SAMPLE_RATE;
 //SID END
 
 
// extern unsigned char *gb_ptr_frame; //No lo necesitamos
 extern unsigned int gb_cols;
 
 extern unsigned char gb_inicia_espera;
 extern unsigned int gb_time_espera_ini;
 extern unsigned int gb_time_tiene_que_esperar;
 
 
 
 
 
 
 
 
 
 
 
 
 //IO_H BEGIN
 extern unsigned char IO_keyboard_matrix_[8];
 extern unsigned short int IO_rows_;
 extern unsigned short int IO_cols_;
// extern unsigned char *IO_frame_; //No lo necesitamos
 extern bool IO_retval_;
 //extern Cpu * IO_cpu_; //no objetos
 //IO_H END
 
 
 //LOADER_H BEGIN 
 extern bool LOADER_booted_up_; 
 extern std::ifstream LOADER_is_;
 extern unsigned char LOADER_format_; //No quiero enums
 //extern Cpu *LOADER_cpu_; //No objetos
 //extern Memory *LOADER_mem_; //No objetos
 //extern C64 *LOADER_c64_; //no objetos
 //LOADER_H END
 
 //CIA1_H BEGIN
 //extern Cpu *CIA1_cpu_; //No objetos
 extern short int CIA1_timer_a_latch_;
 extern short int CIA1_timer_b_latch_;
 extern short int CIA1_timer_a_counter_;
 extern short int CIA1_timer_b_counter_;    
 extern bool CIA1_timer_a_enabled_;
 extern bool CIA1_timer_b_enabled_;
 extern bool CIA1_timer_a_irq_enabled_;
 extern bool CIA1_timer_b_irq_enabled_;
 extern bool CIA1_timer_a_irq_triggered_;
 extern bool CIA1_timer_b_irq_triggered_;
 extern unsigned char CIA1_timer_a_run_mode_;
 extern unsigned char CIA1_timer_b_run_mode_;
 extern unsigned char CIA1_timer_a_input_mode_;
 extern unsigned char CIA1_timer_b_input_mode_;    
 extern unsigned int CIA1_prev_cpu_cycles_;
 extern unsigned char CIA1_pra_, CIA1_prb_;
 //CIA1_H END
 
 //CIA2_H BEGIN
 //extern Cpu *CIA2_cpu_; //No objetos
 extern short int CIA2_timer_a_latch_;
 extern short int CIA2_timer_b_latch_;
 extern short int CIA2_timer_a_counter_;
 extern short int CIA2_timer_b_counter_;
 extern bool CIA2_timer_a_enabled_;
 extern bool CIA2_timer_b_enabled_;
 extern bool CIA2_timer_a_irq_enabled_;
 extern bool CIA2_timer_b_irq_enabled_;
 extern bool CIA2_timer_a_irq_triggered_;
 extern bool CIA2_timer_b_irq_triggered_;
 extern unsigned char CIA2_timer_a_run_mode_;
 extern unsigned char CIA2_timer_b_run_mode_;
 extern unsigned char CIA2_timer_a_input_mode_;
 extern unsigned char CIA2_timer_b_input_mode_;
 extern unsigned int CIA2_prev_cpu_cycles_;
 extern unsigned char CIA2_pra_, CIA2_prb_;         
 //CIA2_H END
 
 
 //CPU_H BEGIN
 //registers
 extern unsigned short int CPU_pc_;
 extern unsigned char CPU_sp_, CPU_a_, CPU_x_, CPU_y_;
 //flags (p/status reg)
 extern bool CPU_cf_,CPU_zf_,CPU_idf_,CPU_dmf_,CPU_bcf_,CPU_of_,CPU_nf_;
 //memory and clock
 //extern Memory *CPU_mem_; //No objetos
 extern unsigned int CPU_cycles_;
 extern unsigned char CPU_retval; //Para no usar pila en retorno CPU_emulate
 //CPU_H END
 
 
 
 //VIC_H BEGIN
 //extern Memory *VIC_mem_; //No objetos
 extern unsigned char VIC_mx_[8];
 extern unsigned char VIC_my_[8];
 extern unsigned char VIC_msbx_;
 extern unsigned char VIC_sprite_enabled_;
 extern unsigned char VIC_sprite_priority_;
 extern unsigned char VIC_sprite_multicolor_;
 extern unsigned char VIC_sprite_double_width_;
 extern unsigned char VIC_sprite_double_height_;
 extern unsigned char VIC_sprite_shared_colors_[2];
 extern unsigned char VIC_sprite_colors_[8];
 //background and border colors
 extern unsigned char VIC_border_color_;
 extern unsigned char VIC_bgcolor_[4];
 //cpu sync
 extern unsigned int VIC_next_raster_at_;
 //frame counter
 extern unsigned int VIC_frame_c_;
 //control registers
 extern unsigned char VIC_cr1_;
 extern unsigned char VIC_cr2_;
 //raster
 extern unsigned char VIC_raster_c_;
 extern int VIC_raster_irq_;
 //interrupt control
 extern unsigned char VIC_irq_status_;
 extern unsigned char VIC_irq_enabled_;
 //screen, character memory and bitmap addresses
 extern unsigned short int VIC_screen_mem_;
 extern unsigned short int VIC_char_mem_;
 extern unsigned short int VIC_bitmap_mem_;
 extern unsigned char VIC_mem_pointers_; 
 
 extern unsigned char VIC_graphic_mode_;
 //VIC_H END


 //MEMORY_H BEGIN
 extern unsigned char *MEMORY_mem_ram_;
 //extern unsigned char *MEMORY_mem_rom_;
 extern unsigned char MEMORY_banks_[7]; 
 //MEMORY_H END


 //C64_H BEGIN
 //C64_H END

 
 
 
 extern unsigned char gb_cpu_fast; //fast cpu emulation
 
 
 #ifdef use_lib_cont_call
  extern unsigned int gb_cont_call;
 #endif 
 
 extern unsigned int gb_cpu_time_cur;
 extern unsigned int gb_cpu_time_ini;
 extern unsigned int gb_cpu_time_last;
 
 extern unsigned char gb_cpu_emu_execute;
 
#endif


