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
//#include <stdexcept> //No lo necesito
#include "io.h"
#include "cpu.h"
#include "vic.h"
//#include <SDL/SDL.h>
#include "osd.h"
#include "Arduino.h"

//SDL_Color gb_colors[256]; //Modo paleta SDL2
//SDL_Surface *screen; //Modo paleta SDL2
//SDL_Texture *sdlTexture; //Modo paleta SDL2
//SDL_Renderer *sdlRenderer; //Modo paleta SDL2

// clas ctor and dtor //////////////////////////////////////////////////////////


//void IO_cpu(Cpu *v) //No necesito objetos
//{
// IO_cpu_=v;
//}

//IO::IO()
void IO_constructor()
{
  IO_retval_= true;//Inicializacion JJ
  
  //JJ SDL2 SDL_Init(SDL_INIT_VIDEO);
  //
  // We create the window double the original pixel size, 
  // the renderer takes care of upscaling 
  //
  //JJ SDL2 
  //window_ = SDL_CreateWindow(
  //      "emudore",
  //      SDL_WINDOWPOS_UNDEFINED,
  //      SDL_WINDOWPOS_UNDEFINED,
  //      //JJ Vic::kVisibleScreenWidth * 2,
  //      //JJ Vic::kVisibleScreenHeight * 2,
  //      Vic::kVisibleScreenWidth,
  //      Vic::kVisibleScreenHeight,
  //      SDL_WINDOW_OPENGL
  //);
  
  IO_cols_ = VIC_kVisibleScreenWidth;
  IO_rows_ = VIC_kVisibleScreenHeight;
  
  Serial.printf("cols %d rows %d\r\n",VIC_kVisibleScreenWidth,VIC_kVisibleScreenHeight);
  //fflush(stdout);
  // use a single texture and hardware acceleration
  //JJ SDL2 
  //renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  //texture_  = SDL_CreateTexture(renderer_,
  //                              SDL_PIXELFORMAT_ARGB8888,
  //                              SDL_TEXTUREACCESS_STREAMING,
  //                              cols_,
  //                              rows_);
  //format_ = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
  
  
  //
  // unfortunately, we need to keep a copy of the rendered frame 
  // in our own memory, there does not seem to be a way around 
  // that would allow manipulating pixels straight on the GPU 
  // memory due to how the image is internally stored, etc..
  //
  // The rendered frame gets uploaded to the GPU on every 
  // screen refresh.
  //
  //JJ SDL2 frame_  = new uint32_t[cols_ * rows_]();
  //frame_  = new unsigned char[cols_ * rows_]();
  
  //IO_cols_ es 403
  //IO_rows_ es 284
//  IO_frame_  = (unsigned char *)malloc(IO_cols_ * IO_rows_); //Lo inicializo en SetupMemory

  //JJ init_color_palette(); //No se necesita
  IO_init_keyboard();
  //JJ next_key_event_at_ = 0; //No lo necesito
  //JJ prev_frame_was_at_ = std::chrono::high_resolution_clock::now();
  
  
  
//  //Uso modo paleta en SDL2
//  for(int i = 0; i < 256; i++)
//  {
//   gb_colors[i].r = gb_colors[i].g = gb_colors[i].b = (Uint8)i;
//  }
//  screen = SDL_CreateRGBSurface(0, Vic::kVisibleScreenWidth, Vic::kVisibleScreenHeight,8,0,0,0,0);
//  SDL_Texture *sdlTexture = SDL_CreateTexture(renderer_,
//                                        SDL_PIXELFORMAT_ARGB8888,
//                                        SDL_TEXTUREACCESS_STREAMING,
//                                        Vic::kVisibleScreenWidth, Vic::kVisibleScreenHeight);
//  
//  SDL_SetPaletteColors(screen->format->palette, gb_colors, 0, 256);
  
  
  Serial.printf("IO_constructor\r\n");
  //fflush(stdout);
}

//IO::~IO()
void IO_destroy()
{
  //JJ SDL2 delete [] frame_;  
  //JJ SDL2 SDL_DestroyRenderer(renderer_);
  //JJ SDL2 SDL_DestroyTexture(texture_);
  //JJ SDL2 SDL_FreeFormat(format_);
  
//  if (IO_frame_!= NULL) //No lo necesitamos
//  {
//   free(IO_frame_);
//  }
  //SDL_Quit();
}

// init io devices  ////////////////////////////////////////////////////////////

