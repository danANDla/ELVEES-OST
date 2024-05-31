#include "uart.h"
#include "mc30sf6.h"
#include <ctype.h>
#include <stdlib.h>
void UART0_conf(void) {
  int dlm;
  int Frq = 0;
  int BaudRate = 0;

  Frq = get_cpu_clock();
  // Frq = 100000000;
  BaudRate = 115200;
  dlm = (Frq) / (BaudRate * 16);
  CLK_EN = 0xFFFFFFFF;
  /*
  UART0.LCR.bits.DLAB =1; //1 – разрешен доступ к регистрам DLL, DLM
  UART0.LCR.bits.SBC = 0;
  UART0.LCR.bits.STP = 0;
  UART0.LCR.bits.EPS = 0;
  UART0.LCR.bits.PEN = 0;
  UART0.LCR.bits.STB = 0;
  UART0.LCR.bits.WLS = 0;
  */

  UART0_LCR = 0x3 | 0x80; // 8 bit and enable DLL Write
  UART0_DLL = dlm & 0xFF; // Регистр делителя младший

  UART0_DLM = (dlm >> 8); // Регистр делителя старший
  UART0_LCR = 0x3; // 8 bit

  UART0_SCLR = 0;
  UART0_SPR = 0;
  UART0_IER = 0x0;

  UART0_MCR = 0;

  UART0_FCR = 0x2 | 0x4 | 0x1; // Claer and enable FIFO
  // UART0.FCR.data  = 0;

  (void)UART0_LSR;
  (void)UART0_RBR;
  (void)UART0_IIR;

} // UART0_conf

void UART0_sendByte(char Sym) {
// пишем символ в порт

again:
  UART0_THR = Sym;

  // ждем, пока он будет отправлен
  while ((UART0_LSR & 0x60) != 0x60)
    ;

  if (Sym == '\n') {
    Sym = '\r';
    goto again;
  }

} // UART_sendByte

void UART0_sendStr(const char *Str) {
  while (*Str != 0)
    UART0_sendByte(*Str++);
}

unsigned short UART0_getchar(void) {
  unsigned char c;

  /* Wait until receive data available. */
  while ((UART0_LSR & 0x1) == 0)
    ;
  c = UART0_RBR;
  return c;
}

unsigned int UART0_getnum(void) {
  unsigned int val = 0;
  val = UART0_getchar() - '0';
  return val;
}

unsigned int UART0_receiveInt(char last_symb) {
  char str[16];
  unsigned char Readed_value;
  unsigned int i = 0;
  //Пока введенное значение не равно заданному символу
  while (Readed_value != last_symb) {
    //Если в принимаемом регистре что-то появилось
    if ((UART0_LSR & 0x01) == 0x01) {
      //Читаем значение
      Readed_value = UART0_RBR;
      //Если это число
      if (isdigit(Readed_value) != 0) {
        //Записываем
        str[i] = Readed_value;
                //Посылаем обратно
                UART0_sendByte(Readed_value);
                i++;
            }//Иначе игнорируем
        }
    }
    return (unsigned int)atoi(str);
}
