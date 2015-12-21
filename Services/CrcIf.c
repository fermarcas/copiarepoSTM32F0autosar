/**
 ******************************************************************************
 * @file    CrcIf.c
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    17.11.2014
 * @brief   Abstracts the Crc Functions.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "CrcIf.h"
#include "stm32f0xx_conf.h"

/****************************************************************
 * Function declarations
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

static const CrcIf_ConfigType* CrcIf_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void CrcIf_Init(const CrcIf_ConfigType* config)
{
  if ((config != NULL_PTR) && (CrcIf_userCfg == NULL_PTR))
  {
    CrcIf_userCfg = config;

    /* Enable CRC AHB clock interface */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

    /* DeInitialize CRC peripheral register to default reset value*/
    CRC_DeInit();

    /* Initialize the INIT register ro reset value*/
    CRC_SetInitRegister(config->crcInitValue);

    /* Select 32-bit polynomial size */
    CRC_PolynomialSizeSelect(config->polSize);

    /* Sets reverse input */
    CRC_ReverseInputDataSelect(config->reverseInput);

    /* Sets reverse output */
    CRC_ReverseOutputDataCmd(config->reverseOutput);
  }
}

uint32_t CrcIf_Calc8(const uint8_t val)
{
  *((uint8_t*) (&CRC->DR)) = val;

  return CRC->DR;
}

uint32_t CrcIf_CalcBlock8(const uint8_t* buffer, const uint32_t length)
{
  /* Local variables */
  uint32_t i;

  for (i = 0u; i < length; i++)
  {
    *((uint8_t*) (&CRC->DR)) = buffer[i];
  }

  return CRC->DR;
}

uint32_t CrcIf_Calc16(const uint16_t val)
{
  *((uint16_t*) (&CRC->DR)) = val;

  return CRC->DR;
}

uint32_t CrcIf_CalcBlock16(const uint16_t* buffer, const uint32_t length)
{
  /* Local variables */
  uint32_t i;

  for (i = 0u; i < length; i++)
  {
    *((uint16_t*) (&CRC->DR)) = buffer[i];
  }

  return CRC->DR;
}
