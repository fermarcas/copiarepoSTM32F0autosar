/**
 ******************************************************************************
 * @file    
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    
 * @brief    
 *
 *
 * (c) Research Group Embedded Systems, FH O� Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH O�
 ******************************************************************************
 */

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Initialize the stopwatch module.
 */
void Stopwatch_Init(void);

/**
 * Gets the current stopwatch value.
 * @return The current stopwatch value.
 */
uint32_t Stopwatch_GetCurrentValue(void);

/**
 * Calculates the difference of timing values.
 * @param start The start value.
 * @param stop The stop value.
 * @return The difference.
 */
uint32_t Stopwatch_ReturnDifference(const uint32_t start, const uint32_t stop);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/**
 * Stopwatch ms timer callback.
 */
void Stopwatch_MsTimer(void);

#endif