//
// @brief init keyboard state and keymap
//
//void IO::IO_init_keyboard()
void IO_init_keyboard()
{
  Serial.printf("init_keyboard\r\n");
  //fflush(stdout);
  // init keyboard matrix state
  for(size_t i=0 ; i < sizeof(IO_keyboard_matrix_) ; i++)
  {
    IO_keyboard_matrix_[i] = 0xff;
  }
    
  //// character to sdl key map
  //charmap_['A']  = {SDL_SCANCODE_A};
  //charmap_['B']  = {SDL_SCANCODE_B};
  //charmap_['C']  = {SDL_SCANCODE_C};
  //charmap_['D']  = {SDL_SCANCODE_D};
  //charmap_['E']  = {SDL_SCANCODE_E};
  //charmap_['F']  = {SDL_SCANCODE_F};
  //charmap_['G']  = {SDL_SCANCODE_G};
  //charmap_['H']  = {SDL_SCANCODE_H};
  //charmap_['I']  = {SDL_SCANCODE_I};
  //charmap_['J']  = {SDL_SCANCODE_J};
  //charmap_['K']  = {SDL_SCANCODE_K};
  //charmap_['L']  = {SDL_SCANCODE_L};
  //charmap_['M']  = {SDL_SCANCODE_M};
  //charmap_['N']  = {SDL_SCANCODE_N};
  //charmap_['O']  = {SDL_SCANCODE_O};
  //charmap_['P']  = {SDL_SCANCODE_P};
  //charmap_['Q']  = {SDL_SCANCODE_Q};
  //charmap_['R']  = {SDL_SCANCODE_R};
  //charmap_['S']  = {SDL_SCANCODE_S};
  //charmap_['T']  = {SDL_SCANCODE_T};
  //charmap_['U']  = {SDL_SCANCODE_U};
  //charmap_['V']  = {SDL_SCANCODE_V};
  //charmap_['W']  = {SDL_SCANCODE_W};
  //charmap_['X']  = {SDL_SCANCODE_X};
  //charmap_['Y']  = {SDL_SCANCODE_Y};
  //charmap_['Z']  = {SDL_SCANCODE_Z};
  //charmap_['1']  = {SDL_SCANCODE_1};
  //charmap_['2']  = {SDL_SCANCODE_2};
  //charmap_['3']  = {SDL_SCANCODE_3};
  //charmap_['4']  = {SDL_SCANCODE_4};
  //charmap_['5']  = {SDL_SCANCODE_5};
  //charmap_['6']  = {SDL_SCANCODE_6};
  //charmap_['7']  = {SDL_SCANCODE_7};
  //charmap_['8']  = {SDL_SCANCODE_8};
  //charmap_['9']  = {SDL_SCANCODE_9};
  //charmap_['0']  = {SDL_SCANCODE_0};
  //charmap_['\n'] = {SDL_SCANCODE_RETURN};
  //charmap_[' ']  = {SDL_SCANCODE_SPACE};
  //charmap_[',']  = {SDL_SCANCODE_COMMA};
  //charmap_['.']  = {SDL_SCANCODE_PERIOD};
  //charmap_['/']  = {SDL_SCANCODE_SLASH};
  //charmap_[';']  = {SDL_SCANCODE_SEMICOLON};
  //charmap_['=']  = {SDL_SCANCODE_EQUALS};
  //charmap_['-']  = {SDL_SCANCODE_MINUS};
  //charmap_[':']  = {SDL_SCANCODE_BACKSLASH};
  //charmap_['+']  = {SDL_SCANCODE_LEFTBRACKET};
  //charmap_['*']  = {SDL_SCANCODE_RIGHTBRACKET};
  //charmap_['@']  = {SDL_SCANCODE_APOSTROPHE};
  //charmap_['(']  = {SDL_SCANCODE_LSHIFT,SDL_SCANCODE_8};
  //charmap_[')']  = {SDL_SCANCODE_LSHIFT,SDL_SCANCODE_9};
  //charmap_['<']  = {SDL_SCANCODE_LSHIFT,SDL_SCANCODE_COMMA};
  //charmap_['>']  = {SDL_SCANCODE_LSHIFT,SDL_SCANCODE_PERIOD};
  //charmap_['"']  = {SDL_SCANCODE_LSHIFT,SDL_SCANCODE_2};
  //charmap_['$']  = {SDL_SCANCODE_LSHIFT,SDL_SCANCODE_4};
  //// keymap letters
  //keymap_[SDL_SCANCODE_A] = std::make_pair(1,2);
  //keymap_[SDL_SCANCODE_B] = std::make_pair(3,4);
  //keymap_[SDL_SCANCODE_C] = std::make_pair(2,4);
  //keymap_[SDL_SCANCODE_D] = std::make_pair(2,2);
  //keymap_[SDL_SCANCODE_E] = std::make_pair(1,6);
  //keymap_[SDL_SCANCODE_F] = std::make_pair(2,5);
  //keymap_[SDL_SCANCODE_G] = std::make_pair(3,2);
  //keymap_[SDL_SCANCODE_H] = std::make_pair(3,5);
  //keymap_[SDL_SCANCODE_I] = std::make_pair(4,1);
  //keymap_[SDL_SCANCODE_J] = std::make_pair(4,2);
  //keymap_[SDL_SCANCODE_K] = std::make_pair(4,5);
  //keymap_[SDL_SCANCODE_L] = std::make_pair(5,2);
  //keymap_[SDL_SCANCODE_M] = std::make_pair(4,4);
  //keymap_[SDL_SCANCODE_N] = std::make_pair(4,7);
  //keymap_[SDL_SCANCODE_O] = std::make_pair(4,6);
  //keymap_[SDL_SCANCODE_P] = std::make_pair(5,1);
  //keymap_[SDL_SCANCODE_Q] = std::make_pair(7,6);
  //keymap_[SDL_SCANCODE_R] = std::make_pair(2,1);
  //keymap_[SDL_SCANCODE_S] = std::make_pair(1,5);
  //keymap_[SDL_SCANCODE_T] = std::make_pair(2,6);
  //keymap_[SDL_SCANCODE_U] = std::make_pair(3,6);
  //keymap_[SDL_SCANCODE_V] = std::make_pair(3,7);
  //keymap_[SDL_SCANCODE_W] = std::make_pair(1,1);
  //keymap_[SDL_SCANCODE_X] = std::make_pair(2,7);
  //keymap_[SDL_SCANCODE_Y] = std::make_pair(3,1);
  //keymap_[SDL_SCANCODE_Z] = std::make_pair(1,4);
  //// keymap numbers
  //keymap_[SDL_SCANCODE_1] = std::make_pair(7,0);
  //keymap_[SDL_SCANCODE_2] = std::make_pair(7,3);
  //keymap_[SDL_SCANCODE_3] = std::make_pair(1,0);
  //keymap_[SDL_SCANCODE_4] = std::make_pair(1,3);
  //keymap_[SDL_SCANCODE_5] = std::make_pair(2,0);
  //keymap_[SDL_SCANCODE_6] = std::make_pair(2,3);
  //keymap_[SDL_SCANCODE_7] = std::make_pair(3,0);
  //keymap_[SDL_SCANCODE_8] = std::make_pair(3,3);
  //keymap_[SDL_SCANCODE_9] = std::make_pair(4,0);
  //keymap_[SDL_SCANCODE_0] = std::make_pair(4,3);
  //// keymap function keys
  //keymap_[SDL_SCANCODE_F1] = std::make_pair(0,4);
  //keymap_[SDL_SCANCODE_F3] = std::make_pair(0,4);
  //keymap_[SDL_SCANCODE_F5] = std::make_pair(0,4);
  //keymap_[SDL_SCANCODE_F7] = std::make_pair(0,4);
  //// keymap: other
  //keymap_[SDL_SCANCODE_RETURN]    = std::make_pair(0,1);
  //keymap_[SDL_SCANCODE_SPACE]     = std::make_pair(7,4);
  //keymap_[SDL_SCANCODE_LSHIFT]    = std::make_pair(1,7);
  //keymap_[SDL_SCANCODE_RSHIFT]    = std::make_pair(6,4);
  //keymap_[SDL_SCANCODE_COMMA]     = std::make_pair(5,7);
  //keymap_[SDL_SCANCODE_PERIOD]    = std::make_pair(5,4);
  //keymap_[SDL_SCANCODE_SLASH]     = std::make_pair(6,7);
  //keymap_[SDL_SCANCODE_SEMICOLON] = std::make_pair(6,2);
  //keymap_[SDL_SCANCODE_EQUALS]    = std::make_pair(6,5);
  //keymap_[SDL_SCANCODE_BACKSPACE] = std::make_pair(0,0);
  //keymap_[SDL_SCANCODE_MINUS]     = std::make_pair(5,3);
  //// keymap: these are mapped to other keys
  //keymap_[SDL_SCANCODE_BACKSLASH]    = std::make_pair(5,5); // : 
  //keymap_[SDL_SCANCODE_LEFTBRACKET]  = std::make_pair(5,0); // +
  //keymap_[SDL_SCANCODE_RIGHTBRACKET] = std::make_pair(6,1); // *
  //keymap_[SDL_SCANCODE_APOSTROPHE]   = std::make_pair(5,6); // @
  //keymap_[SDL_SCANCODE_LGUI]         = std::make_pair(7,5); // commodore key
  
}


