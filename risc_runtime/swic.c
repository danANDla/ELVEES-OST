#include "swic.h"
#include "debug_printf.h"


#define WAITING_IF_BUSY 1

#define CLK_EN *((volatile unsigned int *)0xB82F4004)

void delay (int delay)
{
	int i = 0;
	for(i=0;i<delay;i++)
		asm volatile ("nop");
}

void system_init(void)
{
	unsigned int tmp_RISK_IRQ_MASK = 0;


	tmp_RISK_IRQ_MASK= GIGASPWR_COMM_RISC_IRQ_MASK;

	*((volatile unsigned int *) 0xB82FA0C4) = 0x1D08;
	*((volatile unsigned int *) 0xB82FA114) = 0x128294;
	*((volatile unsigned int *) 0xB82FA0AC) = 0x02C83414;

	*((volatile unsigned int *) 0xB82FA0C8) = 0x1D08;
	*((volatile unsigned int *) 0xB82FA118) = 0x128294;
	*((volatile unsigned int *) 0xB82FA0B0) = 0x02C83414;

	*((volatile unsigned int *) 0xB82FA0CC) = 0x1D08;
	*((volatile unsigned int *) 0xB82FA11C) = 0x128294;
	*((volatile unsigned int *) 0xB82FA0B4) = 0x02C83414;

	*((volatile unsigned int *) 0xB82FA0D0) = 0x1D08;
	*((volatile unsigned int *) 0xB82FA120) = 0x128294;
	*((volatile unsigned int *) 0xB82FA0B8) = 0x02C83414;

	while ((GIGASPWR_COMM_STATE_R & 0x00780000)!=0x00780000);
	GIGASPWR_COMM_STATE_R = 0x00780000;

	*((volatile unsigned int *) 0xB82FA024) = 0xFFF;

	GIGASPWR_COMM_RISC_IRQ_MASK = tmp_RISK_IRQ_MASK;




}

void base_init (void)
{
	unsigned int coef_10 = 0;
	CLK_EN = 0xFFFFFFFF;

	coef_10 = XTI125_FREQ/10;

	write_route_table_as_endpoint_device();
	GIGASPWR_COMM_MODE_R = (1<<25)|(1<<23)|(coef_10 & 0x1FF) ; //WORK_EN | DMA_EN| coef_10

	system_init(); //service regs setup function. Must be called for comm init.

	swic_reset_all();
}


void write_route_table_as_endpoint_device(void)
{
	int i  =0;

	GIGASPWR_COMM_ADG(0) = 0x2; //CONF -> SWIC0 -> SWIC0
	GIGASPWR_COMM_ADG(1) = 0x4; // CONF -> SWIC1 ->SWIC1


	for (i=0;i<ROUTING_TABLE_SIZE;i++)
	/*
	 * ALL RX_PACKET -> 0 PORT
	 */
	ROUTING_TABLE_STRING(i) = 0x080 | 0x1;

}

//TODO CHECK SWIC reset

void swic_reset_port(unsigned int index_port)
{
	GIGASPWR_COMM_SPW_MODE(index_port) =0x1; //LINK DISABLE
	GIGASPWR_COMM_SPW_TX_SPEED(index_port) =0x2; //PLL and LVDS disable . Speed 10Mb/s

	GIGASPWR_COMM_SPW_MODE(index_port) =(1<<3); //BDS_RESET;
	GIGASPWR_COMM_SPW_MODE(index_port) =0x1; //return BDS_RESET back
}


void swic_reset_all(void)
{
	swic_reset_port(0);
	swic_reset_port(1);

	GIGASPWR_COMM_MODE_R|= (1<<24);
	delay(1000);
	GIGASPWR_COMM_MODE_R &= ~(1<<24);
}


void swic_init(unsigned int index_port)
{
	unsigned int coef_10 = 0;

	if (index_port>1)
		return;

	coef_10 = XTI125_FREQ/10;

	GIGASPWR_COMM_SPW_TX_SPEED(index_port)= 0x300|0x2; //PLL Enable | 10 Mb/s
	//
	GIGASPWR_COMM_SPW_MODE(index_port)=((coef_10 & 0x1FF)<<10)|(1<<3)|0x6; //Coef10_set
}


void swic_init_loopback(unsigned int index_port)
{
	unsigned int coef_10 = 0;

	if (index_port>1)
		return;

	coef_10 = XTI125_FREQ/10;

	GIGASPWR_COMM_SPW_TX_SPEED(index_port)= 0x300|0x2; //PLL Enable | 10 Mb/s
	//
	GIGASPWR_COMM_SPW_MODE(index_port)=((coef_10 & 0x1FF)<<10)|(1<<4)|(1<<3)|0x6;

}

int swic_is_connected(unsigned int index_port)
{
	if (index_port>1)
		return -1;

  if((GIGASPWR_COMM_SPW_STATUS(index_port) & 0x70) == 0x50) //BDS_STATE = 0x5 RUN
	  return 1;
  else
	  return 0;
}

