/**
 ******************************************************************************
 * @file    TimerIf_Irq.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    15.01.2015
 * @brief   Interrupts for TimerIf module.
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "TimerIf.h"
#include "TimerIf_Irq.h"

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
 * Timer 2 ISR.
 */
void TIM2_IRQHandler(void);

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * User functions
 ***************************************************************/

void TIM2_IRQHandler(void)
{
  TimerIf_Isr(0u);
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/
