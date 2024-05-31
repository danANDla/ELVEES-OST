// ******************** risc_interrupt.c ***************************************
//                 Risc-Dsp runtime library
//
// Interface to enable/disable risc interrupts
// by setting MASKR0, MASKR1, MASKR2
// This file contains handler function. Don't use it!
// Use library interface
//
// (c) multicore.ru
//
// *******************************************************************

#include "cpu.h"
#include "risc_interrupt.h"
#include "system.h"

#define MASKR_NUM_MAX (5)
static volatile RISC_INTERRUPT risc_hnlr_func[MASKR_NUM_MAX*32+16] = {RISC_INTERRUPT_UNKNOWN};


#if (defined(TARGET_MCT04))
  #define  CPU_NUMS (2)
  #define  MASKR_NUMS (5)
  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0(0), &MASKR1(0), &MASKR2(0), &MASKR3(0), &MASKR4(0),
	  &MASKR0(1), &MASKR1(1), &MASKR2(1), &MASKR3(1), &MASKR4(1)
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0(0), &QSTR1(0), &QSTR2(0), &QSTR3(0), &QSTR4(0),
	  &QSTR0(1), &QSTR1(1), &QSTR2(1), &QSTR3(1), &QSTR4(1)
  };

  #define SET_TR_CRAM() SYS_CSR(_get_cpu_id()) |= 2
  #define UNSET_TR_CRAM() SYS_CSR(_get_cpu_id()) &= ~(2)
#elif (defined(TARGET_MCT05))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (4)
  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0(0), &MASKR1(0), &MASKR2(0), &MASKR3(0)
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0(0), &QSTR1(0), &QSTR2(0), &QSTR3(0)
  };

  #define SET_TR_CRAM() SYS_CSR(_get_cpu_id()) |= 2
  #define UNSET_TR_CRAM() SYS_CSR(_get_cpu_id()) &= ~(2)
#elif (defined(TARGET_MCT07))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (4)
  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0(0), &MASKR1(0), &MASKR2(0), &MASKR3(0)
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0(0), &QSTR1(0), &QSTR2(0), &QSTR3(0)
  };

  #define SET_TR_CRAM() SYS_CSR(_get_cpu_id()) |= 2
  #define UNSET_TR_CRAM() SYS_CSR(_get_cpu_id()) &= ~(2)
#elif (defined(TARGET_MC0428))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (4)

  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0(0), &MASKR1(0), &MASKR2(0), &MASKR3(0)
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0(0), &QSTR1(0), &QSTR2(0), &QSTR3(0)
  };

  #define SET_TR_CRAM() SYS_CSR |= 2
  #define UNSET_TR_CRAM() SYS_CSR &= ~(2)
#elif (defined(TARGET_NVCOM02T))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (4)

  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0(0), &MASKR1(0), &MASKR2(0)
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0(0), &QSTR1(0), &QSTR2(0)
  };

  #define SET_TR_CRAM() SYS_CSR |= 2
  #define UNSET_TR_CRAM() SYS_CSR &= ~(2)
#elif (defined(TARGET_MCT03P))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (4)

  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0(0), &MASKR1(0), &MASKR2(0), &MASKR3(0)
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0(0), &QSTR1(0), &QSTR2(0), &QSTR3(0)
  };

  #define SET_TR_CRAM() SYS_CSR |= 2
  #define UNSET_TR_CRAM() SYS_CSR &= ~(2)
#elif (defined(TARGET_MC30SF6))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (4)

  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
    &MASKR0, &MASKR1, &MASKR2, &MASKR3
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
    &QSTR0, &QSTR1, &QSTR2, &QSTR3
  };

  #define SET_TR_CRAM() SYS_CSR |= 2
  #define UNSET_TR_CRAM() SYS_CSR &= ~(2)
#elif (defined(TARGET_MC24R) || defined(TARGET_NVCOM02T))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (3)

  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0, &MASKR1, &MASKR2
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0, &QSTR1, &QSTR2
  };

 #define SET_TR_CRAM() SYS_CSR |= 2
 #define UNSET_TR_CRAM() SYS_CSR &= ~(2)
#elif (defined(TARGET_MCT06))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (5)
  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0(0), &MASKR1(0), &MASKR2(0), &MASKR3(0), &MASKR4(0)
  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0(0), &QSTR1(0), &QSTR2(0), &QSTR3(0), &QSTR4(0)
  };

  #define SET_TR_CRAM() SYS_CSR(_get_cpu_id()) |= 2
  #define UNSET_TR_CRAM() SYS_CSR(_get_cpu_id()) &= ~(2)

