/**
 ******************************************************************************
 * @file    TimerIf_Cfg.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    15.01.2015
 * @brief   Configurations for TimerIf module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef TIMERIF_CFG_H
#define TIMERIF_CFG_H

#include "Services/StdDef.h"

/****************************************************************
 * General Settings
 ***************************************************************/

#define TIMERIF_NUMBER_OF_CHANNELS                            0u

/****************************************************************
 * Channel 0
 ***************************************************************/

//#define TIMERIF_REGISTERS0                                    TIM2
//#define TIMERIF_CLOCK_DOMAIN0                                 RCC_APB1Periph_TIM2
//#define TIMERIF_CLOCK_DOMAIN_FUNC0                            RCC_APB1PeriphClockCmd
//#define TIMERIF_NVIC_IRQ0                                     TIM2_IRQn
//#define TIMERIF_ISR_CALLBACK0                                 CounterIf_TimerIsr0
//#define TIMERIF_ENABLED0                                      FALSE
//#define TIMERIF_FREQUENCY0                                    100u
//#define TIMERIF_PERIOD0                                       1000u
//#define TIMERIF_CLOCK_DIVISION0                               TIM_CKD_DIV1
//#define TIMERIF_COUNTER_MODE                                  TIM_CounterMode_Up

#endif
