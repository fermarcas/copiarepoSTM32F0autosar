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
 
#ifndef UARTIF_CFG_H
#define UARTIF_CFG_H

/* Generic */
#define UARTIF_NUMBER_OF_CHANNELS     2u

/* Channel 0 */
#define UARTIF_BUFFER_RD_SIZE0        255u
#define UARTIF_BUFFER_WR_SIZE0        255u
#define UARTIF_UART_CHANNEL_NUMBER0   1u
#define UARTIF_RX_NOTIFIER0           NULL_PTR
#define UARTIF_TX_DRE_NOTIFIER0       NULL_PTR
#define UARTIF_TX_COMPLETE_NOTIFIER0  NULL_PTR
#define UARTIF_BAUD0                  115200u
#define UARTIF_WORD_LENGTH0           USART_WordLength_8b
#define UARTIF_STOP_BITS0             USART_StopBits_1
#define UARTIF_PARITY0                USART_Parity_No
#define UARTIF_MODE0                  (USART_Mode_Rx | USART_Mode_Tx)
#define UARTIF_HW_FLOW_CONTROL0       USART_HardwareFlowControl_RTS_CTS
#define UARTIF_CLOCK_DOMAIN0          RCC_APB2Periph_USART1
#define UARTIF_NVIC_IRQ_TYPE0         USART1_IRQn

/* Channel 1 Rs485 */
#define UARTIF_BUFFER_RD_SIZE1        128u
#define UARTIF_BUFFER_WR_SIZE1        128u
#define UARTIF_UART_CHANNEL_NUMBER1   2u
#define UARTIF_RX_NOTIFIER1           Rs485_RxIsr
#define UARTIF_TX_DRE_NOTIFIER1       Rs485_TxDreIsr
#define UARTIF_TX_COMPLETE_NOTIFIER1  Rs485_TxCompleteIsr
#define UARTIF_BAUD1                  38400u
#define UARTIF_WORD_LENGTH1           USART_WordLength_8b
#define UARTIF_STOP_BITS1             USART_StopBits_1
#define UARTIF_PARITY1                USART_Parity_No
#define UARTIF_MODE1                  (USART_Mode_Rx | USART_Mode_Tx)
#define UARTIF_HW_FLOW_CONTROL1       USART_HardwareFlowControl_None
#define UARTIF_CLOCK_DOMAIN1          RCC_APB1Periph_USART2
#define UARTIF_NVIC_IRQ_TYPE1         USART2_IRQn

#endif
