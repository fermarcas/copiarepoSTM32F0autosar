/**
 ******************************************************************************
 * @file    SysState_Cfg.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "SysState.h"
#include "SysState_Cfg.h"

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

static SysState_StateType SysState_currentState0;
static SysState_StateType SysState_fallBackState0;
static uint32_t SysState_currentFallBackTimeout0;
static const SysState_StateConfigType SysState_stateConfigs0[SysState_StateLast] =
{
  {
    /* Uninitialized State */
    SYSSTATE_UNINITIALIZED_LED_CHANNEL0,
    SYSSTATE_UNINITIALIZED_ON_TIME0,
    SYSSTATE_UNINITIALIZED_OFF_TIME0,
    SYSSTATE_UNINITIALIZED_FALL_BACK_TIMEOUT0,
    SYSSTATE_UNINITIALIZED_USED0
  },
  {
    /* Initializing State */
    SYSSTATE_INITIALIZING_LED_CHANNEL0,
    SYSSTATE_INITIALIZING_ON_TIME0,
    SYSSTATE_INITIALIZING_OFF_TIME0,
    SYSSTATE_INITIALIZING_FALL_BACK_TIMEOUT0,
    SYSSTATE_INITIALIZING_USED0
  },
  {
    /* Ready State */
    SYSSTATE_READY_LED_CHANNEL0,
    SYSSTATE_READY_ON_TIME0,
    SYSSTATE_READY_OFF_TIME0,
    SYSSTATE_READY_FALL_BACK_TIMEOUT0,
    SYSSTATE_READY_USED0
  },
  {
    /* Operating State */
    SYSSTATE_OPERATING_LED_CHANNEL0,
    SYSSTATE_OPERATING_ON_TIME0,
    SYSSTATE_OPERATING_OFF_TIME0,
    SYSSTATE_OPERATING_FALL_BACK_TIMEOUT0,
    SYSSTATE_OPERATING_USED0
  },
  {
    /* Event State */
    SYSSTATE_EVENT_LED_CHANNEL0,
    SYSSTATE_EVENT_ON_TIME0,
    SYSSTATE_EVENT_OFF_TIME0,
    SYSSTATE_EVENT_FALL_BACK_TIMEOUT0,
    SYSSTATE_EVENT_USED0
  },
  {
    /* Error State */
    SYSSTATE_ERROR_LED_CHANNEL0,
    SYSSTATE_ERROR_ON_TIME0,
    SYSSTATE_ERROR_OFF_TIME0,
    SYSSTATE_ERROR_FALL_BACK_TIMEOUT0,
    SYSSTATE_ERROR_USED0
  }
};

static SysState_StateType SysState_currentState1;
static SysState_StateType SysState_fallBackState1;
static uint32_t SysState_currentFallBackTimeout1;
static const SysState_StateConfigType SysState_stateConfigs1[SysState_StateLast] =
{
  {
    /* Uninitialized State */
    SYSSTATE_UNINITIALIZED_LED_CHANNEL1,
    SYSSTATE_UNINITIALIZED_ON_TIME1,
    SYSSTATE_UNINITIALIZED_OFF_TIME1,
    SYSSTATE_UNINITIALIZED_FALL_BACK_TIMEOUT1,
    SYSSTATE_UNINITIALIZED_USED1
  },
  {
    /* Initializing State */
    SYSSTATE_INITIALIZING_LED_CHANNEL1,
    SYSSTATE_INITIALIZING_ON_TIME1,
    SYSSTATE_INITIALIZING_OFF_TIME1,
    SYSSTATE_INITIALIZING_FALL_BACK_TIMEOUT1,
    SYSSTATE_INITIALIZING_USED1
  },
  {
    /* Ready State */
    SYSSTATE_READY_LED_CHANNEL1,
    SYSSTATE_READY_ON_TIME1,
    SYSSTATE_READY_OFF_TIME1,
    SYSSTATE_READY_FALL_BACK_TIMEOUT1,
    SYSSTATE_READY_USED1
  },
  {
    /* Operating State */
    SYSSTATE_OPERATING_LED_CHANNEL1,
    SYSSTATE_OPERATING_ON_TIME1,
    SYSSTATE_OPERATING_OFF_TIME1,
    SYSSTATE_OPERATING_FALL_BACK_TIMEOUT1,
    SYSSTATE_OPERATING_USED1
  },
  {
    /* Event State */
    SYSSTATE_EVENT_LED_CHANNEL1,
    SYSSTATE_EVENT_ON_TIME1,
    SYSSTATE_EVENT_OFF_TIME1,
    SYSSTATE_EVENT_FALL_BACK_TIMEOUT1,
    SYSSTATE_EVENT_USED1
  },
  {
    /* Error State */
    SYSSTATE_ERROR_LED_CHANNEL1,
    SYSSTATE_ERROR_ON_TIME1,
    SYSSTATE_ERROR_OFF_TIME1,
    SYSSTATE_ERROR_FALL_BACK_TIMEOUT1,
    SYSSTATE_ERROR_USED1
  }
};

