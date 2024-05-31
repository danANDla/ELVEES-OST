// ***************** risc_cp0_timer.h ***********************************
//                 Risc-Dsp runtime library
//
// This file implements interface to system time access
//
// (c) multicore.ru
//
// *******************************************************************
#ifndef _SYSTEM_TIME_H_
#define _SYSTEM_TIME_H_

#ifdef	__cplusplus
extern "C" {
#endif

//
// initialize timer
//
void starttime(void);

//
// breaks timer
//
void stoptime(void);

//
// receives time between initialization and breaking timer
//
unsigned long  gettime(void);

//
// breaks timer and receives time after initialization
//
unsigned long __clock();






// Function: risc_cp0_tics_start
//
// Setup and starts CP0 timer for clock counting in tics
//
void risc_cp0_tics_start();

// Function: risc_cp0_tics_get
//
// Get clock since "risc_clock_start" in tics. To get seconds -
// divide return value by cpu clock value
//
// Return:
//      unsigned int   - clocks
//
unsigned int risc_cp0_tics_get();

// Function: risc_cp0_ms_get
//
// Get clock since "risc_clock_start" in milliseconds
//
// Return:
//      float   - milliseconds
//
float risc_cp0_ms_get();

#ifdef	__cplusplus
}
#endif

#endif // _SYSTEM_TIME_H_