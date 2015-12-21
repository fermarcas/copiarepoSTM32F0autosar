/**
 ******************************************************************************
 * @file    TimerIf.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    15.01.2015
 * @brief   Abstracts all Timers in channels.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "TimerIf.h"

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

static TimerIf_ConfigType* TimerIf_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void TimerIf_Init(TimerIf_ConfigType* config)
{
  /* Local variables */
  uint8_t i;
  TimerIf_ChannelConfigType* channelConfig;
  BOOL enabled;

  /* Initialize local variables */
  i = 0u;
  enabled = FALSE;

  if ((config != NULL_PTR) && (TimerIf_userCfg == NULL_PTR))
  {
    /* Store configuration pointer */
    TimerIf_userCfg = config;

    for (i = 0u; i < config->numberOfChannels; i++)
    {
      channelConfig = &config->channelConfigs[i];

      /* Get functional state */
      enabled = channelConfig->enabled;

      /* Enable clock */
      channelConfig->clockDomainFunc(channelConfig->clockDomain, ENABLE);
      /* Stop timer in any case */
      TIM_DeInit(channelConfig->registers);

      /* Init time base */
      channelConfig->enabled = FALSE; /* Disable button to change frequency and period */
      TimerIf_SetFreqPeriod(i, channelConfig->frequency, channelConfig->period);

      /* Set priority in any case */
      NVIC_SetPriority(channelConfig->nvicIrq, 2u);

      /* If timer is enabled, start it. */
      if (enabled)
      {
        TimerIf_Start(i);
      }
      else
      {
        TimerIf_Stop(i);
      }
    }
  }
}

BOOL TimerIf_Start(const uint8_t channel)
{
  /* Local variables */
  BOOL ret;
  TimerIf_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  ret = FALSE;
  channelConfig = NULL_PTR;

  if ((TimerIf_userCfg != NULL_PTR)
    && (channel < TimerIf_userCfg->numberOfChannels))
  {
    channelConfig = &TimerIf_userCfg->channelConfigs[channel];

    /* Enable interrupt */
    TIM_ITConfig(channelConfig->registers, TIM_IT_Update, ENABLE);
    NVIC_EnableIRQ(channelConfig->nvicIrq);

    /* Enable timer */
    TIM_Cmd(channelConfig->registers, ENABLE);

    channelConfig->enabled = TRUE;

    ret = TRUE;
  }

  return ret;
}

BOOL TimerIf_Stop(const uint8_t channel)
{
  /* Local variables */
  BOOL ret;
  TimerIf_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  ret = FALSE;
  channelConfig = NULL_PTR;

  if ((TimerIf_userCfg != NULL_PTR)
    && (channel < TimerIf_userCfg->numberOfChannels))
  {
    channelConfig = &TimerIf_userCfg->channelConfigs[channel];

    /* Disable timer */
    TIM_Cmd(channelConfig->registers, DISABLE);

    /* Disable interrupt */
    TIM_ITConfig(channelConfig->registers, TIM_IT_Update, DISABLE);
    NVIC_DisableIRQ(channelConfig->nvicIrq);

    channelConfig->enabled = FALSE;
    ret = TRUE;
  }

  return ret;
}

BOOL TimerIf_SetFreqPeriod(const uint8_t channel,
                           const TimerIf_FrequencyType frequency,
                           const TimerIf_PeriodType period)
{
  /* Local variables */
  BOOL ret;
  TimerIf_ChannelConfigType* channelConfig;
  RCC_ClocksTypeDef rccClocks;
  TIM_TimeBaseInitTypeDef timerConfig;

  /* Initialize local variables */
  ret = FALSE;
  channelConfig = NULL_PTR;

  if ((TimerIf_userCfg != NULL_PTR)
    && (channel < TimerIf_userCfg->numberOfChannels))
  {
    channelConfig = &TimerIf_userCfg->channelConfigs[channel];

    if (!channelConfig->enabled)
    {
      /* Store new frequency and period */
      channelConfig->frequency = frequency;
      channelConfig->period = period;

      /* Initialize RCC clocks */
      RCC_GetClocksFreq(&rccClocks);

      /* Initialize timer base */
      timerConfig.TIM_Prescaler = (uint16_t) ((rccClocks.PCLK_Frequency
        / channelConfig->frequency) - 1u);
      timerConfig.TIM_Period = channelConfig->period;
      timerConfig.TIM_ClockDivision = channelConfig->clockDivision;
      timerConfig.TIM_CounterMode = channelConfig->counterMode;
      TIM_TimeBaseInit(channelConfig->registers, &timerConfig);

      ret = TRUE;
    }
  }

  return ret;
}

BOOL TimerIf_Reset(const uint8_t channel)
{
  /* Local variables */
  BOOL ret;

  /* Initialize local variables */
  ret = FALSE;

  /* Check configuration */
  if ((TimerIf_userCfg != NULL_PTR)
    && (channel < TimerIf_userCfg->numberOfChannels))
  {
    TIM_SetCounter(TimerIf_userCfg->channelConfigs[channel].registers, 0u);
  }

  return ret;
}

void TimerIf_EnterCS(const uint8_t channel)
{
  /* Check configuration */
  if ((TimerIf_userCfg != NULL_PTR)
    && (channel < TimerIf_userCfg->numberOfChannels))
  {
    /* Disable timer interrupt. */
    NVIC_DisableIRQ(TimerIf_userCfg->channelConfigs[channel].nvicIrq);
  }
}

void TimerIf_LeaveCS(const uint8_t channel)
{
  /* Check configuration */
  if ((TimerIf_userCfg != NULL_PTR)
    && (channel < TimerIf_userCfg->numberOfChannels))
  {
    /* Enable timer interrupt if the timer is enabled. */
    if (TimerIf_userCfg->channelConfigs[channel].enabled)
    {
      NVIC_EnableIRQ(TimerIf_userCfg->channelConfigs[channel].nvicIrq);
    }
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void TimerIf_Isr(const uint8_t channel)
{
  /* Local variables */
  TimerIf_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  channelConfig = NULL_PTR;

  /* Check configuration. */
  if ((TimerIf_userCfg != NULL_PTR)
    && (channel < TimerIf_userCfg->numberOfChannels))
  {
    channelConfig = &TimerIf_userCfg->channelConfigs[channel];

    /* Call ISR callback */
    if (channelConfig->isrCallback != NULL_PTR)
    {
      channelConfig->isrCallback();
    }

    /* Clear pending bit */
    TIM_ClearITPendingBit(channelConfig->registers, TIM_IT_Update);
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/

