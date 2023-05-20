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
#include "cia2.h"
#include "gbGlobals.h"
#include "Arduino.h"

// ctor  /////////////////////////////////////////////////////////////////////

//Cia2::Cia2()
void CIA2_constructor()
{
  CIA2_timer_a_latch_ = CIA2_timer_b_latch_ = CIA2_timer_a_counter_ = CIA2_timer_b_counter_ = 0;
  CIA2_timer_a_enabled_ = CIA2_timer_b_enabled_ = CIA2_timer_a_irq_enabled_ = CIA2_timer_b_irq_enabled_ = false;
  CIA2_timer_a_irq_triggered_ = CIA2_timer_b_irq_triggered_ = false;
  CIA2_timer_a_input_mode_ = CIA2_timer_b_input_mode_ = kModeProcessor;
  CIA2_timer_a_run_mode_ = CIA2_timer_b_run_mode_ = kModeRestart;
  CIA2_pra_ = CIA2_prb_ = 0xff;
  CIA2_prev_cpu_cycles_ = 0;
  
  Serial.printf("CIA2_constructor\r\n");
  //fflush(stdout);
}

// DMA register access

//void Cia2::CIA2_write_register(unsigned char r, unsigned char v)
void CIA2_write_register(unsigned char r, unsigned char v)
{
  switch(r)
  {
  //data port a (PRA)
  case 0x0:
    CIA2_pra_ = v;
    break;
  //data port b (PRB)
  case 0x1:
    CIA2_prb_ = v;
    break;
  /* data direction port a (DDRA) */
  case 0x2:
    break;
  /* data direction port b (DDRB) */
  case 0x3:
    break;
  /* timer a low byte */
  case 0x4:
    CIA2_timer_a_latch_ &= 0xff00;
    CIA2_timer_a_latch_ |= v;
    break;
  /* timer a high byte */
  case 0x5:
    CIA2_timer_a_latch_ &= 0x00ff;
    CIA2_timer_a_latch_ |= v << 8;
    break;
  /* timer b low byte */
  case 0x6:
    CIA2_timer_b_latch_ &= 0xff00;
    CIA2_timer_b_latch_ |= v;
    break;
  /* timer b high byte */
  case 0x7: 
    CIA2_timer_b_latch_ &= 0x00ff;
    CIA2_timer_b_latch_ |= v << 8;
    break;
  /* RTC 1/10s  */
  case 0x8:
    break;
  /* RTC seconds */
  case 0x9:
    break;
  /* RTC minutes */
  case 0xa:
    break;
  /* RTC hours */
  case 0xb:
    break;
  /* shift serial */
  case 0xc:
    break;
  /* interrupt control and status */
  case 0xd:
    /**
     * if bit 7 is set, enable selected mask of 
     * interrupts, else disable them
     */
    if(ISSET_BIT(v,0)) CIA2_timer_a_irq_enabled_ = ISSET_BIT(v,7);
    if(ISSET_BIT(v,1)) CIA2_timer_b_irq_enabled_ = ISSET_BIT(v,7);
    break;
  /* control timer a */
  case 0xe:
    CIA2_timer_a_enabled_ = ((v&(1<<0))!=0);
    CIA2_timer_a_input_mode_ = (v&(1<<5)) >> 5;
    /* load latch requested */
    if((v&(1<<4))!=0){
      CIA2_timer_a_counter_ = CIA2_timer_a_latch_;
    }
    break;
  /* control timer b */
  case 0xf:
    CIA2_timer_b_enabled_ = ((v&0x1)!=0);
    CIA2_timer_b_input_mode_ = (v&(1<<5)) | (v&(1<<6)) >> 5;
    /* load latch requested */
    if((v&(1<<4))!=0){
      CIA2_timer_b_counter_ = CIA2_timer_b_latch_;
    }
    break;
  }
}

//unsigned char Cia2::CIA2_read_register(unsigned char r)
unsigned char CIA2_read_register(unsigned char r)
{
  unsigned char retval = 0;

  switch(r)
  {
  /* data port a (PRA) */
  case 0x0:
    retval = CIA2_pra_;
    break;
  /* data port b (PRB) */
  case 0x1:
    retval = CIA2_prb_;
    break;
  /* data direction port a (DDRA) */
  case 0x2:
    break;
  /* data direction port b (DDRB) */
  case 0x3:
    break;
  /* timer a low byte */
  case 0x4:
    retval = (uint8_t)(CIA2_timer_a_counter_ & 0x00ff);
    break;
  /* timer a high byte */
  case 0x5:
    retval = (uint8_t)((CIA2_timer_a_counter_ & 0xff00) >> 8);
    break;
  /* timer b low byte */
  case 0x6:
    retval = (uint8_t)(CIA2_timer_b_counter_ & 0x00ff);
    break;
  /* timer b high byte */
  case 0x7: 
    retval = (uint8_t)((CIA2_timer_b_counter_ & 0xff00) >> 8);
    break;
  /* RTC 1/10s  */
  case 0x8:
    break;
  /* RTC seconds */
  case 0x9:
    break;
  /* RTC minutes */
  case 0xa:
    break;
  /* RTC hours */
  case 0xb:
    break;
  /* shift serial */
  case 0xc:
    break;
  /* interrupt control and status */
  case 0xd:
    if(CIA2_timer_a_irq_triggered_ ||
       CIA2_timer_b_irq_triggered_)
    {
      retval |= (1 << 7); // IRQ occured
      if(CIA2_timer_a_irq_triggered_) retval |= (1 << 0);
      if(CIA2_timer_b_irq_triggered_) retval |= (1 << 1);
    }
    break;
  /* control timer a */
  case 0xe:
    break;
  /* control timer b */
  case 0xf:
    break;
  }
  return retval;
}

