// ******************** risc_timer.c ***************************************
//                 Risc-Dsp runtime library
//
// Realization of interface to IT and WDT timers using ITSCALE,
// ITPERIOD and ITCSR registers
//
// (c) multicore.ru
//
// *******************************************************************

#include "risc_timer.h"
#include "cpu.h"
#include "risc_interrupt.h"
#include "system.h"

unsigned int _risc_clk_counter = 0;
#ifdef __DMA_TIME_COUNTING__
  extern float _dma_working_time_counter;
#endif

static int get_core () { return 0; }

void risc_it_setup(unsigned int period, int clk_src)
{
  ITSCALE0 = 0;
  ITPERIOD0 = period;
  ITCSR0 = (clk_src << 3);
}

void risc_it_start()
{
  ITCSR0 |= 1;
}

void risc_it_stop()
{
  ITCSR0 &= 0xfe;
}

void risc_tics_start()
{
  risc_it_setup(0xffffffff ,0);
  risc_it_start();
}

unsigned int risc_tics_get()
{
  unsigned int clk = 0xffffffff - ITCOUNT0;
  return clk;
}


// ITtimer interrupt handler
static void _risc_timer_handler(int var)
{
  if ( (ITCSR0 & 2) == 2 )
    ITCSR0 = 1;
  _risc_clk_counter += 1;
}

void risc_clk_ms_start()
{
  _risc_clk_counter = 0;
  unsigned int clock = get_cpu_clock();
  risc_enable_interrupt(RISC_INT_IT0, get_core ());
  risc_register_interrupt(_risc_timer_handler, RISC_INT_IT0);
  risc_it_setup((unsigned int)(clock/1000) ,0);
  risc_it_start();
}

float risc_clk_ms_get()
{
  float us = 1 - ((float)ITCOUNT0/(float)ITPERIOD0);
  return us + (float)_risc_clk_counter;
}

void sys_time_init()
{
#if (defined(TARGET_NVCOM02T)||defined(TARGET_MC30SF6)) 
  common_regs* dsp_common = (common_regs*)(_REGSCM);
  dsp_common->TOTAL_CLK_CNTR = 0;
#endif
#ifdef __DMA_TIME_COUNTING__
  _dma_working_time_counter = 0;
#endif
  risc_clk_ms_start();
}

void sys_time_get(struct risc_sys_timer* tmr)
{
#if (defined(TARGET_NVCOM02T)||defined(TARGET_MC30SF6)) 
  unsigned int dsp_clk = get_dsp_clock();
  common_regs* dsp_common = (common_regs*)(_REGSCM);
  tmr->dsp_ms_time = ((float)dsp_common->TOTAL_CLK_CNTR/(float)dsp_clk)*1000;
#else
  tmr->dsp_ms_time = 0;
#endif

#ifdef __DMA_TIME_COUNTING__
  tmr->dma_ms_time = _dma_working_time_counter;
#else
  tmr->dma_ms_time = 0;
#endif
  tmr->it_ms_time = risc_clk_ms_get();
}
#ifndef TARGET_MCT04
//by irs//
void risc_wdt_setup(unsigned int period, unsigned int scale)
{
  WTSCALE = scale;
  WTPERIOD = period;
}


void risc_wdt_start_wdtmode()
{
  WDT_CSR w_csr;
  w_csr._val = 0;
  w_csr.mode = 0;
 // w_csr.rld = rld;
  w_csr.int_ctr = 1;
  WTCSR = w_csr._val;
  WTCSR |= (1<<8);
}

void risc_wdt_stop()
{
  WTCSR &= 0x3EFF;
}
#endif
//by irs
