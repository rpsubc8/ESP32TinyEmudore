#ifndef OSDFILE_H
 #define OSDFILE_H

  #include "gbConfig.h"
  #include "gbGlobals.h"
  //#include "gbsdlfont6x8.h"
 
 //#include <SDL/SDL.h>


//extern Uint32 gbSDLVideoDelay;

//Tengo que revisar
//#define byte unsigned char
//#define boolean unsigned char


void changeSna2Flash(unsigned char id);
void do_tinyOSD(void);
//void SDLAssignSDLSurfaceOSD(SDL_Surface *surface,SDL_Event * ptrEvent);
void SDLActivarOSDMainMenu();
void SDLOSDClear(void);
//SDL_Surface * SDLOSDGetSurface();
//void SDLEsperaTeclado(void);
void load_prg(unsigned char id);
void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor);
void SDLDrawVGA(void);
#ifdef use_lib_video_2bpp_direct_buffer
 void SDLprintTextDirectBuffer(char *cad,unsigned char x,unsigned char y);
 void SDLDumpDirectOSD(void);
#endif
//void jj_fast_putpixel(unsigned short x,unsigned short int y,unsigned char c);
 
 #ifdef use_lib_keyboard_uart
  void do_keyboard_uart();
  void keyboard_uart_poll(void);
  unsigned char checkKey_uart(unsigned char scancode);
 #endif 

#endif