// 
// @brief init c64 color palette 
//
//JJ void IO::init_color_palette() //No se necesita
//JJ {
//JJ  printf("init_color_palette\n");
//JJ  fflush(stdout); 
//JJ 
//JJ  //JJ SDL2
//JJ   color_palette[0]   = SDL_MapRGB(format_, 0x00, 0x00, 0x00);
//JJ   color_palette[1]   = SDL_MapRGB(format_, 0xff, 0xff, 0xff);
//JJ   color_palette[2]   = SDL_MapRGB(format_, 0xab, 0x31, 0x26);
//JJ   color_palette[3]   = SDL_MapRGB(format_, 0x66, 0xda, 0xff);
//JJ   color_palette[4]   = SDL_MapRGB(format_, 0xbb, 0x3f, 0xb8);
//JJ   color_palette[5]   = SDL_MapRGB(format_, 0x55, 0xce, 0x58);
//JJ   color_palette[6]   = SDL_MapRGB(format_, 0x1d, 0x0e, 0x97);
//JJ   color_palette[7]   = SDL_MapRGB(format_, 0xea, 0xf5, 0x7c);
//JJ   color_palette[8]   = SDL_MapRGB(format_, 0xb9, 0x74, 0x18);
//JJ   color_palette[9]   = SDL_MapRGB(format_, 0x78, 0x53, 0x00);
//JJ   color_palette[10]  = SDL_MapRGB(format_, 0xdd, 0x93, 0x87);
//JJ   color_palette[11]  = SDL_MapRGB(format_, 0x5b, 0x5b, 0x5b);
//JJ   color_palette[12]  = SDL_MapRGB(format_, 0x8b, 0x8b, 0x8b);
//JJ   color_palette[13]  = SDL_MapRGB(format_, 0xb0, 0xf4, 0xac);
//JJ   color_palette[14]  = SDL_MapRGB(format_, 0xaa, 0x9d, 0xef);
//JJ   color_palette[15]  = SDL_MapRGB(format_, 0xb8, 0xb8, 0xb8);
//JJ   
//JJ }

