/**
 ******************************************************************************
 * @file    SysState_Cfg.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    03.06.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef SYSSTATE_CFG_H
#define SYSSTATE_CFG_H

#include "Services/StdDef.h"

/****************************************************************
 * Generic Settings
 ***************************************************************/

/* Generic */
#define SYSSTATE_NUMBER_OF_CHANNELS                             3u

/****************************************************************
 * Channel 0
 ***************************************************************/

/* Uninitialized State */
#define SYSSTATE_UNINITIALIZED_LED_CHANNEL0                     0u
#define SYSSTATE_UNINITIALIZED_ON_TIME0                         100u
#define SYSSTATE_UNINITIALIZED_OFF_TIME0                        1000u
#define SYSSTATE_UNINITIALIZED_FALL_BACK_TIMEOUT0               SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_UNINITIALIZED_USED0                            TRUE
/* Initializing State */
#define SYSSTATE_INITIALIZING_LED_CHANNEL0                      0u
#define SYSSTATE_INITIALIZING_ON_TIME0                          0u
#define SYSSTATE_INITIALIZING_OFF_TIME0                         0u
#define SYSSTATE_INITIALIZING_FALL_BACK_TIMEOUT0                SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_INITIALIZING_USED0                             FALSE
/* Ready State */
#define SYSSTATE_READY_LED_CHANNEL0                             0u
#define SYSSTATE_READY_ON_TIME0                                 0u
#define SYSSTATE_READY_OFF_TIME0                                0u
#define SYSSTATE_READY_FALL_BACK_TIMEOUT0                       SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_READY_USED0                                    FALSE
/* Operating State */
#define SYSSTATE_OPERATING_LED_CHANNEL0                         0u
#define SYSSTATE_OPERATING_ON_TIME0                             0u
#define SYSSTATE_OPERATING_OFF_TIME0                            0u
#define SYSSTATE_OPERATING_FALL_BACK_TIMEOUT0                   SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_OPERATING_USED0                                FALSE
/* Event State */
#define SYSSTATE_EVENT_LED_CHANNEL0                             0u
#define SYSSTATE_EVENT_ON_TIME0                                 0u
#define SYSSTATE_EVENT_OFF_TIME0                                0u
#define SYSSTATE_EVENT_FALL_BACK_TIMEOUT0                       SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_EVENT_USED0                                    FALSE
/* Error State */
#define SYSSTATE_ERROR_LED_CHANNEL0                             0u
#define SYSSTATE_ERROR_ON_TIME0                                 0u
#define SYSSTATE_ERROR_OFF_TIME0                                0u
#define SYSSTATE_ERROR_FALL_BACK_TIMEOUT0                       SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_ERROR_USED0                                    FALSE

/****************************************************************
 * Channel 1
 ***************************************************************/

/* Uninitialized State */
#define SYSSTATE_UNINITIALIZED_LED_CHANNEL1                     1u
#define SYSSTATE_UNINITIALIZED_ON_TIME1                         0u
#define SYSSTATE_UNINITIALIZED_OFF_TIME1                        UINT32_MAX
#define SYSSTATE_UNINITIALIZED_FALL_BACK_TIMEOUT1               SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_UNINITIALIZED_USED1                            TRUE
/* Initializing State */
#define SYSSTATE_INITIALIZING_LED_CHANNEL1                      1u
#define SYSSTATE_INITIALIZING_ON_TIME1                          100u
#define SYSSTATE_INITIALIZING_OFF_TIME1                         100u
#define SYSSTATE_INITIALIZING_FALL_BACK_TIMEOUT1                SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_INITIALIZING_USED1                             TRUE
/* Ready State */
#define SYSSTATE_READY_LED_CHANNEL1                             1u
#define SYSSTATE_READY_ON_TIME1                                 150u
#define SYSSTATE_READY_OFF_TIME1                                1000u
#define SYSSTATE_READY_FALL_BACK_TIMEOUT1                       SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_READY_USED1                                    TRUE
/* Operating State */
#define SYSSTATE_OPERATING_LED_CHANNEL1                         1u
#define SYSSTATE_OPERATING_ON_TIME1                             300u
#define SYSSTATE_OPERATING_OFF_TIME1                            200u
#define SYSSTATE_OPERATING_FALL_BACK_TIMEOUT1                   SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_OPERATING_USED1                                TRUE
/* Event State */
#define SYSSTATE_EVENT_LED_CHANNEL1                             1u
#define SYSSTATE_EVENT_ON_TIME1                                 UINT32_MAX
#define SYSSTATE_EVENT_OFF_TIME1                                0u
#define SYSSTATE_EVENT_FALL_BACK_TIMEOUT1                       500u
#define SYSSTATE_EVENT_USED1                                    TRUE
/* Error State */
#define SYSSTATE_ERROR_LED_CHANNEL1                             1u
#define SYSSTATE_ERROR_ON_TIME1                                 UINT32_MAX
#define SYSSTATE_ERROR_OFF_TIME1                                0u
#define SYSSTATE_ERROR_FALL_BACK_TIMEOUT1                       SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_ERROR_USED1                                    TRUE

/****************************************************************
 * Channel 2
 ***************************************************************/

/* Uninitialized State */
#define SYSSTATE_UNINITIALIZED_LED_CHANNEL2                     2u
#define SYSSTATE_UNINITIALIZED_ON_TIME2                         500u
#define SYSSTATE_UNINITIALIZED_OFF_TIME2                        1500u
#define SYSSTATE_UNINITIALIZED_FALL_BACK_TIMEOUT2               SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_UNINITIALIZED_USED2                            FALSE
/* Initializing State */
#define SYSSTATE_INITIALIZING_LED_CHANNEL2                      2u
#define SYSSTATE_INITIALIZING_ON_TIME2                          0u
#define SYSSTATE_INITIALIZING_OFF_TIME2                         UINT32_MAX
#define SYSSTATE_INITIALIZING_FALL_BACK_TIMEOUT2                SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_INITIALIZING_USED2                             FALSE
/* Ready State */
#define SYSSTATE_READY_LED_CHANNEL2                             2u
#define SYSSTATE_READY_ON_TIME2                                 150u
#define SYSSTATE_READY_OFF_TIME2                                1000u
#define SYSSTATE_READY_FALL_BACK_TIMEOUT2                       SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_READY_USED2                                    TRUE
/* Operating State */
#define SYSSTATE_OPERATING_LED_CHANNEL2                         2u
#define SYSSTATE_OPERATING_ON_TIME2                             300u
#define SYSSTATE_OPERATING_OFF_TIME2                            200u
#define SYSSTATE_OPERATING_FALL_BACK_TIMEOUT2                   SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_OPERATING_USED2                                TRUE
/* Event State */
#define SYSSTATE_EVENT_LED_CHANNEL2                             2u
#define SYSSTATE_EVENT_ON_TIME2                                 100u
#define SYSSTATE_EVENT_OFF_TIME2                                100u
#define SYSSTATE_EVENT_FALL_BACK_TIMEOUT2                       1000u
#define SYSSTATE_EVENT_USED2                                    TRUE
/* Error State */       
#define SYSSTATE_ERROR_LED_CHANNEL2                             2u
#define SYSSTATE_ERROR_ON_TIME2                                 UINT32_MAX
#define SYSSTATE_ERROR_OFF_TIME2                                0u
#define SYSSTATE_ERROR_FALL_BACK_TIMEOUT2                       SYSSTATE_FALL_BACK_DISABLE
#define SYSSTATE_ERROR_USED2                                    TRUE

#endif
