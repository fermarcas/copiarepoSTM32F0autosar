/**
 ******************************************************************************
 * @file    SysTimerIf.c
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    14.12.2014
 * @brief   TimerIf Module
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "SysTimerIf.h"
#include "stm32f0xx_conf.h"

/*SysTick clock*/
#include "stm32f0xx_rcc.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

/**
 * Macro for entering the Critical Section, disables the SysTick Interrupt.
 */
#define SYSTIMERIF_ENTER_CS()                                   \
    NVIC_DisableIRQ(SysTick_IRQn)

/**
 * Macro for leaving the Critical Section, enables the SysTick Interrupt.
 */
#define SYSTIMERIF_LEAVE_CS()                                   \
    NVIC_EnableIRQ(SysTick_IRQn)

/****************************************************************
 * Function declarations
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * A pointer to the internal configuration.
 */
static const SysTimerIf_ConfigType* SysTimerIf_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void SysTimerIf_Init(SysTimerIf_ConfigType* config)
{
  RCC_ClocksTypeDef RCC_Clocks;

  if (config != NULL_PTR && SysTimerIf_userCfg == NULL_PTR)
  {
    SysTimerIf_userCfg = config;

    /* SysTick end of count event each 1ms */
    RCC_GetClocksFreq(&RCC_Clocks);

    /* Configures the SysTick clock source. */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    /* Configures the SysTick Timer to generate an interrupt every millisecond (48000000Hz / 1000) */
    SysTick_Config(RCC_Clocks.HCLK_Frequency / SysTimerIf_userCfg->timeResolution);
  }
}

void SysTimerIf_MainFunction(void)
{
  /* Local variables */
  uint8_t i;
  SysTimerIf_ChannelConfigType* channel;

  /* Initialize local variables */
  i = 0u;
  channel = NULL_PTR;

  if (SysTimerIf_userCfg != NULL_PTR)
  {
    /* Enter the Critical Section. */
    SYSTIMERIF_ENTER_CS();
    for (i = 0u; i < SysTimerIf_userCfg->numberOfChannels; i++)
    {
      channel = &SysTimerIf_userCfg->channelConfigs[i];
      if ((channel->expired) && (channel->callback != NULL_PTR))
      {
        channel->callback();
        channel->expired = FALSE;
      }
    }
    /* Leave the Critical Section. */
    SYSTIMERIF_LEAVE_CS();
  }
}

void SysTimerIf_Start(const uint8_t channel, const uint32_t timeout)
{
  /* Local variables */
  SysTimerIf_ChannelConfigType* userCfg;

  /* Initialize local variables */
  userCfg = NULL_PTR;

  if (SysTimerIf_userCfg != NULL_PTR
    && channel < SysTimerIf_userCfg->numberOfChannels)
  {
    /* Enter the Critical Section. */
    SYSTIMERIF_ENTER_CS();
    userCfg = &SysTimerIf_userCfg->channelConfigs[channel];

    if (userCfg != NULL_PTR)
    {
      userCfg->isActivated = TRUE;
      userCfg->timeout = timeout;
      userCfg->reload = timeout;
      userCfg->expired = FALSE;
    }
    /* Leave the Critical Section. */
    SYSTIMERIF_LEAVE_CS();
  }
}

void SysTimerIf_Stop(const uint8_t channel)
{
  /* Local variables */
  SysTimerIf_ChannelConfigType* userCfg;

  /* Initialize local variables */
  userCfg = NULL_PTR;

  if (SysTimerIf_userCfg != NULL_PTR
    && channel < SysTimerIf_userCfg->numberOfChannels)
  {
    /* Enter the Critical Section. */
    SYSTIMERIF_ENTER_CS();
    userCfg = &SysTimerIf_userCfg->channelConfigs[channel];

    if (userCfg != NULL_PTR)
    {
      userCfg->isActivated = FALSE;
      userCfg->timeout = 0;
      userCfg->expired = FALSE;
    }
    /* Leave the Critical Section. */
    SYSTIMERIF_LEAVE_CS();
  }
}

void SysTimerIf_SetCallback(const uint8_t channel, SysTimerIf_CallbackType callback)
{
  if (SysTimerIf_userCfg != NULL_PTR
    && channel < SysTimerIf_userCfg->numberOfChannels&& callback != NULL_PTR)
  {
    SysTimerIf_userCfg->channelConfigs[channel].callback = callback;
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void SysTimerIf_Isr(void)
{
  /* Local variables */
  uint8_t i;
  SysTimerIf_ChannelConfigType* channel;

  /* Initialize local variables */
  i = 0u;
  channel = NULL_PTR;

  if (SysTimerIf_userCfg != NULL_PTR)
  {
    for (i = 0u; i < SysTimerIf_userCfg->numberOfChannels; i++)
    {
      channel = &SysTimerIf_userCfg->channelConfigs[i];
      if (channel->isActivated)
      {
        channel->timeout--;
        /* Time has expired. */
        if (channel->timeout == 0)
        {
          channel->expired = TRUE;
          /* Reload timer with default value. */
          channel->timeout = channel->reload;
        }
      }
    }
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/

