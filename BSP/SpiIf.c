/**
 ******************************************************************************
 * @file    SpiIf.c
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

#include "stm32f0xx_rcc.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

typedef void (*SpiIf_PeriphClockCmd)(uint32_t, FunctionalState);

/****************************************************************
 * Macros
 ***************************************************************/

#define SPIIF_GET_REGISTER(channel)                             \
  SpiIf_registers[SpiIf_userCfg->channelConfigs[(channel)].spiChannelNumber - 1u]

#define SPIIF_GET_NVIC(channel)                                  \
  SpiIf_nvics[SpiIf_userCfg->channelConfigs[(channel)].spiChannelNumber - 1u]

#define SPIIF_GET_CLOCK_DOMAIN(channel)                         \
  SpiIf_clockDomains[SpiIf_userCfg->channelConfigs[(channel)].spiChannelNumber - 1u]

#define SPIIF_SET_CLOCK_CMD(channel, newState)                  \
  SpiIf_clockCmds[SpiIf_userCfg->channelConfigs[(channel)].spiChannelNumber - 1u] \
    (SPIIF_GET_CLOCK_DOMAIN(channel), newState)

#define SPIIF_ENTER_CS(channel)                                 \
  NVIC_DisableIRQ(SPIIF_GET_NVIC(channel))

#define SPIIF_LEAVE_CS(channel)                                 \
  NVIC_EnableIRQ(SPIIF_GET_NVIC(channel))

/****************************************************************
 * Function declarations
 ***************************************************************/

static BOOL SpiIf_ConvertSpiChannelToChannel(uint8_t* channel);

static void SpiIf_TxIsr(uint8_t channel);
static void SpiIf_RxIsr(uint8_t channel);

/****************************************************************
 * Global variables
 ***************************************************************/

static SpiIf_ConfigType* SpiIf_userCfg = NULL_PTR;

static SPI_TypeDef* const SpiIf_registers[] =
{
  SPI1,
  SPI2
};

static const IRQn_Type const SpiIf_nvics[] =
{
  SPI1_IRQn,
  SPI2_IRQn
};

static const uint32_t const SpiIf_clockDomains[] =
{
  RCC_APB2Periph_SPI1,
  RCC_APB1Periph_SPI2
};

static const SpiIf_PeriphClockCmd const SpiIf_clockCmds[] =
{
  RCC_APB2PeriphClockCmd,
  RCC_APB1PeriphClockCmd
};

/****************************************************************
 * User functions
 ***************************************************************/

void SpiIf_Init(SpiIf_ConfigType* config)
{
  /* Local variables */
  uint8_t channel;
  const SpiIf_ChannelConfigType* channelConfig;

  /* Initialize local variables */
  channel = 0u;
  channelConfig = NULL_PTR;

  /* Check user configuration and parameters */
  if ((SpiIf_userCfg == NULL_PTR) && (config != NULL_PTR))
  {
    /* Store given configuration locally */
    SpiIf_userCfg = config;

    /* Iterate through all channels and configure each SPI channel */
    for (channel = 0u; channel < config->numberOfChannels; channel++)
    {
      /* Get pointer to the channel configuration */
      channelConfig = &config->channelConfigs[channel];

      /* Enable peripheral clock */
      SPIIF_SET_CLOCK_CMD(channel, ENABLE);

      /* Initialize SPI */
      SPI_Init(SPIIF_GET_REGISTER(channel), channelConfig->initSettings);

      /* Initialize receive interrupt */
      SPI_I2S_ITConfig(SPIIF_GET_REGISTER(channel), SPI_I2S_IT_RXNE, ENABLE);

      /* Enable interrupts */
      NVIC_EnableIRQ(SPIIF_GET_NVIC(channel));

      /* Enable SPI peripheral */
      SPI_Cmd(SPIIF_GET_REGISTER(channel), ENABLE);
    }
  }
}

