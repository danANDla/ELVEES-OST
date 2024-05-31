#ifndef _UART_H_
#define _UART_H_

#include "system.h"

#ifdef	__cplusplus
extern "C" {
#endif
void UART0_conf(void);

void UART0_sendByte(char Sym);
void UART0_sendStr(const char *Str);
unsigned short UART0_getchar (void);
unsigned int UART0_getnum(void);
unsigned int UART0_receiveInt(char last_symb);



#ifdef	__cplusplus
}
#endif

#endif // _SWIC_H_
