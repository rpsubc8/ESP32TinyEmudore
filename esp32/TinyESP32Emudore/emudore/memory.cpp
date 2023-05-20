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
#include "gbGlobals.h"
#include <fstream>
#include "memory.h"
#include "vic.h"
#include "cia1.h"
#include "cia2.h"
#include "sid.h"
//Para el memset
#include <string.h>
#include "Arduino.h"


unsigned char jj_memory_rom_read(unsigned short int addr,unsigned char aRom, unsigned char aBank)
{//1927000  llamadas 1 segundo
 //gb_cont_call++;
         
 //retval = jj_memory_rom_read(addr,kROM,kBankCharen);         
 unsigned char toReturn=0;
 if (aRom == kROM)
 {
  switch (aBank)
  {
   case kBankCharen: toReturn = gb_charRomC64[(addr - MEMORY_kBaseAddrChars)]; break;
   case kBankBasic: toReturn = gb_basicRomC64[(addr - MEMORY_kBaseAddrBasic)]; break;
   case kBankKernal: toReturn = gb_kernalRomC64[(addr - MEMORY_kBaseAddrKernal)]; break;
  }
 }
 else
 {
  if (aRom == 69)
  {
    toReturn= gb_charRomC64[addr - MEMORY_kBaseAddrChars];// - MEMORY_kBaseAddrChars];           
  }
 }
 
 return toReturn;
}

//void Memory::MEMORY_JJ_reset()
void MEMORY_JJ_reset()
{//Se controla en la carga, no hay casi llamadas
 //gb_cont_call++;
 
 //configure memory layout
 MEMORY_setup_memory_banks(MEMORY_kLORAM|MEMORY_kHIRAM|MEMORY_kCHAREN);
 //configure data directional bits
 MEMORY_mem_ram_[MEMORY_kAddrDataDirection]= 0x2f; //MEMORY_write_byte_no_io(MEMORY_kAddrDataDirection,0x2f);                  
}

//Memory::Memory()
void MEMORY_constructor()
{
  //
  // 64 kB memory buffers, zeroed.
  //
  // We use two buffers to handle special circumstances, for instance,
  // any write to a ROM-mapped location will in turn store data on the 
  // hidden RAM, this trickery is used in certain graphic modes.
  //
  //mem_ram_ = new unsigned char[kMemSize]();
  //mem_rom_ = new unsigned char[kMemSize]();
  
  //MEMORY_mem_ram_ = (unsigned char *)malloc(MEMORY_kMemSize); //Ya lo hace el SetupMemory
  //MEMORY_mem_rom_ = (unsigned char *)malloc(MEMORY_kMemSize);
  
  Serial.printf("MEMORY_constructor Memory call setup_memory_banks\r\n");
  //fflush(stdout);
  //Reset all RAM
  memset(MEMORY_mem_ram_,0,MEMORY_kMemSize);
  //memset(MEMORY_mem_rom_,0,MEMORY_kMemSize);
  Serial.printf("Set 0 all RAM and ROM (not set)\r\n");
  //fflush(stdout);
  
      
  //configure memory layout
  MEMORY_setup_memory_banks(MEMORY_kLORAM|MEMORY_kHIRAM|MEMORY_kCHAREN);
  //configure data directional bits
  MEMORY_mem_ram_[MEMORY_kAddrDataDirection]= 0x2f; //MEMORY_write_byte_no_io(MEMORY_kAddrDataDirection,0x2f);
}

//Memory::~Memory()
void MEMORY_destroy()
{
/* No lo necesitamos  
  //delete [] mem_ram_;
  //delete [] mem_rom_;
  free(MEMORY_mem_ram_);
  //free(MEMORY_mem_rom_);  
  
  printf("MEMORY_destroy free MEMORY_mem_ram_\n");
  fflush(stdout);
*/  
}

