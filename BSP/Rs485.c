/**
 ******************************************************************************
 * @file    Rs485.c
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    24.11.2014
 * @brief   Implementation of Rs485 Module
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#include "Rs485.h"
#include "UartIf.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

/****************************************************************
 * Function declarations
 ***************************************************************/

static BOOL Rs485_GetRs485Channel(const uint8_t uartChannel,
  uint8_t* rs485Channel);

/****************************************************************
 * Global variables
 ***************************************************************/

static const Rs485_ConfigType* Rs485_userConfig = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void Rs485_Init(const Rs485_ConfigType* config)
{
  /* Check config parameter and current config */
  if ((config != NULL_PTR) && (Rs485_userConfig == NULL_PTR))
  {
    /* Save config struct */
    Rs485_userConfig = config;
  }
}

Rs485_LengthType Rs485_GetData(const uint8_t channel, Rs485_DataType* dest,
  const Rs485_LengthType size)
{
  Rs485_LengthType ret = 0;

  /* check parameter and config*/
  if ((Rs485_userConfig != NULL_PTR)
    && (Rs485_userConfig->numberOfChannels > channel) && (dest != NULL_PTR))
  {
    /* get data from uart module */
    ret = UartIf_GetData(
      Rs485_userConfig->channelConfig[channel].uartifChannelNumber, dest, size);
  }

  return ret;
}

Rs485_LengthType Rs485_PutData(const uint8_t channel, const Rs485_DataType* src,
  const Rs485_LengthType len)
{
  /* Local Variables */
  Rs485_LengthType ret = 0;
  const Rs485_ChannelConfigType* channelConfig = NULL_PTR;

  /* Check parameter and config*/
  if ((Rs485_userConfig != NULL_PTR)
    && (Rs485_userConfig->numberOfChannels > channel) && (src != NULL_PTR)
    && (len > 0u))
  {
    channelConfig = &Rs485_userConfig->channelConfig[channel];

    /* set Rs485 control lines */
    GpioIf_SetBit(channelConfig->gpioControlLinePin);

    /* put data in uart module */
    ret = UartIf_PutData(channelConfig->uartifChannelNumber, src, len);
  }

  return ret;
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void Rs485_RxIsr(const uint8_t channel)
{
  /* Local variables */
  uint8_t rs485ChannelNumber = 0;
  Rs485_NotifyType notifier = NULL_PTR;

  /* check config*/
  if (Rs485_userConfig != NULL_PTR)
  {/* check if channel was defined */
    if (Rs485_GetRs485Channel(channel, &rs485ChannelNumber))
    {
      /* get notifier */
      notifier = Rs485_userConfig->channelConfig[rs485ChannelNumber].rxNotifier;

      /* check if notifier was set */
      if (notifier != NULL_PTR)
        /* raise notification */
        notifier(rs485ChannelNumber);
    }
  }
}

void Rs485_TxDreIsr(const uint8_t channel)
{
  /* Local variables */
  uint8_t rs485ChannelNumber = 0;
  Rs485_NotifyType notifier = NULL_PTR;

  /* check config*/
  if (Rs485_userConfig != NULL_PTR)
  {/* check if channel was defined */
    if (Rs485_GetRs485Channel(channel, &rs485ChannelNumber))
    {
      /* get notifier */
      notifier =
        Rs485_userConfig->channelConfig[rs485ChannelNumber].txDreNotifier;

      /* check if notifier was set */
      if (notifier != NULL_PTR)
        /* raise notification */
        notifier(rs485ChannelNumber);
    }
  }
}

void Rs485_TxCompleteIsr(const uint8_t channel)
{
  /* Local variables */
  uint8_t rs485ChannelNumber = 0;
  Rs485_NotifyType notifier = NULL_PTR;

  /* check config*/
  if (Rs485_userConfig != NULL_PTR)
  {
    /* check if channel was defined */
    if (Rs485_GetRs485Channel(channel, &rs485ChannelNumber))
    {
      /* disable Rs485 control line */
      GpioIf_ClrBit(
        Rs485_userConfig->channelConfig[rs485ChannelNumber].gpioControlLinePin);

      /* get notifier */
      notifier =
        Rs485_userConfig->channelConfig[rs485ChannelNumber].txCompleteNotifier;

      /* check if notifier was set */
      if (notifier != NULL_PTR)
        /* raise notification */
        notifier(rs485ChannelNumber);
    }
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/

static BOOL Rs485_GetRs485Channel(const uint8_t uartChannel,
  uint8_t* rs485Channel)
{
  /* Locale variables */
  uint8_t i = 0;
  BOOL ret = FALSE;

  /* find rs485 channel */
  for (; i < Rs485_userConfig->numberOfChannels; i++)
    /* check channel */
    if (Rs485_userConfig->channelConfig[i].uartifChannelNumber == uartChannel)
    {
      *rs485Channel = i;
      ret = TRUE;
      break;
    }

  return ret;
}
