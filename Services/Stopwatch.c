/**
 ******************************************************************************
 * @file    
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    16.12.2014
 * @brief   Implements a stopwatch
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#include "Stopwatch.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

/****************************************************************
 * Function declarations
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

static uint32_t Stopwatch_currentValue;

/****************************************************************
 * User functions
 ***************************************************************/

void Stopwatch_Init(void)
{
  Stopwatch_currentValue = 0u;
}

uint32_t Stopwatch_GetCurrentValue(void)
{
  return Stopwatch_currentValue;
}

uint32_t Stopwatch_ReturnDifference(const uint32_t start, const uint32_t stop)
{
  return (uint32_t) (stop - start);
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void Stopwatch_MsTimer(void)
{
  Stopwatch_currentValue++;
}

/****************************************************************
 * Static functions
 ***************************************************************/
