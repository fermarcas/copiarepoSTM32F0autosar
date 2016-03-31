/**
 ******************************************************************************
 * @file    SpiIf_Cfg.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    31.03.2016
 * @brief   Provides an interface to the hardware serial peripheral interface.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef SPIIF_CFG_H
#define SPIIF_CFG_H

#include "Services/StdDef.h"

/* Generic */
#define SPIIF_NUMBER_OF_CHANNELS          1u

/* Channel 0 */
#define SPIIF_JOB_QUEUE_SIZE0             1u
#define SPIIF_SPI_CHANNEL_NUMBER0         1
#define SPIIF_RX_NOTIFIER0                NULL_PTR
#define SPIIF_DATA_SIZE0                  SPI_DataSize_8b
#define SPIIF_CPOL                        SPI_CPOL_Low
#define SPIIF_CPHA                        SPI_CPHA_1Edge
#define SPIIF_BAUDRATE_PRESCALER          SPI_BaudRatePrescaler_2
#define SPIIF_FIRST_BIT                   SPI_FirstBit_MSB

#endif
