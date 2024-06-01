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

#define PACKET_BUFFER_SIZE 1024*16
#define SIZE 298

int ExitStatus = 3;
#define TRANSMISSION_RETRY 5
#define PACKETS_NUMBER 2

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
	int i = 0;
	unsigned int offset = 0;
	for(i = 0;  i < PACKETS_NUMBER; ++i){
		FillArray(src + offset, SIZE, i);
		if(SIZE % 8) offset += (8 - SIZE % 8);
		offset += SIZE;
	}

	unsigned char dst[PACKET_BUFFER_SIZE] __attribute__((aligned(8))) = {
	  0,
	};
	unsigned int rx_desc[256] __attribute__((aligned(8))) = {
	  0,
	}; // descriptors for 32 packets
	unsigned short rx_desc_id = 0;
	unsigned int rx_offset = 0;

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

	swic_reciver_run(dst, rx_desc);

	rtc_timer_start();
	offset = 0;
	for(i = 0; i < PACKETS_NUMBER; ++i) {
		if(send_packet(&node0, 1, src + offset, SIZE) != 1) {
			debug_printf("error in send_packet");
			shutdown(&node0);
			shutdown(&node1);
			return -1;
		}
		if(SIZE % 8) offset += (8 - SIZE % 8);
		offset += SIZE;
	}

	i = 0;
	while(	node0.ports[0]->tx_window_bottom != PACKETS_NUMBER &&
			node0.ports[0]->queue.interrupt_counter < TRANSMISSION_RETRY) {
		if(node0.ports[0]->queue.interrupt_counter != i) {
			debug_printf("Beps\n");
			i = node0.ports[0]->queue.interrupt_counter;
		}

		swic_reciver_wait(rx_desc, rx_desc_id);
		unsigned char rx_num = rx_desc[rx_desc_id * 2 + 1];

		OstSegment arrived;
		memcpy(&arrived.header, dst + rx_offset, sizeof(OstSegmentHeader));
		if(arrived.header.payload_length)
		{
			uint8_t* payload = (uint8_t*) malloc(arrived.header.payload_length);
			memcpy(payload, dst + rx_offset + sizeof(OstSegmentHeader), arrived.header.payload_length);
			arrived.payload = payload;
		}
		unsigned int arrived_sz = rx_desc[rx_desc_id * 2 + 0] & 0x3FFFFFF;
		rx_desc_id += 1;
		if(arrived_sz % 8 > 0) rx_offset += (8 - arrived_sz % 8);
		rx_offset += arrived_sz;


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
	rtc_timer_stop();

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
