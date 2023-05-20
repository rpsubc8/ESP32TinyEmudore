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
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// 

#ifndef EMUDORE_C64_H
#define EMUDORE_C64_H

#include <functional>

#include "cpu.h"
#include "memory.h"
#include "cia1.h"
#include "cia2.h"
#include "vic.h"
#include "sid.h"
#include "io.h"

//#ifdef DEBUGGER_SUPPORT
//#include "debugger.h"
//#endif

/* 
//
// @brief Commodore 64
//  
// This class glues together all the different
// components in a Commodore 64 computer
//
class C64
{
  private:
    //Cpu *C64_cpu_; //No objetos
    //Memory *C64_mem_; //No quiero objetos
    //Cia1 *cia1_; //no quiero objetos
    //Cia2 *cia2_; //no quiero objetos
    //Vic *C64_vic_; //no quiero objetos
    //Sid *sid_; //No quiero objetos
    //IO *io_;  //No quiero objetos
    //JJ std::function<bool()> callback_;
//#ifdef DEBUGGER_SUPPORT
//    Debugger *debugger_;
//#endif
  public:
    C64();
    ~C64();
    void C64_start();
    void C64_emscripten_loop();
    //JJJ void callback(std::function<bool()> cb){callback_ = cb;};
    //Cpu * C64_cpu(); //JJ{return C64_cpu_;}; //No objetos
    //Memory * C64_memory(); //JJ{return C64_mem_;};
    //IO * io(){return io_;}; //no quiero objetos
    //test cpu
    void C64_test_cpu();
    void C64_leeme_prg_hack(const char *cadFich); //Hack
};

*/


 void C64_constructor();
 void C64_destroy();
 void C64_start();
 //void C64_emscripten_loop();
 //test cpu
 void C64_test_cpu();
 //void C64_leeme_prg_hack(const char *cadFich); //Hack
 void C64_leeme_prg_hack(unsigned short int id); //Hack



 void sid_poll(void);
 void vga_dump_poll(void);

#endif