//by irs for mct08
#elif (defined(TARGET_MCT08))
  #define  CPU_NUMS (1)
  #define  MASKR_NUMS (5)
  static volatile int* MASKR[CPU_NUMS][MASKR_NUMS] =
  {
	  &MASKR0, &MASKR1, &MASKR2, &MASKR3, &MASKR4

  };

  static volatile int* QSTR[CPU_NUMS][MASKR_NUMS] =
  {
	  &QSTR0, &QSTR1, &QSTR2, &QSTR3, &QSTR4
  };

  #define SET_TR_CRAM() SYS_CSR |= (1<<1);
  #define UNSET_TR_CRAM() SYS_CSR &= ~(2)
//end by irs for mct08


#endif

void risc_set_interrupts_vector(INTERRUPT_VECTORS loc)
{
    unsigned int temp = 0;
    asm volatile ("mfc0 %0, $12" :"=r"(temp));
    temp &= ~(4);
    asm volatile ("mtc0 %0, $12" ::"r"(temp));

    switch(loc)
    {
    case INTH_80000180:
        mips_write_c0_register(C0_CAUSE, 0);
        temp = mips_read_c0_register(C0_STATUS);
        temp &= ~(1<<22);
        mips_write_c0_register(C0_STATUS, temp);
        UNSET_TR_CRAM();
        break;
    case INTH_B8000180:
        mips_write_c0_register(C0_CAUSE, 0);
        temp = mips_read_c0_register(C0_STATUS);
      	temp &= ~(1<<22);
			//#if (defined(TARGET_MCT08))
      			temp &= (1<<0);
			//#endif
      	mips_write_c0_register(C0_STATUS, temp);
        SET_TR_CRAM();
        break;
    case INTH_BFC00380:
    	mips_write_c0_register(C0_CAUSE, 0);
    	temp = mips_read_c0_register(C0_STATUS);
        temp |= (1<<22);
        mips_write_c0_register(C0_STATUS, temp);
        break;
    case INTH_80000200:
        mips_write_c0_register(C0_CAUSE, (1<<23));
        temp = mips_read_c0_register(C0_STATUS);
        temp &= ~(1<<22);
        mips_write_c0_register(C0_STATUS, temp);
        UNSET_TR_CRAM();
        break;
    case INTH_B8000200:
    	mips_write_c0_register(C0_CAUSE, (1<<23));
    	temp = mips_read_c0_register(C0_STATUS);
        temp &= ~(1<<22);
        mips_write_c0_register(C0_STATUS, temp);
        SET_TR_CRAM();
        break;
    case INTH_BFC00400:
    	mips_write_c0_register(C0_CAUSE, (1<<23));
        temp = mips_read_c0_register(C0_STATUS);
        temp |= (1<<22);
        mips_write_c0_register(C0_STATUS, temp);
        break;
    };
}

void risc_disable_interrupts()
{
    unsigned int CP0_Status = mips_read_c0_register(C0_STATUS);
    CP0_Status &= ~(0x1);
    mips_write_c0_register(C0_STATUS, CP0_Status);
}

void risc_enable_interrupts()
{
    unsigned int CP0_Status;
    CP0_Status = mips_read_c0_register(C0_STATUS);
    CP0_Status |= ((1<<22)|(1<<2)|(1<<0));
    mips_write_c0_register(C0_STATUS, CP0_Status);
}

static int type_to_index(RISC_INTERRUPT_TYPE type)
{
    unsigned int index = 0;
    unsigned int temp = 0;
    temp = type>>8;
    if (temp == 8)return (129 + (type&0xFF));
    else
    {
      index = (type)&0x1F;
      index |= (type&~0x1F)>>3;
    }
    return index;
}

enum ERL_ERROR risc_register_interrupt(RISC_INTERRUPT fn, RISC_INTERRUPT_TYPE type)
{
  if (fn != 0)
  {
    unsigned int index = type_to_index(type);
    risc_hnlr_func[index] = fn;
  }
  else return ERL_UNITIALIZED_ARG;
  return ERL_NO_ERROR;
}

//TODO

static int get_power_of(unsigned int v)
{
  int cnt = 0;
  asm volatile("move $t2, %1\n\t"
                "clz $t3, $t2\n\t"
                "move %0, $t3\n\t":"=r"(cnt):"r"(v));
  return 31-cnt;
}

