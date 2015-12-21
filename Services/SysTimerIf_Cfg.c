/**
 ******************************************************************************
 * @file    SysTimerIf_Cfg.c
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    14.12.2014
 * @brief   Configuration for TimerIf Module
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "SysTimerIf.h"
#include "SysTimerIf_Cfg.h"

/* callbacks */
#include "Stopwatch.h"
#include "BSP/Led.h"
#include "SysState.h"

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

/**
 * Defines the SysTimerIf_ChannelConfigType.
 */
static SysTimerIf_ChannelConfigType SysTimerIf_channelConfigs[SYSTIMERIF_NUMBER_OF_CHANNELS] =
{
  {
    SYSTIMERIF_CALLBACK0,
    SYSTIMERIF_TIMEOUT0,
    SYSTIMERIF_TIMEOUT0,
    SYSTIMERIF_ACTIVATED0,
    FALSE
  },
  {
    SYSTIMERIF_CALLBACK1,
    SYSTIMERIF_TIMEOUT1,
    SYSTIMERIF_TIMEOUT1,
    SYSTIMERIF_ACTIVATED1,
    FALSE
  },
  {
    SYSTIMERIF_CALLBACK2,
    SYSTIMERIF_TIMEOUT2,
    SYSTIMERIF_TIMEOUT2,
    SYSTIMERIF_ACTIVATED2,
    FALSE
  }
};

SysTimerIf_ConfigType SysTimerIf_config =
{
  SYSTIMERIF_NUMBER_OF_CHANNELS,
  SysTimerIf_channelConfigs,
  SYSTIMERIF_RESOLUTION
};

/****************************************************************
 * User functions
 ***************************************************************/

