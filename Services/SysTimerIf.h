/**
 ******************************************************************************
 * @file    SysTimerIf.h
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    14.12.2014
 * @brief   TimerIf Header 
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef SYSTIMERIF_H
#define SYSTIMERIF_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * Represents the Callback type.
 */
typedef void (*SysTimerIf_CallbackType)(void);

/**
 * Defines one channel.
 */
typedef struct
{
  SysTimerIf_CallbackType callback;
  uint32_t timeout;
  uint32_t reload;
  BOOL isActivated;
  BOOL expired;
} SysTimerIf_ChannelConfigType;

/**
 * A TimerIf module config type.
 */
typedef struct
{
  const uint8_t numberOfChannels;
  SysTimerIf_ChannelConfigType* channelConfigs;
  const uint32_t timeResolution;
} SysTimerIf_ConfigType;

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Compile time configuration.
 */
extern SysTimerIf_ConfigType SysTimerIf_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Configures the Timer .
 * @param configuration for initializing the Systick timer 
 */
void SysTimerIf_Init(SysTimerIf_ConfigType* config);

/**
 * Activates the callback function.
 */
void SysTimerIf_MainFunction(void);

/**
 * Set a timer value for the given channel
 * @param the used channel
 * @param the timeout value
 */
void SysTimerIf_Start(const uint8_t channel, const uint32_t timeout);

/**
 * Stops the timer for the given channel
 * @param
 */
void SysTimerIf_Stop(const uint8_t channel);

/**
 * The Timer ISR Function
 */
void SysTimerIf_Isr(void);

/**
 * Sets the callback function for TimerIf_userCfg->channelConfigs[channel].callback
 * @param the used channel
 * @param the linked callback function
 */
void SysTimerIf_SetCallback(const uint8_t channel, SysTimerIf_CallbackType callback);

#endif

