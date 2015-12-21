/**
 ******************************************************************************
 * @file    TbProt.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    22.04.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "TbProt.h"
#include "Services/CrcIf.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * type for the command field in an frame
 */
typedef uint8_t TbProt_CmdType;

/**
 * data type for frame data
 */
typedef uint8_t TbProt_FrameDataType;

/**
 * length for frame data
 */
typedef uint16_t TbProt_FrameLengthType;

/**
 * type for crc
 */
typedef uint32_t TbProt_CrcType;

typedef struct
  __attribute__((packed))
  {
    /**
     * command identifier
     */
    TbProt_CmdType cmd;

    /**
     * Mesage ID
     */
    TbProt_MessageIdType messageId;

    /**
     * length of received data
     */
    TbProt_FrameLengthType length;

  } TbProt_FrameHeaderType;

  typedef struct
  {
    /**
     * header of frame
     */
    TbProt_FrameHeaderType header;

    /**
     * pointer to data
     */
    TbProt_FrameDataType* data;

    /**
     * crc
     */
    TbProt_CrcType crc;

  } TbProt_FrameType;

  /****************************************************************
   * Macros
   ***************************************************************/

  /**
   * Defined commands
   */
#define TbProt_CmdSubscribeData   0x01
#define TbProt_CmdCommand         0x02
#define TbProt_CmdUnsubscribeData 0x03
#define TbProt_CmdData            0x04
#define TbProt_CmdConfiguration   0x08
#define TbProt_CmdIsAlive         0x10
#define TbProt_CmdError           0x20
#define TbProt_CmdAck             0x40
#define TbProt_CmdNack            0x80 

  /*
   * Length of frame header
   */
#define TbProt_FrameHeaderLength       sizeof(TbProt_FrameHeaderType)
#define TbProt_CrcLength               sizeof(TbProt_CrcType)
#define TbProt_BufferSize              255u