//
// @brief configure memory banks
//
// There are five latch bits that control the configuration allowing
// for a total of 32 different memory layouts, for now we only take
// in count three bits : HIRAM/LORAM/CHAREN
//
//void Memory::MEMORY_setup_memory_banks(unsigned char v)
void MEMORY_setup_memory_banks(unsigned char v)
{//96 veces 1 segundo
 //gb_cont_call++;     
     
 #ifdef use_lib_log_setup_memory_banks
  Serial.printf("setup_memory_banks\r\n");
  //fflush(gb_fichLog);
 #endif 
     
  //get config bits
  unsigned char hiram  = ((v&MEMORY_kHIRAM) != 0); //bool hiram  = ((v&MEMORY_kHIRAM) != 0);
  unsigned char loram  = ((v&MEMORY_kLORAM) != 0); //bool loram  = ((v&MEMORY_kLORAM) != 0);
  unsigned char charen = ((v&MEMORY_kCHAREN)!= 0); //bool charen = ((v&MEMORY_kCHAREN)!= 0);
  //init everything to ram
  for(size_t i=0 ; i < sizeof(MEMORY_banks_) ; i++)
    MEMORY_banks_[i] = kRAM;
  //load ROMs
  //JJ load_rom("basic.901226-01.bin",kBaseAddrBasic);
  //JJ load_rom("characters.901225-01.bin",kBaseAddrChars);
  //JJ load_rom("kernal.901227-03.bin",kBaseAddrKernal);
  //Falta optimizar esto, que se llama cada frame mas o menos
//  memcpy((char *) &MEMORY_mem_rom_[MEMORY_kBaseAddrBasic], gb_basicRomC64, 8192);
//  memcpy((char *) &MEMORY_mem_rom_[MEMORY_kBaseAddrChars], gb_charRomC64, 4096);
//  memcpy((char *) &MEMORY_mem_rom_[MEMORY_kBaseAddrKernal], gb_kernalRomC64, 8192);
  
  //kernal
  if (hiram)  
  {
   MEMORY_banks_[kBankKernal] = kROM;
  }
  //basic
  if (loram && hiram)  
  {
    MEMORY_banks_[kBankBasic] = kROM;
  }
  //charen
  if (charen && (loram || hiram))  
  {
    MEMORY_banks_[kBankCharen] = kIO;
  }  
  else
  {
   if (charen && !loram && !hiram)
   {
    MEMORY_banks_[kBankCharen] = kRAM;
   }
   else
   {
    MEMORY_banks_[kBankCharen] = kROM;
   }
  }
  //write the config to the zero page
  MEMORY_mem_ram_[MEMORY_kAddrMemoryLayout]= v; //MEMORY_write_byte_no_io(MEMORY_kAddrMemoryLayout, v);
}

//
// @brief writes a byte to RAM without performing I/O
//
//void Memory::MEMORY_write_byte_no_io(unsigned short int addr, unsigned char v)
//void MEMORY_write_byte_no_io(unsigned short int addr, unsigned char v)
//{
//  MEMORY_mem_ram_[addr] = v;
//}

//
// @brief writes a byte to RAM handling I/O
//
//void Memory::MEMORY_write_byte(unsigned short int addr, unsigned char v)
void MEMORY_write_byte(unsigned short int addr, unsigned char v)
{//253863  llamadas 1 segundo
  //gb_cont_call++;  
  
  unsigned short int page = addr&0xff00;
  //ZP
  if (page == MEMORY_kAddrZeroPage)
  {
    //bank switching
    if (addr == MEMORY_kAddrMemoryLayout)
    {
     #ifdef use_lib_log_write_byte_setup_memory_banks
      Serial.printf("write_byte call setup_memory_banks addr %lu\r\n",addr);
      //fflush(gb_fichLog);
     #endif 
               
     MEMORY_setup_memory_banks(v);
    }
    else
    {
      MEMORY_mem_ram_[addr] = v;
    }
  }
  //VIC-II DMA or Character ROM
  else if (page >= MEMORY_kAddrVicFirstPage && page <= MEMORY_kAddrVicLastPage)
  {
    if(MEMORY_banks_[kBankCharen] == kIO)
    {
      //MEMORY_vic_->VIC_write_register(addr&0x7f,v); //no quiero objetos
      VIC_write_register(addr&0x7f,v);
    }
    else
    {
      MEMORY_mem_ram_[addr] = v;
    }
  }
  //CIA1
  else if (page == MEMORY_kAddrCIA1Page)
  {
    if(MEMORY_banks_[kBankCharen] == kIO){
      //cia1_->CIA1_write_register(addr&0x0f,v); //no quiero objetos
      CIA1_write_register(addr&0x0f,v);
    }
    else{
      MEMORY_mem_ram_[addr] = v;
    }
  }
  else if (page == MEMORY_kAddrCIA2Page)
  {
    if(MEMORY_banks_[kBankCharen] == kIO){
      //cia2_->CIA2_write_register(addr&0x0f,v); //No quiero objetos
      CIA2_write_register(addr&0x0f,v);
    }
    else{
      MEMORY_mem_ram_[addr] = v;
    }
  }
  else if (page == MEMORY_kAddrSIDPage)
  {//SID JJ musica
   if(MEMORY_banks_[kBankCharen] == kIO){
    //sid_->write_register(addr&0xff,v);
    jj_SID_write_register(addr&0xff,v);
   }
   else{
    MEMORY_mem_ram_[addr] = v;
   }
  }  
  //default
  else
  {
    MEMORY_mem_ram_[addr] = v;
  }
}

