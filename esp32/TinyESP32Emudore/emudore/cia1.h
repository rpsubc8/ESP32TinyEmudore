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
#ifndef EMUDORE_CIA1_H
#define EMUDORE_CIA1_H

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
// @brief MOS 6526 Complex Interface Adapter #1
//
// - Memory area : $DC00-$DCFF
// - Tasks       : Keyboard, Joystick, Paddles, Datasette, IRQ control
//

/* No quiero objetos
class Cia1
{
  private:
    Cpu *CIA1_cpu_;
    //IO *io_; //no quiero objetos
    //int16_t timer_a_latch_;
    //int16_t timer_b_latch_;
    //int16_t timer_a_counter_;
    //int16_t timer_b_counter_;
    short int CIA1_timer_a_latch_;
    short int CIA1_timer_b_latch_;
    short int CIA1_timer_a_counter_;
    short int CIA1_timer_b_counter_;    
    bool CIA1_timer_a_enabled_;
    bool CIA1_timer_b_enabled_;
    bool CIA1_timer_a_irq_enabled_;
    bool CIA1_timer_b_irq_enabled_;
    bool CIA1_timer_a_irq_triggered_;
    bool CIA1_timer_b_irq_triggered_;
    //uint8_t timer_a_run_mode_;
    //uint8_t timer_b_run_mode_;
    //uint8_t timer_a_input_mode_;
    //uint8_t timer_b_input_mode_;
    unsigned char CIA1_timer_a_run_mode_;
    unsigned char CIA1_timer_b_run_mode_;
    unsigned char CIA1_timer_a_input_mode_;
    unsigned char CIA1_timer_b_input_mode_;    
    unsigned int CIA1_prev_cpu_cycles_;
    //uint8_t pra_, prb_;
    unsigned char CIA1_pra_, CIA1_prb_;
  public:
    Cia1();
    void CIA1_cpu(Cpu *v){ CIA1_cpu_ = v;};
    //void io(IO *v){ io_ = v;}; //No quiero objetos
    void CIA1_write_register(uint8_t r, uint8_t v);
    //uint8_t read_register(uint8_t r);
    unsigned char CIA1_read_register(uint8_t r);
    void CIA1_reset_timer_a();
    void CIA1_reset_timer_b();
    bool CIA1_emulate();
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


 void CIA1_constructor();
 //void CIA1_cpu(Cpu *v); //No objetos
 void CIA1_write_register(unsigned char r, unsigned char v);
 unsigned char CIA1_read_register(unsigned char r);
 void CIA1_reset_timer_a(void);
 void CIA1_reset_timer_b(void);
 //bool CIA1_emulate(void);
 void CIA1_emulate(void);
    
#endif
