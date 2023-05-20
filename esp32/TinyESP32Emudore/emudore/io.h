//Convertida a C by ackerman
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

#ifndef EMUDORE_IO_H
 #define EMUDORE_IO_H
 
 #include "cpu.h"

//#include "gb_globals.h"

//JJ #include <SDL2/SDL.h>
//#include <SDL/SDL.h>

//#include <queue>
//JJ #include <chrono>
//JJ #include <thread>
//#include <vector>
//#include <utility>
//#include <unordered_map>

//#include "cpu.h"
//#include "util.h"

//
// @brief IO devices
// 
// This class implements Input/Output devices connected to the 
// Commodore 64 such as the screen and keyboard.
// 
// Current backend is SDL2.
//

/*No quiero objetos

class IO
{
  private:
    //JJ Cpu *cpu_;
    //Cpu * IO_cpu_;
    //JJ SDL2 SDL_Window *window_;
    //JJ SDL2 SDL_Renderer *renderer_;
    //JJ SDL2 SDL_Texture *texture_;
    //JJ SDL2 SDL_PixelFormat *format_;
    //JJ SDL2 uint32_t *frame_;
    //JJ unsigned char *frame_;
    //JJ size_t cols_;
    //JJ size_t rows_;
    //unsigned short int IO_rows_;
    //JJ unsigned int color_palette[16]; //No se necesita
    //JJ uint8_t keyboard_matrix_[8];
    //unsigned char IO_keyboard_matrix_[8];
    //JJ bool retval_ = true;
    //JJbool retval_;
    // keyboard mappings
    //std::unordered_map<SDL_Keycode,std::pair<int,int>> keymap_;
    //JJ std::unordered_map<char,std::vector<SDL_Keycode>> charmap_;
    //JJ enum kKeyEvent //No se necesita
    //JJ{
    //JJ  kPress,
    //JJ  kRelease,
    //JJ};
    // key events
    //JJ std::queue<std::pair<kKeyEvent,SDL_Keycode>> key_event_queue_;
    //JJ unsigned int next_key_event_at_; //No lo necesito
    //JJ static const int kWait = 18000; //No lo necesito
    // vertical refresh sync
    //JJ std::chrono::high_resolution_clock::time_point prev_frame_was_at_;
    void IO_vsync();
  public:
    //JJ size_t cols_;
    //unsigned short int IO_cols_;
    //JJ unsigned char *frame_;
    //unsigned char *IO_frame_;
    IO();
    ~IO();
    //bool IO_retval_;
    bool IO_emulate();
    void IO_process_events();
    void IO_cpu(Cpu *v){IO_cpu_=v;};
    //JJ void init_color_palette(); // No se necesita
    void IO_init_keyboard();
    //JJ SDL2 void handle_keydown(SDL_Keycode k);
    //JJ SDL2 void handle_keyup(SDL_Keycode k);
    //JJ void type_character(char c);
    inline uint8_t IO_keyboard_matrix_row(int col){return IO_keyboard_matrix_[col];};
    //JJ void screen_update_pixel(int x, int y, int color);
    void IO_screen_update_pixel(unsigned short int x, unsigned short int y, unsigned char color);
    //JJ void screen_draw_rect(int x, int y, int n, int color);
    void IO_screen_draw_rect(unsigned short int x, unsigned short int y, unsigned short int n, unsigned char color);
    //JJ void screen_draw_border(int y, int color);
    void IO_screen_draw_border(unsigned short int y, unsigned char color);
    void IO_screen_refresh();
};

// inline member functions accesible from other classes /////////////////////

//JJ inline void IO::screen_update_pixel(int x, int y, int color)
inline void IO::IO_screen_update_pixel(unsigned short int x, unsigned short int y, unsigned char color)
{
  //SDL JJ frame_[y * cols_  + x] = color_palette[color & 0xf];
  //frame_[y * cols_  + x] = color & 0x0F;
  IO_frame_[y * IO_cols_  + x] = color;
};

*/






void IO_vsync(void);
void IO_constructor(void);
void IO_destroy(void);
//bool IO_emulate(void);
void IO_process_events(void);
// void IO_cpu(Cpu *v);//No objetos
void IO_init_keyboard(void);
//void IO_screen_update_pixel(unsigned short int x, unsigned short int y, unsigned char color);
void IO_screen_draw_rect(unsigned short int x, unsigned short int y, unsigned short int n, unsigned char color);
//void IO_screen_draw_border(unsigned short int y, unsigned char color);
void IO_screen_refresh(void);
//unsigned char IO_keyboard_matrix_row(unsigned char col);
void IO_screen_update_pixel(unsigned short int x, unsigned short int y, unsigned char color);


////inline uint8_t IO_keyboard_matrix_row(int col)
//inline unsigned char IO_keyboard_matrix_row(int col)
//{
// return IO_keyboard_matrix_[col];
//};
//
//inline void IO_screen_update_pixel(unsigned short int x, unsigned short int y, unsigned char color)
//{
//  //SDL JJ frame_[y * cols_  + x] = color_palette[color & 0xf];
//  //frame_[y * cols_  + x] = color & 0x0F;
//  IO_frame_[y * IO_cols_  + x] = color;
//};

#endif
