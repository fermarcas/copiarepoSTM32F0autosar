/**
 ******************************************************************************
 * @file    UartIf_Irq.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    17.11.2014
 * @brief   ISRs for UartIf module.
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#include "UartIf.h"
#include "UartIf_Irq.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

/****************************************************************
 * Function declarations
 ***************************************************************/

/**
 * Defines the IRQ handler for USART1.
 */
void USART1_IRQHandler(void);
/**
 * Defines the IRQ handler for USART2.
 */
void USART2_IRQHandler(void);

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void USART1_IRQHandler(void)
{
  UartIf_Isr(1u);
}

void USART2_IRQHandler(void)
{
  UartIf_Isr(2u);
}

/****************************************************************
 * Static functions
 ***************************************************************/
