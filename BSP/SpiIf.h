/**
 ******************************************************************************
 * @file    SpiIf.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    31.03.2016
 * @brief   Provides an interface to the hardware serial peripheral interface.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef SPIIF_H
#define SPIIF_H

#include "Services/StdDef.h"

#include "GpioIf.h"

#include "stm32f0xx_spi.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/* forward declarations */
struct SpiIf_JobStructType;

typedef uint8_t SpiIf_DataType;
typedef uint8_t SpiIf_LengthType;
typedef SPI_InitTypeDef SpiIf_InitSettingsType;
typedef uint32_t SpiIf_ClockDomainType;
typedef IRQn_Type SpiIf_NvicIrqType;

/**
 * Function pointer for job manipulation. This callback can be used
 *  to manipulate the current job during the execution (e.g. dynamic length handling)
 * @param job The current job.
 * @return TRUE if the job was manipulated; otherwise FALSE.
 */
typedef BOOL (*SpiIf_JobNotifier)(struct SpiIf_JobStructType* job);

typedef enum
{
  SpiIf_JobUnprocessed,
  SpiIf_JobQueued,
  SpiIf_JobReverted,
  SpiIf_JobProcessing,
  SpiIf_JobProcessed
} SpiIf_JobStateType;

struct SpiIf_JobStructType
{
  GpioIf_ChannelType cs;
  const SpiIf_DataType* transmittBuffer;
  SpiIf_LengthType transmittLength;
  SpiIf_LengthType currentTxIdx;
  SpiIf_DataType* receiveBuffer;
  SpiIf_LengthType receiveLength;
  SpiIf_LengthType receiveBegin;
  SpiIf_LengthType currentRxIdx;
  /**
   * Receive notification callback. Will be called each time a item was received.
   * Be carefully, it's interrupt driven!
   */
  SpiIf_JobNotifier rxNotifier;
  SpiIf_JobStateType state;
};
typedef struct SpiIf_JobStructType SpiIf_JobType;

typedef struct
{
  SpiIf_JobType** const jobs;
  SpiIf_LengthType rdIdx;
  SpiIf_LengthType wrIdx;
  SpiIf_LengthType space;
  const SpiIf_LengthType size;
} SpiIf_QueueType;

typedef struct
{
  SpiIf_QueueType* const jobQueue;
  const uint8_t spiChannelNumber;
  const SpiIf_InitSettingsType* initSettings;
} SpiIf_ChannelConfigType;

typedef struct
{
  const uint8_t numberOfChannels;
  SpiIf_ChannelConfigType* channelConfigs;
} SpiIf_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

extern SpiIf_ConfigType SpiIf_config;

/****************************************************************
 * User functions
 ***************************************************************/

void SpiIf_Init(SpiIf_ConfigType* config);
void SpiIf_EnqueueJob(uint8_t channel, SpiIf_JobType* job);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void SpiIf_Isr(uint8_t channel);

#endif
