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
#include "loader.h"
#include "gbGlobals.h"

//JJ Aniadido
#include<iostream>
#include<math.h>
#include<fstream>
#include "Arduino.h"

using namespace std;
//JJ Aniadido

//Loader::Loader(C64 *c64)
//void Loader_constructor(C64 *c64)
void Loader_constructor()
{
  Serial.printf("BEGIN constructor Loader\r\n");
  //fflush(stdout);
                   
  //LOADER_c64_ = c64; //No objetos
  //io_  = c64_->io(); //no quiero objetos
  // LOADER_cpu_ = LOADER_c64_->C64_cpu(); //No quiero objetos
  //LOADER_mem_ = LOADER_c64_->C64_memory(); //No objetos
  
  LOADER_booted_up_ = false;
  LOADER_format_ = kNone;

  Serial.printf("END constructor Loader\r\n");
  //fflush(stdout);  
}
// common ///////////////////////////////////////////////////////////////////

//uint16_t Loader::LOADER_read_short_le()
unsigned short int LOADER_read_short_le()
{
  char b;
  uint16_t v = 0;
  LOADER_is_.get(b);
  v |= (b);
  LOADER_is_.get(b);
  v |= b << 8;
  return v;
}

// BASIC listings ///////////////////////////////////////////////////////////

//void Loader::LOADER_bas(const std::string &f)
void LOADER_bas(const std::string &f)
{
  LOADER_format_ = kBasic; 
//JJ  is_.open(f,std::ios::in);
}

//void Loader::LOADER_load_basic()
void LOADER_load_basic()
{
  char c;
  if(LOADER_is_.is_open())
  {
    while(LOADER_is_.good())
    {
      LOADER_is_.get(c);
      //JJ io_->IO::type_character(c);
    }
  }
}

// PRG //////////////////////////////////////////////////////////////////////

//void Loader::LOADER_prg(const std::string &f)
void LOADER_prg(const std::string &f)
{
 const char *cstr = f.c_str();
 Serial.printf("Loader::prgprg(string) %s\r\n",cstr);
 //fflush(stdout);
  
  LOADER_format_ = kPRG;
  //JJis_.open(f,std::ios::in|std::ios::binary);
  
  LOADER_load_prg_hack_jj("monopole.prg"); //Hack JJ
}

//void Loader::LOADER_load_prg_hack_jj(const char *cadFich)
void LOADER_load_prg_hack_jj(const char *cadFich)
{
  char b;
  uint16_t pbuf, addr;
  unsigned char a0,a1;
  
  FILE * fich = fopen(cadFich,"rb");
  if (fich!=NULL)
  {
   fseek(fich, 0L, SEEK_END);
   unsigned int lon= ftell(fich);
  
   fseek(fich, 0L, SEEK_SET);
   fread(&a0,1,1,fich);
   fread(&a1,1,1,fich);
   addr= (a1<<8)|a0;
          
   pbuf = addr = addr;
  
   Serial.printf("Loader::load_prg prg %s len:%d addr:%lu\r\n",cadFich,lon,addr);
   //fflush(stdout);     
  
   for (unsigned int cont=2;cont<lon;cont++)
   {
    fread(&b,1,1,fich);
    //LOADER_mem_->MEMORY_write_byte_no_io(pbuf++,b);
    MEMORY_mem_ram_[pbuf++]= b; //MEMORY_write_byte_no_io(pbuf++,b); //No objetos
   }

   if(addr == LOADER_kBasicPrgStart)
   {
      // make BASIC happy
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicTxtTab,LOADER_kBasicPrgStart);
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicVarTab,pbuf);
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicAryTab,pbuf);
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicStrEnd,pbuf);
      MEMORY_write_word_no_io(LOADER_kBasicTxtTab,LOADER_kBasicPrgStart);//No objetos
      MEMORY_write_word_no_io(LOADER_kBasicVarTab,pbuf);
      MEMORY_write_word_no_io(LOADER_kBasicAryTab,pbuf);
      MEMORY_write_word_no_io(LOADER_kBasicStrEnd,pbuf);      
      // exec RUN
//JJ      for(char &c: std::string("RUN\n"))
//JJ      {
//JJ        io_->IO::type_character(c);
//JJ      }
    }
    // ML
    else
    {
     //LOADER_cpu_->CPU_pc(addr); //No quiero CPU
     CPU_pc_= addr; //CPU_pc_set(addr)
    }
   

  
  fclose(fich);
 }
}

//void Loader::LOADER_load_prg()
void LOADER_load_prg()
{    
  char b;
  uint16_t pbuf, addr;
  pbuf = addr = LOADER_read_short_le();
  
  Serial.printf("Loader::load_prg prg addr:%lu\r\n",addr);
  //fflush(stdout);     
   
  if(LOADER_is_.is_open())
  {
    while(LOADER_is_.good())
    {
      LOADER_is_.get(b);
      //LOADER_mem_->MEMORY_write_byte_no_io(pbuf++,b);
      MEMORY_mem_ram_[pbuf++]= b; //MEMORY_write_byte_no_io(pbuf++,b); //No objetos
    }
    // basic-tokenized prg
    if(addr == LOADER_kBasicPrgStart)
    {
      // make BASIC happy
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicTxtTab,LOADER_kBasicPrgStart);
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicVarTab,pbuf);
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicAryTab,pbuf);
      //LOADER_mem_->MEMORY_write_word_no_io(LOADER_kBasicStrEnd,pbuf);
      MEMORY_write_word_no_io(LOADER_kBasicTxtTab,LOADER_kBasicPrgStart); //Objetos
      MEMORY_write_word_no_io(LOADER_kBasicVarTab,pbuf);
      MEMORY_write_word_no_io(LOADER_kBasicAryTab,pbuf);
      MEMORY_write_word_no_io(LOADER_kBasicStrEnd,pbuf);      
      // exec RUN
//JJ      for(char &c: std::string("RUN\n"))
//JJ      {
//JJ        io_->IO::type_character(c);
//JJ      }
    }
    // ML
    else
    {
     //LOADER_cpu_->CPU_pc(addr); //No quiero objetos
     CPU_pc_= addr; //CPU_pc_set(addr)
    }
  }
}
 
// emulate //////////////////////////////////////////////////////////////////

//bool Loader::LOADER_emulate()
bool LOADER_emulate()
{
  if(LOADER_booted_up_)
  {
    switch(LOADER_format_)
    {
    case kBasic:
      Serial.printf("format BASIC\r\n");
      //fflush(stdout); 
      LOADER_load_basic();
      break;
    case kPRG:
      Serial.printf("format kPRG\r\n");
      //fflush(stdout);          
      LOADER_load_prg();
      break;
    default: 
      break;
    }
    return false;
  }
  else
  {
    // at this point BASIC is ready
    //if(LOADER_cpu_->CPU_pc() == 0xa65c) //No quiero objetos
    //if(CPU_pc_get() == 0xa65c)
    if(CPU_pc_ == 0xa65c)
    {
      LOADER_booted_up_ = true;
    }
  }
  return true;
}