//
// @brief reads a byte from RAM or ROM (depending on bank config)
//
//unsigned char Memory::MEMORY_read_byte(unsigned short int addr)
unsigned char MEMORY_read_byte(unsigned short int addr)
{
  unsigned char  retval = 0;
  unsigned short int page = addr&0xff00;
  //VIC-II DMA or Character ROM
  if (page >= MEMORY_kAddrVicFirstPage && page <= MEMORY_kAddrVicLastPage)
  {
    if(MEMORY_banks_[kBankCharen] == kIO)
    {
     //retval = MEMORY_vic_->VIC_read_register(addr&0x7f); //No quiero objetos
     retval = VIC_read_register(addr&0x7f);
    }
    else
    { 
     if(MEMORY_banks_[kBankCharen] == kROM)
     {                                         
      //retval = MEMORY_mem_rom_[addr];
      retval = jj_memory_rom_read(addr,kROM,kBankCharen);
     }
     else
     {
      retval = MEMORY_mem_ram_[addr];
     }
    }
  }
  //CIA1
  else if (page == MEMORY_kAddrCIA1Page)
  {
    if(MEMORY_banks_[kBankCharen] == kIO){
      //retval = cia1_->CIA1_read_register(addr&0x0f); //No quiero objetos
      retval = CIA1_read_register(addr&0x0f);
    }
    else{
      retval = MEMORY_mem_ram_[addr];
    }
  }
  //CIA2
  else if (page == MEMORY_kAddrCIA2Page)
  {
    if(MEMORY_banks_[kBankCharen] == kIO){
      //retval = cia2_->CIA2_read_register(addr&0x0f); //No quiero objetos
      retval = CIA2_read_register(addr&0x0f);
    }
    else{
      retval = MEMORY_mem_ram_[addr];
    }
  }       
  //BASIC or RAM
  else if (page >= MEMORY_kAddrBasicFirstPage && page <= MEMORY_kAddrBasicLastPage)
  {
   if (MEMORY_banks_[kBankBasic] == kROM)
   {
    //retval = MEMORY_mem_rom_[addr];
    retval = jj_memory_rom_read(addr,kROM,kBankBasic);
   }
   else
   {
    retval = MEMORY_mem_ram_[addr];
   }
  }
  //KERNAL
  else if (page >= MEMORY_kAddrKernalFirstPage && page <= MEMORY_kAddrKernalLastPage)
  {
   if (MEMORY_banks_[kBankKernal] == kROM)
   {
    //retval = MEMORY_mem_rom_[addr];
    retval = jj_memory_rom_read(addr,kROM,kBankKernal);
   }
   else
   {
    retval = MEMORY_mem_ram_[addr];
   }
  }
  //default
  else
  {
    retval = MEMORY_mem_ram_[addr];
  }
  return retval;
}

//
//@brief writes a byte without performing I/O (always to RAM)
//
//unsigned char Memory::MEMORY_read_byte_no_io(unsigned short int addr)
//unsigned char MEMORY_read_byte_no_io(unsigned short int addr)
//{
//  return MEMORY_mem_ram_[addr];
//}

//
//@brief reads a word performing I/O
//
//unsigned short int Memory::MEMORY_read_word(unsigned short int addr)
unsigned short int MEMORY_read_word(unsigned short int addr)
{
  return MEMORY_read_byte(addr) | (MEMORY_read_byte(addr+1) << 8);
}

//
//@brief reads a word withouth performing I/O
//
//unsigned short int Memory::MEMORY_read_word_no_io(unsigned short int addr)
//inline unsigned short int MEMORY_read_word_no_io(unsigned short int addr)
//{
//  //return MEMORY_read_byte_no_io(addr) | (MEMORY_read_byte_no_io(addr+1) << 8);
//  return MEMORY_mem_ram_[addr] | (MEMORY_mem_ram_[(addr+1)] << 8);
//}

// 
//@brief writes a word performing I/O
//
//void Memory::MEMORY_write_word(unsigned short int addr, unsigned short int v)
void MEMORY_write_word(unsigned short int addr, unsigned short int v)
{
  MEMORY_write_byte(addr, (unsigned char)(v));
  MEMORY_write_byte(addr+1, (unsigned char)(v>>8));
}

// 
//@brief writes a word without performing I/O
//
//void Memory::MEMORY_write_word_no_io(unsigned short int addr, unsigned short int v)
void MEMORY_write_word_no_io(unsigned short int addr, unsigned short int v)
{
 MEMORY_mem_ram_[addr]= (unsigned char)(v); //MEMORY_write_byte_no_io(addr, (unsigned char)(v));
 MEMORY_mem_ram_[(addr+1)]= (unsigned char)(v>>8); //MEMORY_write_byte_no_io(addr+1, (unsigned char)(v>>8));
}

