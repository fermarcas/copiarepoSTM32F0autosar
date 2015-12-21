/**
 ******************************************************************************
 * @file    Rs485.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    24.11.2014
 * @brief   Header of Rs485 Module
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#ifndef RS485_H
#define RS485_H

#include "Services/StdDef.h"
#include "GpioIf.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * Data type
 */
typedef uint8_t Rs485_DataType;

/**
 * Type for index and lengths
 */
typedef uint8_t Rs485_LengthType;

/*
 * Type for all notifications
 */
typedef void (*Rs485_NotifyType)(const uint8_t);

/* Configuration Type for single RS485 channel */
typedef struct
{
  /* channel number of uartif */
  const uint8_t uartifChannelNumber;
  /* */
  const GpioIf_ChannelType gpioControlLinePin;
  /* Rx notifier */
  const Rs485_NotifyType rxNotifier;
  /* Tx data register empty notifier */
  const Rs485_NotifyType txDreNotifier;
  /* Tx transfer complete notifier */
  const Rs485_NotifyType txCompleteNotifier;
}
Rs485_ChannelConfigType;


/**
 *Configruation type
 */
typedef struct
{
  /**
   * Number of defined Rs485 channels
   */
  const uint8_t numberOfChannels;

  /**
   * Array of all channel configs
   */
  const Rs485_ChannelConfigType* channelConfig;
}
Rs485_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/
extern const Rs485_ConfigType Rs485_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Initializes the Rs485 Module
 * @param config A pointer to the config structure
 */
void Rs485_Init(const Rs485_ConfigType* config);

/**
 * Fucntion to Read the available Data from the Rs485 interface
 * @param channel defined Rs485 channel
 * @param dest destination buffer
 * @param len size of the destination buffer
 * @return length of read data
 */
Rs485_LengthType Rs485_GetData(const uint8_t channel,
                               Rs485_DataType* dest,
                               const Rs485_LengthType size);

/**
 * Function to Send data over the Rs485 Interface
 * @param channel defined Rs485 channel
 * @param src source field
 * @param len length of data in source field
 * @return length of sent data
 */
Rs485_LengthType Rs485_PutData(const uint8_t channel,
                               const Rs485_DataType* src,
                               const Rs485_LengthType len);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void Rs485_RxIsr(const uint8_t channel);
void Rs485_TxDreIsr(const uint8_t channel);
void Rs485_TxCompleteIsr(const uint8_t channel);

#endif
