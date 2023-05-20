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
#ifndef EMUDORE_CIA2_H
#define EMUDORE_CIA2_H

#include "gbConfig.h"
#include "io.h"
#include "cpu.h"


#define kModeProcessor 0
#define kModeCNT 1
#define kModeTimerA 2
#define kModeTimerACNT 3

#define kModeRestart 0
#define kModeOneTime 1

//
// @brief MOS 6526 Complex Interface Adapter #2
//
// - Memory area : $DD00-$DDFF
// - Tasks       : Serial bus, RS-232, VIC banking, NMI control
//

/*No quiero objetos
class Cia2
{
  private:
    Cpu *CIA2_cpu_;
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
  public:
    Cia2();
    void CIA2_cpu(Cpu *v){ CIA2_cpu_ = v;};
    void CIA2_write_register(unsigned char r, unsigned char v);
    unsigned char CIA2_read_register(unsigned char r);
    void CIA2_reset_timer_a();
    void CIA2_reset_timer_b();
    unsigned short int CIA2_vic_base_address();
    bool CIA2_emulate();
    //constants
    //enum kInputMode //No quiero enum
    //{
    //  kModeProcessor,
    //  kModeCNT,
    //  kModeTimerA,
    //  kModeTimerACNT
    //};
    //enum kRunMode
    //{
    //  kModeRestart,
    //  kModeOneTime
    //};                        
};
*/


 void CIA2_constructor(void);
 //void CIA2_cpu(Cpu *v); //No objetos
 void CIA2_write_register(unsigned char r, unsigned char v);
 unsigned char CIA2_read_register(unsigned char r);
 void CIA2_reset_timer_a(void);
 void CIA2_reset_timer_b(void);
 //unsigned short int CIA2_vic_base_address(void);
 //bool CIA2_emulate(void);
 void CIA2_emulate(void);
    

#endif
