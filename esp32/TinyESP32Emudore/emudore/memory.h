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
 
#ifndef EMUDORE_MEMORY_H
#define EMUDORE_MEMORY_H 

#include "gbConfig.h"
//#include <iostream> //Sobra
//#include <cstdint>
#include <stdint.h>

// forward declarations

//class Vic; //No quiero objetos
//class Cia1;
//class Cia2;
//class Sid;

//kBankCfg
#define kROM 0
#define kRAM 1
#define kIO 2

//Banks
#define kBankBasic 3
#define kBankCharen 5
#define kBankKernal 6


//constants
#define MEMORY_kMemSize 0x10000
//memory addresses
#define MEMORY_kBaseAddrBasic 0xa000
#define MEMORY_kBaseAddrKernal 0xe000
#define MEMORY_kBaseAddrStack 0x0100
#define MEMORY_kBaseAddrScreen 0x0400
#define MEMORY_kBaseAddrChars 0xd000
#define MEMORY_kBaseAddrBitmap 0x0000
#define MEMORY_kBaseAddrColorRAM 0xd800
#define MEMORY_kAddrResetVector 0xfffc
#define MEMORY_kAddrIRQVector 0xfffe
#define MEMORY_kAddrNMIVector 0xfffa
#define MEMORY_kAddrDataDirection 0x0000
#define MEMORY_kAddrMemoryLayout 0x0001
#define MEMORY_kAddrColorRAM 0xd800
//memory layout
#define MEMORY_kAddrZeroPage 0x0000
#define MEMORY_kAddrVicFirstPage 0xd000
#define MEMORY_kAddrVicLastPage 0xd300
#define MEMORY_kAddrCIA1Page 0xdc00
#define MEMORY_kAddrCIA2Page 0xdd00
#define MEMORY_kAddrBasicFirstPage 0xa000
#define MEMORY_kAddrBasicLastPage 0xbf00
#define MEMORY_kAddrKernalFirstPage 0xe000
#define MEMORY_kAddrKernalLastPage 0xff00
#define MEMORY_kAddrSIDPage 0xd400
//bank switching
//1 << 0
#define MEMORY_kLORAM 0x01
//1 << 1
#define MEMORY_kHIRAM 0x02
//1 << 2
#define MEMORY_kCHAREN 0x04
    

