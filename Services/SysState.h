/**
 ******************************************************************************
 * @file    SysState.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef SYSSTATE_H
#define SYSSTATE_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

typedef uint32_t SysState_TimeType;

/**
 * Enumeration for available types
 */
typedef enum
{
  SysState_StateUninitialized = 0u,
  SysState_StateInitializing,
  SysState_StateReady,
  SysState_StateOperating,
  SysState_StateEvent,
  SysState_StateError,

  SysState_StateLast
} SysState_StateType;

typedef struct
{
  /**
   * channel number of output led
   */
  const uint8_t ledChannel;
  /**
   * On-Time for state led
   */
  const SysState_TimeType onTime;
  /**
   * Off-Time for state led
   */
  const SysState_TimeType offTime;
  /**
   * Fall back timeout
   */
  const uint32_t fallBackTimeout;
  /**
   * Boolean flag for enabling state output
   */
  const BOOL used;
} SysState_StateConfigType;

typedef struct
{
  /**
   * pointer to current state
   */
  SysState_StateType* currentState;
  /**
   * fall back state
   */
  SysState_StateType* fallBackState;
  /**
   * Current fall back timeout
   */
  uint32_t* fallBackTime;
  /**
   * configuration of all states
   */
  const SysState_StateConfigType* stateConfig;

} SysState_ChannelConfigType;

typedef struct
{
  /**
   * number of channels
   */
  const uint8_t numberOfChannels;
  /**
   * configuration of all channels
   */
  const SysState_ChannelConfigType* channels;
} SysState_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

#define SYSSTATE_FALL_BACK_DISABLE UINT32_MAX

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Compile time configuration.
 */
extern const SysState_ConfigType SysState_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Initializes the SysState module.
 * @param config A pointer to the configuration.
 */
void SysState_Init(const SysState_ConfigType* config);
/**
 * Gets the current state for a channel.
 * @param channel The system status channel.
 * @return The current system state.
 */
SysState_StateType SysState_GetState(const uint8_t channel);
/**
 * Sets the current state for a channel. (Switches the Led output).
 * @param channel The system status channel.
 * @param newState The new state.
 * @return TRUE if the operation succeeded; otherwise FALSE.
 */
BOOL SysState_SetState(const uint8_t channel,
                       const SysState_StateType newState);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void SysState_MsTimer(void);

#endif
