/**
 ******************************************************************************
 * @file    GpioIf_Irq.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    15.01.2015
 * @brief   ISRs for GpioIf module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "GpioIf.h"
#include "GpioIf_Irq.h"

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
 * Defines the IRQ handler for EXTI0-1.
 */
void EXTI0_1_IRQHandler(void);
/**
 * Defines the IRQ handler for EXTI2-3.
 */
void EXTI2_3_IRQHandler(void);
/**
 * Defines the IRQ handler for EXTI4-15.
 */
void EXTI4_15_IRQHandler(void);

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void EXTI0_1_IRQHandler(void)
{
  /* Local variables */
  uint8_t i;
  uint32_t line;

  /* Initialize local variables */
  line = 0u;

  for (i = 0u; i < 2u; i++)
  {
    line = 1u << i;

    if (EXTI_GetITStatus(line) == SET)
    {
      GpioIf_Isr(i);
      EXTI_ClearITPendingBit(line);
    }
  }
}

void EXTI2_3_IRQHandler(void)
{
  /* Local variables */
  uint8_t i;
  uint32_t line;

  /* Initialize local variables */
  line = 0u;

  for (i = 2u; i < 4u; i++)
  {
    line = 1u << i;

    if (EXTI_GetITStatus(line) == SET)
    {
      GpioIf_Isr(i);
      EXTI_ClearITPendingBit(line);
    }
  }
}

void EXTI4_15_IRQHandler(void)
{
  /* Local variables */
  uint8_t i;
  uint32_t line;

  /* Initialize local variables */
  line = 0u;

  for (i = 4u; i < 16u; i++)
  {
    line = 1u << i;

    if (EXTI_GetITStatus(line) == SET)
    {
      GpioIf_Isr(i);
      EXTI_ClearITPendingBit(line);
    }
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/