int swic_set_tx_speed(unsigned int index_port,unsigned int speed)
{
	if (index_port>1)
		return -1;

	if (speed < 5)
	{
		GIGASPWR_COMM_SPW_TX_SPEED(index_port) = 0x300 |0x1;
		delay(100);
		return (swic_is_connected(index_port));
	}

	if (speed >400)
	{
		GIGASPWR_COMM_SPW_TX_SPEED(index_port) = 0x300 |0x50;
		delay(100);
		return (swic_is_connected(index_port));
	}

	GIGASPWR_COMM_SPW_TX_SPEED(index_port) = 0x300 |((speed/5)&0xFF);
	delay(100);
	return (swic_is_connected(index_port));

}

int swic_get_rx_speed(unsigned int index_port)
{
	if (index_port>1)
		return -1;

	return (GIGASPWR_COMM_SPW_RX_SPEED(index_port)&0x3FF);
}

int swic_send_packege(SWIC_SEND index_port, void *src, unsigned int size)
{
	unsigned int desc_tx[2] __attribute__ ((aligned(8))) = {0,};
	unsigned int wsize =0;

#ifndef WAITING_IF_BUSY
	if (((DMA_GIGASPWR_COMM_TX_DESC_RUN & 1) ==1)||((DMA_GIGASPWR_COMM_TX_DATA_RUN & 1) ==1))
		return -1; //TX_CHANEL_BUSY
#endif
#ifdef WAITING_IF_BUSY
	while((((DMA_GIGASPWR_COMM_TX_DESC_RUN & 1) ==1)||((DMA_GIGASPWR_COMM_TX_DATA_RUN & 1) ==1)));
#endif

	if (((unsigned)src % 8)!=0)
		return -2;  //if src array not aligned

	if ((size % 8)!=0)
		wsize = size/8 + 1;
	else
		wsize = size/8;

	desc_tx[0] = 0xA0000000|(size & 0x3FFFFFF);
	desc_tx[1] = index_port & 0x7F;

	DMA_GIGASPWR_COMM_TX_DESC_IR = sys_kernel_va_to_pa((unsigned)desc_tx);

	DMA_GIGASPWR_COMM_TX_DATA_IR = sys_kernel_va_to_pa((unsigned)src);
	DMA_GIGASPWR_COMM_TX_DATA_CSR = ((wsize-1) << 16);
	DMA_GIGASPWR_COMM_TX_DESC_CSR = 0;

	DMA_GIGASPWR_COMM_TX_DATA_CSR|=  0x1;
	DMA_GIGASPWR_COMM_TX_DESC_CSR|= 1;

	return 1;
}

int swic_reciver_run(void * dst, unsigned int *desc)
{

#ifndef WAITING_IF_BUSY
	if (((DMA_GIGASPWR_COMM_RX_DESC_RUN & 1) ==1)&&((DMA_GIGASPWR_COMM_RX_DATA_RUN & 1) ==1))
		return -1; //RX_CHANEL_BUSY
#endif
#ifdef WAITING_IF_BUSY
	while(((DMA_GIGASPWR_COMM_RX_DESC_RUN & 1) ==1)&&((DMA_GIGASPWR_COMM_RX_DATA_RUN & 1) ==1));
#endif

	desc[0]=0x0;
	desc[1]=0x0;

	if (((unsigned)dst % 8)!=0)
		return -2;  //if dst array not aligned

	if (((unsigned)desc % 8)!=0)
		return -2;  //if descriptor not aligned


	DMA_GIGASPWR_COMM_RX_DESC_IR = sys_kernel_va_to_pa((unsigned)desc);
	DMA_GIGASPWR_COMM_RX_DESC_CSR = (0x0020 << 16) | 0x1; // mem area for 256 bytes (32 descriptors); run dma
	DMA_GIGASPWR_COMM_RX_DATA_IR = sys_kernel_va_to_pa((unsigned)dst);
	DMA_GIGASPWR_COMM_RX_DATA_CSR = (0xFFFE << 16) | 0x1;

	return 1;
}

int swic_reciver_wait(unsigned int *desc, unsigned char desc_to_receive)
{
	if (((unsigned)desc % 8)!=0)
		return -2;  //if descriptor not aligned

//	while((DMA_GIGASPWR_COMM_RX_DESC_RUN & 0x1) == 1) //Wait if one desc recive
//	{
//		if ((DMA_GIGASPWR_COMM_RX_DATA_RUN & 0x1) == 0) //If DATA DMA STOP, restart
//			DMA_GIGASPWR_COMM_RX_DATA_CSR = (0xFFFE << 16)| 0x1;
//	}

	while(32 - (DMA_GIGASPWR_COMM_RX_DESC_CSR >> 16) == desc_to_receive) ;

	//	if ((desc[0] & 0xA0000000) != 0xA0000000)
	//	{
	//		debug_printf("PACKET END IS NOT EOP! RX_DESCR0: %x \n",desc[0]);
	//		return -1;
	//	}

	// if ((desc[0] & 0x3FFFFFF)!=size)
	// {
	// 	debug_printf("SIZE! RX_DESCR0: %x \n",desc[0]);
	// 	return -1;
	// }

	return 1;
}