// emulation /////////////////////////////////////////////////////////////////// 

//bool IO::IO_emulate()
//bool IO_emulate()
//{
//  return IO_retval_;
//}

//void IO::IO_process_events()
void IO_process_events()
{
// SDL_Event event;     
// SDL_PollEvent (&event);     
     
//JJ SDL2 const Uint8 *keystates = SDL_GetKeyboardState(NULL);


/*

while(SDL_PollEvent(&gb_event) != 0)
{
 switch (gb_event.type)
 {
   case SDL_KEYDOWN:
    switch (gb_event.key.keysym.sym)
    {
     //case SDLK_F1: gb_show_osd_main_menu= 1; break;
     case SDLK_F2:
      //jj_load_prg();
      //load_file("monopole.prg"); //Es con el que venia
      //gb_ptr_loader->prg("monopole.prg");
       gb_load_ptr= 1;
       sprintf(gb_load_ptr_name,"monopole.prg");
      break;
     
     case SDLK_F10: case SDLK_F12:
      gb_show_osd_main_menu= 1;            
      //retval_= false;  //salir
      break;
     //case SDLK_1: keyboard_matrix_[5] = 0xDF; break; //1  01111111
     //case SDLK_3: keyboard_matrix_[0] = 0xFD; break; //3  10111111
     //case SDLK_5: keyboard_matrix_[0] = 0xFB; break; //5  11011111
     //case SDLK_7: keyboard_matrix_[0] = 0xF7; break; //7    11101111
     //case SDLK_9: keyboard_matrix_[0] = 0xEF; break; //9    11110111
          
     //row 0
     case SDLK_DELETE: IO_keyboard_matrix_[0]= 0xFE; break; //DEL
     case SDLK_RETURN: case SDLK_KP_ENTER: IO_keyboard_matrix_[0]= 0xFD; break; //RETURN
     case SDLK_LEFT: 
      IO_keyboard_matrix_[0]= 0xFB;
      IO_keyboard_matrix_[7]= 0xFD;
      break; //LEFT
     case SDLK_F7: IO_keyboard_matrix_[0]= 0xF7; break; //F7
     case SDLK_F1: IO_keyboard_matrix_[0]= 0xEF; break; //F1
     case SDLK_F3: IO_keyboard_matrix_[0]= 0xDF; break; //F3
     case SDLK_F5: IO_keyboard_matrix_[0]= 0xBF; break; //F5
     case SDLK_UP: 
      IO_keyboard_matrix_[0]= 0x7F;
      IO_keyboard_matrix_[6]= 0xBF;
      break; //UP
          
     //row 1
     case SDLK_3: IO_keyboard_matrix_[1]= 0xFE; break; //3
     case SDLK_w: IO_keyboard_matrix_[1]= 0xFD; break; //W
     case SDLK_a: IO_keyboard_matrix_[1]= 0xFB; break; //A
     case SDLK_4: IO_keyboard_matrix_[1]= 0xF7; break; //4
     case SDLK_z: IO_keyboard_matrix_[1]= 0xEF; break; //Z
     case SDLK_s: IO_keyboard_matrix_[1]= 0xDF; break; //S
     case SDLK_e: IO_keyboard_matrix_[1]= 0xBF; break; //R
     case SDLK_LSHIFT: IO_keyboard_matrix_[1]= 0x7F; break; //left shift 
     
     //row 2
     case SDLK_5: IO_keyboard_matrix_[2]= 0xFE; break; //5
     case SDLK_r: IO_keyboard_matrix_[2]= 0xFD; break; //r
     case SDLK_d: IO_keyboard_matrix_[2]= 0xFB; break; //d
     case SDLK_6: IO_keyboard_matrix_[2]= 0xF7; break; //6
     case SDLK_c: IO_keyboard_matrix_[2]= 0xEF; break; //c
     case SDLK_f: IO_keyboard_matrix_[2]= 0xDF; break; //f
     case SDLK_t: IO_keyboard_matrix_[2]= 0xBF; break; //t
     case SDLK_x: IO_keyboard_matrix_[2]= 0x7F; break; //x
     
     //row 3
     case SDLK_7: IO_keyboard_matrix_[3]= 0xFE; break; //7
     case SDLK_y: IO_keyboard_matrix_[3]= 0xFD; break; //y
     case SDLK_g: IO_keyboard_matrix_[3]= 0xFB; break; //g
     case SDLK_8: IO_keyboard_matrix_[3]= 0xF7; break; //8
     case SDLK_b: IO_keyboard_matrix_[3]= 0xEF; break; //b
     case SDLK_h: IO_keyboard_matrix_[3]= 0xDF; break; //h
     case SDLK_u: IO_keyboard_matrix_[3]= 0xBF; break; //u
     case SDLK_v: IO_keyboard_matrix_[3]= 0x7F; break; //v     
     
     //row 4
     case SDLK_9: IO_keyboard_matrix_[4]= 0xFE; break; //9
     case SDLK_i: IO_keyboard_matrix_[4]= 0xFD; break; //i
     case SDLK_j: IO_keyboard_matrix_[4]= 0xFB; break; //j
     case SDLK_0: IO_keyboard_matrix_[4]= 0xF7; break; //0
     case SDLK_m: IO_keyboard_matrix_[4]= 0xEF; break; //m
     case SDLK_k: IO_keyboard_matrix_[4]= 0xDF; break; //k
     case SDLK_o: IO_keyboard_matrix_[4]= 0xBF; break; //o
     case SDLK_n: IO_keyboard_matrix_[4]= 0x7F; break; //n          
     
     //row 5     
     case SDLK_PLUS: IO_keyboard_matrix_[5]= 0xFE; break; //+
     case SDLK_p: IO_keyboard_matrix_[5]= 0xFD; break; //p
     case SDLK_l: IO_keyboard_matrix_[5]= 0xFB; break; //l
     case SDLK_MINUS: IO_keyboard_matrix_[5]= 0xF7; break; //-
     case SDLK_PERIOD: IO_keyboard_matrix_[5]= 0xEF; break; //.
     case SDLK_COLON: IO_keyboard_matrix_[5]= 0xDF; break; //:
     case SDLK_AT: IO_keyboard_matrix_[5]= 0xBF; break; //@
     case SDLK_COMMA: IO_keyboard_matrix_[5]= 0x7F; break; //,                             
     
     //row6
     //case SDLK_PLUS: keyboard_matrix_[6]= 0xFE; break; //Libra
     case SDLK_ASTERISK: case SDLK_KP_MULTIPLY: IO_keyboard_matrix_[6]= 0xFD; break; //Asterisco
     case SDLK_SEMICOLON: IO_keyboard_matrix_[6]= 0xFB; break; //;
     case SDLK_HOME: IO_keyboard_matrix_[6]= 0xF7; break; //CLR HOME
     case SDLK_RSHIFT: IO_keyboard_matrix_[6]= 0xEF; break; //RIGHT SHIFT
     case SDLK_EQUALS: IO_keyboard_matrix_[6]= 0xDF; break; //=
     //case SDLK_UP: keyboard_matrix_[6]= 0xBF; break; //UP Ya existe
     case SDLK_SLASH: IO_keyboard_matrix_[6]= 0x7F; break; //SLASH                                  
     
     //row7
     case SDLK_1: IO_keyboard_matrix_[7]= 0xFE; break; //1
     //case SDLK_LEFT: keyboard_matrix_[7]= 0xFD; break; //LEFT Ya existe
     case SDLK_RCTRL: case SDLK_LCTRL: IO_keyboard_matrix_[7]= 0xFB; break; //CONTROL
     case SDLK_2: IO_keyboard_matrix_[7]= 0xF7; break; //2
     case SDLK_SPACE: IO_keyboard_matrix_[7]= 0xEF; break; //SPACE
     //case SDLK_: keyboard_matrix_[7]= 0xDF; break; //COMMODRE
     case SDLK_q: IO_keyboard_matrix_[7]= 0xBF; break; //Q
     //case SDLK_: keyboard_matrix_[7]= 0x7F; break; //RUN STOP
    }
    break;

   case SDL_KEYUP:        
    memset(IO_keyboard_matrix_,0xFF,sizeof(IO_keyboard_matrix_));
    break;  
    
   case SDL_QUIT:
    IO_retval_ = false;
    break;

   default:
    break;   
 }
 
} //fin del while 

*/

 
// 
// if (keystates[SDLK_1]) keyboard_matrix_[0]= 0xFE; //1
// if (keystates[SDLK_3]) keyboard_matrix_[0]= 0xFD; //3
// if (keystates[SDLK_5]) keyboard_matrix_[0]= 0xFB; //4
// if (keystates[SDLK_7]) keyboard_matrix_[0]= 0xF7; //7
// if (keystates[SDLK_9]) keyboard_matrix_[0]= 0xEF; //9
     
     
   
//  SDL_Event event;
//  while(SDL_PollEvent(&event))
//  {
//   switch(event.type)
//   {
//    case SDL_KEYDOWN:
//      //JJ handle_keydown(event.key.keysym.scancode);
//      
//      switch( event.key.keysym.sym )
//      {
//       case SDLK_F10 : retval_ = false; break; //Salir
//       //ROW 0
//       case SDLK_1: keyboard_matrix_[0] &= 0x7F; break; //1  01111111
//       case SDLK_3: keyboard_matrix_[0] &= 0xBF; break; //3  10111111
//       case SDLK_5: keyboard_matrix_[0] &= 0xDF; break; //5  11011111
//       case SDLK_7: keyboard_matrix_[0] &= 0xEF; break; //7    11101111
//       case SDLK_9: keyboard_matrix_[0] &= 0xF7; break; //9    11110111
//       //case SDLK_: keyboard_matrix_[0] &= 0xFB; break; //+ 11111011
//       //case SDLK_3: keyboard_matrix_[0] &= 0xFD; break; //Libra  11111101
//       //case SDLK_3: keyboard_matrix_[0] &= 0xFE; break; //Del borrar    11111110              
//       
//      }
//        
//      
//      
//      break;
//    case SDL_KEYUP:
//      //JJ handle_keyup(event.key.keysym.scancode);
//      switch( event.key.keysym.sym )
//      {
//       case SDLK_1: keyboard_matrix_[0] = FF; break; //1  01111111
//       case SDLK_3: keyboard_matrix_[0] |= 0xBF; break; //3  10111111
//       case SDLK_5: keyboard_matrix_[0] |= 0xDF; break; //5  11011111
//       case SDLK_7: keyboard_matrix_[0] |= 0xEF; break; //7    11101111
//       case SDLK_9: keyboard_matrix_[0] |= 0xF7; break; //9    11110111
//       //case SDLK_: keyboard_matrix_[0] |= 0xFB; break; //+ 11111011
//       //case SDLK_3: keyboard_matrix_[0] |= 0xFD; break; //Libra  11111101
//       //case SDLK_3: keyboard_matrix_[0] |= 0xFE; break; //Del borrar    11111110       
//      }
//      break;
//    case SDL_QUIT:
//      retval_ = false;
//      break;
//   }
//  }
  



  
  // process fake keystrokes if any
////JJ
//  if(!key_event_queue_.empty() && 
//     cpu_->cycles() > next_key_event_at_)
//  {
//    std::pair<kKeyEvent,SDL_Keycode> &ev = key_event_queue_.front();
//    key_event_queue_.pop();
//    switch(ev.first)
//    {
//    case kPress:
//      handle_keydown(ev.second);
//      break;
//    case kRelease:
//      handle_keyup(ev.second);
//      break;
//    }
//    next_key_event_at_ = cpu_->cycles() + kWait;
//  }
  
}