//
// @brief read byte (from VIC's perspective)
//
// The VIC has only 14 address lines so it can only access 
// 16kB of memory at once, the two missing address bits are 
// provided by CIA2.
//
// The VIC always reads from RAM ignoring the memory configuration,
// there's one exception: the character generator ROM. Unless the 
// Ultimax mode is selected, VIC sees the character generator ROM 
// in the memory areas:
//
//  1000-1FFF
//  9000-9FFF
//
//unsigned char Memory::MEMORY_vic_read_byte(unsigned short int addr)
unsigned char MEMORY_vic_read_byte(unsigned short int addr)
{
  unsigned char v;
  //uint16_t vic_addr = cia2_->CIA2_vic_base_address() + (addr & 0x3fff);
  //unsigned short int vic_addr = CIA2_vic_base_address() + (addr & 0x3fff); //No quiero objetos
  unsigned short int vic_addr = ((unsigned short int)(~CIA2_pra_&0x3) << 14) + (addr & 0x3fff); //No quiero objetos
  if((vic_addr >= 0x1000 && vic_addr <  0x2000) ||
     (vic_addr >= 0x9000 && vic_addr <  0xa000))
  {
   //v = MEMORY_mem_rom_[MEMORY_kBaseAddrChars + (vic_addr & 0xfff)];
   v = jj_memory_rom_read(MEMORY_kBaseAddrChars + (vic_addr & 0xfff),69,69); //Puedo restar MEMORY_kBaseAddrChars
  }
  else
  {
   v = MEMORY_mem_ram_[vic_addr]; //v = MEMORY_read_byte_no_io(vic_addr);
  }
  return v;
}

/*
//
//@brief loads a external binary into ROM
//
//void Memory::MEMORY_load_rom(const std::string &f, unsigned short int baseaddr)
void MEMORY_load_rom(const std::string &f, unsigned short int baseaddr)
{
//JJ  std::string path = "./assets/roms/" + f;
//JJ  std::ifstream is(path, std::ios::in | std::ios::binary);
//JJ  if(is)
//JJ  {
//JJ    is.seekg (0, is.end);
//JJ    std::streamoff length = is.tellg();
//JJ    is.seekg (0, is.beg);
//JJ    is.read ((char *) &mem_rom_[baseaddr],length);
//JJ  }

 const char *cstr = f.c_str();
 printf("load_rom %s\n",cstr);
 fflush(stdout); 
 
 FILE * fich = fopen(cstr,"rb");
 if (fich!=NULL)
 {
  fseek(fich, 0L, SEEK_END);
  unsigned int lon= ftell(fich);
  
  fseek(fich, 0L, SEEK_SET);
  fread((char *) &MEMORY_mem_rom_[baseaddr],lon,1,fich);
  
  fclose(fich);
 }
}
*/

//
//@brief loads a external binary into RAM
//
//void Memory::MEMORY_load_ram(const std::string &f, unsigned short int baseaddr)
void MEMORY_load_ram(const std::string &f, unsigned short int baseaddr)
{
//JJ  std::string path = "./assets/" + f;
//JJ  std::ifstream is(path, std::ios::in | std::ios::binary);
//JJ  if(is)
//JJ  {
//JJ    is.seekg (0, is.end);
//JJ    std::streamoff length = is.tellg();
//JJ    is.seekg (0, is.beg);
//JJ    is.read ((char *) &mem_ram_[baseaddr],length);
//JJ  }

 const char *cstr = f.c_str();
 Serial.printf("load_ram %s\r\n",cstr);
 //fflush(stdout);
 
 FILE * fich = fopen(cstr,"rb");
 if (fich!=NULL)
 {
  fseek(fich, 0L, SEEK_END);
  unsigned int lon= ftell(fich);
  
  fseek(fich, 0L, SEEK_SET);
  fread((char *) &MEMORY_mem_ram_[baseaddr],lon,1,fich);
  
  fclose(fich);
 } 

}

// debug ////////////////////////////////////////////////////////////////////

//
//@brief dumps memory as seen by the CPU to stdout
//
//void Memory::MEMORY_dump()
void MEMORY_dump()
{
  for(unsigned int p=0 ; p < MEMORY_kMemSize ; p++)
  {
    std::cout << MEMORY_read_byte(p);
  }
}



//void Memory::MEMORY_vic(Vic *v) //No quiero objetos
//{
// MEMORY_vic_ = v;
//};

//void Memory::MEMORY_cia1(Cia1 *v)
//{
// MEMORY_cia1_ = v;
//};

//void Memory::MEMORY_cia2(Cia2 *v)
//{
// MEMORY_cia2_ = v;
//};
