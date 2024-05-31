#ifndef _INTERRUPT_MC30SF_H_
#define _INTERRUPT_MC30SF_H_

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum
{
        RISC_INT_IRQ0 = 1,		// QSTR0	
        RISC_INT_IRQ1          = (0x000 | ( 1)),
        RISC_INT_IRQ2          = (0x000 | ( 2)),
        RISC_INT_IRQ3          = (0x000 | ( 3)),
        RISC_INT_UART0         = (0x000 | ( 4)),
        RISC_INT_UART1         = (0x000 | ( 5)),
        RISC_INT_SPI           = (0x000 | ( 6)),
        RISC_INT_USB           = (0x000 | ( 7)),
        RISC_INT_USB_EP1       = (0x000 | ( 8)),
        RISC_INT_USB_EP2       = (0x000 | ( 9)),
        RISC_INT_USB_EP3       = (0x000 | (10)),
        RISC_INT_USB_EP4       = (0x000 | (11)),
        RISC_INT_EMAC_RX_FRAME = (0x000 | (12)),		
        RISC_INT_EMAC_TX_FRAME = (0x000 | (13)) ,		
        RISC_INT_EMAC_DMA_RX   = (0x000 | (14)),		
        RISC_INT_EMAC_DMA_TX   = (0x000 | (15)),		
        RISC_INT_SPWR_RX_DES   = (0x000 | (16)),
        RISC_INT_SPWR_RX_DAT   = (0x000 | (17)),
        RISC_INT_SPWR_TX_DES   = (0x000 | (18)),
        RISC_INT_SPWR_TX_DAT   = (0x000 | (19)),	
        RISC_INT_WDT           = (0x000 | (20)),		
        RISC_INT_IT0           = (0x000 | (21)),		
        RISC_INT_IT1           = (0x000 | (22)),		
        RISC_INT_SPWR0         = (0x000 | (24)),
        RISC_INT_SPWR1         = (0x000 | (25)),
        RISC_INT_SPWR2         = (0x000 | (26)),
        RISC_INT_SPWR3         = (0x000 | (27)),
        RISC_INT_SPWR4         = (0x000 | (28)),
        RISC_INT_SPWR5         = (0x000 | (29)),
        RISC_INT_SPWR6         = (0x000 | (30)),
        RISC_INT_SPWR7         = (0x000 | (31)),     
	
        RISC_INT_MEMCH0 = (0x100 | ( 0)),    // QSTR1
        RISC_INT_MEMCH1 = (0x100 | ( 1)),
        RISC_INT_MEMCH2 = (0x100 | ( 2)),
        RISC_INT_MEMCH3 = (0x100 | ( 3)),
        RISC_INT_MEMCH4 = (0x100 | ( 4)),
        RISC_INT_MEMCH5 = (0x100 | ( 5)),
        RISC_INT_MEMCH6 = (0x100 | ( 6)),
        RISC_INT_MEMCH7 = (0x100 | ( 7)),
        RISC_INT_MEMCH10 = (0x100 | ( 8)),
        RISC_INT_MEMCH11 = (0x100 | ( 9)),
        RISC_INT_MEMCH12 = (0x100 | (10)),
        RISC_INT_MEMCH13 = (0x100 | (11)), 
        RISC_INT_MEMCH14 = (0x100 | (12)),
        RISC_INT_MEMCH15 = (0x100 | (13)),
        RISC_INT_MEMCH16 = (0x100 | (14)),
        RISC_INT_MEMCH17 = (0x100 | (15)),
	RISC_INT_MEMCH_MAX = RISC_INT_MEMCH17,
        RISC_INT_MEMCH,

        RISC_INT_SRQ0 = 0x200,		     // QSTR2
        RISC_INT_MFBSP_RXBUF0,		
        RISC_INT_MFBSP_TXBUF0,		
        RISC_INT_DMA_MFBSP0,		
        RISC_INT_SRQ1,		
        RISC_INT_MFBSP_RXBUF1,		
        RISC_INT_MFBSP_TXBUF1,		
        RISC_INT_DMA_MFBSP1,		
        RISC_INT_SRQ2,		
        RISC_INT_MFBSP_RXBUF2,		
        RISC_INT_MFBSP_TXBUF2,		
        RISC_INT_DMA_MFBSP2,		
        RISC_INT_SRQ3,		
        RISC_INT_MFBSP_RXBUF3,		
        RISC_INT_MFBSP_TXBUF3,		
        RISC_INT_DMA_MFBSP3,		
        RISC_INT_SPFMIC0_0,
        RISC_INT_SPFMIC0_1,
        RISC_INT_SPFMIC0_2,
        RISC_INT_SPFMIC_RX_DES_CH0,		
        RISC_INT_SPFMIC_RX_DAT_CH0,		
        RISC_INT_SPFMIC_TX_DES_CH0,		
        RISC_INT_SPFMIC_TX_DAT_CH0,
        RISC_INT_SPFMIC1_0,
        RISC_INT_SPFMIC1_1,
        RISC_INT_SPFMIC1_2,
        RISC_INT_SPFMIC_RX_DES_CH1,		
        RISC_INT_SPFMIC_RX_DAT_CH1,		
        RISC_INT_SPFMIC_TX_DES_CH1,		
        RISC_INT_SPFMIC_TX_DAT_CH1,  		

        RISC_INT_HMCRAM0 = 0x300,	   //QSTR3	
        RISC_INT_HMCRAM1, 		
        RISC_INT_HMCRAM2, 		
        RISC_INT_HMCRAM3, 					
        RISC_INT_HMICACHE,				
        RISC_INT_HMDCACHE, 				
        RISC_INT_HMMPORT = 0x307,  
        RISC_INT_HMDSP0,
        RISC_INT_HMDSP1,
        RISC_INT_HMDDR0 = 0x30c,
        RISC_INT_HMDDR1,
        RISC_INT_HMACC = 0x31f,		

        RISC_FROM_DSP = 0x400,    
        RISC_COMPARE = 0x500,
		
        RICS_EXC_MOD = 0x801,
        RICS_EXC_TLBL,
        RICS_EXC_TLBS,
        RICS_EXC_ADEL,
        RICS_EXC_ADES,
        RICS_EXC_SYS,
        RICS_EXC_BP,
        RICS_EXC_RI,
        RICS_EXC_CPU,
        RICS_EXC_OV,
        RICS_EXC_TRAP,
        RICS_EXC_FPE,
        RICS_EXC_MCHECK
} RISC_INTERRUPT_TYPE;

#ifdef	__cplusplus
}
#endif

#endif 