// keyboard handling /////////////////////////////////////////////////////////// 

//
// @brief emulate keydown
//
//JJ SDL2 void IO::handle_keydown(SDL_Keycode k)
//JJ SDL2 {
//JJ SDL2   fprintf(gb_fichLog,"handle_keydown\n");
//JJ SDL2   fflush(gb_fichLog);
//JJ SDL2   
//JJ SDL2   try
//JJ SDL2   {
//JJ SDL2     //uint8_t mask = ~(1 << keymap_.at(k).second);
//JJ SDL2     //JJ keyboard_matrix_[keymap_.at(k).first] &= mask;
//JJ SDL2   }
//JJ SDL2   catch(const std::out_of_range){}
//JJ SDL2 }

//
// @brief emulate keyup
//
//JJ SDL2 void IO::handle_keyup(SDL_Keycode k)
//JJ SDL2 {
//JJ SDL2   try
//JJ SDL2   {
//JJ SDL2     //JJ uint8_t mask = (1 << keymap_.at(k).second);
//JJ SDL2     //JJ keyboard_matrix_[keymap_.at(k).first] |= mask;
//JJ SDL2   }
//JJ SDL2   catch(const std::out_of_range){}  
//JJ SDL2 }

//
// @brief fake press a key, monkeys love it
//
// Characters are added to a queue and processed within 
// the emulation loop.
//
//JJ void IO::type_character(char c)
//JJ {
//JJ   try
//JJ   {
//JJ     //JJ for(SDL_Keycode &k: charmap_.at(toupper(c)))
//JJ     //JJ   key_event_queue_.push(std::make_pair(kPress,k));
//JJ     //JJ for(SDL_Keycode &k: charmap_.at(toupper(c)))
//JJ     //JJ   key_event_queue_.push(std::make_pair(kRelease,k));
//JJ   }
//JJ   catch(const std::out_of_range){}   
//JJ }

