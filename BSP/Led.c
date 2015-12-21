/**
 ******************************************************************************
 * @file    Led.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 * @brief   Abstracts Led handling.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "Led.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

/****************************************************************
 * Function declarations
 ***************************************************************/

static BOOL Led_GetValue(const uint8_t channel);
static void Led_SwitchOn(const uint8_t channel);
static void Led_SwitchOff(const uint8_t channel);

/****************************************************************
 * Global variables
 ***************************************************************/

static const Led_ConfigType* Led_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void Led_Init(const Led_ConfigType* config)
{
  /* Local variables */
  uint8_t channel;

  /* Initialize local variables */
  channel = 0u;

  if ((config != NULL_PTR) && (Led_userCfg == NULL_PTR))
  {
    Led_userCfg = config;

    /* per default turn all leds off */
    for (channel = 0u; channel < config->numberOfLeds; channel++)
    {
      Led_TurnOff(channel);
    }
  }
}

BOOL Led_TurnOn(const uint8_t channel)
{
  /* Local variables */
  BOOL ret;

  /* Initialize local variables */
  ret = Led_BlinkOnOffTime(channel, UINT32_MAX, 0u);

  return ret;
}

BOOL Led_TurnOff(const uint8_t channel)
{
  /* Local variables */
  BOOL ret;

  /* Initialize local variables */
  ret = Led_BlinkOnOffTime(channel, 0u, UINT32_MAX);

  return ret;
}

BOOL Led_BlinkDuty(const uint8_t channel, const uint32_t duty,
                   const uint32_t period)
{
  /* Local variables */
  BOOL ret;
  uint32_t onTime;

  /* Initialize local variables */
  ret = FALSE;
  onTime = (duty * period) / 100;

  ret = Led_BlinkOnOffTime(channel, onTime, period - onTime);

  return ret;
}

BOOL Led_BlinkOnOffTime(const uint8_t channel, const uint32_t onTime,
                        const uint32_t offTime)
{
  /* Local variables */
  BOOL ret;
  Led_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  ret = FALSE;
  channelConfig = NULL_PTR;

  /* Check configuration */
  if ((Led_userCfg != NULL_PTR) && (channel < Led_userCfg->numberOfLeds)
      && ((onTime > 0u) || (offTime > 0u)))
  {
    channelConfig = &Led_userCfg->channelConfigs[channel];

    channelConfig->onTime = onTime;
    channelConfig->offTime = offTime;

    ret = TRUE;
  }

  return ret;
}

void Led_MsTimer(void)
{
  /* Local variables */
  uint8_t channel;
  Led_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  channel = 0u;
  channelConfig = NULL_PTR;

  /* Check configuration */
  if (Led_userCfg != NULL_PTR)
  {
    for (channel = 0u; channel < Led_userCfg->numberOfLeds; channel++)
    {
      channelConfig = &Led_userCfg->channelConfigs[channel];

      channelConfig->currentTime++;
      if ((Led_GetValue(channel) == TRUE)
        && (channelConfig->currentTime > channelConfig->onTime))
      {
        /* Reset time */
        channelConfig->currentTime = 0u;
        Led_SwitchOff(channel);
      }
      else if ((Led_GetValue(channel) == FALSE)
        && (channelConfig->currentTime > channelConfig->offTime))
      {
        channelConfig->currentTime = 0u;
        Led_SwitchOn(channel);
      }
    }
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

static BOOL Led_GetValue(const uint8_t channel)
{
  /* Local variables */
  BOOL ret;
  const Led_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  channelConfig = &Led_userCfg->channelConfigs[channel];
  ret = !channelConfig->activeHigh;

  if (GpioIf_GetBit(channelConfig->gpioChannel))
    ret = channelConfig->activeHigh;

  return ret;
}

static void Led_SwitchOn(const uint8_t channel)
{
  /* Local variables */
  const Led_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  channelConfig = &Led_userCfg->channelConfigs[channel];

  if (channelConfig->activeHigh)
    GpioIf_SetBit(channelConfig->gpioChannel);
  else
    GpioIf_ClrBit(channelConfig->gpioChannel);
}

static void Led_SwitchOff(const uint8_t channel)
{
  /* Local variables */
  const Led_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  channelConfig = &Led_userCfg->channelConfigs[channel];

  if (channelConfig->activeHigh)
    GpioIf_ClrBit(channelConfig->gpioChannel);
  else
    GpioIf_SetBit(channelConfig->gpioChannel);
}