static int get_cause_risc(unsigned int v, unsigned int cpu_id)
{
    int i = 0;
    unsigned int qstr_num = 6, temp;
    for (i=0; i<5; i++)
    {
      temp = (v>>(10+i)) & 1;
      if (temp) qstr_num = i;
    }

	if ((qstr_num>=0) && (qstr_num<=4))
	{
		int maskr_val = *MASKR[cpu_id][qstr_num];
		int qstr_val  = *QSTR[cpu_id][qstr_num];
		int bit = get_power_of(maskr_val&qstr_val);
		return (qstr_num<<8)|bit;
	}
    else if(v&0x8000)
    {
        // RISC_INT_COMPARE
        return RISC_COMPARE;
    }
    //Exceptions
    if (((v&0x7F)>>2) == 1) return RICS_EXC_MOD;
    else if (((v&0x7F)>>2) == 2) return RICS_EXC_TLBL;
    else if (((v&0x7F)>>2) == 3) return RICS_EXC_TLBS;
    else if (((v&0x7F)>>2) == 4) return RICS_EXC_ADEL;
    else if (((v&0x7F)>>2) == 5) return RICS_EXC_ADES;
    else if (((v&0x7F)>>2) == 8) return RICS_EXC_SYS;
    else if (((v&0x7F)>>2) == 9) return RICS_EXC_BP;
    else if (((v&0x7F)>>2) == 10) return RICS_EXC_RI;
    else if (((v&0x7F)>>2) == 11) return RICS_EXC_CPU;
    else if (((v&0x7F)>>2) == 12) return RICS_EXC_OV;
    else if (((v&0x7F)>>2) == 13) return RICS_EXC_TRAP;
    else if (((v&0x7F)>>2) == 15) return RICS_EXC_FPE;
    else if (((v&0x7F)>>2) == 24) return RICS_EXC_MCHECK;
	return 0;
}

void risc_enable_interrupt(RISC_INTERRUPT_TYPE type, int cpu_id)
{
    unsigned int CP0_Status = 0;
    unsigned int qstr_num = 0;
    unsigned int qstr_bit = 0;

    qstr_num = (type>>8) & 0xFF;
    qstr_bit = type & 0xFF;
    if ((qstr_num>=0) && (qstr_num<6))
    {
        CP0_Status = mips_read_c0_register(C0_STATUS);

        CP0_Status |= 1<<(10+qstr_num);
    	CP0_Status |= 1;

    	mips_write_c0_register(C0_STATUS, CP0_Status );

        *MASKR[cpu_id][qstr_num] |= 1<<(qstr_bit);
    }
    else if(type == RISC_COMPARE)
    {
        // RISC_COMPARE
        CP0_Status = mips_read_c0_register(C0_STATUS);
        mips_write_c0_register(C0_STATUS, (CP0_Status|0x8001) );
    }
}

void risc_disable_interrupt(RISC_INTERRUPT_TYPE type, int cpu_id)
{
    unsigned int CP0_Status = 0;
	unsigned int qstr_num = 0;
	unsigned int qstr_bit = 0;

	qstr_num = (type>>8) & 0xFF;
	qstr_bit = type & 0xFF;
	if (qstr_bit != 0)
	{
	  *MASKR[cpu_id][qstr_num] &= ~(1<<(qstr_bit));
	}
	else if (qstr_num < 6)
	{
	  asm volatile ("mfc0 %0, $12" :"=r"(CP0_Status));
      CP0_Status &= ~(1<<(10+qstr_num));
      asm volatile ("mtc0 %0, $12" ::"r"(CP0_Status));
	}
}

void risc_common_handler(int cpu_id)
{
   unsigned int v = 0;
   int cause = 0;
   asm volatile ("mfc0 %0, $13" :"=r"(v));
   //cause = get_cause_risc(v, cpu_id);
   cause = get_cause_risc(v, 0);
   int index = type_to_index(cause);

   if ((cause <= RISC_INT_MEMCH_MAX)&&(cause >= RISC_INT_MEMCH0)  // calls handler
		   &&(risc_hnlr_func[index] != RISC_INTERRUPT_UNKNOWN)) // for all DMA memch interrupts
   {
      risc_hnlr_func[index](cause); // arg - RISC_INT_MEMCHx value
   }

else if ((cause <= 31)&&(cause >= 0)  // calls handler
		   &&(risc_hnlr_func[index] != RISC_INTERRUPT_UNKNOWN)) // for all interrupts
   {
	   risc_hnlr_func[index](cause); // arg - RISC_INT_... value
   }

   #if (defined(TARGET_MC30SF6)||defined(TARGET_NVCOM02T))
   else if ((cause == RISC_FROM_DSP) && (risc_hnlr_func[RISC_FROM_DSP] != RISC_INTERRUPT_UNKNOWN))
   {
     if (DCSR(0)&0x1)
     {
       risc_hnlr_func[index](R30_L(0));
     }
     else if (DCSR(1)&0x1)
     {
       risc_hnlr_func[index](R30_L(1));
     }
   }
#endif
   else if (risc_hnlr_func[index] != RISC_INTERRUPT_UNKNOWN)
   {
       risc_hnlr_func[index](-1);
   }
}
