/**
 ******************************************************************************
 * @file    WdgIf.c
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    11.03.2015
 * @brief   Abstracts the Watchdog functions
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "WdgIf.h"
#include "stm32f0xx_conf.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

#define WDGIF_MAX_RELOAD_VALUE                       4095u
#define WDGIF_MIN_PRESCALER                             4u
#define WDGIF_MAX_PRESCALER                           256u
#define WDGIF_MAX_PRESCALER_BIT								          6u /*  b110*/

/****************************************************************
 * Function declarations
 ***************************************************************/

static void WdgIf_SetValues(void);

/****************************************************************
 * Global variables
 ***************************************************************/

static const WdgIf_ConfigType* WdgIf_userCfg = NULL_PTR;
static uint8_t WdgIf_prescalerValue = 0u;
static uint32_t WdgIf_reloadValue = 0u;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Init function for the independent watchdog(IWDG)
 */
void WdgIf_Init(const WdgIf_ConfigType* config)
{
  if ((config != NULL_PTR) && (WdgIf_userCfg == NULL_PTR))
  {
    WdgIf_userCfg = config;

    if (WdgIf_userCfg != NULL_PTR)
    {
      /* Switch on LSI (40khZ internal oscillator) */
      RCC_LSICmd(ENABLE);

      /* Wait till LSI is ready */
      while (!RCC_GetFlagStatus(RCC_FLAG_LSIRDY))
        ;

      /* Activate write access on register for prescaler and reload value */
      IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

      /* Calculates the prescaler and the reload value for given timevalue */
      /* max Timeout values @40KHz (LSI): 100us …26.2s */
      WdgIf_SetValues();

      /* Passes the prescaler value to the IWDG_SetPrescaler function */
      IWDG_SetPrescaler(WdgIf_prescalerValue);

      /* Passes the reload value to the IWDG_SetReload function */
      IWDG_SetReload((uint16_t) WdgIf_reloadValue);

      /* Sets the Watchdog to the max value.
       Must be called periodic in the main loop */
      IWDG_ReloadCounter();

      /* Activates the Watchdog. */
      IWDG_Enable();
    }
  }
}

/**
 * Reload the Watchdog counter
 */
void WdgIf_Reload(void)
{
  if (WdgIf_userCfg != NULL_PTR)
  {
    /* Sets the Watchdog to the max value.
     Must be called periodic in the main loop */
    IWDG_ReloadCounter();
  }
}
/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

/**
 * Calculates the prescaler (has to be a multiplie of 4) and the
 * Reload Value (max 4095) if timervalue is greater than possible value,
 * set (prescaler and reload value) to the max possible time = 26.2 sec .
 * If no timervalue is set, than the reset time of the IWDG is set 
 * to((4*4096)/40000) = 0,4096 s 
 * @param registerValue: IWDG needs the prescaler value as bit values
 * Prescaler 4 = 0 (b000),8 = 1 (b001), 16 = 2 (b010), 32 = 3 (b011)
 * 64 = 4 (b100), 128 = 5 (b101), 256 = 6 (b110)
 */
static void WdgIf_SetValues(void)
{
  BOOL running = TRUE;
  uint8_t registerValue = 0u;
  uint32_t actualReloadValue = 0u;
  /* Factor for calculating in ms*/
  uint32_t timeFactor = 1000u;

  /* calculate the max. possible Timer load value */
  /* (256 * 4095 *1000)/(40000) */
  uint32_t maxPosReloadVal = ((WDGIF_MAX_PRESCALER * WDGIF_MAX_RELOAD_VALUE
                               * timeFactor)
                              / LSI_VALUE);

  uint32_t actualPrescaler = WDGIF_MIN_PRESCALER;

  /* If timerValue is >0 and <= max. possible timer load value */
  if ((WdgIf_userCfg->timerValue > 0)
    && (WdgIf_userCfg->timerValue <= maxPosReloadVal))
  {
    while (running)
    {
      /* calculate the actual reload value with given parameters*/
      actualReloadValue = ((WdgIf_userCfg->timerValue * LSI_VALUE)
        / (timeFactor * actualPrescaler));

      /* Reload Value is still out of range, raise the prescaler-divider */
      if (actualReloadValue > WDGIF_MAX_RELOAD_VALUE)
      {
        /* multiplicate by 2, to get next Prescaler-divider*/
        actualPrescaler = actualPrescaler << 1;
        registerValue++;
      }
      /* given time value is in reload value area*/
      else
      {
        running = FALSE;
      }
    }

    WdgIf_prescalerValue = registerValue;
    WdgIf_reloadValue = (uint16_t) actualReloadValue;
  }
  /* given timeValue is out of the maximum possible range*/
  /* set the values to the max possible time = 26.2 s*/
  else if (WdgIf_userCfg->timerValue > maxPosReloadVal)
  {
    /*max Prescaler Divider 256 = 110 in bits*/
    WdgIf_prescalerValue = WDGIF_MAX_PRESCALER_BIT;

    WdgIf_reloadValue = WDGIF_MAX_RELOAD_VALUE;
  }
  /* timeValue is zero*/
  else
  {
    /*sets the IWDG to an reset time with ((4*4096)/40000) = 0,4096 s */
    WdgIf_reloadValue = WDGIF_MAX_RELOAD_VALUE;
    WdgIf_prescalerValue = registerValue;
  }
}

