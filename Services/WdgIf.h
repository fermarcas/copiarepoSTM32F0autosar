/**
 ******************************************************************************
 * @file    WdgIf.h
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    11.03.2015
 * @brief   Abstracts the Watchdogs Functions
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef WDGIF_H
#define WDGIF_H

#include "Services/StdDef.h"
#include "stm32f0xx_iwdg.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * Watchdog Interface module config type.
 * @param timerValue : timeout Value for the IWDG in milliseconds 
 * max timeoutvalue of the IWDG is 26.2 seconds =>  timerValue should
 * not be greater than 26200 
 */
typedef struct
{
  const uint32_t timerValue;  
}
WdgIf_ConfigType;


/**
 * declares a WdgIf_Config, will be setup in WdgIf_Cfg module 
 */
extern const WdgIf_ConfigType WdgIf_config;

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
 * Configures the Watchdog .
 * @param config : For initializing the Watchdog 
 */
void WdgIf_Init(const WdgIf_ConfigType* config);

/**
 * Sets the Watchdog to the max value.
 * Must be called periodic in the main loop to prevent a reset
 */
void WdgIf_Reload(void);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

#endif
