/**
 ******************************************************************************
 * @file    Led.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 * @brief   Abstracts Led handling.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef LED_H
#define LED_H

#include "Services/StdDef.h"

#include "GpioIf.h"

typedef struct
{
  const GpioIf_ChannelType gpioChannel;
  const BOOL activeHigh;
  uint32_t onTime;
  uint32_t offTime;
  uint32_t currentTime;
} Led_ChannelConfigType;

typedef struct
{
  const uint8_t numberOfLeds;
  Led_ChannelConfigType* channelConfigs;
} Led_ConfigType;

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

extern const Led_ConfigType Led_config;

/****************************************************************
 * User functions
 ***************************************************************/

void Led_Init(const Led_ConfigType* config);

BOOL Led_TurnOn(const uint8_t channel);
BOOL Led_TurnOff(const uint8_t channel);
BOOL Led_BlinkDuty(const uint8_t channel, const uint32_t duty,
                   const uint32_t period);
BOOL Led_BlinkOnOffTime(const uint8_t channel, const uint32_t onTime,
                        const uint32_t offTime);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void Led_MsTimer(void);

#endif
