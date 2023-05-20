// Mod by ackerman (convert C, refactor, optimice)
// Modificacion del codigo de Scott Hutter's
//
// emudore, Commodore 64 emulator
// Copyright (c) 2016, Mario Ballano <mballano@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
 
#ifndef EMUDORE_SID_H
#define EMUDORE_SID_H

//
//  @brief MOS 6581 SID (Sound Interface Device)
// 
//  - Memory area : $D400-$D7FF
//  - Tasks       : Sound
 
//class Sid
//{
//  //JJ private:
//   //JJ myos::drivers::SpeakerDriver *speaker_;
//  public:
//   unsigned char volume;
//   unsigned char freqLo;
//   unsigned char freqHi;
//   unsigned int frequency;          
//            
//   //Sid();
//   //~Sid();
//   //JJ void speaker(myos::drivers::SpeakerDriver *m) { speaker_ = m; };
//    
//   void write_register(unsigned char r, unsigned char v);
//   unsigned int getFrequency(unsigned char hi, unsigned char lo);
//   void play();         
//};


void jj_SID_Init(void);
void jj_SID_write_register(unsigned char r, unsigned char v);
unsigned int jj_SID_getFrequency(unsigned char hi, unsigned char lo);
void jj_SID_play(unsigned char canal);
void jj_sid_mute(void);
void jj_sound_mute(void);
void jj_sound_no_mute(void);

#endif

