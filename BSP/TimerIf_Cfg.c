/**
 ******************************************************************************
 * @file    TimerIf_Cfg.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    15.01.2015
 * @brief   Configurations for TimerIf module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "TimerIf.h"
#include "TimerIf_Cfg.h"

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
 * CounterIf Timer ISR for channel 0. Must be included with extern in the
 * GpioIf configuration.
 */
extern void CounterIf_TimerIsr0(void);

/****************************************************************
 * Global variables
 ***************************************************************/

//TimerIf_ChannelConfigType TimerIf_channelConfigs[TIMERIF_NUMBER_OF_CHANNELS] =
//{
//  {
//    TIMERIF_REGISTERS0,
//    TIMERIF_CLOCK_DOMAIN0,
//    TIMERIF_CLOCK_DOMAIN_FUNC0,
//    TIMERIF_NVIC_IRQ0,
//    TIMERIF_ISR_CALLBACK0,
//    TIMERIF_ENABLED0,
//    TIMERIF_FREQUENCY0,
//    TIMERIF_PERIOD0,
//    TIMERIF_CLOCK_DIVISION0,
//    TIMERIF_COUNTER_MODE
//  }
//};

/**
 * Compile time configuration.
 */
TimerIf_ConfigType TimerIf_config =
{
  TIMERIF_NUMBER_OF_CHANNELS,
  NULL_PTR//TimerIf_channelConfigs
};

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

