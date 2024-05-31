// ***************** debug_printf.c ************************************
//                 Risc-Dsp runtime library
//
// This file implements printf function with output to selected port
//
// (c) multicore.ru
//
// *********************************************************************
#include "debug_printf.h"
#include <stdio.h>
#include <stdarg.h>

#include "uart.h"

#define VT0_OUT *(volatile int *)(0xb8400000)

static unsigned long out_mode = 0;

//
// Function: set_out_mode
//  set output mode
//
// Parameters: mode - OUT_DISABLE,OUT_FROM_UART or OUT_FROM_VT
void set_out_mode(unsigned mode) { out_mode = mode; }

//
// Function: vt_puts
//  out from vitrual terminal
//
// Parameters: const char* str - string
//
int vt_puts(const char *str) {
  do {
    VT0_OUT = *str;
  } while (*str++);
  return 0;
}

//
// Function: vt_puts
//  put string to uart port
//
// Parameters:
//      const char* str - string
//
int uart_puts(const char *str) {
  UART0_sendStr(str);
  return 0;
}

//
// Function: debug_puts
//  put string selected port
//
// Parameters: const char* str - string
//
int debug_puts(const char *str) {
  int ret = 0;
  if (out_mode == OUT_FROM_UART) {
    ret = uart_puts(str);
  } else if (out_mode == OUT_FROM_VT) {
    ret = vt_puts(str);
  } else {
    // none
    ret = 0;
  }
  return ret;
}

//
// Function: debug_printf
//  print string to selected port, analog of c-library printf function
//
// Parameters: const char* format - string to be printed
//
int debug_printf(const char *format, ...) {
  char buffer[MAX_UART_BUFFER_LENGTH];

  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  return debug_puts(buffer);
}