// screen handling /////////////////////////////////////////////////////////////


/*
void IO_screen_draw_rect_border32(unsigned short int y,unsigned char color)
{
 unsigned a32= gb_color_vga[color];
 a32= (a32<<24)|(a32<<16)|(a32<<8)|a32;     
 for(unsigned char i=0; i < 100 ; i++)
 {
  gb_buffer_vga32[y][i] = a32;
 }   
}

void IO_screen_draw_rect_border_lateral32(unsigned char x32,unsigned short int y,unsigned char color)
{//40 pixel 10 bytes de 32 bits
 unsigned a32= gb_color_vga[color];
 a32= (a32<<24)|(a32<<16)|(a32<<8)|a32;
 for(unsigned char i=x32; i < (x32+10) ; i++)
 {
  gb_buffer_vga32[y][i] = a32;
 }   
}
*/

//JJ void IO::screen_draw_rect(int x, int y, int n, int color)
//void IO::IO_screen_draw_rect(unsigned short int x, unsigned short int y, unsigned short int n, unsigned char color)
void IO_screen_draw_rect(unsigned short int x, unsigned short int y, unsigned short int n, unsigned char color)
{
  unsigned int a32= gb_color_vga[color];
  unsigned int cont= x>>2; //DIV 4;
  a32= (a32<<24)|(a32<<16)|(a32<<8)|a32;

  n=n>>2; //DIV 4  
  for(unsigned short int i=0; i < n ; i++)
  {  
   gb_buffer_vga32[y][cont++] = a32;
  }

  //for(unsigned short int i=0; i < n ; i++)
  //{
  //  //IO_screen_update_pixel(x+i,y,color);
  //  gb_buffer_vga[y][(x+i)^2] = gb_color_vga[color];
  //}
}
 
