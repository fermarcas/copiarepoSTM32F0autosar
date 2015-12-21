/**
 ******************************************************************************
 * @file    WdgIf_Cfg.h
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    11.03.2015
 * @brief   Configuration for the WdgIf_Cfg module
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef WDGIF_CFG_H
#define WDGIF_CFG_H

#include "Services/StdDef.h"
#include "stm32f0xx_iwdg.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/* Default Timevalue for the independent watchdog in ms */
#define WDGIF_TIMER_VALUE                                   1000u

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

#endif
