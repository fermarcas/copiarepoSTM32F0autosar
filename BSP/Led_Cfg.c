/**
 ******************************************************************************
 * @file    Led_Cfg.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 * @brief   Configuration for Led module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "Led.h"
#include "Led_Cfg.h"

#include "GpioIf.h"

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

static Led_ChannelConfigType Led_channelConfigs[LED_NUMBER_OF_CHANNELS] =
{
  {
    LED_GPIO_CHANNEL0,
    LED_GPIO_ACTIVE_HIGH0,
    0u, 0u, 0u
  },
  {
    LED_GPIO_CHANNEL1,
    LED_GPIO_ACTIVE_HIGH1,
    0u, 0u, 0u
  },
  {
    LED_GPIO_CHANNEL2,
    LED_GPIO_ACTIVE_HIGH2,
    0u, 0u, 0u
  }
};

const Led_ConfigType Led_config =
{
  LED_NUMBER_OF_CHANNELS,
  Led_channelConfigs
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

