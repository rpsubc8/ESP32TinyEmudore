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

#ifndef EMUDORE_LOADER_H
#define EMUDORE_LOADER_H

#include "gbConfig.h"
#include <fstream>
#include "c64.h"

//JJ Aniadido
#include<iostream>
#include<math.h>
#include<fstream>
//JJ Aniadido

#define kNone 0
#define kBasic 1
#define kPRG 2

#define LOADER_kBasicPrgStart 0x0801
#define LOADER_kBasicTxtTab 0x002b
#define LOADER_kBasicVarTab 0x002d
#define LOADER_kBasicAryTab 0x002f
#define LOADER_kBasicStrEnd 0x0031


/* Fuera objetos
//
// @brief Program loader
//
class Loader
{
  private:
    bool LOADER_booted_up_;
    C64 *LOADER_c64_;
    //IO *io_; //No quiero objetos
    Cpu *LOADER_cpu_;
    Memory *LOADER_mem_;
    std::ifstream LOADER_is_;
    //JJ enum LOADER_kFormat
    //JJ {
    //JJ   kNone,
    //JJ   kBasic,
    //JJ   kPRG
    //JJ };
    //JJ LOADER_kFormat LOADER_format_;
    unsigned char LOADER_format_; //No quiero enums
    void LOADER_load_basic();
    void LOADER_load_prg();
    uint16_t LOADER_read_short_le();
  public:
    Loader(C64 *c64);
    void LOADER_bas(const std::string &f);
    void LOADER_prg(const std::string &f);
    void LOADER_load_prg_hack_jj(const char *cadFich); //Hack JJ
    bool LOADER_emulate();
    // constants
    //JJ static const uint16_t LOADER_kBasicPrgStart = 0x0801;
    //JJ static const uint16_t LOADER_kBasicTxtTab   = 0x002b;
    //JJ static const uint16_t LOADER_kBasicVarTab   = 0x002d; 
    //JJ static const uint16_t LOADER_kBasicAryTab   = 0x002f;
    //JJ static const uint16_t LOADER_kBasicStrEnd   = 0x0031;        
};

*/




 //Solo estructurado
 void LOADER_load_basic();
 void LOADER_load_prg();
 unsigned short int LOADER_read_short_le(void);
 //void Loader_constructor(C64 *c64);
 void Loader_constructor();
 void LOADER_bas(const std::string &f);
 void LOADER_prg(const std::string &f);
 void LOADER_load_prg_hack_jj(const char *cadFich); //Hack JJ
 bool LOADER_emulate(void);

#endif
