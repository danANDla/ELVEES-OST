#ifndef SWIC_H
#define SWIC_H
#include "system.h"


//TYPES
#ifndef uint
   #define uint unsigned int
#endif

typedef enum {
    LOOP = 0x1, // it is called configuration port in doc
	SWIC0 = 0x2,
	SWIC1 = 0x4,
	ALL = 0x6,
} SWIC_SEND;
// quartz frequency
#define MHz             1000000

// uart (?) frequency
#define CLOCK           5*MHz

#define XTI125_FREQ 125 // MHz

#define ROUTING_TABLE_SIZE 256

#define ROUTING_TABLE_STRING(i)  *((volatile unsigned int *)(0xB82FA400 +i*0x4))


#define  GIGASPWR_COMM_ID_VER *((volatile unsigned int *) 0xB82FA000)
#define  GIGASPWR_COMM_ID_SWITCH *((volatile unsigned int *) 0xB82FA004)
#define  GIGASPWR_COMM_ID_NET *((volatile unsigned int *) 0xB82FA008)
#define  GIGASPWR_COMM_MODE_R *((volatile unsigned int *) 0xB82FA00C)
#define  GIGASPWR_COMM_MODE_R1 *((volatile unsigned int *) 0xB82FA010)
#define  GIGASPWR_COMM_STATE_R *((volatile unsigned int *) 0xB82FA014)
#define  GIGASPWR_COMM_RISC_IRQ_MASK *((volatile unsigned int *) 0xB82FA018)
#define  GIGASPWR_COMM_AUTO_COU *((volatile unsigned int *) 0xB82FA01C)
#define  GIGASPWR_COMM_CONTROL_CONNECTION *((volatile unsigned int *) 0xB82FA020)
#define  GIGASPWR_COMM_STATE_CONNECTION *((volatile unsigned int *) 0xB82FA024)
#define  GIGASPWR_COMM_SW_DAT_TOUTS *((volatile unsigned int *) 0xB82FA028)
#define  GIGASPWR_COMM_SW_DAT_TOUTS2 *((volatile unsigned int *) 0xB82FA02C)
#define  GIGASPWR_COMM_SW_DAT_TOUTS3 *((volatile unsigned int *) 0xB82FA030)
#define  GIGASPWR_COMM_SPEC_ARB *((volatile unsigned int *) 0xB82FA034)
#define  GIGASPWR_COMM_CCODE_OUT *((volatile unsigned int *) 0xB82FA038)
#define  GIGASPWR_COMM_CUR_TIME *((volatile unsigned int *) 0xB82FA03C)
#define  GIGASPWR_COMM_ISR_L *((volatile unsigned int *) 0xB82FA040)
#define  GIGASPWR_COMM_ISR_H *((volatile unsigned int *) 0xB82FA044)
#define  GIGASPWR_COMM_INTR_IRQ_MASK_L *((volatile unsigned int *) 0xB82FA048)
#define  GIGASPWR_COMM_INTR_IRQ_MASK_H *((volatile unsigned int *) 0xB82FA04C)
#define  GIGASPWR_COMM_INTA_IRQ_MASK_L *((volatile unsigned int *) 0xB82FA050)
#define  GIGASPWR_COMM_INTA_IRQ_MASK_H *((volatile unsigned int *) 0xB82FA054)
#define  GIGASPWR_COMM_CCODES_MASK1 *((volatile unsigned int *) 0xB82FA058)
#define  GIGASPWR_COMM_CCODES_MASK2 *((volatile unsigned int *) 0xB82FA05C)
#define  GIGASPWR_COMM_DIST_INTS_TOUTS1 *((volatile unsigned int *) 0xB82FA060)
#define  GIGASPWR_COMM_DIST_INTS_TOUTS2 *((volatile unsigned int *) 0xB82FA064)
#define  GIGASPWR_COMM_ACK_NON_ACK_REGIME *((volatile unsigned int *) 0xB82FA068)
#define  GIGASPWR_COMM_CCODES_SPEC_REGIME *((volatile unsigned int *) 0xB82FA06C)
#define  GIGASPWR_COMM_SPEC_ISR_REGIME *((volatile unsigned int *) 0xB82FA070)
#define  GIGASPWR_COMM_INTER_HANDLER_TERM_FUNCT *((volatile unsigned int *) 0xB82FA074)
#define  GIGASPWR_COMM_ISR_SOURCE_TERM_FUNCT *((volatile unsigned int *) 0xB82FA078)
#define  GIGASPWR_COMM_ISR_TOUTS_FLS_L *((volatile unsigned int *) 0xB82FA07C)
#define  GIGASPWR_COMM_ISR_TOUTS_FLS_H *((volatile unsigned int *) 0xB82FA080)
#define  GIGASPWR_COMM_ISR_1101 *((volatile unsigned int *) 0xB82FA084)
#define  GIGASPWR_COMM_EXTERNAL_RESET_PARAMETERS *((volatile unsigned int *) 0xB82FA088)
#define  GIGASPWR_COMM_SPW_STATUS(i) *((volatile unsigned int *)(0xB82FA08C +i*0x4))
#define  GIGASPWR_COMM_SPW_MODE(i)  *((volatile unsigned int *)(0xB82FA0A4 +i*0x4))
#define  GIGASPWR_COMM_SPW_TX_SPEED(i) *((volatile unsigned int *)(0xB82FA0BC +i*0x4))
#define  GIGASPWR_COMM_GIGA_SPW_TX_SPEED(i) *((volatile unsigned int *)(0xB82FA0C4 +i*0x4))
#define  GIGASPWR_COMM_SPW_RX_SPEED(i) *((volatile unsigned int *)(0xB82FA0D4 +i*0x4))
#define  GIGASPWR_COMM_ADG(i) *((volatile unsigned int *)(0xB82FA0EC +i*0x4))
#define  GIGASPWR_COMM_GIGA_PMA_STATUS(i)  *((volatile unsigned int *)(0xB82FA104 +i*0x4))
#define  GIGASPWR_COMM_GIGA_SPW_PMA_MODE(i) *((volatile unsigned int *)(0xB82FA114 +i*0x4))


