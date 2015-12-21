/**
 ******************************************************************************
 * @file    SysState.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "SysState.h"

#include "BSP/Led.h"

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

static const SysState_ConfigType* SysState_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void SysState_Init(const SysState_ConfigType* config)
{
  /* Local variables */
  uint8_t channel;

  /* Check parameters and configuration */
  if ((config != NULL_PTR) && (SysState_userCfg == NULL_PTR))
  {
    SysState_userCfg = config;

    for (channel = 0u; channel < config->numberOfChannels; channel++)
    {
      SysState_SetState(channel, SysState_StateUninitialized);
    }
  }
}

SysState_StateType SysState_GetState(const uint8_t channel)
{
  /* Local variable */
  SysState_StateType ret;

  /* Initialize local variables */
  ret = SysState_StateUninitialized;

  /* Check configuration and parameters */
  if ((SysState_userCfg != NULL_PTR)
    && (channel < SysState_userCfg->numberOfChannels))
  {
    ret = *(SysState_userCfg->channels[channel].currentState);
  }

  return ret;
}

BOOL SysState_SetState(const uint8_t channel, const SysState_StateType newState)
{
  /* Local variables */
  BOOL ret;
  const SysState_ChannelConfigType* channelConfig;
  const SysState_StateConfigType* oldStateConfig;
  const SysState_StateConfigType* newStateConfig;

  /* Initialize local variables */
  ret = FALSE;
  channelConfig = NULL_PTR;
  oldStateConfig = NULL_PTR;
  newStateConfig = NULL_PTR;

  /* Check configuration and parameters */
  if ((SysState_userCfg != NULL_PTR)
    && (channel < SysState_userCfg->numberOfChannels)
    && (newState < SysState_StateLast))
  {
    channelConfig = &SysState_userCfg->channels[channel];
    oldStateConfig = &channelConfig->stateConfig[*channelConfig->currentState];
    newStateConfig = &channelConfig->stateConfig[newState];

    ret = TRUE;
    if (newStateConfig->used)
    {
      ret = Led_TurnOff(oldStateConfig->ledChannel);

      if (ret)
      {
        ret = Led_BlinkOnOffTime(newStateConfig->ledChannel,
                                 newStateConfig->onTime,
                                 newStateConfig->offTime);
      }

      if (ret)
      {
        *channelConfig->currentState = newState;
        *channelConfig->fallBackTime = 0u;
        if (newStateConfig->fallBackTimeout == SYSSTATE_FALL_BACK_DISABLE)
        {
          *channelConfig->fallBackState = newState;
        }
      }
    }
  }

  return ret;
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void SysState_MsTimer(void)
{
  /* Local variables */
  uint8_t channel;
  const SysState_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  channel = 0u;
  channelConfig = NULL_PTR;

  /* Check configuration */
  if (SysState_userCfg != NULL_PTR)
  {
    for (channel = 0u; channel < SysState_userCfg->numberOfChannels; channel++)
    {
      channelConfig = &SysState_userCfg->channels[channel];

      (*channelConfig->fallBackTime)++;

      if ((*channelConfig->fallBackTime)
        > channelConfig->stateConfig[*channelConfig->currentState].fallBackTimeout)
      {
        SysState_SetState(channel, *channelConfig->fallBackState);
      }
    }
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/

