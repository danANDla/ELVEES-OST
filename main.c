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
#define PACKET_BUFFER_SIZE 200
#define SIZE 100
#define TEST_LEN 500

int ExitStatus = 3;
#define TRANSMISSION_RETRY 5
#define PACKETS_NUMBER 1

void FillArray(unsigned char *array, unsigned int len, int first) {
	unsigned int i;
	for (i = 0; i < len; i++) {
		array[i] = ((i + first) % 256);
	}
}

int VerifyArray(unsigned char *array, unsigned int offset, unsigned int len, int first) {
	unsigned int i;
	for (i = 0; i < len; i++) { // it is called configuration port in doc // it is called configuration port in doc // it is called configuration port in doc // it is called configuration port in doc
		if (array[i] != ((i + first + offset) % 256))
		  return 0;
	}
	return 1;
}

OstNode* ost_node_0;
OstNode* ost_node_1;

void spw0_handler(int a) {
	timeout_handler(0);
	return;
}

void spw1_handler(int a) {
	timeout_handler(1);
	return;
}

void timeout_handler(uint8_t fifo_id) {
	volatile int* itcsr;
	RISC_INTERRUPT_TYPE interrupt;
	if(fifo_id == 0) {
		itcsr = (volatile int *) 0xb82F5020;
		interrupt = RISC_INT_IT0;
	}
	else {
		itcsr = (volatile int *) 0xb82F5000;
		interrupt = RISC_INT_IT1;
	}
	if ( (*itcsr & 2) == 2 ){
		TimerFifo* q = &ost_node_0->ports[0]->queue;
		q->interrupt_counter += 1; //инкрементируем счетчик прерываний
		hw_timer_stop(q);
		risc_disable_interrupt(interrupt, 0);
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
		  .self_address = timer_id,
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
	risc_enable_interrupt(RISC_INT_IT0, 0);
	risc_enable_interrupt(RISC_INT_IT1, 0);
	risc_register_interrupt(spw0_handler, RISC_INT_IT0);
	risc_register_interrupt(spw1_handler, RISC_INT_IT1);
}

void initial_all() {
	UART0_conf();
	set_out_mode(1);
	init_interrupts();
	base_init();
}

int main() {
	unsigned char src[PACKET_BUFFER_SIZE] __attribute__((aligned(8))) = {
	  0,
	};
	FillArray(src, SIZE, 1);

	unsigned char dst[PACKET_BUFFER_SIZE] __attribute__((aligned(8))) = {
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
	ost_node_1 = &node1;

	if(open_connection(&node0, 1, CONNECTIONLESS) != 1) {
	   shutdown(&node0);
	   return -1;
	}
	if(open_connection(&node1, 0, CONNECTIONLESS) != 1) {
	   shutdown(&node1);
	   return -1;
	}

	swic_reciver_run(dst, desc);
	int i = 0;
	for(i = 0; i < PACKETS_NUMBER; ++i) {
		FillArray(src, SIZE, i);
		if(send_packet(&node0, 1, src, SIZE) != 1) {
			debug_printf("error in send_packet");
			shutdown(&node0);
			shutdown(&node1);
			return -1;
		}
	}

	i = 0;
	while(node0.ports[0]->tx_window_bottom != PACKETS_NUMBER &&
			node0.ports[0]->queue.interrupt_counter < TRANSMISSION_RETRY) {
		if(node0.ports[0]->queue.interrupt_counter != i) {
			debug_printf("Beps\n");
			i = node0.ports[0]->queue.interrupt_counter + 1;
		}

		unsigned int rx_num = swic_reciver_wait(desc);
		OstSegment arrived;
		memcpy(&arrived.header, dst, sizeof(OstSegmentHeader));
		if(arrived.header.payload_length)
		{
			uint8_t* payload = (uint8_t*) malloc(arrived.header.payload_length);
			memcpy(payload, dst + sizeof(OstSegmentHeader), arrived.header.payload_length);
			arrived.payload = payload;
		}
		unsigned int arrived_sz = desc[0] & 0x3FFFFFF;

		swic_reciver_run(dst, desc);
		if(rx_num == 1) {
			debug_printf("SWIC[0] received packet, of size = %u\n", arrived_sz);
			print_header(&arrived.header);
			node0.that_arrived = &arrived;
			event_handler(&node0, PACKET_ARRIVED_FROM_NETWORK);
		} else if (rx_num == 2) {
			debug_printf("SWIC[1] received packet, of size = %u\n", arrived_sz);
			print_header(&arrived.header);
			node1.that_arrived = &arrived;
			event_handler(&node1, PACKET_ARRIVED_FROM_NETWORK);
		}
	}

	if (node1.ports[0]->verified_received == PACKETS_NUMBER)
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