void SpiIf_EnqueueJob(uint8_t channel, SpiIf_JobType* job)
{
  /* Local variables */
  BOOL isEmpty;
  SpiIf_ChannelConfigType* channelConfig;
  SpiIf_QueueType* jobQueue;

  /* Initialize local variables */
  isEmpty = FALSE;
  channelConfig = NULL_PTR;
  jobQueue = NULL_PTR;

  /* Check user configuration and parameters */
  if ((SpiIf_userCfg != NULL_PTR) && (channel < SpiIf_userCfg->numberOfChannels)
      && (job != NULL_PTR))
  {
    /* Get configurations */
    channelConfig = &SpiIf_userCfg->channelConfigs[channel];
    jobQueue = channelConfig->jobQueue;

    /* Update job variables for correct handling */
    job->currentTxIdx = 0u;
    job->currentRxIdx = 0u;
    job->state = SpiIf_JobReverted;

    /* Enter critical section (stop SPI interrupts) */
    SPIIF_ENTER_CS(channel);

    /* Check if queue is empty */
    isEmpty = jobQueue->space == jobQueue->size;

    if (jobQueue->space > 0u)
    {
      /* Queue new job and update queue variables */
      jobQueue->jobs[jobQueue->wrIdx] = job;
      jobQueue->wrIdx = (SpiIf_LengthType) ((jobQueue->wrIdx + 1u)
        % jobQueue->size);
      jobQueue->space--;

      /* Update state to job queued */
      job->state = SpiIf_JobQueued;
    }

    /* Leave critical section (restart SPI interrupts) */
    SPIIF_LEAVE_CS(channel);

    /* If queue was empty, start ISR handling */
    if (isEmpty)
      SPI_I2S_ITConfig(SPIIF_GET_REGISTER(channel), SPI_I2S_IT_TXE, ENABLE);
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void SpiIf_Isr(uint8_t channel)
{
  /* Check user configuration and if the given SPI channel exists
   *  This functions converts the SPI channel to internal channel number */
  if ((SpiIf_userCfg != NULL_PTR) && SpiIf_ConvertSpiChannelToChannel(&channel))
  {
    if (SPI_I2S_GetITStatus(SPIIF_GET_REGISTER(channel), SPI_I2S_IT_RXNE))
    {
      SpiIf_RxIsr(channel);
    }

    if (SPI_I2S_GetITStatus(SPIIF_GET_REGISTER(channel), SPI_I2S_IT_TXE))
    {
      SpiIf_TxIsr(channel);
    }
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/

static BOOL SpiIf_ConvertSpiChannelToChannel(uint8_t* channel)
{
  /* Local variables */
  BOOL ret;
  uint8_t i;

  /* Initialize local variables */
  ret = FALSE;
  i = 0u;

  /* Iterate through channels and check if the the given SPI channel exists */
  for (i = 0u; i < SpiIf_userCfg->numberOfChannels; i++)
  {
    if (SpiIf_userCfg->channelConfigs[i].spiChannelNumber == *channel)
    {
      *channel = i;
      ret = TRUE;
      break;
    }
  }

  return ret;
}

static void SpiIf_TxIsr(uint8_t channel)
{
  /* Local variables */
  SpiIf_DataType data;
  SpiIf_QueueType* jobQueue;
  SpiIf_JobType* currentJob;
  SpiIf_LengthType jobLength;

  /* Initialize local variables */
  data = 0u;
  jobQueue = SpiIf_userCfg->channelConfigs[channel].jobQueue;
  currentJob = jobQueue->jobs[jobQueue->rdIdx];
  jobLength = (SpiIf_LengthType)(currentJob->receiveLength + currentJob->receiveLength);
  if (jobLength < currentJob->transmittLength)
    jobLength = currentJob->transmittLength;

  /* Update job state */
  currentJob->state = SpiIf_JobProcessing;
  /* Set CS in any case */
  GpioIf_SetBit(currentJob->cs);

  /* Check if all bytes were send */
  if (currentJob->currentTxIdx < jobLength)
  {
    /* If not, check if there are some input bytes */
    if (currentJob->currentTxIdx < currentJob->transmittLength)
      /* If yes, update data field */
      data = currentJob->transmittBuffer[currentJob->currentTxIdx];

    /* Send input data or dummy data to read values */
    SPI_SendData8(SPIIF_GET_REGISTER(channel), data);
    /* Update job variables */
    currentJob->currentTxIdx++;
  }
  else
  {
    /* Disable transmit ISR */
    SPI_I2S_ITConfig(SPIIF_GET_REGISTER(channel), SPI_I2S_IT_TXE, DISABLE);
  }
}

static void SpiIf_RxIsr(uint8_t channel)
{
  /* Local variables */
  BOOL enableTransmission;
  SpiIf_DataType data;
  SpiIf_QueueType* jobQueue;
  SpiIf_JobType* currentJob;
  SpiIf_LengthType jobLength;
  SpiIf_LengthType idx;

  /* Initialize local variables */
  enableTransmission = FALSE;
  data = 0u;
  jobQueue = SpiIf_userCfg->channelConfigs[channel].jobQueue;
  currentJob = jobQueue->jobs[jobQueue->rdIdx];
  idx = (SpiIf_LengthType) (currentJob->currentRxIdx - currentJob->receiveBegin);
  jobLength = (SpiIf_LengthType)(currentJob->receiveLength + currentJob->receiveLength);
  if (jobLength < currentJob->transmittLength)
    jobLength = currentJob->transmittLength;

  /* Get data from SPI */
  data = SPI_ReceiveData8(SPIIF_GET_REGISTER(channel));

  /* Store data to destination buffer */
  if (idx < currentJob->receiveLength)
    currentJob->receiveBuffer[idx] = data;

  /* Update job variables */
  currentJob->currentRxIdx++;

  /* Call job manipulation callback and check if the job was
   *  manipulated through it */
  enableTransmission = ((currentJob->rxNotifier != NULL_PTR)
    && currentJob->rxNotifier(currentJob));

  /* Check if job was finished */
  if (currentJob->currentRxIdx >= jobLength)
  {
    /* If yes, update job state and switch to next job */
    GpioIf_ClrBit(currentJob->cs);
    currentJob->state = SpiIf_JobProcessed;
    jobQueue->rdIdx = (SpiIf_LengthType) ((jobQueue->rdIdx + 1u) % jobQueue->size);
    jobQueue->space++;
    enableTransmission |= jobQueue->space < jobQueue->size;
  }

  /* Enable transmission, if the job was manipulated or a new job was scheduled */
  if (enableTransmission)
    SPI_I2S_ITConfig(SPIIF_GET_REGISTER(channel), SPI_I2S_IT_TXE, ENABLE);
}
