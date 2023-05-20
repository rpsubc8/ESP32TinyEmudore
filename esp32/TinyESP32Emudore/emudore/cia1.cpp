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
#include "cia1.h"
#include "gbGlobals.h"
#include "Arduino.h"

//ctor

//Cia1::Cia1()
void CIA1_constructor()
{
  CIA1_timer_a_latch_ = CIA1_timer_b_latch_ = CIA1_timer_a_counter_ = CIA1_timer_b_counter_ = 0;
  CIA1_timer_a_enabled_ = CIA1_timer_b_enabled_ = CIA1_timer_a_irq_enabled_ = CIA1_timer_b_irq_enabled_ = false;
  CIA1_timer_a_irq_triggered_ = CIA1_timer_b_irq_triggered_ = false;
  CIA1_timer_a_input_mode_ = CIA1_timer_b_input_mode_ = kModeProcessor;
  CIA1_timer_a_run_mode_ = CIA1_timer_b_run_mode_ = kModeRestart;
  CIA1_pra_ = CIA1_prb_ = 0xff;
  CIA1_prev_cpu_cycles_ = 0;
  
  Serial.printf("CIA1_constructor\r\n");
  //fflush(stdout);
}

//DMA register access

//void Cia1::CIA1_write_register(uint8_t r, uint8_t v)
void CIA1_write_register(unsigned char r, unsigned char v)
{
 switch(r)
 {
  //data port a (PRA), keyboard matrix cols and joystick #2 
  case 0x0:
    CIA1_pra_ = v;
    break;
  //data port b (PRB), keyboard matrix rows and joystick #1
  case 0x1:
    break;
  //data direction port a (DDRA)
  case 0x2:
    break;
  //data direction port b (DDRB)
  case 0x3:
    break;
  //timer a low byte
  case 0x4:
    CIA1_timer_a_latch_ &= 0xff00;
    CIA1_timer_a_latch_ |= v;
    break;
  //timer a high byte
  case 0x5:
    CIA1_timer_a_latch_ &= 0x00ff;
    CIA1_timer_a_latch_ |= v << 8;
    break;
  //timer b low byte
  case 0x6:
    CIA1_timer_b_latch_ &= 0xff00;
    CIA1_timer_b_latch_ |= v;
    break;
  //timer b high byte
  case 0x7: 
    CIA1_timer_b_latch_ &= 0x00ff;
    CIA1_timer_b_latch_ |= v << 8;
    break;
  //RTC 1/10s
  case 0x8:
    break;
  //RTC seconds
  case 0x9:
    break;
  //RTC minutes
  case 0xa:
    break;
  //RTC hours
  case 0xb:
    break;
  //shift serial
  case 0xc:
    break;
  //interrupt control and status
  case 0xd:
    //
    // if bit 7 is set, enable selected mask of 
    // interrupts, else disable them
    //
    if(ISSET_BIT(v,0)) CIA1_timer_a_irq_enabled_ = ISSET_BIT(v,7);
    if(ISSET_BIT(v,1)) CIA1_timer_b_irq_enabled_ = ISSET_BIT(v,7);
    break;
  //control timer a
  case 0xe:
    CIA1_timer_a_enabled_ = ((v&(1<<0))!=0);
    CIA1_timer_a_input_mode_ = (v&(1<<5)) >> 5;
    //load latch requested
    if((v&(1<<4))!=0){
      CIA1_timer_a_counter_ = CIA1_timer_a_latch_;
    }
    break;
  //control timer b
  case 0xf:
    CIA1_timer_b_enabled_ = ((v&0x1)!=0);
    CIA1_timer_b_input_mode_ = (v&(1<<5)) | (v&(1<<6)) >> 5;
    //load latch requested
    if((v&(1<<4))!=0){
      CIA1_timer_b_counter_ = CIA1_timer_b_latch_;
    }
    break;
 }
}