// timer reset ///////////////////////////////////////////////////////////////

//void Cia2::CIA2_reset_timer_a()
void CIA2_reset_timer_a()
{
  switch(CIA2_timer_a_run_mode_)
  {
  case kModeRestart:
    CIA2_timer_a_counter_ = CIA2_timer_a_latch_;
    break;
  case kModeOneTime:
    CIA2_timer_a_enabled_ = false;
    break;
  }
}

//void Cia2::CIA2_reset_timer_b()
void CIA2_reset_timer_b()
{
  switch(CIA2_timer_b_run_mode_)
  {
  case kModeRestart:
    CIA2_timer_b_counter_ = CIA2_timer_b_latch_;
    break;
  case kModeOneTime:
    CIA2_timer_b_enabled_ = false;
    break;
  }      
}

// VIC banking ///////////////////////////////////////////////////////////////

/**
 * @brief retrieves vic base address
 *
 * PRA bits (0..1)
 *
 *  %00, 0: Bank 3: $C000-$FFFF, 49152-65535
 *  %01, 1: Bank 2: $8000-$BFFF, 32768-49151
 *  %10, 2: Bank 1: $4000-$7FFF, 16384-32767
 *  %11, 3: Bank 0: $0000-$3FFF, 0-16383 (standard)
 */
//unsigned short int Cia2::CIA2_vic_base_address()
//unsigned short int CIA2_vic_base_address()
//{
//  return ((~CIA2_pra_&0x3) << 14);
//}

// emulation  ////////////////////////////////////////////////////////////////

//bool Cia2::CIA2_emulate()
//bool CIA2_emulate()
void CIA2_emulate()
{
  //timer a
  if(CIA2_timer_a_enabled_)
  {
    switch(CIA2_timer_a_input_mode_)
    {
    case kModeProcessor:
      //CIA2_timer_a_counter_ -= CIA2_cpu_->CPU_cycles() - CIA2_prev_cpu_cycles_; //No objetos
      CIA2_timer_a_counter_ -= CPU_cycles_ - CIA2_prev_cpu_cycles_;//CIA2_timer_a_counter_ -= CPU_cycles_get() - CIA2_prev_cpu_cycles_;
      if (CIA2_timer_a_counter_ <= 0)
      {
        if(CIA2_timer_a_irq_enabled_) 
        {
          CIA2_timer_a_irq_triggered_ = true;
          //CIA2_cpu_->CPU_nmi(); //No objetos
          CPU_nmi();
        }
        CIA2_reset_timer_a();
      }
      break;
    case kModeCNT:
      break;
    }
  }
  //timer b
  if(CIA2_timer_b_enabled_)
  {
    switch(CIA2_timer_b_input_mode_)
    {
    case kModeProcessor:
      //CIA2_timer_b_counter_ -= CIA2_cpu_->CPU_cycles() - CIA2_prev_cpu_cycles_; //No objetos
      CIA2_timer_b_counter_ -= CPU_cycles_ - CIA2_prev_cpu_cycles_;//CIA2_timer_b_counter_ -= CPU_cycles_get() - CIA2_prev_cpu_cycles_;
      if (CIA2_timer_b_counter_ <= 0)
      {
        if(CIA2_timer_b_irq_enabled_)
        {
          CIA2_timer_b_irq_triggered_ = true;
          //CIA2_cpu_->CPU_nmi();
          CPU_nmi(); //No objetos
        }
        CIA2_reset_timer_b();
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
  //CIA2_prev_cpu_cycles_ = CIA2_cpu_->CPU_cycles(); //No objetos
  CIA2_prev_cpu_cycles_ = CPU_cycles_;//CIA2_prev_cpu_cycles_ = CPU_cycles_get();
  //return true;
}


//void CIA2_cpu(Cpu *v) //No objetos
//{ 
// CIA2_cpu_ = v;
//};
