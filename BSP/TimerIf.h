/**
 ******************************************************************************
 * @file    TimerIf.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    15.01.2015
 * @brief   Abstracts all Timers in channels.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef TIMERIF_H
#define TIMERIF_H

#include "Services/StdDef.h"

#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * Defines timer register type.
 */
typedef TIM_TypeDef TimerIf_TimerRegisterType;
/**
 * Defines timer clock domain type.
 */
typedef uint32_t TimerIf_ClockDomainType;
/**
 * Defines timer clock domain setter method type.
 * @param clockDomain The clock domain
 * @param newState ENABLE if the clock should be enabled; otherwise FALSE.
 */
typedef void (*TimerIf_ClockDomainFuncType)(uint32_t rcc,
                                            FunctionalState newState);
/**
 * Defines the NVIC interrupt type.
 */
typedef IRQn_Type TimerIf_NvicIrqType;
/**
 * Defines the frequency type.
 */
typedef uint32_t TimerIf_FrequencyType;
/**
 * Defines the period type.
 */
typedef uint32_t TimerIf_PeriodType;
/**
 * Defines the clock division type.
 */
typedef uint16_t TimerIf_ClockDivisionType;
/**
 * Defines the counter mode type.
 */
typedef uint16_t TimerIf_CounterModeType;
/**
 * Defines a function pointer for the interrupt service routine callback.
 */
typedef void (*TimerIf_IsrCallbackType)(void);

/**
 * Defines one channel of the timer interface.
 */
typedef struct
{
  /* Registers. */
  TimerIf_TimerRegisterType* registers;
  /* Clock domain. */
  const TimerIf_ClockDomainType clockDomain;
  /* Clock domain function. */
  const TimerIf_ClockDomainFuncType clockDomainFunc;
  /* NVIC interrupt. */
  const TimerIf_NvicIrqType nvicIrq;
  /* ISR callback. */
  const TimerIf_IsrCallbackType isrCallback;
  /* If enabled is TRUE, the timer is active; oterwise FALSE. */
  BOOL enabled;
  /* The frequency at startup. */
  TimerIf_FrequencyType frequency;
  /* The period at startup. */
  TimerIf_PeriodType period;
  /* The clock division. */
  TimerIf_ClockDivisionType clockDivision;
  /* The counter mode. */
  TimerIf_CounterModeType counterMode;
} TimerIf_ChannelConfigType;

/**
 * Defines the configuration of the timer interface.
 */
typedef struct
{
  /* The number of channels. */
  const uint8_t numberOfChannels;
  /* The channel configurations. */
  TimerIf_ChannelConfigType* channelConfigs;
} TimerIf_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Compile time configuration.
 */
extern TimerIf_ConfigType TimerIf_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Initializes the timer interface.
 * @param config A pointer to the configuratoin structure.
 */
void TimerIf_Init(TimerIf_ConfigType* config);
/**
 * Starts a timer channel.
 * @param channel The channel which should be started.
 * @return TRUE if the operation succeeded; otherwise FALSE.
 */
BOOL TimerIf_Start(const uint8_t channel);
/**
 * Stops a timer channel.
 * @param channel The channel which should be stopped.
 * @return TRUE if the operation succeeded; otherwise FALSE.
 */
BOOL TimerIf_Stop(const uint8_t channel);
/**
 * Sets the period and the frequency of a timer channel.
 * @param channel The timer channel which should be changed.
 * @param frequency The new frequency.
 * @param period The new period.
 * @return TRUE if the operation succeeded; otherwise FALSE.
 */
BOOL TimerIf_SetFreqPeriod(const uint8_t channel,
                           const TimerIf_FrequencyType frequency,
                           const TimerIf_PeriodType period);
/**
 * Resets the current timer value.
 * @param channel The used channel.
 * @return TRUE if the operation succeeded; otherwise FALSE.
 */
BOOL TimerIf_Reset(const uint8_t channel);

/**
 * Enters the critical section of the timer.
 * @param channel The used channel.
 */
void TimerIf_EnterCS(const uint8_t channel);
/**
 * Leaves the critical section of the timer.
 * @param channel The used channel.
 */
void TimerIf_LeaveCS(const uint8_t channel);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/**
 * Generic TimerIf ISR.
 * @param channel The channel which ISR is meant.
 */
void TimerIf_Isr(const uint8_t channel);

#endif
