/**
 ******************************************************************************
 * @file    UartIf.c
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

#include "Services/StdDef.h"
#include "UartIf.h"

/* Clocks */
#include "stm32f0xx_rcc.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

#define UARTIF_APB2_CLOCK_DOMAIN_CHANNEL                              1u

#define UARTIF_ENTER_CS()                                               \
    NVIC_DisableIRQ(UartIf_userCfg->channelConfigs[channel].nvicIrq);

#define UARTIF_LEAVE_CS()                                               \
    NVIC_EnableIRQ(UartIf_userCfg->channelConfigs[channel].nvicIrq);

/****************************************************************
 * Function declarations
 ***************************************************************/
 
 static void UartIf_RxIsr(const uint8_t channel, const UartIf_DataType data);
 static void UartIf_TxDreIsr(const uint8_t channel);
 static void UartIf_TxCompleteIsr(const uint8_t channel);

/****************************************************************
 * Global variables
 ***************************************************************/

static const UartIf_ConfigType* UartIf_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void UartIf_Init(const UartIf_ConfigType* config)
{
  uint8_t i;
  UartIf_ChannelConfigType* channelConfig;
  
  i = 0u;
  channelConfig = NULL_PTR;
  
  if((config != NULL_PTR) && (UartIf_userCfg == NULL_PTR))
  {
    UartIf_userCfg = config;
    
    for(i = 0u; i < config->numberOfChannels; i++)
    {
      channelConfig = &config->channelConfigs[i];
      
      if(channelConfig->uartChannelNumber == UARTIF_APB2_CLOCK_DOMAIN_CHANNEL)
      {
        RCC_APB2PeriphClockCmd(channelConfig->clockDomain, ENABLE);
      }
      else
      {
        RCC_APB1PeriphClockCmd(channelConfig->clockDomain, ENABLE);
      }

      USART_Init(channelConfig->registers,
                 channelConfig->portSettings);
      
      USART_ClockInit(channelConfig->registers,
                      channelConfig->clockSettings);
      
      /* Enable receive ISR */
      USART_ITConfig(channelConfig->registers, USART_IT_RXNE, ENABLE);
      /* Disables overrun detection due to an error in the ST-library */
      USART_OverrunDetectionConfig(channelConfig->registers, USART_OVRDetection_Disable);
      /* Enable ISR in generell */
      NVIC_EnableIRQ(channelConfig->nvicIrq);
      /* Enable UART channel */
      USART_Cmd(channelConfig->registers, ENABLE);
    }
  }
}

UartIf_LengthType UartIf_GetData(const uint8_t channel, 
                                 UartIf_DataType* dst,
                                 const UartIf_LengthType len)
{
  UartIf_LengthType read;
  UartIf_BufferType* buffer;
  
  read = 0u;
  buffer = NULL_PTR;
  
  if((UartIf_userCfg != NULL_PTR) && (channel < UartIf_userCfg->numberOfChannels) &&
     (dst != NULL_PTR))
  {
    buffer = &UartIf_userCfg->channelConfigs[channel].rx;
    
    UARTIF_ENTER_CS();
    for(read = 0u; (read < len) && (buffer->space < buffer->size); read++)
    {
      *dst = buffer->buffer[buffer->rdIdx];
      
      dst++;
      buffer->rdIdx = (UartIf_LengthType)((buffer->rdIdx + 1u) % buffer->size);

      buffer->space++;
    }
    UARTIF_LEAVE_CS();
  }
  
  return read;
}