//JJ void IO::screen_draw_border(int y, int color)
//void IO::IO_screen_draw_border(unsigned short int y, unsigned char color)
//void IO_screen_draw_border(unsigned short int y, unsigned char color)
//{
//  IO_screen_draw_rect(0,y,IO_cols_,color);
//}
 
//
// @brief refresh screen 
//
// Upload the texture to the GPU 
//
//void IO::IO_screen_refresh()
void IO_screen_refresh()
{
  unsigned int auxTime;
  
  //JJ SDL2 SDL_UpdateTexture(texture_, NULL, frame_, cols_ * sizeof(uint32_t));
  //JJ SDL2 SDL_RenderClear(renderer_);
  //JJ SDL2 SDL_RenderCopy(renderer_,texture_, NULL, NULL);
  //JJ SDL2 SDL_RenderPresent(renderer_);
  
  //process SDL events once every frame
  gb_keyboard_time_cur= millis();
  auxTime= (gb_keyboard_time_cur - gb_keyboard_time_last);
  if (auxTime >= 19)
  {   
   gb_keyboard_time_last= gb_keyboard_time_cur;
   IO_process_events();
   #ifdef use_lib_keyboard_uart       
    keyboard_uart_poll();
    do_keyboard_uart();
   #endif      
  }
  
  if (gb_show_osd_main_menu == 1)
  {
   do_tinyOSD();
  }   
  
  //perform vertical refresh sync
  IO_vsync();
  
  
  ////Paleta en SDL2
  //unsigned int auxLine = screen->pitch;  
  //for (int y=0;y<100;y++)
  //{
  // Uint8 *p = (Uint8 *)screen->pixels + (y*auxLine);
  // for (int x=0;x<100;x++)
  // {
  //  *p++ = y;
  // }
  //} 
  // 
  //SDL_UpdateTexture(texture_, NULL, screen->pixels, screen->pitch);
  //SDL_RenderClear(renderer_);
  //SDL_RenderCopy(renderer_,texture_, NULL, NULL);
  //SDL_RenderPresent(renderer_);
  
  
  
   
  //gb_vga_time_cur= SDL_GetTicks();
  //auxTime= (gb_vga_time_cur - gb_vga_time_last);
  //if (auxTime >= 19)
  //{
  // gb_vga_time_last= gb_vga_time_cur;
  //    
  // unsigned int auxLine = screen->pitch;  
  // unsigned int auxDest= 0;
  // for (unsigned int y=0;y<284;y++)
  // {
  //  Uint8 *p = (Uint8 *)screen->pixels + (y*auxLine);   
  //  //memcpy(p,&frame_[y * cols_],403);
  //  memcpy(p,&frame_[auxDest],403);
  //  auxDest+= cols_;
  // }
  //   
  // SDL_Flip (screen);
  // //SDL_Delay (1);
  //}  
  
}

