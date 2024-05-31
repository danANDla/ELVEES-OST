/*
 * File:   debug_printf.h
 *
 * This file implements printf function with output to selected port
 *
 * (c) multicore.ru
 */

#ifndef _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef _MAX_UART_BUFFER_LENGTH_
#define MAX_UART_BUFFER_LENGTH _MAX_UART_BUFFER_LENGTH_
#else
#define MAX_UART_BUFFER_LENGTH 512
#endif

/*
   Section :  Constants

   OUT_DISABLE   - output disabled.
   OUT_FROM_UART - output is uart.
   OUT_FROM_VT   - output is virtual terminal memory for sim3 printing.
*/
#define OUT_DISABLE   0
#define OUT_FROM_UART 1
#define OUT_FROM_VT   2

// Section: Functions
//
// Function: set_out_mode
//
// Sets output mode
//
// Parameters:
//
//     mode - OUT_DISABLE,OUT_FROM_UART or OUT_FROM_VT
//
// Examples:
//  > set_out_mode(OUT_FROM_VT);
//  > set_out_mode(OUT_FROM_UART);
//  > set_out_mode(OUT_DISABLE;)
//
//
// See also:
//
//      <vt_puts>
//      <uart_puts>
//
void set_out_mode(unsigned mode);

// Function: vt_puts
// ends buffer to virtual terminal port of program model (sim3)
//
// Parameters: const char* str - string
//
int vt_puts(const char* str);

// Function: uart_puts
// Sends buffer to uart port
//
// Parameters: const char* str - string
//
int uart_puts(const char* str);

// Function: debug_printf
//
// Prints string to previously selected terminal, analog of c-library printf
// function
//
// Parameters:
//     const char* format - string to be printed
//
int debug_printf(const char *format, ...);

#ifdef	__cplusplus
}
#endif

#endif // _DEBUG_PRINTF_H_