#define TbProt_SppChannel              0u

  /****************************************************************
   * Function declarations
   ***************************************************************/

  static TbProt_CrcType TbProt_CalcCrc(const TbProt_FrameDataType* data,
                                       const TbProt_LentghType length,
                                       const BOOL resetCrc);

  static void TbProt_ProcessFrame(const TbProt_FrameHeaderType* header,
                                  const TbProt_BluemodIfDataType* data);

  static void TbProt_SendAck(const TbProt_MessageIdType messageId);
  static void TbProt_SendNack(const TbProt_MessageIdType messageId);

  static void TbProt_SendFrame(const TbProt_FrameType* frame);

  static TbProt_BluemodIfLengthType TbProt_CopyBytes(
    TbProt_BluemodIfDataType* dst, const TbProt_BluemodIfDataType* src,
    const TbProt_BluemodIfLengthType length);

  /****************************************************************
   * Global variables
   ***************************************************************/

  static TbProt_BluemodIfDataType TbProt_ReceiveBuffer[TbProt_BufferSize] =
  {
    0 };

  static TbProt_BluemodIfDataType TbProt_SendingBuffer[TbProt_BufferSize] =
  {
    0 };

  TbProt_ConfigType* TbProt_userConfig;

  /****************************************************************
   * User functions
   ***************************************************************/

  void TbProt_Init(TbProt_ConfigType* config)
  {
    /* check user config and parameter */
    if ((TbProt_userConfig == NULL_PTR) && (config != NULL_PTR))
    {
      /* set user config */
      TbProt_userConfig = config;
    }
  }

  void TbProt_SendError(TbProt_MessageIdType const * messageId,
                        TbProt_ErrorDataType const * errorData,
                        TbProt_LentghType const length)
  {
    /* local variables */
    TbProt_FrameType frame;

    /* check user config and parameter */
    if ((TbProt_userConfig != NULL_PTR) && (errorData != NULL_PTR))
    {
      frame.header.cmd = TbProt_CmdError;
      frame.header.messageId = 0; /* TODO fix */
      frame.header.length = (TbProt_FrameLengthType) length;
      frame.data = (TbProt_FrameDataType*) errorData;

      /* calc crc */
      TbProt_CalcCrc((TbProt_FrameDataType*) &frame.header,
      TbProt_FrameHeaderLength,
      TRUE);
      frame.crc = TbProt_CalcCrc(frame.data, frame.header.length, FALSE);

      TbProt_SendFrame(&frame);
    }
  }

  void TbProt_SendData(TbProt_ValueIdType const * id,
                       TbProt_DataType const * data)
  {
    /* local variables */
    TbProt_FrameType frame;

    /* check user config and parameter */
    if ((TbProt_userConfig != NULL_PTR) && (id != NULL_PTR)
      && (data != NULL_PTR))
    {
      frame.header.cmd = TbProt_CmdData;
      frame.header.messageId = *((TbProt_MessageIdType*) id);
      frame.header.length = sizeof(TbProt_DataType);
      frame.data = (TbProt_FrameDataType*) data;

      /* calc crc */
      TbProt_CalcCrc((TbProt_FrameDataType*) &frame.header,
      TbProt_FrameHeaderLength,
      TRUE);
      frame.crc = TbProt_CalcCrc(frame.data, frame.header.length, FALSE);

      TbProt_SendFrame(&frame);
    }
  }

  void TbProt_SendConfiguration(TbProt_MessageIdType const * messageId,
                                TbProt_ConfigDataType const * configData,
                                TbProt_LentghType const length)
  {
    /* local variables */
    TbProt_FrameType frame;

    /* check user config and parameter */
    if ((TbProt_userConfig != NULL_PTR) && (configData != NULL_PTR))
    {
      frame.header.cmd = TbProt_CmdConfiguration;
      frame.header.messageId = *messageId;
      frame.header.length =
        (TbProt_FrameLengthType) (sizeof(TbProt_ConfigDataType) * length);
      frame.data = (TbProt_FrameDataType*) configData;

      /* calc crc */
      TbProt_CalcCrc((TbProt_FrameDataType*) &frame.header,
      TbProt_FrameHeaderLength,
      TRUE);
      frame.crc = TbProt_CalcCrc(frame.data, frame.header.length, FALSE);

      TbProt_SendFrame(&frame);
    }
  }

  /****************************************************************
   * Internal functions/callbacks
   ***************************************************************/

  void TbProt_OnDataReceived(uint8_t const bluetoothDeviceNumber,
                             uint8_t const channelNumber)
  {
    /* local variables */
    TbProt_BluemodIfLengthType read, unprocessed;
    TbProt_FrameHeaderType* header;
    TbProt_CrcType crc;

    /* check config */
    if (TbProt_userConfig != NULL_PTR)
    {
      /* get data from bluemod */
      read = TbProt_userConfig->getBluemodData(
        TbProt_userConfig->bluemodDeviceNumber, channelNumber,
        TbProt_ReceiveBuffer,
        TbProt_BufferSize);

      /* set all read data as unprocessed */
      unprocessed = read;

      /* check if header was received completely */
      while (unprocessed >= TbProt_FrameHeaderLength)
      {
        /* cast header */
        header = (TbProt_FrameHeaderType*) (TbProt_ReceiveBuffer
          + (read - unprocessed));

        /* check if data and crc were received completely */
        if (unprocessed
          >= TbProt_FrameHeaderLength + header->length + TbProt_CrcLength)
        {
          /* calc crc of frame*/
          crc =
            TbProt_CalcCrc(
              (TbProt_FrameDataType*) (TbProt_ReceiveBuffer
                + (read - unprocessed)),
              (TbProt_LentghType) (TbProt_FrameHeaderLength + header->length
                + TbProt_CrcLength),
              TRUE);

          /* check if crc is correct */
          if (crc == 0u)
          {
            /* process data in buffer */
            TbProt_ProcessFrame(header,
              (TbProt_BluemodIfDataType*) (TbProt_ReceiveBuffer
                + TbProt_FrameHeaderLength + (read - unprocessed)));
          }
          else
            TbProt_SendNack(header->messageId);

          /* decrease number of unprocessed bytes in buffer */
          unprocessed = (TbProt_BluemodIfLengthType) (unprocessed
            - (TbProt_FrameHeaderLength + header->length + TbProt_CrcLength));
        }
        else
        {
          /* decrease number of unprocessed bytes in buffer */
          unprocessed = (TbProt_BluemodIfLengthType) (unprocessed
            - TbProt_FrameHeaderLength);
        }
      }
    }
  }

  /****************************************************************
   * Static functions
   ***************************************************************/

  static void TbProt_ProcessFrame(const TbProt_FrameHeaderType* header,
                                  const TbProt_BluemodIfDataType* data)
  {
    /* local variables */
    BOOL result = FALSE;
    TbProt_MessageIdType messageId;
    TbProt_ValueIdType valueId;
    TbProt_DataType dataVal;

    /* check command type */
    switch (header->cmd)
    {
    case TbProt_CmdSubscribeData:
      /* check notification */
      if (TbProt_userConfig->subscribeData != NULL_PTR)
      {
        /* alignment */
        TbProt_CopyBytes((TbProt_BluemodIfDataType*) &valueId, data,
          sizeof(TbProt_ValueIdType));

        /* copy client id (message id) and value id */
        result = TbProt_userConfig->subscribeData(header->messageId, valueId);
      }
      break;

    case TbProt_CmdCommand:
      /* check notification */
      if (TbProt_userConfig->commandReceived != NULL_PTR)
      {
        /* alignment */
        messageId = header->messageId;
        TbProt_CopyBytes((TbProt_BluemodIfDataType*) &dataVal, data,
          sizeof(TbProt_DataType));

        result = TbProt_userConfig->commandReceived(
          *((TbProt_ValueIdType*) (&messageId)), dataVal);
      }
      break;

    case TbProt_CmdUnsubscribeData:
      /* check notification */
      if (TbProt_userConfig->unsubscribeData != NULL_PTR)
      {
        /* alignment */
        TbProt_CopyBytes((TbProt_BluemodIfDataType*) &valueId, data,
          sizeof(TbProt_ValueIdType));

        /* copy client id (message id) and value id */
        result = TbProt_userConfig->unsubscribeData(header->messageId, valueId);
      }
      break;

    case TbProt_CmdConfiguration:
      /* check notification */
      if (TbProt_userConfig->configRequested != NULL_PTR)
        result = TbProt_userConfig->configRequested(header->messageId);
      break;

    case TbProt_CmdIsAlive:
      /* check notification */
      if (TbProt_userConfig->isAliveReiceived != NULL_PTR)
        result = TbProt_userConfig->isAliveReiceived(header->messageId);
      break;

    default:
      return;
    }

    /* check if successful */
    if (result)
      TbProt_SendAck(header->messageId);
    else
      TbProt_SendNack(header->messageId);
  }

  static TbProt_CrcType TbProt_CalcCrc(const TbProt_FrameDataType* data,
                                       const TbProt_LentghType length,
                                       const BOOL resetCrc)
  {
    if (resetCrc)
      /* reset crc */
      CrcIf_Reset();

    /* write data into crc */
    CrcIf_CalcBlock8(data, length);

    /* return calculated crc */
    return CrcIf_GetCrc();
  }

  static void TbProt_SendAck(const TbProt_MessageIdType messageId)
  {
    /* local variables */
    TbProt_FrameType frame;

    /* init frame */
    frame.header.cmd = TbProt_CmdAck;
    frame.header.messageId = messageId;
    frame.header.length = 0;
    frame.crc = TbProt_CalcCrc((TbProt_FrameDataType*) &frame.header,
    TbProt_FrameHeaderLength,
    TRUE);

    TbProt_SendFrame(&frame);
  }

  static void TbProt_SendNack(const TbProt_MessageIdType messageId)
  {
    /* local variables */
    TbProt_FrameType frame;

    /* init frame */
    frame.header.cmd = TbProt_CmdNack;
    frame.header.messageId = messageId;
    frame.header.length = 0;
    frame.crc = TbProt_CalcCrc((TbProt_FrameDataType*) &frame.header,
    TbProt_FrameHeaderLength,
    TRUE);

    TbProt_SendFrame(&frame);
  }

  static void TbProt_SendFrame(const TbProt_FrameType* frame)
  {
    /* Local variables */
    TbProt_FrameLengthType i;
    TbProt_FrameLengthType len = 0u;

    /* Header */
    for (i = 0u; i < TbProt_FrameHeaderLength; i++, len++)
    {
      TbProt_SendingBuffer[len] =
        ((const TbProt_BluemodIfDataType*) &frame->header)[i];
    }

    /* Data */
    for (i = 0u; i < frame->header.length; i++, len++)
      TbProt_SendingBuffer[len] = frame->data[i];

    /* Crc */
    for (i = 0u; i < TbProt_CrcLength; i++, len++)
    {
      TbProt_SendingBuffer[len] =
        ((const TbProt_FrameDataType*) &frame->crc)[i];
    }

    /* check function pointer */
    if (TbProt_userConfig->sendBluemodData != NULL_PTR)
    {
      /* send data */
      TbProt_userConfig->sendBluemodData(TbProt_userConfig->bluemodDeviceNumber,
      TbProt_SppChannel, (TbProt_BluemodIfDataType*) TbProt_SendingBuffer,
        (TbProt_BluemodIfLengthType) len);
    }
  }

  static TbProt_BluemodIfLengthType TbProt_CopyBytes(
    TbProt_BluemodIfDataType* dst, const TbProt_BluemodIfDataType* src,
    const TbProt_BluemodIfLengthType length)
  {
    /* local variables */
    TbProt_BluemodIfLengthType i;

    /* copy data */
    for (i = 0u; i < length; i++)
      dst[i] = src[i];

    return i;
  }

