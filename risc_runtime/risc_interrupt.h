// ******************** risc_interrupt.h ***************************************
//                 Risc-Dsp runtime library
//
// This file implements the interface to risk interrupt
// by setting MASKR registers and set handler vector
//
// (c) multicore.ru
//
// *******************************************************************

#ifndef _RISC_INTERRUPT_H_
#define _RISC_INTERRUPT_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "erlcommon.h"

#if defined(TARGET_NVCOM02T)
	#include "interrupt_nvcom02t.h"
#elif defined(TARGET_MCT03P)
	#include "interrupt_mct03p.h"
#elif defined(TARGET_MC30SF6)
	#include "interrupt_mc30sf6.h"
#elif defined(TARGET_MCT04)
	#include "interrupt_mct04.h"
#elif defined(TARGET_MCT05)
	#include "interrupt_mct05.h"
#elif defined(TARGET_MC0428)
	#include "interrupt_mc0428.h"
#elif defined(TARGET_MC24R)
	#include "interrupt_mc24r.h"
#elif defined(TARGET_MCT06)
	#include "interrupt_mct06.h"
#elif defined(TARGET_MCT07)
	#include "interrupt_mct07.h"
#elif defined(TARGET_MCT08)
	#include "interrupt_mct08.h"
#endif

typedef void( *RISC_INTERRUPT )(int a);

#define RISC_INTERRUPT_UNKNOWN ((RISC_INTERRUPT)(-1))

//Enumination: INTERRUPT_VECTORS
//
//List of location of interrupt vectors
typedef enum
{
    // Constant: INTH_80000180
    // BEV=0, IV=0, adress 0x80000180
    INTH_80000180,
    // Constant: INTH_BFC00380
    // BEV=1, IV=0, adress 0xBFC00380
    INTH_BFC00380,
    // Constant: INTH_80000200
    // BEV=0, IV=1, adress 0x80000200
    INTH_80000200,
    // Constant: INTH_BFC00400
    // BEV=1, IV=1, adress 0xBFC00400
    INTH_BFC00400,
	// Constant: INTH_B8000180
    // BEV=0, IV=0, SYS_CSR.TR_CRAM=1, adress 0xB8000180
    INTH_B8000180,
	// Constant: INTH_B8000200
    // BEV=0, IV=1, SYS_CSR.TR_CRAM=1, adress 0xB8000200
    INTH_B8000200,
} INTERRUPT_VECTORS;


// Section: Functions
//
// Function: risc_set_interrupts_vector
//
// Set adress of interrupts vector
//
// Parameters:
//      loc - location of interrupt vectors (from enum INTERRUPT_VECTORS)
//
// Examples:
//  > set_interrupts_vector(INTH_BFC00400);
//
void risc_set_interrupts_vector(INTERRUPT_VECTORS loc);

// Function: risc_enable_interrupt
//
// Enable risc interrupt by setting mask MASKR
//
// Parameters:
//      type - type of interrupt (from enum RISC_INTERRUPT_TYPE)
//
// Examples:
//  > risc_enable_interrupt(RISC_INT_IT);
//
// See also:
//
//      risc_disable_interrupt
//
void risc_enable_interrupt (RISC_INTERRUPT_TYPE type, int cpu_id);

// Function: risc_disable_interrupt
//
// Disable risc interrupt by setting mask MASKR
//
// Parameters:
//      type - type of interrupt (from enum RISC_INTERRUPT_TYPE)
//
// Examples:
//  > risc_disable_interrupt(RISC_INT_IT);
//
// See also:
//
//      risc_enable_interrupt
//      risc_disable_interrupts
//
void risc_disable_interrupt (RISC_INTERRUPT_TYPE type, int cpu_id);

// Function: risc_disable_interrupts
//
// Disable all risc interrupts by setting CP0 Status register
//
// Examples:
//  > risc_disable_interrupts();
//
// See also:
//
//      risc_enable_interrupt
//      risc_disable_interrupt
//
void risc_disable_interrupts();

// Function: risc_enable_interrupts
//
// Enable all risc interrupts by setting CP0 Status register
//
// Examples:
//  > risc_enable_interrupts();
//
// See also:
//
//      risc_enable_interrupt
//      risc_disable_interrupts
//
void risc_enable_interrupts();

// Function: risc_register_interrupt
//
// Register risc interrupt handler function in handlers vector
//
// Parameters:
//      fn - your handler function
//      type - type of interrupt, for which you set handler
//
// Returns:
//      ERL_ERROR - status of error (from enum ERL_ERROR)
//
// Examples:
//  > risc_register_interrupt(risc_tmr_hnlr, RISC_INT_IT);
//
// See also:
//
//      risc_enable_interrupt
//
enum ERL_ERROR risc_register_interrupt(RISC_INTERRUPT fn, RISC_INTERRUPT_TYPE type);

#ifdef	__cplusplus
}
#endif

#endif // _RISC_INTERRUPT_H_
