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

#ifndef EMUDORE_VIC_H
#define EMUDORE_VIC_H

#include "io.h"
#include "cpu.h"
#include "memory.h"

#define kCharMode 0
#define kMCCharMode 1
#define kBitmapMode 2
#define kMCBitmapMode 3
#define kExtBgMode 4
#define kIllegalMode 5

//graphics constants
#define VIC_kGResX 320
#define VIC_kGResY 200
#define VIC_kGCols 40
#define VIC_kGRows 25
#define VIC_kGFirstLine 56
#define VIC_kGLastLine 256
#define VIC_kGFirstCol 42
//sprites
#define VIC_kSpriteWidth 24
#define VIC_kSpriteHeight 21
#define VIC_kSpriteSize 64
#define VIC_kSpritesFirstLine 6
#define VIC_kSpritesFirstCol 18


#define VIC_kScreenLines 312
#define VIC_kScreenCols 504
#define VIC_kVisibleScreenWidth 403
#define VIC_kVisibleScreenHeight 284
#define VIC_kFirstVisibleLine 14
#define VIC_kLastVisibleLine 298
#define VIC_kLineCycles 63
#define VIC_kBadLineCycles 23
#define VIC_kSpritePtrsOffset 0x3f8


/*
//
// @brief MOS Video Interface Chip II (VIC-II)
//
// MOS 6567 NTSC
// MOS 6569 PAL
//
// This class implements the PAL version of the chip
//
class Vic
{
  private:
    Memory *VIC_mem_;
    //Cpu    *VIC_cpu_; //No objetos
    //IO     *io_; //No quiero objetos
    //sprites
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
    //helpers
    inline void VIC_raster_counter(int v);
    inline int VIC_raster_counter();
    inline bool VIC_is_screen_off();
    inline bool VIC_is_bad_line();
    inline bool VIC_raster_irq_enabled();
    inline unsigned char VIC_vertical_scroll();
    inline unsigned char VIC_horizontal_scroll();
    inline bool VIC_is_sprite_enabled(int n);
    inline bool VIC_is_background_sprite(int n);
    inline bool VIC_is_double_width_sprite(int n);
    inline bool VIC_is_double_height_sprite(int n);
    inline bool VIC_is_multicolor_sprite(int n);
    inline int VIC_sprite_x(int n);
    //graphics
    inline void VIC_draw_raster_char_mode();
    inline void VIC_draw_raster_bitmap_mode();
    inline void VIC_draw_raster_sprites();
    inline void VIC_draw_sprite(int x, int y, int sprite, int row);
    inline void VIC_draw_mcsprite(int x, int y, int sprite, int row);
    inline void VIC_draw_char(int x, int y, unsigned char data, unsigned char color);
    inline void VIC_draw_mcchar(int x, int y, unsigned char data, unsigned char color);
    inline void VIC_draw_bitmap(int x, int y, unsigned char data, unsigned char color);
    inline void VIC_draw_mcbitmap(int x, int y, unsigned char data, unsigned char scolor, unsigned char rcolor);
    inline unsigned char VIC_get_screen_char(int column, int row);
    inline unsigned char VIC_get_char_color(int column, int row);
    inline unsigned char VIC_get_char_data(int chr, int line);
    inline unsigned char VIC_get_bitmap_data(int column, int row, int line);
    inline unsigned short int VIC_get_sprite_ptr(int n);
    inline void VIC_set_graphic_mode();
  public:
    Vic();
    bool VIC_emulate();
    //void VIC_memory(Memory *v){VIC_mem_ = v;};
    //void VIC_cpu(Cpu *v){VIC_cpu_ = v;};
    void VIC_memory(Memory *v);
    //void VIC_cpu(Cpu *v); //No objetos
    //void io(IO *v){io_ = v;}; //No quiero objetos
    void VIC_write_register(unsigned char r, unsigned char v);
    unsigned char VIC_read_register(unsigned char r);
    //unsigned int VIC_frames(){return VIC_frame_c_;};
    unsigned int VIC_frames();
    //constants
//    static const int kScreenLines = 312;
//    static const int kScreenCols  = 504;
//    static const int kVisibleScreenWidth  = 403;    
//    static const int kVisibleScreenHeight = 284;
//    static const int kFirstVisibleLine = 14;
//    static const int kLastVisibleLine = 298;
//    static const int kLineCycles = 63;
//    static const int kBadLineCycles = 23;
//    //JJ static constexpr double kRefreshRate = 1 / 50.125; // ~50Hz (PAL)
////JJ    static double kRefreshRate;// = 1 / 50.125; // ~50Hz (PAL)
//    static const int kSpritePtrsOffset = 0x3f8;
    //graphic modes
    //enum kGraphicMode
    //{
    //  kCharMode,
    //  kMCCharMode,
    //  kBitmapMode,
    //  kMCBitmapMode,
    //  kExtBgMode,
    //  kIllegalMode,
    //};
    //kGraphicMode VIC_graphic_mode_;
    unsigned char VIC_graphic_mode_;
    //graphics constants
    //static const int VIC_kGResX = 320;
    //static const int VIC_kGResY = 200;
    //static const int VIC_kGCols = 40;
    //static const int VIC_kGRows = 25;
    //static const int VIC_kGFirstLine = 56;
    //static const int VIC_kGLastLine  = 256;
    //static const int VIC_kGFirstCol  = 42;
    //sprites
    //static const int VIC_kSpriteWidth = 24;
    //static const int VIC_kSpriteHeight = 21;
    //static const int VIC_kSpriteSize = 64;
    //static const int VIC_kSpritesFirstLine = 6;
    //static const int VIC_kSpritesFirstCol = 18;
};


*/


    void VIC_raster_counter_set(unsigned short int v);
    //unsigned short int VIC_raster_counter_get();
    inline bool VIC_is_screen_off();
    inline bool VIC_is_bad_line();
    //inline bool VIC_raster_irq_enabled();
    //inline unsigned char VIC_vertical_scroll();            
    //inline unsigned char VIC_horizontal_scroll();
    inline bool VIC_is_sprite_enabled(unsigned char n);
    inline bool VIC_is_background_sprite(unsigned char n);
    inline bool VIC_is_double_width_sprite(unsigned char n);
    inline bool VIC_is_double_height_sprite(unsigned char n);
    inline bool VIC_is_multicolor_sprite(unsigned char n);
    unsigned short int VIC_sprite_x(unsigned char n);
    //graphics
    void VIC_draw_raster_char_mode();
    void VIC_draw_raster_bitmap_mode();
    void VIC_draw_raster_sprites();
    void VIC_draw_sprite(unsigned short int x, unsigned short int y, unsigned char sprite, unsigned short int row);
    void VIC_draw_mcsprite(unsigned short int x, unsigned short int y, unsigned char sprite, unsigned short int row);
    void VIC_draw_char(unsigned short int x, unsigned short int y, unsigned char data, unsigned char color);
    void VIC_draw_mcchar(unsigned short int x, unsigned short int y, unsigned char data, unsigned char color);
    void VIC_draw_bitmap(unsigned short int x, unsigned short int y, unsigned char data, unsigned char color);
    void VIC_draw_mcbitmap(unsigned short int x, unsigned short int y, unsigned char data, unsigned char scolor, unsigned char rcolor);
    unsigned char VIC_get_screen_char(unsigned short int column, unsigned short int row);
    inline unsigned char VIC_get_char_color(unsigned short int column, unsigned short int row);
    unsigned char VIC_get_char_data(unsigned short int chr, unsigned short int line);
    unsigned char VIC_get_bitmap_data(unsigned short int column, unsigned short int row, unsigned short int line);
    unsigned short int VIC_get_sprite_ptr(unsigned char n);
    void VIC_set_graphic_mode();

    void Vic_constructor();
    bool VIC_emulate();
    //void VIC_memory(Memory *v); //No quiero objetos
    void VIC_write_register(unsigned char r, unsigned char v);
    unsigned char VIC_read_register(unsigned char r);
    unsigned int VIC_frames();




#endif
