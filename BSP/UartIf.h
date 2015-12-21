/**
 ******************************************************************************
 * @file    UartIf.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    17.11.2014
 * @brief   Abstracts the USART module and implements a FIFO memory.
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#ifndef UARTIF_H
#define UARTIF_H

#include "Services/StdDef.h"

#include "stm32f0xx_usart.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * Data type of one value.
 */
typedef uint8_t UartIf_DataType;
/**
 * Length data type.
 */
typedef uint8_t UartIf_LengthType;

/**
 * Function pointer definition for notifications.
 * @param channel The notification channel.
 */
typedef void (*UartIf_NotifyType)(const uint8_t channel);

/**
 * Defines the UART register type.
 */
typedef USART_TypeDef UartIf_RegistersType;
/**
 * Defines port settings type.
 */
typedef USART_InitTypeDef UartIf_PortSettingsType;
/**
 * Defines clock settings type.
 */
typedef USART_ClockInitTypeDef UartIf_ClockSettingsType;
/**
 * Defines the clock domain type.
 */
typedef uint32_t UartIf_ClockDomainType;

/**
 * Defines the NVIC IRQ type.
 */
typedef IRQn_Type UartIf_NvicIrqType;

/**
 * Defines a fifo buffer.
 */
typedef struct
{
  /* A pointer to the buffer. */
  UartIf_DataType* buffer;
  /* The current read index in the buffer. */
  UartIf_LengthType rdIdx;
  /* The current write index in the buffer. */
  UartIf_LengthType wrIdx;
  /* The current space of the buffer. */
  UartIf_LengthType space;
  /* The size of the buffer. */
  const UartIf_LengthType size;
} UartIf_BufferType;

/**
 * Defines one channel.
 */
typedef struct
{
  /* Rx buffer. */
  UartIf_BufferType rx;
  /* Tx buffer. */
  UartIf_BufferType tx;
  /* Used USART channel number. */
  const uint8_t uartChannelNumber;
  /* Rx notifier */
  const UartIf_NotifyType rxNotifier;
  /* Tx data register empty notifier */
  const UartIf_NotifyType txDreNotifier;
  /* Tx transfer complete notifier */
  const UartIf_NotifyType txCompleteNotifier;
  /* A pointer to the usart registers. */
  UartIf_RegistersType* registers;
  /* A pointer to the port settings. */
  const UartIf_PortSettingsType* portSettings;
  /* A pointer to the clock settings. */
  const UartIf_ClockSettingsType* clockSettings;
  /* The clock domain. */
  const UartIf_ClockDomainType clockDomain;
  /* Sets the NVIC IRQ type. */
  const UartIf_NvicIrqType nvicIrq;
} UartIf_ChannelConfigType;

/**
 * UartIf module config type.
 */
typedef struct
{
  /* Number of used channels. */
  const uint8_t numberOfChannels;
  /* The channel configs. */
  UartIf_ChannelConfigType* channelConfigs;
} UartIf_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Declare a UartIf config, which is setup in the UartIf_Cfg module.
 */
extern UartIf_ConfigType UartIf_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Initializes the UartIf module.
 * @param config A pointer to the configuration.
 */
void UartIf_Init(const UartIf_ConfigType* config);

/**
 * Gets some data from the receive queue.
 * @param channel The used channel.
 * @param dst A pointer to the destination buffer.
 * @param len The length of the destination buffer.
 * @return The length of data, which were readed.
 */
UartIf_LengthType UartIf_GetData(const uint8_t channel, UartIf_DataType* dst,
                                 const UartIf_LengthType len);

/**
 * Puts some data in the transmit queue.
 * @param channel The used channel.
 * @param src A pointer to the source buffer.
 * @param len The length of the source buffer.
 * @return The length of data, which were queued.
 */
UartIf_LengthType UartIf_PutData(const uint8_t channel,
                                 const UartIf_DataType* src,
                                 const UartIf_LengthType len);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/**
 * Isr handler for all UARTS. The given channel determines the source interrupt.
 * @param channel The channel of the isr.
 */
void UartIf_Isr(const uint8_t channel);

#endif