UartIf_LengthType UartIf_PutData(const uint8_t channel, 
                                 const UartIf_DataType* src,
                                 const UartIf_LengthType len)
{
  /* Local variables */
  UartIf_LengthType written;
  UartIf_BufferType* buffer;
  BOOL sendData;
  
  /* Initialize local variables */
  written = 0u;
  buffer = NULL_PTR;
  sendData = FALSE;
  
  /* Check config and parameters */
  if((UartIf_userCfg != NULL_PTR) && (channel < UartIf_userCfg->numberOfChannels) &&
     (src != NULL_PTR) && (len > 0u))
  {
    /* Get TX buffer */
    buffer = &UartIf_userCfg->channelConfigs[channel].tx;

    UARTIF_ENTER_CS();
    if(buffer->space == buffer->size)
    {
      sendData = TRUE;
    }
    for(written = 0u; (written < len) && (buffer->space > 0u); written++)
    {
      buffer->buffer[buffer->wrIdx] = *src;
      
      src++;
      buffer->wrIdx = (UartIf_LengthType)((buffer->wrIdx + 1u) % buffer->size);

      buffer->space--;
    }
    UARTIF_LEAVE_CS();
    
    if(sendData)
    {
      USART_ITConfig(UartIf_userCfg->channelConfigs[channel].registers,
                     USART_IT_TXE, ENABLE);
    }
  }
  
  return written;
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void UartIf_Isr(const uint8_t uartChannel)
{
  /* Local variables */
  uint8_t channel;
  UartIf_RegistersType* registers;
  
  /* Initialize local variables */
  registers = NULL_PTR;
  channel = 0u;
  
  if(UartIf_userCfg != NULL_PTR)
  {
    for(; channel < UartIf_userCfg->numberOfChannels; channel++)
    {
      if(UartIf_userCfg->channelConfigs[channel].uartChannelNumber == uartChannel)
      {
        break;
      }
    }
    
    if(channel < UartIf_userCfg->numberOfChannels)
    {
      registers = UartIf_userCfg->channelConfigs[channel].registers;
      
      if(USART_GetITStatus(registers, USART_IT_RXNE) == SET)
      {
        UartIf_RxIsr(channel, (UartIf_DataType)USART_ReceiveData(registers));
      }
      
      if(USART_GetITStatus(registers, USART_IT_TXE) == SET)
      {
        UartIf_TxDreIsr(channel);
      }
      
      if(USART_GetITStatus(registers, USART_IT_TC) == SET)
      {
        UartIf_TxCompleteIsr(channel);
        USART_ClearITPendingBit(registers, USART_IT_TC);
      }

      if(USART_GetITStatus(registers, USART_IT_CTS) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_CTS);
      }

      if(USART_GetITStatus(registers, USART_IT_IDLE) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_IDLE);
      }

      if(USART_GetITStatus(registers, USART_IT_ORE) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_ORE);
      }

      if(USART_GetITStatus(registers, USART_IT_NE) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_NE);
      }

      if(USART_GetITStatus(registers, USART_IT_FE) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_FE);
      }

      if(USART_GetITStatus(registers, USART_IT_WU) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_WU);
      }

      if(USART_GetITStatus(registers, USART_IT_CM) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_CM);
      }

      if(USART_GetITStatus(registers, USART_IT_EOB) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_EOB);
      }

      if(USART_GetITStatus(registers, USART_IT_RTO) == SET)
      {
        USART_ClearITPendingBit(registers, USART_IT_RTO);
      }
    }
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/

static void UartIf_RxIsr(const uint8_t channel, const UartIf_DataType data)
{
  /* Local variables */
  UartIf_BufferType* buffer;
  UartIf_NotifyType notifier;
  
  /* Initialize local variables */
  buffer = &UartIf_userCfg->channelConfigs[channel].rx;
  notifier = UartIf_userCfg->channelConfigs[channel].rxNotifier;
  
  /* Check if enough space is available */
  if(buffer->space > 0u)
  {
    buffer->buffer[buffer->wrIdx] = data;
    
    buffer->wrIdx = (UartIf_LengthType)((buffer->wrIdx + 1u) % buffer->size);
    buffer->space--;
  }

  if(notifier != NULL_PTR)
  {
    notifier(channel);
  }
}

static void UartIf_TxDreIsr(const uint8_t channel)
{
  /* Local variables */
  UartIf_BufferType* buffer;
  UartIf_DataType data;
  UartIf_NotifyType notifier;

  /* Initialize local variables */
  buffer = &UartIf_userCfg->channelConfigs[channel].tx;
  data = 0u;
  notifier = UartIf_userCfg->channelConfigs[channel].txDreNotifier;

  if(buffer->space < buffer->size)
  {
    data = buffer->buffer[buffer->rdIdx];

    buffer->rdIdx = (UartIf_LengthType)((buffer->rdIdx + 1u) % buffer->size);
    buffer->space++;

    /* Send data */
    USART_SendData(UartIf_userCfg->channelConfigs[channel].registers, data);
    
    /* If FIFO is empty, enable transmit complete ISR for notifiers */
    if(buffer->space == buffer->size)
    {
      USART_ITConfig(UartIf_userCfg->channelConfigs[channel].registers,
                     USART_IT_TC, ENABLE);
    }
  }
  else
  {
    /* If no data is in the FIFO, disable the data register empty interrupt */
    USART_ITConfig(UartIf_userCfg->channelConfigs[channel].registers,
                   USART_IT_TXE, DISABLE);
  }

  if(notifier != NULL_PTR)
  {
    notifier(channel);
  }
}

static void UartIf_TxCompleteIsr(const uint8_t channel)
{
  /* Local variables */
  UartIf_BufferType* buffer;
  UartIf_NotifyType notifier;

  /* Initialize local variables */
  buffer = &UartIf_userCfg->channelConfigs[channel].tx;
  notifier = NULL_PTR;

  if(buffer->space == buffer->size)
  {
    notifier = UartIf_userCfg->channelConfigs[channel].txCompleteNotifier;

    if(notifier != NULL_PTR)
    {
      notifier(channel);
    }
  }
  
  /* Disable ISR in any case */
  USART_ITConfig(UartIf_userCfg->channelConfigs[channel].registers,
                 USART_IT_TC, DISABLE);
}