//uint8_t Cia1::read_register(uint8_t r)
//unsigned char Cia1::CIA1_read_register(uint8_t r)
unsigned char CIA1_read_register(unsigned char r)
{
  uint8_t retval = 0;

  switch(r)
  {
  //data port a (PRA), keyboard matrix cols and joystick #2
  case 0x0:
    break;
  //data port b (PRB), keyboard matrix rows and joystick #1
  case 0x1:
    if (CIA1_pra_ == 0xff) retval = 0xff;
    else if(CIA1_pra_)
    {
      int col = 0;
      uint8_t v = ~CIA1_pra_;
      while (v >>= 1) { col++; }
      //retval = io_->IO_keyboard_matrix_row(col);
      retval = IO_keyboard_matrix_[col]; //IO_keyboard_matrix_row(col) //no quiero objetos
    }
    break;
  //data direction port a (DDRA)
  case 0x2:
    break;
  //data direction port b (DDRB)
  case 0x3:
    break;
  //timer a low byte
  case 0x4:
    retval = (uint8_t)(CIA1_timer_a_counter_ & 0x00ff);
    break;
  //timer a high byte
  case 0x5:
    retval = (uint8_t)((CIA1_timer_a_counter_ & 0xff00) >> 8);
    break;
  //timer b low byte
  case 0x6:
    retval = (uint8_t)(CIA1_timer_b_counter_ & 0x00ff);
    break;
  //timer b high byte
  case 0x7: 
    retval = (uint8_t)((CIA1_timer_b_counter_ & 0xff00) >> 8);
    break;
  //RTC 1/10s
  case 0x8:
    break;
  // RTC seconds
  case 0x9:
    break;
  // RTC minutes
  case 0xa:
    break;
  //RTC hours
  case 0xb:
    break;
  //shift serial
  case 0xc:
    break;
  //interrupt control and status
  case 0xd:
    if(CIA1_timer_a_irq_triggered_ ||
       CIA1_timer_b_irq_triggered_)
    {
      retval |= (1 << 7); // IRQ occured
      if(CIA1_timer_a_irq_triggered_) retval |= (1 << 0);
      if(CIA1_timer_b_irq_triggered_) retval |= (1 << 1);
    }
    break;
  //control timer a
  case 0xe:
    break;
  //control timer b
  case 0xf:
    break;
  }
  return retval;
}

//timer reset

//void Cia1::CIA1_reset_timer_a()
void CIA1_reset_timer_a()
{
 switch(CIA1_timer_a_run_mode_)
 {
  case kModeRestart: CIA1_timer_a_counter_ = CIA1_timer_a_latch_; break;
  case kModeOneTime: CIA1_timer_a_enabled_ = false; break;  
 } 
}

//void Cia1::CIA1_reset_timer_b()
void CIA1_reset_timer_b()
{
 switch(CIA1_timer_b_run_mode_)
 {
  case kModeRestart:
    CIA1_timer_b_counter_ = CIA1_timer_b_latch_;
    break;
  case kModeOneTime:
    CIA1_timer_b_enabled_ = false;
    break;
 }      
}

//emulation

//bool Cia1::CIA1_emulate()
//bool CIA1_emulate()
void CIA1_emulate()
{ 
 //timer a
 if(CIA1_timer_a_enabled_)
 {
  switch(CIA1_timer_a_input_mode_)
  {
   case kModeProcessor:
      //CIA1_timer_a_counter_ -= CIA1_cpu_->CPU_cycles() - CIA1_prev_cpu_cycles_; //No objetos
      CIA1_timer_a_counter_ -= CPU_cycles_ - CIA1_prev_cpu_cycles_;//CIA1_timer_a_counter_ -= CPU_cycles_get() - CIA1_prev_cpu_cycles_;
      if (CIA1_timer_a_counter_ <= 0)
      {
        if(CIA1_timer_a_irq_enabled_) 
        {
          CIA1_timer_a_irq_triggered_ = true;
          //CIA1_cpu_->CPU_irq(); //No objetos
          if(!CPU_idf_)
          {
           CPU_irq();
          }
        }
        CIA1_reset_timer_a();
      }
      break;
   case kModeCNT:
      break;
   }
 }
 //timer b
 if(CIA1_timer_b_enabled_)
 {
  switch(CIA1_timer_b_input_mode_)
  {
   case kModeProcessor:
      //CIA1_timer_b_counter_ -= CIA1_cpu_->CPU_cycles() - CIA1_prev_cpu_cycles_;
      CIA1_timer_b_counter_ -= CPU_cycles_ - CIA1_prev_cpu_cycles_; //No objetos //CIA1_timer_b_counter_ -= CPU_cycles_get() - CIA1_prev_cpu_cycles_; //No objetos
      if (CIA1_timer_b_counter_ <= 0)
      {
        if(CIA1_timer_b_irq_enabled_)
        {
          CIA1_timer_b_irq_triggered_ = true;
          //CIA1_cpu_->CPU_irq();
          if(!CPU_idf_)
          {
           CPU_irq(); //No objetos
          }
        }
        CIA1_reset_timer_b();
      }   
      break;
   case kModeCNT:
      break;
   case kModeTimerA:
      break;
   case kModeTimerACNT:
      break;
  }
 }
 //CIA1_prev_cpu_cycles_ = CIA1_cpu_->CPU_cycles();
 CIA1_prev_cpu_cycles_ = CPU_cycles_; //CIA1_prev_cpu_cycles_ = CPU_cycles_get(); //No objetos
 //return true;
}


//void CIA1_cpu(Cpu *v) //No objetos
//{ 
// CIA1_cpu_ = v;
//};
