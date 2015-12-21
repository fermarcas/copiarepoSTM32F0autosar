/**
 ******************************************************************************
 * @file    CrcIf_Cfg.h
 * @author  Mitter Gilbert
 * @version V1.0.0
 * @date    24.11.2014
 * @brief   Configuration for the CrcIf module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef CRCIF_CFG_H
#define CRCIF_CFG_H

#include "Services/StdDef.h"
#include "stm32f0xx_crc.h"

/* Generic */

#define CRCIF_POLYSIZE        CRC_PolSize_32  /* 32-bit polynomial for CRC calculation */
#define CRCIF_INITVALUE       0xFFFFFFFF      /* default init value */
#define CRCIF_REVERSE_INPUT   CRC_ReverseInputData_32bits
#define CRCIF_REVERSE_OUTPUT  ENABLE

#endif