//DMA_GIGASPWR_COMM
#define DMA_GIGASPWR_COMM_RX_DESC_CSR *((volatile uint *)0xB82FA800) //Kанал записи в память микросхемы дескрипторов принимаемых пакетов
#define DMA_GIGASPWR_COMM_RX_DESC_CP *((volatile uint *)0xB82FA804)
#define DMA_GIGASPWR_COMM_RX_DESC_IR *((volatile uint *)0xB82FA808)
#define DMA_GIGASPWR_COMM_RX_DESC_RUN *((volatile uint *)0xB82FA80C)

#define DMA_GIGASPWR_COMM_RX_DATA_CSR *((volatile uint *)0xB82FA840) //Канал записи в память микросхемы принимаемых слов данных
#define DMA_GIGASPWR_COMM_RX_DATA_CP *((volatile uint *)0xB82FA844)
#define DMA_GIGASPWR_COMM_RX_DATA_IR *((volatile uint *)0xB82FA848)
#define DMA_GIGASPWR_COMM_RX_DATA_RUN *((volatile uint *)0xB82FA84C)

#define DMA_GIGASPWR_COMM_TX_DESC_CSR *((volatile uint *)0xB82FA880) //Канал чтения из памяти микросхемы дескрипторов передаваемых пакетов
#define DMA_GIGASPWR_COMM_TX_DESC_CP *((volatile uint *)0xB82FA884)
#define DMA_GIGASPWR_COMM_TX_DESC_IR *((volatile uint *)0xB82FA888)
#define DMA_GIGASPWR_COMM_TX_DESC_RUN *((volatile uint *)0xB82FA88C)

#define DMA_GIGASPWR_COMM_TX_DATA_CSR *((volatile uint *)0xB82FA8C0) //Канал чтения из памяти микросхемы передаваемых слов данных
#define DMA_GIGASPWR_COMM_TX_DATA_CP *((volatile uint *)0xB82FA8C4)
#define DMA_GIGASPWR_COMM_TX_DATA_IR *((volatile uint *)0xB82FA8C8)
#define DMA_GIGASPWR_COMM_TX_DATA_RUN *((volatile uint *)0xB82FA8CC)

//DMA_SPWMIC0_COMM
#define DMA_SPWMIC0_COMM_RX_DESC_CSR *((volatile uint *)0xB82FC800) //Kанал записи в память микросхемы дескрипторов принимаемых пакетов
#define DMA_SPWMIC0_COMM_RX_DESC_CP *((volatile uint *)0xB82FC804)
#define DMA_SPWMIC0_COMM_RX_DESC_IR *((volatile uint *)0xB82FC808)
#define DMA_SPWMIC0_COMM_RX_DESC_RUN *((volatile uint *)0xB82FC80C)

#define DMA_SPWMIC0_COMM_RX_DATA_CSR *((volatile uint *)0xB82FC840) //Канал записи в память микросхемы принимаемых слов данных
#define DMA_SPWMIC0_COMM_RX_DATA_CP *((volatile uint *)0xB82FC844)
#define DMA_SPWMIC0_COMM_RX_DATA_IR *((volatile uint *)0xB82FC848)
#define DMA_SPWMIC0_COMM_RX_DATA_RUN *((volatile uint *)0xB82FC84C)

#define DMA_SPWMIC0_COMM_TX_DESC_CSR *((volatile uint *)0xB82FC880) //Канал чтения из памяти микросхемы дескрипторов передаваемых пакетов
#define DMA_SPWMIC0_COMM_TX_DESC_CP *((volatile uint *)0xB82FC884)
#define DMA_SPWMIC0_COMM_TX_DESC_IR *((volatile uint *)0xB82FC888)
#define DMA_SPWMIC0_COMM_TX_DESC_RUN *((volatile uint *)0xB82FC88C)

#define DMA_SPWMIC0_COMM_TX_DATA_CSR *((volatile uint *)0xB82FC8C0) //Канал чтения из памяти микросхемы передаваемых слов данных
#define DMA_SPWMIC0_COMM_TX_DATA_CP *((volatile uint *)0xB82FC8C4)
#define DMA_SPWMIC0_COMM_TX_DATA_IR *((volatile uint *)0xB82FC8C8)
#define DMA_SPWMIC0_COMM_TX_DATA_RUN *((volatile uint *)0xB82FC8CC)


void base_init (void);
void write_route_table_as_endpoint_device(void);
void swic_reset_all(void);
void swic_init(unsigned int index_port);
void swic_init_loopback(unsigned int index_port);
int swic_is_connected(unsigned int index_port);
int swic_set_tx_speed(unsigned int index_port,unsigned int speed);
int swic_get_rx_speed(unsigned int index_port);
void delay (int delay);
int swic_send_packege(SWIC_SEND index_port, void *src, unsigned int size);
int swic_reciver_run(void * dst, unsigned int *desc);
int swic_reciver_wait(unsigned int *desc, unsigned char desc_to_receive);

#endif
