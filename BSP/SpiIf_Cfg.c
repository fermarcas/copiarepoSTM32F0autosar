/**
 ******************************************************************************
 * @file    SpiIf_Cfg.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    31.03.2016
 * @brief   Provides an interface to the hardware serial peripheral interface.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "SpiIf.h"
#include "SpiIf_Cfg.h"

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

static SpiIf_JobType* SpiIf_jobs0[SPIIF_JOB_QUEUE_SIZE0];
static SpiIf_QueueType SpiIf_queue0 =
{
  SpiIf_jobs0,
  0u,
  0u,
  SPIIF_JOB_QUEUE_SIZE0,
  SPIIF_JOB_QUEUE_SIZE0
};

static const SpiIf_InitSettingsType SpiIf_initSettings0 =
{
  SPI_Direction_2Lines_FullDuplex,
  SPI_Mode_Master,
  SPIIF_DATA_SIZE0,
  SPIIF_CPOL,
  SPIIF_CPHA,
  SPI_NSS_Soft,
  SPIIF_BAUDRATE_PRESCALER,
  SPIIF_FIRST_BIT,
  0u
};

static SpiIf_ChannelConfigType SpiIf_channels[SPIIF_NUMBER_OF_CHANNELS] =
{
  {
    &SpiIf_queue0,
    SPIIF_SPI_CHANNEL_NUMBER0,
    &SpiIf_initSettings0
  }
};

SpiIf_ConfigType SpiIf_config =
{
  SPIIF_NUMBER_OF_CHANNELS,
  SpiIf_channels
};

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

