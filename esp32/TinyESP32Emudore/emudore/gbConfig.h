#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 #define use_lib_tinybitluni_fast

 //Ver numero de llamadas de funcion
 //#define use_lib_cont_call
 

 //Use terminal keyboard, putty
 #define use_lib_keyboard_uart
 
 //timeout read millis
 #define use_lib_keyboard_uart_timeout 0

 //log keyboard remote uart 
 //#define use_lib_log_keyboard_uart



 //Poll keyboard remote uart
 #define gb_current_ms_poll_keyboard_uart 50  


 //mascara para saltar frames fps modo fast
 #define gb_fps_fast_mask 0x0F
 
 //Usar sonido
 #define use_lib_sound

 //#define use_lib_log_write_byte_setup_memory_banks

 //#define use_lib_log_setup_memory_banks
 
 //log del sid
 //#define use_lib_log_sid
 
 //log fps
 #define use_lib_log_fps
 

 //Show Logs
 #define use_lib_log_serial
 
 
 
 //Lo que estaba en util.h
 #define ISSET_BIT(v,b)  ((v&(1<<b))!=0)

#endif