/*

//
// @brief DRAM
// 
// - @c $0000-$00FF  Page 0        Zeropage addressing
// - @c $0100-$01FF  Page 1        Enhanced Zeropage contains the stack
// - @c $0200-$02FF  Page 2        Operating System and BASIC pointers
// - @c $0300-$03FF  Page 3        Operating System and BASIC pointers
// - @c $0400-$07FF  Page 4-7      Screen Memory
// - @c $0800-$9FFF  Page 8-159    Free BASIC program storage area (38911 bytes)
// - @c $A000-$BFFF  Page 160-191  Free machine language program storage area (when switched-out with ROM)
// - @c $C000-$CFFF  Page 192-207  Free machine language program storage area
// - @c $D000-$D3FF  Page 208-211  
// - @c $D400-$D4FF  Page 212-215  
// - @c $D800-$DBFF  Page 216-219  
// - @c $DC00-$DCFF  Page 220  
// - @c $DD00-$DDFF  Page 221  
// - @c $DE00-$DFFF  Page 222-223  Reserved for interface extensions
// - @c $E000-$FFFF  Page 224-255  Free machine language program storage area (when switched-out with ROM)
//

class Memory
{
  private:
    unsigned char *MEMORY_mem_ram_;
    unsigned char *MEMORY_mem_rom_;
    unsigned char MEMORY_banks_[7];
    //Vic *MEMORY_vic_; //no quiero objetos
    //Cia1 *MEMORY_cia1_; //No quiero objetos
    //Cia2 *MEMORY_cia2_; //No quiero objetos
    //Sid *MEMORY_sid_; //No quiero objetos
  public:
    Memory();
    ~Memory();
    //void MEMORY_vic(Vic *v){MEMORY_vic_ = v;};
    //void MEMORY_cia1(Cia1 *v){MEMORY_cia1_ = v;};
    //void MEMORY_cia2(Cia2 *v){MEMORY_cia2_ = v;};
    //void MEMORY_vic(Vic *v); //No quiero objetos
    //void MEMORY_cia1(Cia1 *v); //No quiero objetos
    //void MEMORY_cia2(Cia2 *v); //No quiero objetos
    //bank switching
    //enum kBankCfg
    //{
    //  kROM,
    //  kRAM,
    //  kIO
    //};
    //enum Banks
    //{
    //  kBankBasic =  3,
    //  kBankCharen = 5,
    //  kBankKernal =  6,
    //};
    void MEMORY_JJ_reset();
    void MEMORY_setup_memory_banks(unsigned char v);
    //read/write memory
    unsigned char MEMORY_read_byte(unsigned short int addr);
    unsigned char MEMORY_read_byte_no_io(unsigned short int addr);
    void MEMORY_write_byte(unsigned short int addr, unsigned char v);
    void MEMORY_write_byte_no_io(unsigned short int addr, unsigned char v);
    unsigned short int MEMORY_read_word(unsigned short int addr);
    unsigned short int MEMORY_read_word_no_io(unsigned short int addr);
    void MEMORY_write_word(unsigned short int addr, unsigned short int v);
    void MEMORY_write_word_no_io(unsigned short int addr, unsigned short int v);
    //vic memory access
    unsigned char MEMORY_vic_read_byte(unsigned short int addr);
    unsigned char MEMORY_read_byte_rom(unsigned short int addr);
    //load external binaries
    void MEMORY_load_rom(const std::string &f, unsigned short int baseaddr);
    void MEMORY_load_ram(const std::string &f, unsigned short int baseaddr);
    //debug
    void MEMORY_dump();
    void MEMORY_print_screen_text();

//    //constants
//    static const size_t kMemSize = 0x10000;
//    //memory addresses
//    static const unsigned short int kBaseAddrBasic  = 0xa000;
//    static const unsigned short int kBaseAddrKernal = 0xe000;
//    static const unsigned short int kBaseAddrStack  = 0x0100;
//    static const unsigned short int kBaseAddrScreen = 0x0400;
//    static const unsigned short int kBaseAddrChars  = 0xd000;
//    static const unsigned short int kBaseAddrBitmap = 0x0000;
//    static const unsigned short int kBaseAddrColorRAM = 0xd800;
//    static const unsigned short int kAddrResetVector = 0xfffc;
//    static const unsigned short int kAddrIRQVector = 0xfffe;
//    static const unsigned short int kAddrNMIVector = 0xfffa;
//    static const unsigned short int kAddrDataDirection = 0x0000;
//    static const unsigned short int kAddrMemoryLayout  = 0x0001;
//    static const unsigned short int kAddrColorRAM = 0xd800;
//    //memory layout
//    static const unsigned short int kAddrZeroPage     = 0x0000;
//    static const unsigned short int kAddrVicFirstPage = 0xd000;
//    static const unsigned short int kAddrVicLastPage  = 0xd300;
//    static const unsigned short int kAddrCIA1Page = 0xdc00;
//    static const unsigned short int kAddrCIA2Page = 0xdd00;    
//    static const unsigned short int kAddrBasicFirstPage = 0xa000; 
//    static const unsigned short int kAddrBasicLastPage  = 0xbf00;
//    static const unsigned short int kAddrKernalFirstPage = 0xe000;
//    static const unsigned short int kAddrKernalLastPage = 0xff00;
//    static const unsigned short int kAddrSIDPage = 0xd400;
//    //bank switching
//    static const unsigned char kLORAM  = 1 << 0;
//    static const unsigned char kHIRAM  = 1 << 1;
//    static const unsigned char kCHAREN = 1 << 2;
};

*/


 void MEMORY_constructor();
 void MEMORY_destroy();
    void MEMORY_JJ_reset();
    void MEMORY_setup_memory_banks(unsigned char v);
    //read/write memory
    unsigned char MEMORY_read_byte(unsigned short int addr);
    //unsigned char MEMORY_read_byte_no_io(unsigned short int addr);
    void MEMORY_write_byte(unsigned short int addr, unsigned char v);
    //void MEMORY_write_byte_no_io(unsigned short int addr, unsigned char v);
    unsigned short int MEMORY_read_word(unsigned short int addr);
    //inline unsigned short int MEMORY_read_word_no_io(unsigned short int addr);
    void MEMORY_write_word(unsigned short int addr, unsigned short int v);
    void MEMORY_write_word_no_io(unsigned short int addr, unsigned short int v);
    //vic memory access
    unsigned char MEMORY_vic_read_byte(unsigned short int addr);
    unsigned char MEMORY_read_byte_rom(unsigned short int addr);
    //load external binaries
    //void MEMORY_load_rom(const std::string &f, unsigned short int baseaddr);
    //void MEMORY_load_ram(const std::string &f, unsigned short int baseaddr); //No se necesita
    //debug
    //void MEMORY_dump(); //Sobra
    void MEMORY_print_screen_text(); 



#endif
