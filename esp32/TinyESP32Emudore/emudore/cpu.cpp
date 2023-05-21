// Mod by ackerman (convert C, refactor, optimice)
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
#include "cpu.h"
//#include <sstream> //No se necesita
#include "Arduino.h"

//
// @brief Cold reset
//
// https://www.c64-wiki.com/index.php/Reset_(Process)
//
//void Cpu::CPU_reset()
void CPU_reset()
{
  CPU_a_ = CPU_x_ = CPU_y_ = CPU_sp_ = 0;
  CPU_cf_ = CPU_zf_ = CPU_idf_ = CPU_dmf_ = CPU_bcf_ = CPU_of_ = CPU_nf_ = false;
  //pc(mem_->read_word(Memory::kAddrResetVector));
  //CPU_pc_set(CPU_mem_->MEMORY_read_word(MEMORY_kAddrResetVector));
  CPU_pc_= MEMORY_read_word(MEMORY_kAddrResetVector); //CPU_pc_set(MEMORY_read_word(MEMORY_kAddrResetVector)) No objetos
  CPU_cycles_ = 6;
  
  Serial.printf("CPU_reset\r\n");
  //fflush(stdout);
}

// 
// @brief emulate instruction 
// @return returns false if something goes wrong (e.g. illegal instruction)
//
// Current limitations:
// 
// - Illegal instructions are not implemented
// - Excess cycles due to page boundary crossing are not calculated
// - Some known architectural bugs are not emulated
//
//bool Cpu::CPU_emulate()
//bool CPU_emulate()
void CPU_emulate()
{
  //fetch instruction  
  unsigned char insn= MEMORY_read_byte(CPU_pc_++); //unsigned char insn = CPU_fetch_op()
  
  CPU_retval= 1; //bool retval = true; //No uso pila
  //emulate instruction
  switch(insn)
  {
  //BRK
  case 0x0: CPU_brk(); break;
  //ORA (nn,X)
  case 0x1: CPU_ora(MEMORY_read_byte(CPU_addr_indx()),6); break; //CPU_ora(CPU_load_byte(CPU_addr_indx()),6)
  //ORA nn
  case 0x5: CPU_ora(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_ora(CPU_load_byte(CPU_addr_zero()),3)
  //ASL nn
  case 0x6: CPU_asl_mem(CPU_addr_zero(),5); break;
  //PHP
  case 0x8: CPU_php(); break;
  //ORA #nn
  case 0x9: CPU_ora(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_ora(CPU_fetch_op(),2)
  //ASL A
  case 0xA: CPU_asl_a(); break;
  //ORA nnnn
  case 0xD: CPU_ora(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_ora(CPU_load_byte(CPU_addr_abs()),4)
  //ASL nnnn
  case 0xE: CPU_asl_mem(CPU_addr_abs(),6); break; 
  //BPL nn
  case 0x10: CPU_bpl(); break;
  //ORA (nn,Y)
  case 0x11: CPU_ora(MEMORY_read_byte(CPU_addr_indy()),5); break; //CPU_ora(CPU_load_byte(CPU_addr_indy()),5)
  //ORA nn,X
  case 0x15: CPU_ora(MEMORY_read_byte(CPU_addr_zerox()),4); break; //CPU_ora(CPU_load_byte(CPU_addr_zerox()),4)
  //ASL nn,X
  case 0x16: CPU_asl_mem(CPU_addr_zerox(),6); break;
  //CLC
  case 0x18: CPU_clc(); break;
  //ORA nnnn,Y
  case 0x19: CPU_ora(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU_ora(CPU_load_byte(CPU_addr_absy()),4)
  //ORA nnnn,X
  case 0x1D: CPU_ora(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU_ora(CPU_load_byte(CPU_addr_absx()),4)
  //ASL nnnn,X
  case 0x1E: CPU_asl_mem(CPU_addr_absx(),7); break;
  //JSR
  case 0x20: CPU_jsr(); break;
  //AND (nn,X)
  case 0x21: CPU__and(MEMORY_read_byte(CPU_addr_indx()),6); break; //CPU__and(CPU_load_byte(CPU_addr_indx()),6)
  //BIT nn
  case 0x24: CPU_bit(CPU_addr_zero(),3); break;
  //AND nn
  case 0x25: CPU__and(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU__and(CPU_load_byte(CPU_addr_zero()),3)
  //ROL nn
  case 0x26: CPU_rol_mem(CPU_addr_zero(),5); break;
  //PLP
  case 0x28: CPU_plp(); break;
  //AND #nn
  case 0x29: CPU__and(MEMORY_read_byte(CPU_pc_++),2); break; //CPU__and(CPU_fetch_op(),2)
  //ROL A
  case 0x2A: CPU_rol_a(); break;
  //BIT nnnn
  case 0x2C: CPU_bit(CPU_addr_abs(),4); break;
  //AND nnnn
  case 0x2D: CPU__and(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU__and(CPU_load_byte(CPU_addr_abs()),4)
  //ROL nnnn
  case 0x2E: CPU_rol_mem(CPU_addr_abs(),6); break;
  //BMI nn
  case 0x30: CPU_bmi(); break;
  //AND (nn,Y)
  case 0x31: CPU__and(MEMORY_read_byte(CPU_addr_indy()),5); break; //CPU__and(CPU_load_byte(CPU_addr_indy()),5)
  //AND nn,X
  case 0x35: CPU__and(MEMORY_read_byte(CPU_addr_zerox()),4); break; //CPU__and(CPU_load_byte(CPU_addr_zerox()),4)
  //ROL nn,X
  case 0x36: CPU_rol_mem(CPU_addr_zerox(),6); break;
  //SEC
  case 0x38: CPU_sec(); break;
  //AND nnnn,Y
  case 0x39: CPU__and(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU__and(CPU_load_byte(CPU_addr_absy()),4)
  //AND nnnn,X
  case 0x3D: CPU__and(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU__and(CPU_load_byte(CPU_addr_absx()),4)
  //ROL nnnn,X
  case 0x3E: CPU_rol_mem(CPU_addr_absx(),7); break;
  //RTI
  case 0x40: CPU_rti(); break;
  //EOR (nn,X)
  case 0x41: CPU_eor(MEMORY_read_byte(CPU_addr_indx()),6); break; //CPU_eor(CPU_load_byte(CPU_addr_indx()),6)
  //EOR nn
  case 0x45: CPU_eor(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_eor(CPU_load_byte(CPU_addr_zero()),3)
  //LSR nn
  case 0x46: CPU_lsr_mem(CPU_addr_zero(),5); break;
  //PHA
  case 0x48: CPU_pha(); break;
  //EOR #nn
  case 0x49: CPU_eor(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_eor(CPU_fetch_op(),2)
  //BVC
  case 0x50: CPU_bvc(); break;
  //JMP nnnn
  case 0x4C: CPU_jmp(); break;
  //EOR nnnn
  case 0x4D: CPU_eor(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_eor(CPU_load_byte(CPU_addr_abs()),4)
  //LSR A
  case 0x4A: CPU_lsr_a(); break;
  //LSR nnnn
  case 0x4E: CPU_lsr_mem(CPU_addr_abs(),6); break;
  //EOR (nn,Y)
  case 0x51: CPU_eor(MEMORY_read_byte(CPU_addr_indy()),5); break; //CPU_eor(CPU_load_byte(CPU_addr_indy()),5)
  //EOR nn,X
  case 0x55: CPU_eor(MEMORY_read_byte(CPU_addr_zerox()),4); break; //CPU_eor(CPU_load_byte(CPU_addr_zerox()),4)
  //LSR nn,X
  case 0x56: CPU_lsr_mem(CPU_addr_zerox(),6); break;
  //CLI
  case 0x58: CPU_cli(); break;
  //EOR nnnn,Y
  case 0x59: CPU_eor(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU_eor(CPU_load_byte(CPU_addr_absy()),4)
  //EOR nnnn,X
  case 0x5D: CPU_eor(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU_eor(CPU_load_byte(CPU_addr_absx()),4)
  //LSR nnnn,X
  case 0x5E: CPU_lsr_mem(CPU_addr_absx(),7); break;
  //RTS
  case 0x60: CPU_rts(); break;
  //ADC (nn,X)
  case 0x61: CPU_adc(MEMORY_read_byte(CPU_addr_indx()),6); break; //CPU_adc(CPU_load_byte(CPU_addr_indx()),6)
  //ADC nn
  case 0x65: CPU_adc(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_adc(CPU_load_byte(CPU_addr_zero()),3)
  //ROR nn
  case 0x66: CPU_ror_mem(CPU_addr_zero(),5); break;
  //PLA
  case 0x68: CPU_pla(); break;
  //ADC #nn
  case 0x69: CPU_adc(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_adc(CPU_fetch_op(),2)
  //ROR A
  case 0x6A: CPU_ror_a(); break;
  //JMP (nnnn)
  case 0x6C: CPU_jmp_ind(); break;
  //ADC nnnn
  case 0x6D: CPU_adc(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_adc(CPU_load_byte(CPU_addr_abs()),4)
  //ROR nnnn
  case 0x6E: CPU_ror_mem(CPU_addr_abs(),6); break;
  //BVS
  case 0x70: CPU_bvs(); break;
  //ADC (nn,Y)
  case 0x71: CPU_adc(MEMORY_read_byte(CPU_addr_indy()),5); break; //CPU_adc(CPU_load_byte(CPU_addr_indy()),5)
  //ADC nn,X
  case 0x75: CPU_adc(MEMORY_read_byte(CPU_addr_zerox()),4); break; //CPU_adc(CPU_load_byte(CPU_addr_zerox()),4)
  //ROR nn,X
  case 0x76: CPU_ror_mem(CPU_addr_zerox(),6); break;
  //SEI
  case 0x78: CPU_sei(); break;
  //ADC nnnn,Y
  case 0x79: CPU_adc(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU_adc(CPU_load_byte(CPU_addr_absy()),4)
  //ADC nnnn,X
  case 0x7D: CPU_adc(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU_adc(CPU_load_byte(CPU_addr_absx()),4)
  //ROR nnnn,X
  case 0x7E: CPU_ror_mem(CPU_addr_absx(),7); break;
  //STA (nn,X)
  case 0x81: CPU_sta(CPU_addr_indx(),6); break;
  //STY nn
  case 0x84: CPU_sty(CPU_addr_zero(),3); break;
  //STA nn
  case 0x85: CPU_sta(CPU_addr_zero(),3); break;
  //STX nn
  case 0x86: CPU_stx(CPU_addr_zero(),3); break;
  //DEY
  case 0x88: CPU_dey(); break;
  //TXA
  case 0x8A: CPU_txa(); break;
  //STY nnnn
  case 0x8C: CPU_sty(CPU_addr_abs(),4); break;
  //STA nnnn
  case 0x8D: CPU_sta(CPU_addr_abs(),4); break;
  //STX nnnn
  case 0x8E: CPU_stx(CPU_addr_abs(),4); break;
  //BCC nn
  case 0x90: CPU_bcc(); break;
  //STA (nn,Y)
  case 0x91: CPU_sta(CPU_addr_indy(),6); break;
  //STY nn,X
  case 0x94: CPU_sty(CPU_addr_zerox(),4); break;
  //STA nn,X
  case 0x95: CPU_sta(CPU_addr_zerox(),4); break;
  //STX nn,Y
  case 0x96: CPU_stx(CPU_addr_zeroy(),4); break;
  //TYA
  case 0x98: CPU_tya(); break;
  //STA nnnn,Y
  case 0x99: CPU_sta(CPU_addr_absy(),5); break;
  //TXS
  case 0x9A: CPU_txs(); break;
  //STA nnnn,X
  case 0x9D: CPU_sta(CPU_addr_absx(),5); break;
  //LDY #nn
  case 0xA0: CPU_ldy(MEMORY_read_byte(CPU_pc_++),2); break;  //CPU_ldy(CPU_fetch_op(),2)
  //LDA (nn,X)
  case 0xA1: CPU_lda(MEMORY_read_byte(CPU_addr_indx()),6); break; //CPU_lda(CPU_load_byte(CPU_addr_indx()),6)
  //LDX #nn
  case 0xA2: CPU_ldx(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_ldx(CPU_fetch_op(),2)
  //LDY nn
  case 0xA4: CPU_ldy(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_ldy(CPU_load_byte(CPU_addr_zero()),3)
  //LDA nn
  case 0xA5: CPU_lda(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_lda(CPU_load_byte(CPU_addr_zero()),3)
  //LDX nn
  case 0xA6: CPU_ldx(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_ldx(CPU_load_byte(CPU_addr_zero()),3)
  //TAY
  case 0xA8: CPU_tay(); break;
  //LDA #nn
  case 0xA9: CPU_lda(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_lda(CPU_fetch_op(),2)
  //TAX
  case 0xAA: CPU_tax(); break;
  //LDY nnnn
  case 0xAC: CPU_ldy(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_ldy(CPU_load_byte(CPU_addr_abs()),4)
  //LDA nnnn
  case 0xAD: CPU_lda(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_lda(CPU_load_byte(CPU_addr_abs()),4)
  //LDX nnnn
  case 0xAE: CPU_ldx(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_ldx(CPU_load_byte(CPU_addr_abs()),4)
  //BCS nn
  case 0xB0: CPU_bcs(); break;
  //LDA (nn,Y)
  case 0xB1: CPU_lda(MEMORY_read_byte(CPU_addr_indy()),5); break; //CPU_lda(CPU_load_byte(CPU_addr_indy()),5)
  //LDY nn,X
  case 0xB4: CPU_ldy(MEMORY_read_byte(CPU_addr_zerox()),3); break; //CPU_ldy(CPU_load_byte(CPU_addr_zerox()),3)
  //LDA nn,X
  case 0xB5: CPU_lda(MEMORY_read_byte(CPU_addr_zerox()),3); break; //CPU_lda(CPU_load_byte(CPU_addr_zerox()),3)
  //LDX nn,Y
  case 0xB6: CPU_ldx(MEMORY_read_byte(CPU_addr_zeroy()),3); break; //CPU_ldx(CPU_load_byte(CPU_addr_zeroy()),3)
  //CLV
  case 0xB8: CPU_clv(); break;
  //LDA nnnn,Y
  case 0xB9: CPU_lda(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU_lda(CPU_load_byte(CPU_addr_absy()),4)
  //TSX
  case 0xBA: CPU_tsx(); break;
  //LDY nnnn,X
  case 0xBC: CPU_ldy(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU_ldy(CPU_load_byte(CPU_addr_absx()),4)
  //LDA nnnn,X
  case 0xBD: CPU_lda(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU_lda(CPU_load_byte(CPU_addr_absx()),4)
  //LDX nnnn,Y
  case 0xBE: CPU_ldx(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU_ldx(CPU_load_byte(CPU_addr_absy()),4)
  //CPY #nn
  case 0xC0: CPU_cpy(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_cpy(CPU_fetch_op(),2)
  //CMP (nn,X)
  case 0xC1: CPU_cmp(MEMORY_read_byte(CPU_addr_indx()),6); break; //CPU_cmp(CPU_load_byte(CPU_addr_indx()),6)
  //CPY nn
  case 0xC4: CPU_cpy(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_cpy(CPU_load_byte(CPU_addr_zero()),3)
  //CMP nn
  case 0xC5: CPU_cmp(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_cmp(CPU_load_byte(CPU_addr_zero()),3)
  //DEC nn
  case 0xC6: CPU_dec(CPU_addr_zero(),5); break;
  //INY
  case 0xC8: CPU_iny(); break;
  //CMP #nn
  case 0xC9: CPU_cmp(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_cmp(CPU_fetch_op(),2)
  //DEX
  case 0xCA: CPU_dex(); break;
  //CPY nnnn
  case 0xCC: CPU_cpy(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_cpy(CPU_load_byte(CPU_addr_abs()),4)
  //CMP nnnn
  case 0xCD: CPU_cmp(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_cmp(CPU_load_byte(CPU_addr_abs()),4)
  //DEC nnnn
  case 0xCE: CPU_dec(CPU_addr_abs(),6); break;
  //BNE nn
  case 0xD0: CPU_bne(); break;
  //CMP (nn,Y)
  case 0xD1: CPU_cmp(MEMORY_read_byte(CPU_addr_indy()),5); break; //CPU_cmp(CPU_load_byte(CPU_addr_indy()),5)
  //CMP nn,X
  case 0xD5: CPU_cmp(MEMORY_read_byte(CPU_addr_zerox()),4); break; //CPU_cmp(CPU_load_byte(CPU_addr_zerox()),4)
  //DEC nn,X
  case 0xD6: CPU_dec(CPU_addr_zerox(),6); break;
  //CLD
  case 0xD8: CPU_cld(); break;
  //CMP nnnn,Y
  case 0xD9: CPU_cmp(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU_cmp(CPU_load_byte(CPU_addr_absy()),4)
  //CMP nnnn,X
  case 0xDD: CPU_cmp(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU_cmp(CPU_load_byte(CPU_addr_absx()),4)
  //DEC nnnn,X
  case 0xDE: CPU_dec(CPU_addr_absx(),7); break;
  //CPX #nn
  case 0xE0: CPU_cpx(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_cpx(CPU_fetch_op(),2)
  //SBC (nn,X)
  case 0xE1: CPU_sbc(MEMORY_read_byte(CPU_addr_indx()),6); break; //CPU_sbc(CPU_load_byte(CPU_addr_indx()),6)
  //CPX nn
  case 0xE4: CPU_cpx(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_cpx(CPU_load_byte(CPU_addr_zero()),3)
  //SBC nn
  case 0xE5: CPU_sbc(MEMORY_read_byte(CPU_addr_zero()),3); break; //CPU_sbc(CPU_load_byte(CPU_addr_zero()),3)
  //INC nn
  case 0xE6: CPU_inc(CPU_addr_zero(),5); break;
  //INX
  case 0xE8: CPU_inx(); break;
  //SBC #nn
  case 0xE9: CPU_sbc(MEMORY_read_byte(CPU_pc_++),2); break; //CPU_sbc(CPU_fetch_op(),2)
  //NOP
  case 0xEA: CPU_cycles_+= 2; break; //CPU_nop();
  //CPX nnnn
  case 0xEC: CPU_cpx(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_cpx(CPU_load_byte(CPU_addr_abs()),4)
  //SBC nnnn
  case 0xED: CPU_sbc(MEMORY_read_byte(CPU_addr_abs()),4); break; //CPU_sbc(CPU_load_byte(CPU_addr_abs()),4)
  //INC nnnn
  case 0xEE: CPU_inc(CPU_addr_abs(),6); break;
  //BEQ nn
  case 0xF0: CPU_beq(); break;
  //SBC (nn,Y)
  case 0xF1: CPU_sbc(MEMORY_read_byte(CPU_addr_indy()),5); break; //CPU_sbc(CPU_load_byte(CPU_addr_indy()),5)
  //SBC nn,X
  case 0xF5: CPU_sbc(MEMORY_read_byte(CPU_addr_zerox()),4); break; //CPU_sbc(CPU_load_byte(CPU_addr_zerox()),4)
  //INC nn,X
  case 0xF6: CPU_inc(CPU_addr_zerox(),6); break;
  //SED
  case 0xF8: CPU_sed(); break;
  //SBC nnnn,Y
  case 0xF9: CPU_sbc(MEMORY_read_byte(CPU_addr_absy()),4); break; //CPU_sbc(CPU_load_byte(CPU_addr_absy()),4)
  //SBC nnnn,X
  case 0xFD: CPU_sbc(MEMORY_read_byte(CPU_addr_absx()),4); break; //CPU_sbc(CPU_load_byte(CPU_addr_absx()),4)
  //INC nnnn,X
  case 0xFE: CPU_inc(CPU_addr_absx(),7); break;
  //Unknown or illegal instruction
  default:
    Serial.printf("Unknown instruction: %X at %04x\r\n", insn,CPU_pc_);
    //fflush(stdout);
    //D("Unknown instruction: %X at %04x\n", insn,pc());
    gb_reset= 1;    
    CPU_retval= 0; //retval = false; //Quito pila
  }
  //return retval; //No uso pila Tengo CPU_retval
}

// helpers ///////////////////////////////////////////////////////////////////

//unsigned char Cpu::CPU_load_byte(unsigned short int addr)
//unsigned char CPU_load_byte(unsigned short int addr)
//{//128125  llamadas 1 segundo
//  //gb_cont_call++;
// 
//  //return CPU_mem_->MEMORY_read_byte(addr);
//  return MEMORY_read_byte(addr); //No objetos
//}

//void Cpu::CPU_push(unsigned char v)
void CPU_push(unsigned char v)
{
 unsigned short int addr = MEMORY_kBaseAddrStack + CPU_sp_;
 //CPU_mem_->MEMORY_write_byte(addr,v);
 MEMORY_write_byte(addr,v); //No objetos
 CPU_sp_--;
}

//unsigned char Cpu::CPU_pop()
unsigned char CPU_pop()
{
 unsigned short int addr = ++CPU_sp_+MEMORY_kBaseAddrStack;
 return MEMORY_read_byte(addr); //return CPU_load_byte(addr)
}
 
//unsigned char Cpu::CPU_fetch_op()
//unsigned char CPU_fetch_op()
//{//371628 llamadas 1 segundo
//  //gb_cont_call++;
//  return MEMORY_read_byte(CPU_pc_++); //return CPU_load_byte(CPU_pc_++)
//}

//unsigned short int Cpu::CPU_fetch_opw()
unsigned short int CPU_fetch_opw()
{
  //unsigned short int retval = CPU_mem_->MEMORY_read_word(CPU_pc_);
  unsigned short int retval = MEMORY_read_word(CPU_pc_); //No objetos
  CPU_pc_+=2;
  return retval;
}

//unsigned short int Cpu::CPU_addr_zero()
unsigned short int CPU_addr_zero()
{
 unsigned short int addr = MEMORY_read_byte(CPU_pc_++); //CPU_fetch_op()
 return addr;
}

//unsigned short int Cpu::CPU_addr_zerox()
unsigned short int CPU_addr_zerox()
{
 //wraps around the zeropage
 //unsigned short int addr = (MEMORY_read_byte(CPU_pc_++) + CPU_x_get()) & 0xff; //(CPU_fetch_op() + CPU_x()) & 0xff
 unsigned short int addr = (MEMORY_read_byte(CPU_pc_++) + CPU_x_) & 0xff; //(CPU_fetch_op() + CPU_x()) & 0xff
 return addr;
}
 
//unsigned short int Cpu::CPU_addr_zeroy()
unsigned short int CPU_addr_zeroy()
{
 //wraps around the zeropage
 unsigned short int addr = (MEMORY_read_byte(CPU_pc_++) + CPU_y_) & 0xff; //(CPU_fetch_op() + CPU_y()) & 0xff
 return addr;
}

//unsigned short int Cpu::CPU_addr_abs()
unsigned short int CPU_addr_abs()
{
 unsigned short int addr = CPU_fetch_opw();
 return addr;
}

//unsigned short int Cpu::CPU_addr_absy()
unsigned short int CPU_addr_absy()
{
 unsigned short int addr = CPU_fetch_opw() + CPU_y_;
 return addr;
}

//unsigned short int Cpu::CPU_addr_absx()
unsigned short int CPU_addr_absx()
{
 unsigned short int addr = CPU_fetch_opw() + CPU_x_;
 return addr;  
}

//unsigned short int Cpu::CPU_addr_indx()
unsigned short int CPU_addr_indx()
{
 //wraps around the zeropage
 //unsigned short int addr = CPU_mem_->MEMORY_read_word((CPU_addr_zero() + CPU_x()) & 0xff);
 unsigned short int addr = MEMORY_read_word((CPU_addr_zero() + CPU_x_) & 0xff); //No objetos
 return addr;
}

//unsigned short int Cpu::CPU_addr_indy()
unsigned short int CPU_addr_indy()
{
 //unsigned short int addr = CPU_mem_->MEMORY_read_word(CPU_addr_zero()) + CPU_y();
 unsigned short int addr = MEMORY_read_word(CPU_addr_zero()) + CPU_y_; //No objetos
 return addr;
}

// Instructions: data handling and memory operations  ////////////////////////

//
//@brief STore Accumulator
//
//void Cpu::CPU_sta(unsigned short int addr, unsigned char cycles)
void CPU_sta(unsigned short int addr, unsigned char cycles)
{//250681 llamadas 1 segundo
 //gb_cont_call++;
    
 //CPU_mem_->MEMORY_write_byte(addr,CPU_a_get());
 MEMORY_write_byte(addr,CPU_a_); //No objetos MEMORY_write_byte(addr,CPU_a_get())
 CPU_cycles_+= cycles;//CPU_tick(cycles);
}

//
//@brief STore X
//
//void Cpu::CPU_stx(unsigned short int addr, unsigned char cycles)
void CPU_stx(unsigned short int addr, unsigned char cycles)
{
 //CPU_mem_->MEMORY_write_byte(addr,CPU_x());
 MEMORY_write_byte(addr,CPU_x_); //No objetos
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief STore Y
//
//void Cpu::CPU_sty(unsigned short int addr, unsigned char cycles)
void CPU_sty(unsigned short int addr, unsigned char cycles)
{
 //CPU_mem_->MEMORY_write_byte(addr,CPU_y());
 MEMORY_write_byte(addr,CPU_y_); //No objetos
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief Transfer X to Stack pointer
//
//void Cpu::CPU_txs()
inline void CPU_txs()
{
 CPU_sp_= CPU_x_; //CPU_sp_set(CPU_x_)
 CPU_cycles_+= 2;//CPU_tick(2);
}

//
//@brief Transfer Stack pointer to X
//
//void Cpu::CPU_tsx()
inline void CPU_tsx()
{
 CPU_x_= CPU_sp_; //CPU_x_set(CPU_sp_get());
 SET_ZF(CPU_x_);
 SET_NF(CPU_x_);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief LoaD Accumulator
//
//void Cpu::CPU_lda(unsigned char v, unsigned char cycles)
inline void CPU_lda(unsigned char v, unsigned char cycles)
{
 CPU_a_= v; //CPU_a_set(v)
 SET_ZF(CPU_a_); //SET_ZF(CPU_a_get())
 SET_NF(CPU_a_); //SET_NF(CPU_a_get())
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief LoaD X
//
//void Cpu::CPU_ldx(unsigned char v, unsigned char cycles)
inline void CPU_ldx(unsigned char v, unsigned char cycles)
{
 CPU_x_= v; //CPU_x_set(v);
 SET_ZF(CPU_x_);
 SET_NF(CPU_x_);
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief LoaD Y
//
//void Cpu::CPU_ldy(unsigned char v, unsigned char cycles)
inline void CPU_ldy(unsigned char v, unsigned char cycles)
{
 CPU_y_= v; //CPU_y_set(v);
 SET_ZF(CPU_y_);
 SET_NF(CPU_y_);
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief Transfer X to Accumulator 
//
//void Cpu::CPU_txa()
inline void CPU_txa()
{
 CPU_a_= CPU_x_; //CPU_a_set(CPU_x_)
 SET_ZF(CPU_a_); //SET_ZF(CPU_a_get())
 SET_NF(CPU_a_); //SET_NF(CPU_a_get())
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief Transfer Accumulator to X 
//
//void Cpu::CPU_tax()
inline void CPU_tax()
{
 CPU_x_= CPU_a_; //CPU_x_= CPU_a_get() //CPU_x_set(CPU_a_get());
 SET_ZF(CPU_x_);
 SET_NF(CPU_x_);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief Transfer Accumulator to Y 
//
//void Cpu::CPU_tay()
inline void CPU_tay()
{
 CPU_y_= CPU_a_; //CPU_y_= CPU_a_get() //CPU_y_set(CPU_a_get());
 SET_ZF(CPU_y_);
 SET_NF(CPU_y_);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief Transfer Y to Accumulator 
//
//void Cpu::CPU_tya()
void CPU_tya()
{
 CPU_a_= CPU_y_; //CPU_a_set(CPU_y_)
 SET_ZF(CPU_a_); //SET_ZF(CPU_a_get())
 SET_NF(CPU_a_); //SET_NF(CPU_a_get())
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief PusH Accumulator 
//
//void Cpu::CPU_pha()
void CPU_pha()
{
 CPU_push(CPU_a_); //CPU_push(CPU_a_get())
 CPU_cycles_+= 3; //CPU_tick(3);
}

//
//@brief PuLl Accumulator 
//
//void Cpu::CPU_pla()
void CPU_pla()
{
 CPU_a_= CPU_pop(); //CPU_a_set(CPU_pop())
 SET_ZF(CPU_a_); //SET_ZF(CPU_a_get())
 SET_NF(CPU_a_); //SET_NF(CPU_a_get())
 CPU_cycles_+= 4; //CPU_tick(4);
}
 
// Instructions: logic operations  ///////////////////////////////////////////

//
//@brief Logical OR on Accumulator
//
//void Cpu::CPU_ora(unsigned char v, unsigned char cycles)
inline void CPU_ora(unsigned char v, unsigned char cycles)
{
 CPU_a_= (CPU_a_ | v); //CPU_a_set(CPU_a_|v) //CPU_a_set(CPU_a_get()|v)
 SET_ZF(CPU_a_); //SET_ZF(CPU_a_get())
 SET_NF(CPU_a_); //SET_NF(CPU_a_get())
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief Logical AND
//
//void Cpu::CPU__and(unsigned char v, unsigned char cycles)
inline void CPU__and(unsigned char v, unsigned char cycles)
{
 CPU_a_= (CPU_a_ & v); //CPU_a_set(CPU_a_&v) //CPU_a_set(CPU_a_get()&v)
 SET_ZF(CPU_a_); //SET_ZF(CPU_a_get())
 SET_NF(CPU_a_); //SET_NF(CPU_a_get())
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief BIT test
//
//void Cpu::CPU_bit(unsigned short int addr, unsigned char cycles)
void CPU_bit(unsigned short int addr, unsigned char cycles)
{
 unsigned char t = MEMORY_read_byte(addr); //CPU_load_byte(addr)
 CPU_of_= ((t&0x40)!=0); //CPU_of((t&0x40)!=0)
 SET_NF(t);
 SET_ZF(t&CPU_a_); //SET_ZF(t&CPU_a_get())
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}
 
//
//@brief ROtate Left
//
//unsigned char Cpu::CPU_rol(unsigned char v)
inline unsigned char CPU_rol(unsigned char v)
{
 unsigned short int t = (v << 1) | (unsigned char)CPU_cf_; //(v << 1) | (unsigned char)CPU_cf_get()
 CPU_cf_= ((t&0x100)!=0); //CPU_cf_set((t&0x100)!=0)
 SET_ZF(t);
 SET_NF(t);
 return (unsigned char)t;
}

// 
//@brief ROL A register
//
//void Cpu::CPU_rol_a()
void CPU_rol_a()
{
 CPU_a_= CPU_rol(CPU_a_); //CPU_a_set(CPU_rol(CPU_a_)) //CPU_a_set(CPU_rol(CPU_a_get()))
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief ROL mem 
//
//void Cpu::CPU_rol_mem(unsigned short int addr, unsigned char cycles)
void CPU_rol_mem(unsigned short int addr, unsigned char cycles)
{
 unsigned char v = MEMORY_read_byte(addr); //CPU_load_byte(addr)
 //see ASL doc
 //CPU_mem_->MEMORY_write_byte(addr,v);
 //CPU_mem_->MEMORY_write_byte(addr,CPU_rol(v));
 MEMORY_write_byte(addr,v); //No objetos
 MEMORY_write_byte(addr,CPU_rol(v));  
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief ROtate Right
//
//unsigned char Cpu::CPU_ror(unsigned char v)
inline unsigned char CPU_ror(unsigned char v)
{
  unsigned short int t = (v >> 1) | (unsigned char)(CPU_cf_ << 7); //(v >> 1) | (unsigned char)(CPU_cf_get() << 7)
  CPU_cf_= ((v&0x1)!=0); //CPU_cf_set((v&0x1)!=0)
  SET_ZF(t);
  SET_NF(t);
  return (unsigned char)t;
}

// 
//@brief ROR A register
//
//void Cpu::CPU_ror_a()
void CPU_ror_a()
{
 CPU_a_= CPU_ror(CPU_a_); //CPU_a_set(CPU_ror(CPU_a_)) //CPU_a_set(CPU_ror(CPU_a_get()))
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief ROR mem 
//
//void Cpu::CPU_ror_mem(unsigned short int addr, unsigned char cycles)
void CPU_ror_mem(unsigned short int addr, unsigned char cycles)
{
 unsigned char v = MEMORY_read_byte(addr); //CPU_load_byte(addr)
 //see ASL doc
 //CPU_mem_->MEMORY_write_byte(addr,v);
 //CPU_mem_->MEMORY_write_byte(addr,CPU_ror(v));
 MEMORY_write_byte(addr,v); //No objetos
 MEMORY_write_byte(addr,CPU_ror(v));  
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}       

//
//@brief Logic Shift Right
//
//unsigned char Cpu::CPU_lsr(unsigned char v)
inline unsigned char CPU_lsr(unsigned char v)
{
  unsigned char t = v >> 1;
  CPU_cf_= ((v&0x1)!=0); //CPU_cf_set((v&0x1)!=0)
  SET_ZF(t);
  SET_NF(t);
  return t;
}

// 
//@brief LSR A
//
//void Cpu::CPU_lsr_a()
void CPU_lsr_a()
{
 CPU_a_= CPU_lsr(CPU_a_); //CPU_a_set(CPU_lsr(CPU_a_)) //CPU_a_set(CPU_lsr(CPU_a_get()))
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief LSR mem
//
//void Cpu::CPU_lsr_mem(unsigned short int addr, unsigned char cycles)
void CPU_lsr_mem(unsigned short int addr, unsigned char cycles)
{
 unsigned char v = MEMORY_read_byte(addr); //CPU_load_byte(addr)
 //see ASL doc
 //CPU_mem_->MEMORY_write_byte(addr,v);
 //CPU_mem_->MEMORY_write_byte(addr,CPU_lsr(v));
 MEMORY_write_byte(addr,v);
 MEMORY_write_byte(addr,CPU_lsr(v));  
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}

//
//@brief Arithmetic Shift Left
//
//unsigned char Cpu::CPU_asl(unsigned char v)
unsigned char CPU_asl(unsigned char v)
{
  unsigned char t = (v << 1) & 0xff;
  CPU_cf_= ((v&0x80)!=0); //CPU_cf_set((v&0x80)!=0)
  SET_ZF(t);
  SET_NF(t);
  return t;
}

// 
//@brief ASL A
//
//void Cpu::CPU_asl_a()
void CPU_asl_a()
{
 CPU_a_= CPU_asl(CPU_a_); //CPU_a_set(CPU_asl(CPU_a_)) //CPU_a_set(CPU_asl(CPU_a_get()))
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
// @brief ASL mem 
//
// ASL and the other read-modify-write instructions contain a bug (wikipedia):
//
// --
// The 6502's read-modify-write instructions perform one read and two write 
// cycles. First the unmodified data that was read is written back, and then 
// the modified data is written. This characteristic may cause issues by 
// twice accessing hardware that acts on a write. This anomaly continued 
// through the entire NMOS line, but was fixed in the CMOS derivatives, in 
// which the processor will do two reads and one write cycle. 
// --
//
// I have come across code that uses this side-effect as a feature, for
// instance, the following instruction will acknowledge VIC interrupts 
// on the first write cycle:
//
// ASL $d019
//
// So.. we need to mimic the behaviour.
//
//void Cpu::CPU_asl_mem(unsigned short int addr, unsigned char cycles)
void CPU_asl_mem(unsigned short int addr, unsigned char cycles)
{
 unsigned char v = MEMORY_read_byte(addr); //CPU_load_byte(addr)
 //CPU_mem_->MEMORY_write_byte(addr,v); 
 //CPU_mem_->MEMORY_write_byte(addr,CPU_asl(v));
 MEMORY_write_byte(addr,v); //No objetos
 MEMORY_write_byte(addr,CPU_asl(v));  
 CPU_cycles_+= cycles; //CPU_tick(cycles);
} 

//
//@brief Exclusive OR 
//
//void Cpu::CPU_eor(unsigned char v, unsigned char cycles)
inline void CPU_eor(unsigned char v, unsigned char cycles)
{
 CPU_a_= (CPU_a_ ^ v); //CPU_a_set(CPU_a_^v) //CPU_a_set(CPU_a_get()^v)
 SET_ZF(CPU_a_); //SET_ZF(CPU_a_get())
 SET_NF(CPU_a_); //SET_NF(CPU_a_get())
 CPU_cycles_+= cycles; //CPU_tick(cycles);
}
 
// Instructions: arithmetic operations  //////////////////////////////////////

//
//@brief INCrement
//
//void Cpu::CPU_inc(unsigned short int addr, unsigned char cycles)
void CPU_inc(unsigned short int addr, unsigned char cycles)
{
 unsigned char v = MEMORY_read_byte(addr); //CPU_load_byte(addr)
 //see ASL doc
 //CPU_mem_->MEMORY_write_byte(addr,v);
 MEMORY_write_byte(addr,v); //No objetos
 v++;
 //CPU_mem_->MEMORY_write_byte(addr,v);
 MEMORY_write_byte(addr,v); //No objetos
 SET_ZF(v);
 SET_NF(v);
}

//
//@brief DECrement
//
//void Cpu::CPU_dec(unsigned short int addr, unsigned char cycles)
void CPU_dec(unsigned short int addr, unsigned char cycles)
{
 unsigned char v = MEMORY_read_byte(addr); //CPU_load_byte(addr)
 //see ASL doc
 //CPU_mem_->MEMORY_write_byte(addr,v);
 MEMORY_write_byte(addr,v); //No objetos
 v--;
 //CPU_mem_->MEMORY_write_byte(addr,v);
 MEMORY_write_byte(addr,v); //No objetos
 SET_ZF(v);
 SET_NF(v);
}

//
//@brief INcrement X
//
//void Cpu::CPU_inx()
inline void CPU_inx()
{
 CPU_x_+=1;
 SET_ZF(CPU_x_);
 SET_NF(CPU_x_);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief INcrement Y
//
//void Cpu::CPU_iny()
inline void CPU_iny()
{
 CPU_y_+=1;
 SET_ZF(CPU_y_);
 SET_NF(CPU_y_);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief DEcrement X
//
//void Cpu::CPU_dex()
inline void CPU_dex()
{
 CPU_x_-=1;
 SET_ZF(CPU_x_);
 SET_NF(CPU_x_);
 CPU_cycles_+= 2;//CPU_tick(2);
}

//
//@brief DEcrement Y
//
//void Cpu::CPU_dey()
inline void CPU_dey()
{
 CPU_y_-=1;
 SET_ZF(CPU_y_);
 SET_NF(CPU_y_);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief ADd with Carry
//
//void Cpu::CPU_adc(unsigned char v, unsigned char cycles)
void CPU_adc(unsigned char v, unsigned char cycles)
{
  unsigned short int t;
  //if(CPU_dmf())
  if(CPU_dmf_)
  {
    t = (CPU_a_&0xf) + (v&0xf) + (CPU_cf_ ? 1 : 0); //(CPU_a_&0xf) + (v&0xf) + (CPU_cf_get() ? 1 : 0)
    if (t > 0x09) 
      t += 0x6;
    t += (CPU_a_&0xf0) + (v&0xf0);
    if((t & 0x1f0) > 0x90) 
      t += 0x60;
  }
  else
  {
    t = CPU_a_ + v + (CPU_cf_ ? 1 : 0); //CPU_a_ + v + (CPU_cf_get() ? 1 : 0)
  }
  CPU_cf_= (t>0xff); //CPU_cf_set(t>0xff)
  t=t&0xff;
  CPU_of_= (!((CPU_a_^v)&0x80) && ((CPU_a_^t) & 0x80)); //CPU_of(!((CPU_a_^v)&0x80) && ((CPU_a_^t) & 0x80))
  SET_ZF(t);
  SET_NF(t);
  CPU_a_= (unsigned char)t; //CPU_a_set((unsigned char)t)
}

//
//@brief SuBstract with Carry
//
//void Cpu::CPU_sbc(unsigned char v, unsigned char cycles)
void CPU_sbc(unsigned char v, unsigned char cycles)
{
  unsigned short int t;
  //if(CPU_dmf())
  if(CPU_dmf_)
  {
    t = (CPU_a_&0xf) - (v&0xf) - (CPU_cf_ ? 0 : 1); //(CPU_a_&0xf) - (v&0xf) - (CPU_cf_get() ? 0 : 1)
    if((t & 0x10) != 0)
      t = ((t-0x6)&0xf) | ((CPU_a_&0xf0) - (v&0xf0) - 0x10);
    else
      t = (t&0xf) | ((CPU_a_&0xf0) - (v&0xf0));
    if((t&0x100)!=0)
      t -= 0x60;
  }
  else
  {
    t = CPU_a_ - v - (CPU_cf_ ? 0 : 1); //CPU_a_ - v - (CPU_cf_get() ? 0 : 1)
  }
  CPU_cf_= (t<0x100); //CPU_cf_set(t<0x100)
  t=t&0xff;
  CPU_of_= (((CPU_a_^t)&0x80) && ((CPU_a_^v) & 0x80)); //CPU_of(((CPU_a_^t)&0x80) && ((CPU_a_^v) & 0x80))
  SET_ZF(t);
  SET_NF(t);
  CPU_a_= (unsigned char)t; //CPU_a_set((unsigned char)t)
}
 
// Instructions: flag access ///////////////////////////////////////////////// 

// 
//@brief SEt Interrupt flag
//
//void Cpu::CPU_sei()
inline void CPU_sei()
{
 CPU_idf_= true;//CPU_idf_set(true);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief CLear Interrupt flag
//
//void Cpu::CPU_cli()
inline void CPU_cli()
{
 CPU_idf_= false; //CPU_idf_set(false);
 CPU_cycles_+= 2; //CPU_tick(2);
}

//
//@brief SEt Carry flag 
//
//void Cpu::CPU_sec()
inline void CPU_sec()
{
 CPU_cf_= true; //CPU_cf_set(true)
 CPU_cycles_+= 2; //CPU_tick(2);
}
 
//
//@brief CLear Carry flag 
//
//void Cpu::CPU_clc()
inline void CPU_clc()
{
 CPU_cf_= false; //CPU_cf_set(false)
 CPU_cycles_+= 2;//CPU_tick(2);
}

//
//@brief SEt Decimal flag 
//
//void Cpu::CPU_sed()
inline void CPU_sed()
{
 CPU_dmf_= true; //CPU_dmf(true)
 CPU_cycles_+= 2;//CPU_tick(2);
}
 
//
//@brief CLear Decimal flag 
//
//void Cpu::CPU_cld()
inline void CPU_cld()
{
  CPU_dmf_= false; //CPU_dmf(false)
  CPU_cycles_+= 2;//CPU_tick(2);
}

//
//@brief CLear oVerflow flag 
//
//void Cpu::CPU_clv()
inline void CPU_clv()
{//0 llamadas
 //gb_cont_call++; 
 CPU_of_= false; //CPU_of(false)
 CPU_cycles_+= 2;//CPU_tick(2);
}

//unsigned char Cpu::CPU_flags()
inline unsigned char CPU_flags_get()
{
  unsigned char v=0;
  v |= CPU_cf_ << 0; //CPU_cf_get()  << 0
  v |= CPU_zf_ << 1; //v |= CPU_zf_get()  << 1
  v |= (CPU_idf_ << 2);//CPU_idf_get() << 2;
  v |= CPU_dmf_ << 3; //CPU_dmf() << 3
  //brk & php instructions push the bcf flag active
  v |= 1 << 4;
  //unused, always set
  v |= 1     << 5;
  v |= CPU_of_ << 6; //CPU_of()  << 6
  v |= CPU_nf_ << 7; //CPU_nf()  << 7
  return v;
}

//void Cpu::CPU_flags(unsigned char v)
inline void CPU_flags_set(unsigned char v)
{
  CPU_cf_= (ISSET_BIT(v,0)); //CPU_cf_set(ISSET_BIT(v,0))
  CPU_zf_= (ISSET_BIT(v,1)); //CPU_zf_set(ISSET_BIT(v,1))
  CPU_idf_= ISSET_BIT(v,2); //CPU_idf_set(ISSET_BIT(v,2));
  CPU_dmf_= (ISSET_BIT(v,3)); //CPU_dmf(ISSET_BIT(v,3))
  CPU_of_= (ISSET_BIT(v,6)); //CPU_of(ISSET_BIT(v,6))
  CPU_nf_= (ISSET_BIT(v,7)); //CPU_nf(ISSET_BIT(v,7))
}

//
//@brief PusH Processor flags
//
//void Cpu::CPU_php()
void CPU_php()
{
  CPU_push(CPU_flags_get());
  CPU_cycles_+= 3; //CPU_tick(3);
}

//
//@brief PuLl Processor flags
//
//void Cpu::CPU_plp()
void CPU_plp()
{
  CPU_flags_set(CPU_pop());
  CPU_cycles_+= 4;//CPU_tick(4);
}

//
//@brief Jump to SubRoutine
// 
//Note that JSR does not push the address of the next instruction
//to the stack but the address to the last byte of its own 
//instruction.
//
//void Cpu::CPU_jsr()
void CPU_jsr()
{
  unsigned short int addr = CPU_addr_abs();
  CPU_push(((CPU_pc_-1) >> 8) & 0xff); //CPU_push(((CPU_pc_get()-1) >> 8) & 0xff)
  CPU_push(((CPU_pc_-1) & 0xff)); //CPU_push(((CPU_pc_get()-1) & 0xff))
  CPU_pc_= addr; //CPU_pc_set(addr)
  CPU_cycles_+= 6;//CPU_tick(6);
}

//
//@brief JuMP
//
//void Cpu::CPU_jmp()
void CPU_jmp()
{
  unsigned short int addr = CPU_addr_abs();
  CPU_pc_= addr; //CPU_pc_set(addr)
  CPU_cycles_+= 3;//CPU_tick(3);
}

//
//@brief JuMP (indirect)
//
//void Cpu::CPU_jmp_ind()
void CPU_jmp_ind()
{//192 llamadas 1 segundo
  //gb_cont_call++;
  //unsigned short int addr = CPU_mem_->MEMORY_read_word(CPU_addr_abs());
  unsigned short int addr = MEMORY_read_word(CPU_addr_abs()); //No objetos
  CPU_pc_= addr; //CPU_pc_set(addr)
  CPU_cycles_+= 3;//CPU_tick(3);    
}
 
//
//@brief ReTurn from SubRoutine
//
//void Cpu::CPU_rts()
void CPU_rts()
{
  unsigned short int addr = (CPU_pop() + (CPU_pop() << 8)) + 1;
  CPU_pc_= addr; //CPU_pc_set(addr)
  CPU_cycles_+= 6;//CPU_tick(6);
}

// 
//@brief Branch if Not Equal
//
//void Cpu::CPU_bne()
void CPU_bne()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(!CPU_zf_get())
  if(!CPU_zf_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);
}

// 
//@brief CoMPare
//
//void Cpu::CPU_cmp(unsigned char v, unsigned char cycles)
inline void CPU_cmp(unsigned char v, unsigned char cycles)
{
  unsigned short int t;
  t = CPU_a_ - v;
  CPU_cf_= (t<0x100); //CPU_cf_set(t<0x100)
  t = t&0xff;
  SET_ZF(t);
  SET_NF(t);
  CPU_cycles_+= cycles;//CPU_tick(cycles);
}

// 
//@brief CoMPare X
//
//void Cpu::CPU_cpx(unsigned char v, unsigned char cycles)
inline void CPU_cpx(unsigned char v, unsigned char cycles)
{
  unsigned short int t;
  t = CPU_x_ - v;
  CPU_cf_= (t<0x100); //CPU_cf_set(t<0x100)
  t = t&0xff;
  SET_ZF(t);
  SET_NF(t);
  CPU_cycles_+= cycles;//CPU_tick(cycles);
}

// 
//@brief CoMPare Y
//
//void Cpu::CPU_cpy(unsigned char v, unsigned char cycles)
inline void CPU_cpy(unsigned char v, unsigned char cycles)
{
  unsigned short int t;
  t = CPU_y_ - v;
  CPU_cf_= (t<0x100); //CPU_cf_set(t<0x100)
  t = t&0xff;
  SET_ZF(t);
  SET_NF(t);
  CPU_cycles_+= cycles;//CPU_tick(cycles);
}
 
// 
//@brief Branch if Equal
//
//void Cpu::CPU_beq()
void CPU_beq()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(CPU_zf_get())
  if(CPU_zf_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);
}

// 
//@brief Branch if Carry is Set
//
//void Cpu::CPU_bcs()
void CPU_bcs()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(CPU_cf_get())
  if(CPU_cf_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);
}

// 
//@brief Branch if Carry is Clear
//
//void Cpu::CPU_bcc()
void CPU_bcc()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(!CPU_cf_get())
  if(!CPU_cf_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);
}
 
//
//@brief, Branch if PLus
//
//void Cpu::CPU_bpl()
void CPU_bpl()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(!CPU_nf())
  if(!CPU_nf_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);
}

//
//@brief Branch if MInus
//
//void Cpu::CPU_bmi()
void CPU_bmi()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(CPU_nf())
  if(CPU_nf_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);    
}

//
//@brief Branch if oVerflow Clear
//
//void Cpu::CPU_bvc()
void CPU_bvc()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(!CPU_of())
  if(!CPU_of_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);    
}

//
//@brief Branch if oVerflow Set
//
//void Cpu::CPU_bvs()
void CPU_bvs()
{
  unsigned short int addr = (signed char) MEMORY_read_byte(CPU_pc_++) + CPU_pc_; //(signed char) CPU_fetch_op() + CPU_pc_get()
  //if(CPU_of())
  if(CPU_of_)
  {
   CPU_pc_= addr; //CPU_pc_set(addr)
  }
  CPU_cycles_+= 2;//CPU_tick(2);    
}

// misc //////////////////////////////////////////////////////////////////////

//
//@brief No OPeration
//
//void Cpu::CPU_nop()
//void CPU_nop()
//{//0
//// gb_cont_call++;
// CPU_cycles_+= 2;//CPU_tick(2);
//}

//
//@brief BReaKpoint
//
//void Cpu::CPU_brk()
void CPU_brk()
{
  CPU_push(((CPU_pc_+1) >> 8) & 0xff); //CPU_push(((CPU_pc_get()+1) >> 8) & 0xff)
  CPU_push(((CPU_pc_+1) & 0xff)); //CPU_push(((CPU_pc_get()+1) & 0xff))
  CPU_push(CPU_flags_get());
  //CPU_pc_set(CPU_mem_->MEMORY_read_word(MEMORY_kAddrIRQVector));
  CPU_pc_= MEMORY_read_word(MEMORY_kAddrIRQVector); //No objetos
  CPU_idf_= true; //CPU_idf_set(true);
  CPU_bcf_= true; //CPU_bcf(true)
  CPU_cycles_+= 7;//CPU_tick(7);
}

//
//@brief ReTurn from Interrupt 
//
//void Cpu::CPU_rti()
void CPU_rti()
{
  CPU_flags_set(CPU_pop());
  CPU_pc_= (CPU_pop() + (CPU_pop() << 8)); //CPU_pc_set(CPU_pop() + (CPU_pop() << 8))
  CPU_cycles_+= 7;//CPU_tick(7);
}

// interrupts  ///////////////////////////////////////////////////////////////

//
//@brief Interrupt ReQuest
//
//void Cpu::CPU_irq()
void CPU_irq()
{          
 //if(!CPU_idf_get())
 if(!CPU_idf_)
 {
  CPU_push((CPU_pc_ >> 8) & 0xff);//CPU_push(((CPU_pc_get()) >> 8) & 0xff);
  CPU_push((CPU_pc_ & 0xff));
  //push flags with bcf cleared
  CPU_push((CPU_flags_get()&0xef));
  //CPU_pc_set(CPU_mem_->MEMORY_read_word(MEMORY_kAddrIRQVector));
  CPU_pc_= (MEMORY_read_word(MEMORY_kAddrIRQVector)); //CPU_pc_set(MEMORY_read_word(MEMORY_kAddrIRQVector)); //No objetos
  CPU_idf_= true; //CPU_idf_set(true);
  CPU_cycles_+= 7;//CPU_tick(7);
 }
}

//
//@brief Non Maskable Interrupt
//
//void Cpu::CPU_nmi()
void CPU_nmi()
{
  CPU_push(((CPU_pc_) >> 8) & 0xff); //CPU_push(((CPU_pc_get()) >> 8) & 0xff)
  CPU_push(((CPU_pc_) & 0xff)); //CPU_push(((CPU_pc_get()) & 0xff))
  //push flags with bcf cleared
  CPU_push((CPU_flags_get() & 0xef));
  //CPU_pc_set(CPU_mem_->MEMORY_read_word(MEMORY_kAddrNMIVector));
  CPU_pc_= MEMORY_read_word(MEMORY_kAddrNMIVector); //No objetos
  CPU_cycles_+= 7;//CPU_tick(7);
}

// debugging /////////////////////////////////////////////////////////////////

//void Cpu::CPU_dump_regs()
void CPU_dump_regs()
{
//  std::stringstream sflags;
//  //if(CPU_cf_get())
//  if(CPU_cf_)
//  {
//   sflags << "CF ";
//  }
//  //if(CPU_zf_get())
//  if(CPU_zf_)
//  {
//   sflags << "ZF ";
//  }
//  if(CPU_idf_) sflags << "IDF "; //if(CPU_idf_get()) sflags << "IDF ";
//  //if(CPU_dmf())
//  if(CPU_dmf_)
//  {
//   sflags << "DMF ";
//  }
//  //if(CPU_bcf())
//  if(CPU_bcf_)
//  {
//   sflags << "BCF ";
//  }
//  //if(CPU_of())
//  if(CPU_of_)
//  {
//   sflags << "OF ";
//  }
//  //if(CPU_nf())
//  if(CPU_nf_)
//  {
//   sflags << "NF ";
//  }
//  //D("pc=%04x, a=%02x x=%02x y=%02x sp=%02x flags= %s\n",
//  //  pc(),a(),x(),y(),sp(),sflags.str().c_str());
//  Serial.printf("pc=%04x, a=%02x x=%02x y=%02x sp=%02x flags= %s\r\n", CPU_pc_,CPU_a_,CPU_x_,CPU_y_,CPU_sp_,sflags.str().c_str());
//Falta flags, da igual para ESP32, lo tengo en PC. No quiero STD
Serial.printf("pc=%04x, a=%02x x=%02x y=%02x sp=%02x flags= Falta\r\n", CPU_pc_,CPU_a_,CPU_x_,CPU_y_,CPU_sp_);
  //fflush(stdout);
}

//void Cpu::CPU_dump_regs_json()
void CPU_dump_regs_json()
{
 //D("{");
 //D("\"pc\":%d,",pc());
 //D("\"a\":%d,",a());
 //D("\"x\":%d,",x());
 //D("\"y\":%d,",y());
 //D("\"sp\":%d",sp());
 //D("}\n");

 Serial.printf("{\r\n");
 Serial.printf("\"pc\":%d,\r\n",CPU_pc_);
 Serial.printf("\"a\":%d,\r\n",CPU_a_);
 Serial.printf("\"x\":%d,\r\n",CPU_x_);
 Serial.printf("\"y\":%d,\r\n",CPU_y_);
 Serial.printf("\"sp\":%d\r\n",CPU_sp_);
 Serial.printf("}\n");
 //fflush(stdout);
}






//void Cpu::CPU_memory(Memory *v)
//void CPU_memory(Memory *v) //No quiero objetos
//{
// CPU_mem_ = v;
//};

//Memory* Cpu::CPU_memory()
//Memory* CPU_memory() //No quiero objetos
//{
// return CPU_mem_;
//};

//register access
//unsigned short int Cpu::CPU_pc() 
//unsigned short int CPU_pc_get() 
//{//146464 
// gb_cont_call++;
// return CPU_pc_;
//};

//void Cpu::CPU_pc(unsigned short int v)
//void CPU_pc_set(unsigned short int v)
//{//146464 
// gb_cont_call++;     
// CPU_pc_=v;
//};

//unsigned char Cpu::CPU_sp()
//unsigned char CPU_sp_get()
//{//97
// gb_cont_call++;
// return CPU_sp_;
//};

//void Cpu::CPU_sp(unsigned char v)
//void CPU_sp_set(unsigned char v)
//{//1
// gb_cont_call++;
// CPU_sp_=v;
//};

//unsigned char Cpu::CPU_a()
//unsigned char CPU_a_get()
//{//522938 llamadas 1 segundos
// //gb_cont_call++;
// return CPU_a_;
//};

//void Cpu::CPU_a(unsigned char v)
//void CPU_a_set(unsigned char v)
//{//135582 llamadas 1 segundo
// //gb_cont_call++;     
// CPU_a_=v;
//};

//unsigned char Cpu::CPU_x()
//unsigned char CPU_x_get()
//{// Tenia 15900 Ya no se usa
// //gb_cont_call++;
// 
// return CPU_x_;
//};

//void Cpu::CPU_x(unsigned char v)
//void CPU_x_set(unsigned char v)
//{//1500 llamada en 1 segundo. Ya no se usa
// //gb_cont_call++;
// 
// CPU_x_=v;
//};

//unsigned char Cpu::CPU_y()
//unsigned char CPU_y_get()
//{//20486 llamadas 1 segundo. Ya no se usa
// //gb_cont_call++;
// 
// return CPU_y_;
//};

//void Cpu::CPU_y(unsigned char v)
//void CPU_y_set(unsigned char v)
//{//574 llamadas 1 segundo. Ya no se usa
// //gb_cont_call++;     
//     
// CPU_y_=v;
//};

//flags
//bool Cpu::CPU_cf()
//bool CPU_cf_get()
//{//13540 
// //gb_cont_call++;
// return CPU_cf_;
//};

//void Cpu::CPU_cf(bool v)
//void CPU_cf_set(bool v)
//{//16133
// //gb_cont_call++;     
// CPU_cf_=v;
//};

//bool Cpu::CPU_zf()
//bool CPU_zf_get()
//{//134325 
// //gb_cont_call++;
// return CPU_zf_;
//};

//void Cpu::CPU_zf(bool v)
//void CPU_zf_set(bool v)
//{//156236 
// //gb_cont_call++;     
// CPU_zf_=v;
//};

//bool Cpu::CPU_idf()
//bool CPU_idf_get()
//{
// return CPU_idf_;
//};

//void Cpu::CPU_idf(bool v)
//void CPU_idf_set(bool v)
//{
// CPU_idf_=v;
//};

//bool Cpu::CPU_dmf()
//bool CPU_dmf()
//{//376
// //gb_cont_call++;
// return CPU_dmf_;
//};

//void Cpu::CPU_dmf(bool v)
//void CPU_dmf(bool v)
//{//95
// gb_cont_call++;
// CPU_dmf_=v;
//};

//bool Cpu::CPU_bcf()
//bool CPU_bcf()
//{//0
// //gb_cont_call++;
// return CPU_bcf_;
//};

//void Cpu::CPU_bcf(bool v)
//void CPU_bcf(bool v)
//{
// //gb_cont_call++;     
// CPU_bcf_=v;
//};

//bool Cpu::CPU_of()
//bool CPU_of()
//{
// gb_cont_call++;
// return CPU_of_;
//};

//void Cpu::CPU_of(bool v)
//void CPU_of(bool v)
//{
// gb_cont_call++;
// CPU_of_=v;
//};

//bool Cpu::CPU_nf()
//bool CPU_nf()
//{
// gb_cont_call++;     
// return CPU_nf_;
//};

//void Cpu::CPU_nf(bool v)
//void CPU_nf(bool v)
//{
// gb_cont_call++;     
// CPU_nf_=v;
//};

//clock
//unsigned int Cpu::CPU_cycles()
//unsigned int CPU_cycles_get()
//{
// return CPU_cycles_;
//};

//void Cpu::CPU_cycles(unsigned int v)
//void CPU_cycles_set(unsigned int v)
//{
// CPU_cycles_=v;
//};


//void Cpu::CPU_tick(unsigned char v)
//inline void CPU_tick(unsigned char v)
//{
// CPU_cycles_+=v;
//};