static SysState_StateType SysState_currentState2;
static SysState_StateType SysState_fallBackState2;
static uint32_t SysState_currentFallBackTimeout2;
static const SysState_StateConfigType SysState_stateConfigs2[SysState_StateLast] =
{
  {
    /* Uninitialized State */
    SYSSTATE_UNINITIALIZED_LED_CHANNEL2,
    SYSSTATE_UNINITIALIZED_ON_TIME2,
    SYSSTATE_UNINITIALIZED_OFF_TIME2,
    SYSSTATE_UNINITIALIZED_FALL_BACK_TIMEOUT2,
    SYSSTATE_UNINITIALIZED_USED2
  },
  {
    /* Initializing State */
    SYSSTATE_INITIALIZING_LED_CHANNEL2,
    SYSSTATE_INITIALIZING_ON_TIME2,
    SYSSTATE_INITIALIZING_OFF_TIME2,
    SYSSTATE_INITIALIZING_FALL_BACK_TIMEOUT2,
    SYSSTATE_INITIALIZING_USED2
  },
  {
    /* Ready State */
    SYSSTATE_READY_LED_CHANNEL2,
    SYSSTATE_READY_ON_TIME2,
    SYSSTATE_READY_OFF_TIME2,
    SYSSTATE_READY_FALL_BACK_TIMEOUT2,
    SYSSTATE_READY_USED2
  },
  {
    /* Operating State */
    SYSSTATE_OPERATING_LED_CHANNEL2,
    SYSSTATE_OPERATING_ON_TIME2,
    SYSSTATE_OPERATING_OFF_TIME2,
    SYSSTATE_OPERATING_FALL_BACK_TIMEOUT2,
    SYSSTATE_OPERATING_USED2
  },
  {
    /* Event State */
    SYSSTATE_EVENT_LED_CHANNEL2,
    SYSSTATE_EVENT_ON_TIME2,
    SYSSTATE_EVENT_OFF_TIME2,
    SYSSTATE_EVENT_FALL_BACK_TIMEOUT2,
    SYSSTATE_EVENT_USED2
  },
  {
    /* Error State */
    SYSSTATE_ERROR_LED_CHANNEL2,
    SYSSTATE_ERROR_ON_TIME2,
    SYSSTATE_ERROR_OFF_TIME2,
    SYSSTATE_ERROR_FALL_BACK_TIMEOUT2,
    SYSSTATE_ERROR_USED2
  }
};

static const SysState_ChannelConfigType SysState_channelConfigs[SYSSTATE_NUMBER_OF_CHANNELS] =
{
  /* Channel 0 */
  {
    &SysState_currentState0,
    &SysState_fallBackState0,
    &SysState_currentFallBackTimeout0,
    SysState_stateConfigs0
  },
  /* Channel 1 */
  {
    &SysState_currentState1,
    &SysState_fallBackState1,
    &SysState_currentFallBackTimeout1,
    SysState_stateConfigs1
  },
  /* Channel 2 */
  {
    &SysState_currentState2,
    &SysState_fallBackState2,
    &SysState_currentFallBackTimeout2,
    SysState_stateConfigs2
  }
};

const SysState_ConfigType SysState_config =
{
  SYSSTATE_NUMBER_OF_CHANNELS,
  SysState_channelConfigs
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

