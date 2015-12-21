/**
 ******************************************************************************
 * @file    CrcIf.h
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    17.11.2014
 * @brief   Abstracts the Crc Functions.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef CRCIF_H
#define CRCIF_H

#include "Services/StdDef.h"
#include "stm32f0xx_crc.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * CrcIf module config type.
 */
typedef struct
{
  /* CRC Init Value */
  const uint32_t crcInitValue;
  /* CRC polynomial size */
  const uint32_t polSize;
  /* Defines reverse input */
  const uint32_t reverseInput;
  /* Defines reverse output */
  const FunctionalState reverseOutput;
}
CrcIf_ConfigType;

/****************************************************************
 * Macros
 ***************************************************************/

/**
  * Computes the 32-bit CRC of a given data word(32-bit).
  * @param  CRC_Data: data word(32-bit) to compute its CRC
  * @return 32-bit CRC
  */
#define CrcIf_Calc32(crc)                   CRC_CalcCRC(crc)

/**
  * Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  pBuffer: pointer to the buffer containing the data to be computed
  * @param  BufferLength: length of the buffer to be computed
  * @return 32-bit CRC
  */
#define CrcIf_CalcBlock32(buffer, length)   CRC_CalcBlock(buffer, length)

/**
 * Resets the CRC unit.
 */
#define CrcIf_Reset()                       CRC_ResetDR()

/**
  * Returns the current CRC value.
  * @return 32-bit CRC
  */
#define CrcIf_GetCrc()                      CRC_GetCRC()

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * declares a CrcIf_Config, will be setup in CrcIf_Cfg module
 */
extern CrcIf_ConfigType CrcIf_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
  * Configure CRC peripheral to use 32-bit polynomials.
  */
void CrcIf_Init(const CrcIf_ConfigType* config);

/**
  * Computes the 32-bit CRC of a given byte (8-bit).
  * @param  val byte (8-bit) to compute its CRC
  * @return 32-bit CRC
  */
uint32_t CrcIf_Calc8(const uint8_t val);

/**
  * Computes the 32-bit CRC of a given buffer of data word (8-bit).
  * @param  buffer pointer to the buffer containing the data to be computed
  * @param  length length of the buffer to be computed
  * @return 32-bit CRC
  */
uint32_t CrcIf_CalcBlock8(const uint8_t* buffer, const uint32_t length);

/**
  * Computes the 32-bit CRC of a given half data word (16-bit).
  * @param  val half data word (16-bit) to compute its CRC
  * @return 32-bit CRC
  */
uint32_t CrcIf_Calc16(const uint16_t val);

/**
  * Computes the 32-bit CRC of a given buffer of data word (16-bit).
  * @param  buffer pointer to the buffer containing the data to be computed
  * @param  length length of the buffer to be computed
  * @return 32-bit CRC
  */
uint32_t CrcIf_CalcBlock16(const uint16_t* buffer, const uint32_t length);

#endif
