/****************************************************************
 *                  SIMPLE_INTERRUPT_ITIMER                     *
 * Демонстрирует обработку прерываний от интервального таймера. *
 * В конце программы значение interrupt_counter                 *
 * должно быть равным 1                                         *
 * ExitStatus = 0;                                              *
 ***************************************************************/
#include "risc_runtime/cpu.h"
#include "risc_runtime/risc_interrupt.h"
#include "risc_runtime/risc_timer.h"
#include "risc_runtime/erlcommon.h"
#include "risc_runtime/uart.h"
#include "risc_runtime/debug_printf.h"
#include "risc_runtime/timer_fifo.h"

#include "risc_runtime/ost_node.h"
#include "risc_runtime/ost_socket.h"

#define RX_TX_MULTITEST 1
// TEST CONFIG
#define SWIC_SPEED 100
#define TRY 10
#define SIZE 100
#define TEST_LEN 500

int ExitStatus = 3;
#define TRANSMISSION_RETRY 5

void FillArray(unsigned char *array, unsigned int len, int first) {
  unsigned int i;
  for (i = 0; i < len; i++) {
    array[i] = ((i + first) % 256);
  }
}

int VerifyArray(unsigned char *array, unsigned int len, int first) {
  unsigned int i;
  for (i = 0; i < len; i++) { // it is called configuration port in doc // it is called configuration port in doc // it is called configuration port in doc // it is called configuration port in doc
    if (array[i] != ((i + first) % 256))
      return 0;
  }
  return 1;
}

OstNode* ost_node_0;

void spw0_handler(int a) {
	if ( (ITCSR0 & 2) == 2 ){
		TimerFifo* q = &ost_node_0->ports[0]->queue;
		q->interrupt_counter += 1; //инкрементируем счетчик прерываний
		hw_timer_stop(q);
		risc_disable_interrupt(RISC_INT_IT0, 0);
		if(q->interrupt_counter >= TRANSMISSION_RETRY) {

		}
		else {
			uint8_t seq_n = q->data[q->tail].for_packet;
			uint32_t to_set;
			int8_t r = pop_timer(q, seq_n, &to_set);
			if(r == 0 && to_set != 0) {
				activate_timer(q, to_set);
			}
			socket_event_handler(ost_node_0->ports[0], RETRANSMISSION_INTERRUPT, 0, seq_n);
		}
	}
}

int init_node(OstNode* const node, SWIC_SEND swic, uint8_t timer_id) {
	*node = (OstNode) {
		  .self_address = 0,
		  .that_arrived = 0
	};
	start(node, CONNECTIONLESS, timer_id);
	*node->ports[0] = (OstSocket) {
		  .to_address = 1,
		  .self_port = 0,
		  .self_address = timer_id,
		  .spw_layer = swic,
	};
	node->ports[0]->ost = node;
	node->ports[0]->queue.interrupt_counter = 0;

	return 1;
}

void init_interrupts() {
	risc_set_interrupts_vector(INTH_80000180);
	risc_enable_interrupt(RISC_INT_IT0,0);
	enum ERL_ERROR error_status = risc_register_interrupt(spw0_handler, RISC_INT_IT0);
}

void initial_all() {
	UART0_conf();
	set_out_mode(1);

	init_interrupts();
	base_init();
}

int main() {
	unsigned char src[SIZE] __attribute__((aligned(8))) = {
	  0,
	};
	FillArray(src, SIZE, 1);

	unsigned char dst[SIZE] __attribute__((aligned(8))) = {
	  0,
	};
	unsigned int desc[2] __attribute__((aligned(8))) = {
	  0,
	};
	initial_all();


	debug_printf("---------------------- \n");

	OstNode node0;
	init_node(&node0, SWIC0, 0);
	ost_node_0 = &node0;
	OstNode node1;
	init_node(&node1, SWIC1, 1);

	if(open_connection(&node0, 1, CONNECTIONLESS) != 1) {
	   shutdown(&node0);
	   return -1;
	}
	if(open_connection(&node1, 0, CONNECTIONLESS) != 1) {
	   shutdown(&node1);
	   return -1;
	}

	swic_reciver_run(dst, desc);

	if(send_packet(&node0, 1, src, SIZE) != 1) {
	  debug_printf("error in send_packet");
	  shutdown(&node0);
	  return -1;
	}

//	int i;
//	for(i = 0; i < TRANSMISSION_RETRY; ++i) {
//		while (node0.ports[0]->queue.interrupt_counter == i) ;
//		debug_printf("Beps\n");
//	}

	unsigned int rx_num = swic_reciver_wait(desc, 106);
	OstSegmentHeader header;
	memcpy(&header, dst, sizeof(OstSegmentHeader));
	debug_printf("rx_num = %d\n", rx_num);
	print_header(&header);
	VerifyArray(dst + 6,  SIZE, 1);

	if (node0.ports[0]->queue.interrupt_counter < TRANSMISSION_RETRY)
	{
	  ExitStatus = 0;
	  debug_printf("success\n");
	}
	else
	{
	  ExitStatus=1;
	  debug_printf("fail\n");
	}

	return ExitStatus;
}
