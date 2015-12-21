/**
 ******************************************************************************
 * @file    Led_Cfg.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 * @brief   Configuration for Led module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef LED_CFG_H
#define LED_CFG_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/* Generic settings */
#define LED_NUMBER_OF_CHANNELS                                3u

/* Channel 0 */
#define LED_GPIO_CHANNEL0                                     GpioIf_ChannelA07
#define LED_GPIO_ACTIVE_HIGH0                                 FALSE

/* Channel 1 */
#define LED_GPIO_CHANNEL1                                     GpioIf_ChannelB01
#define LED_GPIO_ACTIVE_HIGH1                                 FALSE

/* Channel 2 */
#define LED_GPIO_CHANNEL2                                     GpioIf_ChannelB00
#define LED_GPIO_ACTIVE_HIGH2                                 FALSE

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
