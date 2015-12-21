/**
 ******************************************************************************
 * @file    UartIf_Cfg.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    17.11.2014
 * @brief   Configuration for UartIf module.
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#include "UartIf.h"
#include "UartIf_Cfg.h"

/* Clocks */
#include "stm32f0xx_rcc.h"

#include "Rs485.h"

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

/* Channel 0 */
static UartIf_DataType UartIf_dataRx0[UARTIF_BUFFER_RD_SIZE0];
static UartIf_DataType UartIf_dataTx0[UARTIF_BUFFER_WR_SIZE0];

static const UartIf_PortSettingsType UartIf_portSettings0 =
{
  UARTIF_BAUD0,
  UARTIF_WORD_LENGTH0,
  UARTIF_STOP_BITS0,
  UARTIF_PARITY0,
  UARTIF_MODE0,
  UARTIF_HW_FLOW_CONTROL0
};

static const UartIf_ClockSettingsType UartIf_clockSettings =
{
  USART_Clock_Disable,
  USART_CPOL_Low,
  USART_CPHA_1Edge,
  USART_LastBit_Disable
};

/* Channel 1 */
static UartIf_DataType UartIf_dataRx1[UARTIF_BUFFER_RD_SIZE1];
static UartIf_DataType UartIf_dataTx1[UARTIF_BUFFER_WR_SIZE1];

static const UartIf_PortSettingsType UartIf_portSettings1 =
{
  UARTIF_BAUD1,
  UARTIF_WORD_LENGTH1,
  UARTIF_STOP_BITS1,
  UARTIF_PARITY1,
  UARTIF_MODE1,
  UARTIF_HW_FLOW_CONTROL1
};

/* Configs */
static UartIf_ChannelConfigType UartIf_channelConfigs[UARTIF_NUMBER_OF_CHANNELS] =
{
  /* channel 0 */
  {
    { UartIf_dataRx0, 0u, 0u, UARTIF_BUFFER_RD_SIZE0, UARTIF_BUFFER_RD_SIZE0 },  /* Rx buffer */
    { UartIf_dataTx0, 0u, 0u, UARTIF_BUFFER_WR_SIZE0, UARTIF_BUFFER_RD_SIZE0 },  /* Tx buffer */
    UARTIF_UART_CHANNEL_NUMBER0,
    UARTIF_RX_NOTIFIER0,
    UARTIF_TX_DRE_NOTIFIER0,
    UARTIF_TX_COMPLETE_NOTIFIER0,
    USART1,
    &UartIf_portSettings0,
    &UartIf_clockSettings,
    UARTIF_CLOCK_DOMAIN0,
    UARTIF_NVIC_IRQ_TYPE0
  },
  /* channel 1 */
  {
    { UartIf_dataRx1, 0u, 0u, UARTIF_BUFFER_RD_SIZE1, UARTIF_BUFFER_RD_SIZE1 },  /* Rx buffer */
    { UartIf_dataTx1, 0u, 0u, UARTIF_BUFFER_WR_SIZE1, UARTIF_BUFFER_RD_SIZE1 },  /* Tx buffer */
    UARTIF_UART_CHANNEL_NUMBER1,
    UARTIF_RX_NOTIFIER1,
    UARTIF_TX_DRE_NOTIFIER1,
    UARTIF_TX_COMPLETE_NOTIFIER1,
    USART2,
    &UartIf_portSettings1,
    &UartIf_clockSettings,
    UARTIF_CLOCK_DOMAIN1,
    UARTIF_NVIC_IRQ_TYPE1
  }
};

UartIf_ConfigType UartIf_config =
{
  UARTIF_NUMBER_OF_CHANNELS,
  UartIf_channelConfigs
};

/****************************************************************
 * User functions
 ***************************************************************/
