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

#ifndef EMUDORE_CPU_H
#define EMUDORE_CPU_H

//JJ #include <cstdint>
#include <stdint.h>
#include "memory.h"


//macro helpers

//#define SET_ZF(val)     (CPU_zf_set(!(unsigned char)(val)))
//#define SET_NF(val)     (CPU_nf(((unsigned char)(val)&0x80)!=0))
#define SET_ZF(val)     (CPU_zf_=(!(unsigned char)(val)))
#define SET_NF(val)     (CPU_nf_=(((unsigned char)(val)&0x80)!=0))


/*
//
//@brief MOS 6510 microprocessor
//
class Cpu
{
  private:
    //registers
    unsigned short int CPU_pc_;
    unsigned char CPU_sp_, CPU_a_, CPU_x_, CPU_y_;
    //flags (p/status reg)
    bool CPU_cf_,CPU_zf_,CPU_idf_,CPU_dmf_,CPU_bcf_,CPU_of_,CPU_nf_;
    //memory and clock
    Memory *CPU_mem_;
    unsigned int CPU_cycles_;
    //helpers
    inline unsigned char CPU_load_byte(unsigned short int addr);
    inline void CPU_push(unsigned char v);
    inline unsigned char CPU_pop();
    inline unsigned char CPU_fetch_op();
    inline unsigned short int CPU_fetch_opw();
    inline unsigned short int CPU_addr_zero();
    inline unsigned short int CPU_addr_zerox();
    inline unsigned short int CPU_addr_zeroy();
    inline unsigned short int CPU_addr_abs();
    inline unsigned short int CPU_addr_absy();
    inline unsigned short int CPU_addr_absx();
    inline unsigned short int CPU_addr_indx();
    inline unsigned short int CPU_addr_indy();
    inline unsigned char CPU_rol(unsigned char v);
    inline unsigned char CPU_ror(unsigned char v);
    inline unsigned char CPU_lsr(unsigned char v);
    inline unsigned char CPU_asl(unsigned char v);
    inline void CPU_tick(unsigned char v);//JJ{CPU_cycles_+=v;};
    inline unsigned char CPU_flags();
    inline void CPU_flags(unsigned char v);
    //instructions : data handling and memory operations
    inline void CPU_sta(unsigned short int addr, unsigned char cycles);
    inline void CPU_stx(unsigned short int addr, unsigned char cycles);
    inline void CPU_sty(unsigned short int addr, unsigned char cycles);
    inline void CPU_lda(unsigned char v, unsigned char cycles);
    inline void CPU_ldx(unsigned char v, unsigned char cycles);
    inline void CPU_ldy(unsigned char v, unsigned char cycles);
    inline void CPU_txs();
    inline void CPU_tsx();
    inline void CPU_tax();
    inline void CPU_txa();
    inline void CPU_tay();
    inline void CPU_tya();
    inline void CPU_pha();
    inline void CPU_pla();
    //instructions: logic operations
    inline void CPU_ora(unsigned char v, unsigned char cycles);
    inline void CPU__and(unsigned char v, unsigned char cycles);
    inline void CPU_bit(unsigned short int addr, unsigned char cycles);
    inline void CPU_rol_a();
    inline void CPU_rol_mem(unsigned short int addr, unsigned char cycles);
    inline void CPU_ror_a();
    inline void CPU_ror_mem(unsigned short int addr, unsigned char cycles);
    inline void CPU_asl_a();
    inline void CPU_asl_mem(unsigned short int addr, unsigned char cycles);
    inline void CPU_lsr_a();
    inline void CPU_lsr_mem(unsigned short int addr, unsigned char cycles);
    inline void CPU_eor(unsigned char v, unsigned char cycles);
    //instructions: arithmetic operations
    inline void CPU_inc(unsigned short int addr, unsigned char cycles);
    inline void CPU_dec(unsigned short int addr, unsigned char cycles);
    inline void CPU_inx();
    inline void CPU_iny();
    inline void CPU_dex();
    inline void CPU_dey();
    inline void CPU_adc(unsigned char v, unsigned char cycles);
    inline void CPU_sbc(unsigned char v, unsigned char cycles);
    //instructions: flag access
    inline void CPU_sei();
    inline void CPU_cli();
    inline void CPU_sec();
    inline void CPU_clc();
    inline void CPU_sed();
    inline void CPU_cld();
    inline void CPU_clv();
    inline void CPU_php();
    inline void CPU_plp();
    //instructions: control flow
    inline void CPU_cmp(unsigned char v, unsigned char cycles);
    inline void CPU_cpx(unsigned char v, unsigned char cycles);
    inline void CPU_cpy(unsigned char v, unsigned char cycles);
    inline void CPU_rts();
    inline void CPU_jsr();
    inline void CPU_bne();
    inline void CPU_beq();
    inline void CPU_bcs();
    inline void CPU_bcc();
    inline void CPU_bpl();
    inline void CPU_bmi();
    inline void CPU_bvc();
    inline void CPU_bvs();
    inline void CPU_jmp();
    inline void CPU_jmp_ind();
    //instructions: misc
    inline void CPU_nop();
    inline void CPU_brk();
    inline void CPU_rti();
  public:
    //cpu state
    void CPU_reset();
    bool CPU_emulate();
    //memory
    void CPU_memory(Memory *v); //JJ {CPU_mem_ = v;};
    Memory* CPU_memory(); //JJ {return CPU_mem_;};
    //register access
    unsigned short int CPU_pc(); //JJ {return CPU_pc_;}; //inline
    void CPU_pc(unsigned short int v); //JJ {CPU_pc_=v;}; //inline
    unsigned char CPU_sp(); //JJ {return CPU_sp_;}; //inline
    void CPU_sp(unsigned char v);//JJ {CPU_sp_=v;}; //inline
    unsigned char CPU_a();//JJ {return CPU_a_;}; //inline
    void CPU_a(unsigned char v);//JJ {CPU_a_=v;}; //inline
    unsigned char CPU_x();//JJ {return CPU_x_;}; //inline
    void CPU_x(unsigned char v);//JJ {CPU_x_=v;}; //inline
    unsigned char CPU_y();//JJ {return CPU_y_;}; //inline
    void CPU_y(unsigned char v);//JJ {CPU_y_=v;}; //inline
    //flags
    bool CPU_cf();//JJ {return CPU_cf_;}; //inline
    void CPU_cf(bool v);//JJ { CPU_cf_=v;}; //inline
    bool CPU_zf();//JJ {return CPU_zf_;}; //inline
    void CPU_zf(bool v);//JJ {CPU_zf_=v;}; //inline
    bool CPU_idf();//JJ {return CPU_idf_;}; //inline
    void CPU_idf(bool v);//JJ { CPU_idf_=v;}; //inline
    bool CPU_dmf();//JJ {return CPU_dmf_;}; //inline
    void CPU_dmf(bool v);//JJ { CPU_dmf_=v;}; //inline
    bool CPU_bcf();//JJ {return CPU_bcf_;}; //inline
    void CPU_bcf(bool v);//JJ { CPU_bcf_=v;}; //inline
    bool CPU_of();//JJ {return CPU_of_;}; //inline
    void CPU_of(bool v);//JJ { CPU_of_=v;}; //inline
    bool CPU_nf();//JJ {return CPU_nf_;}; //inline
    void CPU_nf(bool v);//JJ {CPU_nf_=v;}; //inline
    //clock
    unsigned int CPU_cycles();//JJ{return CPU_cycles_;}; //inline
    void CPU_cycles(unsigned int v);//JJ{CPU_cycles_=v;}; //inline
    //interrupts
    void CPU_nmi();
    void CPU_irq();
    //debug
    void CPU_dump_regs();
    void CPU_dump_regs_json();
};

*/






    //unsigned char CPU_load_byte(unsigned short int addr);
    void CPU_push(unsigned char v);
    unsigned char CPU_pop(void);
    //unsigned char CPU_fetch_op(void);
    unsigned short int CPU_fetch_opw(void);
    unsigned short int CPU_addr_zero(void);
    unsigned short int CPU_addr_zerox(void);
    unsigned short int CPU_addr_zeroy(void);
    unsigned short int CPU_addr_abs(void);
    unsigned short int CPU_addr_absy(void);
    unsigned short int CPU_addr_absx(void);
    unsigned short int CPU_addr_indx(void);
    unsigned short int CPU_addr_indy(void);
    inline unsigned char CPU_rol(unsigned char v);
    inline unsigned char CPU_ror(unsigned char v);
    inline unsigned char CPU_lsr(unsigned char v);
    unsigned char CPU_asl(unsigned char v);
    //inline void CPU_tick(unsigned char v);//JJ{CPU_cycles_+=v;};
    inline unsigned char CPU_flags_get(void);
    inline void CPU_flags_set(unsigned char v);
    //instructions : data handling and memory operations
    void CPU_sta(unsigned short int addr, unsigned char cycles);
    void CPU_stx(unsigned short int addr, unsigned char cycles);
    void CPU_sty(unsigned short int addr, unsigned char cycles);
    inline void CPU_lda(unsigned char v, unsigned char cycles);
    inline void CPU_ldx(unsigned char v, unsigned char cycles);
    inline void CPU_ldy(unsigned char v, unsigned char cycles);
    inline void CPU_txs(void);
    inline void CPU_tsx(void);
    inline void CPU_tax(void);
    inline void CPU_txa(void);
    inline void CPU_tay(void);
    void CPU_tya(void);
    void CPU_pha(void);
    void CPU_pla(void);
    //instructions: logic operations
    inline void CPU_ora(unsigned char v, unsigned char cycles);
    inline void CPU__and(unsigned char v, unsigned char cycles);
    void CPU_bit(unsigned short int addr, unsigned char cycles);
    void CPU_rol_a(void);
    void CPU_rol_mem(unsigned short int addr, unsigned char cycles);
    void CPU_ror_a(void);
    void CPU_ror_mem(unsigned short int addr, unsigned char cycles);
    void CPU_asl_a(void);
    void CPU_asl_mem(unsigned short int addr, unsigned char cycles);
    void CPU_lsr_a(void);
    void CPU_lsr_mem(unsigned short int addr, unsigned char cycles);
    inline void CPU_eor(unsigned char v, unsigned char cycles);
    //instructions: arithmetic operations
    void CPU_inc(unsigned short int addr, unsigned char cycles);
    void CPU_dec(unsigned short int addr, unsigned char cycles);
    inline void CPU_inx(void);
    inline void CPU_iny(void);
    inline void CPU_dex(void);
    inline void CPU_dey(void);
    void CPU_adc(unsigned char v, unsigned char cycles);
    void CPU_sbc(unsigned char v, unsigned char cycles);
    //instructions: flag access
    inline void CPU_sei(void);
    inline void CPU_cli(void);
    inline void CPU_sec(void);
    inline void CPU_clc(void);
    inline void CPU_sed(void);
    inline void CPU_cld(void);
    inline void CPU_clv(void);
    void CPU_php(void);
    void CPU_plp(void);
    //instructions: control flow
    inline void CPU_cmp(unsigned char v, unsigned char cycles);
    inline void CPU_cpx(unsigned char v, unsigned char cycles);
    inline void CPU_cpy(unsigned char v, unsigned char cycles);
    void CPU_rts(void);
    void CPU_jsr(void);
    void CPU_bne(void);
    void CPU_beq(void);
    void CPU_bcs(void);
    void CPU_bcc(void);
    void CPU_bpl(void);
    void CPU_bmi(void);
    void CPU_bvc(void);
    void CPU_bvs(void);
    void CPU_jmp(void);
    void CPU_jmp_ind(void);
    //instructions: misc
    //void CPU_nop();
    void CPU_brk(void);
    void CPU_rti(void);





    //cpu state
    void CPU_reset(void);
    //bool CPU_emulate();
    void CPU_emulate(void); //Quito retorno pila
    //memory
    //void CPU_memory(Memory *v); //JJ {CPU_mem_ = v;};
    //Memory* CPU_memory(); //JJ {return CPU_mem_;};
    //register access
    //unsigned short int CPU_pc_get(); //JJ {return CPU_pc_;}; //inline
    //void CPU_pc_set(unsigned short int v); //JJ {CPU_pc_=v;}; //inline
    //unsigned char CPU_sp_get(); //JJ {return CPU_sp_;}; //inline
    //void CPU_sp_set(unsigned char v);//JJ {CPU_sp_=v;}; //inline
    //unsigned char CPU_a_get();//JJ {return CPU_a_;}; //inline
    //void CPU_a_set(unsigned char v);//JJ {CPU_a_=v;}; //inline
    //unsigned char CPU_x_get();//JJ {return CPU_x_;}; //inline
    //void CPU_x_set(unsigned char v);//JJ {CPU_x_=v;}; //inline
    //unsigned char CPU_y_get();//JJ {return CPU_y_;}; //inline
    //void CPU_y_set(unsigned char v);//JJ {CPU_y_=v;}; //inline
    //flags
    //bool CPU_cf_get(void);//JJ {return CPU_cf_;}; //inline
    //void CPU_cf_set(bool v);//JJ { CPU_cf_=v;}; //inline
    //bool CPU_zf_get();//JJ {return CPU_zf_;}; //inline
    //void CPU_zf_set(bool v);//JJ {CPU_zf_=v;}; //inline
    //bool CPU_idf_get();//JJ {return CPU_idf_;}; //inline
    //void CPU_idf_set(bool v);//JJ { CPU_idf_=v;}; //inline
    //bool CPU_dmf();//JJ {return CPU_dmf_;}; //inline
    //void CPU_dmf(bool v);//JJ { CPU_dmf_=v;}; //inline
    //bool CPU_bcf();//JJ {return CPU_bcf_;}; //inline
    //void CPU_bcf(bool v);//JJ { CPU_bcf_=v;}; //inline
    //bool CPU_of();//JJ {return CPU_of_;}; //inline
    //void CPU_of(bool v);//JJ { CPU_of_=v;}; //inline
    //bool CPU_nf();//JJ {return CPU_nf_;}; //inline
    //void CPU_nf(bool v);//JJ {CPU_nf_=v;}; //inline
    //clock
    //unsigned int CPU_cycles_get();//JJ{return CPU_cycles_;}; //inline
    //void CPU_cycles_set(unsigned int v);//JJ{CPU_cycles_=v;}; //inline
    //interrupts
    void CPU_nmi(void);
    void CPU_irq(void);
    //debug
    void CPU_dump_regs(void);
    void CPU_dump_regs_json(void);










#endif
