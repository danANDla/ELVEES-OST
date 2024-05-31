// ***************** simple_time.ñ ***********************************
//                 Risc-Dsp runtime library
//
// This file implements interface to system time access
//
// (c) multicore.ru
//
// *******************************************************************
#include "system.h"
#include "risc_cp0_timer.h"
#include "cpu.h"
#include "risc_interrupt.h"

//
// macro resets register with clock counter
//
#define ZERO_COUNT_REG mips_write_c0_register (9,0)

//
// macro defines time as clock counter
//
#define	clock()		mips_read_c0_register(9)// get processor cycle count, the counter increments every clock

//
// macro defines current time
//
#define GETMYTIME(_t) (*_t=clock())

//
// macro defines time difference
//
#define MYTIMEDIFF(fin,ini) ((fin)-(ini))


unsigned long startmsec, finimsec;

//
// @name: starttime
// @description: Initialize timer
//
void starttime(void)
{
  ZERO_COUNT_REG;
  GETMYTIME(&startmsec);
}

//
// @name: stoptime
// @description: Breaks timer
//
void stoptime(void)
{
  GETMYTIME(&finimsec);
}

//
// @name: gettime
// @description: Receives time between initialization and breaking timer
// @return: unsigned long - time value between init and stop
//
unsigned long  gettime(void)
{
   return finimsec - startmsec;
}

//
// @name: __clock
// @description: Breaks timer and receives time after initialization
// 			 Function may be called some times without repeat of starttime()
// @return: unsigned long - time value after init
//
unsigned long __clock()
{
	stoptime();
	return gettime();
}


unsigned int _risc_cp0_clk_counter = 0;

void risc_cp0_tics_start()
{
  asm volatile ("mfc0 %0, $9" :"=r"(_risc_cp0_clk_counter));
}

unsigned int risc_cp0_tics_get()
{
  unsigned int Count= 0;
  asm volatile ("mfc0 %0, $9" :"=r"(Count));
  unsigned int clk = 0;
  if (Count >= _risc_cp0_clk_counter)
  {
    clk = Count - _risc_cp0_clk_counter;
  }
  else clk = 0xffffffff - _risc_cp0_clk_counter + Count;
  return clk;
}

float risc_cp0_ms_get()
{
  unsigned int clock = get_cpu_clock();
  unsigned int tics = risc_cp0_tics_get();
  return ((float)tics*1000.0f)/((float)clock);
}
