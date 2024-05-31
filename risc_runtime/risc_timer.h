// ******************** risc_timer.c ***************************************
//                 Risc-Dsp runtime library
//
// Interface to IT and WDT timers using ITSCALE, ITPERIOD and
// ITCSR registers
//
// (c) multicore.ru
//
// *******************************************************************
#ifndef _RISC_TIMER_H_
#define _RISC_TIMER_H_

#define RISC_CLK_PER_MSEC 240000

#ifdef	__cplusplus
extern "C" {
#endif

// Uncomment term below if you don't want dma time counting
//#define DMA_NO_TIME_COUNTING
//by irs
typedef union
{
  unsigned int _val;
  struct
  {
    unsigned key        :8;
    unsigned en         :1;
    unsigned field_int  :1;
    unsigned mode       :1;
    unsigned rld        :1;
    unsigned int_ctr    :2;
    unsigned            :18;
  };
} WDT_CSR;
//end by irs

struct risc_sys_timer
{
  float it_ms_time;
  float dsp_ms_time;
  float dma_ms_time;
};

// Section: Functions

// Function: risc_it_setup
//
// Setup IT timer
//
// Parameters:
//      period - Period of timer. Frequency = CLK/period.
//      clk_src - (0,1,2) - 0 for CPU CLK, 1 for XTI, 2 for RTCXTI
//
// Examples:
//  > risc_it_setup(10000, 0);
//
void risc_it_setup(unsigned int period, int clk_src);

// Function: risc_it_start
//
// Starts IT timer
//
void risc_it_start();

// Function: risc_it_stop
//
// Stops IT timer
//
void risc_it_stop();

// Function: risc_tics_start
//
// Setup and starts ITtimer for clock counting in tics
//
void risc_tics_start();

// Function: risc_tics_get
//
// Get clock since "risc_clock_start" in tics. To get miliseconds -
// divide return value by "RISC_CLK_PER_MSEC"
//
// Return:
//      unsigned int   - clocks
//
unsigned int risc_tics_get();

// Function: risc_clk_ms_start
//
// Setup and starts ITtimer for clock counting in milliseconds. Uses timer interrupt.
// You must use interrupt handler from library ("risc_interrupt_handler.s") or use our own.
// If you use your own handler, you must extern "_risc_clk_counter" and increment it by 1.
//
void risc_clk_ms_start();

// Function: risc_clk_ms_get
//
// Get clock since "risc_clock_start" in milliseconds. Uses timer interrupt.
//
// Return:
//      float   - time in ms
//
float risc_clk_ms_get();

// Function: sys_time_init
//
// Initialize timers
//
void sys_time_init();

// Function: sys_time_get
//
// Fills struct risc_sys_timer to get dma and IT time in milliseconds.
//
void sys_time_get(struct risc_sys_timer* tmr);
//by irs
// Function: risc_wdt_setup
//
// Setup WDT timer
//
// Parameters:
//      period - Period of timer. Frequency = CLK/period.
//      clk_src - (0,1,2) - 0 for CPU CLK, 1 for XTI, 2 for RTCXTI
//
// Examples:
//  > risc_wdt_setup(10000, 0);
void risc_wdt_setup(unsigned int period, unsigned int scale);

// Function: risc_wdt_start_wdtmode
//
// Starts WDT timer in WDM mode.
//
void risc_wdt_start_wdtmode();

// Function: risc_wdt_stop
//
// Stops WDT timer
//
void risc_wdt_stop();
//end by irs
#ifdef	__cplusplus
}
#endif

#endif // _RISC_TIMER_H_