//
// @brief vsync
//
// vsync() is called at the end of every frame, if we are ahead 
// of time compared to a real C64 (very likely) we sleep for a bit, 
// this way we avoid running at full speed allowing the host CPU to 
// take a little nap before getting back to work.
//
// This should also help with performance runing on slow computers, 
// uploading data to the GPU is a relatively slow operation, doing 
// more fps obviously has a performance impact.
//
// Also, and more importantly, by doing this we emulate the actual 
// speed of the C64 so visual effects do not look accelerated and 
// games become playable :)
//
//void IO::IO_vsync()
void IO_vsync()
{
 unsigned int auxTime;
  //JJ using namespace std::chrono;
  //JJ auto t = high_resolution_clock::now() - prev_frame_was_at_;
  //JJ duration<double> rr(Vic::kRefreshRate);
  //
  // Microsoft's chrono is buggy and does not properly handle 
  // doubles, we need to recast to milliseconds.
  //
  //JJ auto ttw = duration_cast<milliseconds>(rr - t);
  //JJ std::this_thread::sleep_for(ttw);
  //JJ prev_frame_was_at_ = std::chrono::high_resolution_clock::now();

  //SDL_Delay(20); //Aniadido JJ

  gb_vsync_cur= millis();
  
  
  gb_cpu_time_cur= millis(); //Leemos tiempo inicial CPU controlar 20 ms    
  auxTime = (gb_cpu_time_cur - gb_cpu_time_last);
  gb_time_tiene_que_esperar= (auxTime < 20) ? (20 - auxTime) : 0; 

  gb_inicia_espera= 1;
  gb_time_espera_ini= gb_vsync_cur;
  
  //SDL_Delay(gb_time_tiene_que_esperar);
  
//  printf("vsync %u esp %u t %u %u\n",auxTime,gb_time_tiene_que_esperar,gb_cpu_time_last,gb_cpu_time_cur);
  gb_cpu_time_last= gb_cpu_time_cur;
  

/*  
  auxTime= (gb_vsync_cur - gb_vsync_last);
  unsigned char espera= ((auxTime <= 19)?1:0);
  //if (auxTime <= 19)
  {                 
   //printf("vsync %u %u %u %u\n",gb_vsync_cur,gb_vsync_last,auxTime,espera);
   //fflush(stdout);
   gb_vsync_last= gb_vsync_cur;
   if (espera==1)
   {
    unsigned int auxDelay= (20-auxTime);
    if (auxDelay>=20){
     auxDelay= 19;
    }
    //SDL_Delay(auxDelay); //Aniadido JJ    
    gb_inicia_espera= 1;
    gb_time_espera_ini= gb_vsync_cur;
    //gb_time_tiene_que_esperar= auxDelay;    
    //printf("vsync %u esp %u\n",auxDelay,gb_time_tiene_que_esperar);
    printf("vsync %u esp %u\n",auxDelay,gb_time_tiene_que_esperar);
    //fflush(stdout);
   }
   //SDL_Delay(20-auxTime); //Aniadido JJ    
  }
  */
}



//inline uint8_t IO_keyboard_matrix_row(int col)
//unsigned char IO_keyboard_matrix_row(unsigned char col)
//{//1710 veces 1 segundo
// //gb_cont_call++;
//         
// return IO_keyboard_matrix_[col];
//};

//void IO_screen_update_pixel(unsigned short int x, unsigned short int y, unsigned char color)
//{
//  //SDL JJ frame_[y * cols_  + x] = color_palette[color & 0xf];
//  //frame_[y * cols_  + x] = color & 0x0F;
//
////  IO_frame_[y * IO_cols_  + x] = color;  
//  gb_buffer_vga[y][x] = color;
//};
