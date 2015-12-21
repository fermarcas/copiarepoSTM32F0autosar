/**
 ******************************************************************************
 * @file    Bluemod.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    11.03.2015
 * @brief   Abstraction of Stollmann Bluemod + SR
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "Devices/Bluemod.h"
#include "Services/Stopwatch.h"
#include "Services/SysState.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * function type for different main functions
 * @param deviceNumber
 */
typedef void (*Bluemod_MainFunctionType)(const uint8_t deviceNumber);

typedef char Bluemod_CmdLookupEntryValue;
typedef uint8_t Bluemod_CmdLookupEntryLength;

typedef struct
{
  const Bluemod_CmdLookupEntryValue* str;
  const Bluemod_CmdLookupEntryLength length;
} Bluemod_CmdLookupEntry;

typedef void (*Bluemod_ReceivedDataFunctionType)(const uint8_t deviceNumber);

/****************************************************************
 * Macros
 ***************************************************************/

/**
 * Makro for sending data for an given device
 * @param deviceNumber Number of current Bluemod device
 * @param data Pointer to the data source
 * @param length Length of data at data
 */
#define Bluemod_SendToIf(deviceNumber, data, length, sentData)                 \
  if (Bluemod_userConifg->deviceConfigs[(deviceNumber)].interface.ifPutData != \
    NULL_PTR)                                                                  \
    Bluemod_userConifg->deviceConfigs[(deviceNumber)].interface.ifPutData(     \
     Bluemod_userConifg->deviceConfigs[deviceNumber].interface.ifChannelNumber,\
     (Bluemod_IfDataType*)(data), (length))

/**
 * Makro for reading data in given buffer
 * @param deviceNumber Number of current Bluemod device
 * @param buffer Pointer to the receiving buffer
 * @param length Length of data, max number of read data
 * @param readData number of data read
 */
#define Bluemod_GetFromIf(deviceNumber, buffer, length, readData)              \
  (readData) = 0u;                                                             \
  if (Bluemod_userConifg->deviceConfigs[(deviceNumber)].interface.ifGetData != \
    NULL_PTR)                                                                  \
    (readData) =                                                               \
    Bluemod_userConifg->deviceConfigs[(deviceNumber)].interface.ifGetData(     \
     Bluemod_userConifg->deviceConfigs[deviceNumber].interface.ifChannelNumber,\
     (Bluemod_IfDataType*)(buffer), (length))                                             \
  
/**
 * Buffer size for frame buffer
 */
#define Bluemod_FrameBufferSize 256u

#define Bluemod_PostFixLen 1u
#define Bluemod_MuxStartLen 1u
#define Bluemod_MuxAtChannelLen 1u

/**
 * Positions of values within received frame
 */
#define Bluemod_ReceivedChannelPos 0u
#define Bluemod_ReceivedLengthPos 1u
#define Bluemod_ReceivedDataPos 2u

#define Bluemod_DataReceived(deviceNumber, channelNumber)   \
  if((channelNumber) == 0xffu)                              \
    Bluemod_DataReceivedAt(deviceNumber, channelNumber);    \
  else if((channelNumber) > 1u)                             \
    Bluemod_DataReceivedBle(deviceNumber, channelNumber);   \
  else                                                      \
    Bluemod_DataReceivedSpp(deviceNumber, channelNumber);

/****************************************************************
 * Function declarations
 ***************************************************************/

static void Bluemod_SendAt(const uint8_t deviceNumber,
                           const Bluemod_DataType* data,
                           const Bluemod_LengthType length);
static void Bluemod_SendMuxCmd(const uint8_t deviceNumber,
                               const Bluemod_DataType* data,
                               const Bluemod_LengthType length);
static void Bluemod_SendMuxData(const uint8_t deviceNumber,
                                const Bluemod_DataType channel,
                                const Bluemod_DataType* data,
                                const Bluemod_LengthType length);

static void Bluemod_MainUninitialized(const uint8_t deviceNumber);
static void Bluemod_MainBooting(const uint8_t deviceNumber);
static void Bluemod_MainInitializing(const uint8_t deviceNumber);
static void Bluemod_MainMux(const uint8_t deviceNumber);

static void Bluemod_ProcessCommands(const uint8_t deviceNumber);
static void Bluemod_ReadInput(const uint8_t deviceNumber);
static void Bluemod_ParseInputAt(const uint8_t deviceNumber);
static void Bluemod_ParseInputMux(const uint8_t deviceNumber);

/**
 * add an cmd to the cmd queue with initial trycount = 0, the command
 * will be repeatedly transmitted
 */
static void Bluemod_EnqueueRetryCmd(const uint8_t deviceNumber,
                                    const Bluemod_CmdType cmd,
                                    const Bluemod_CmdDataType data);

/**
 * add an cmd to the cmd queue with initial trycount = maxTryCount -1, the 
 * command transmitted once
 */
static void Bluemod_EnqueueSingleCmd(const uint8_t deviceNumber,
                                     const Bluemod_CmdType cmd,
                                     const Bluemod_CmdDataType data);

static void Bluemod_DequeueCmd(const uint8_t deviceNumber);

static void Bluemod_ClearLastFrame(const uint8_t deviceNumber);

static void Bluemod_DataReceivedAt(const uint8_t deviceNumber,
                                   const uint8_t channelNumber);
static void Bluemod_DataReceivedSpp(const uint8_t deviceNumber,
                                    const uint8_t channelNumber);
static void Bluemod_DataReceivedBle(const uint8_t, const uint8_t channelNumber);

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Current configuration
 */
static Bluemod_ConfigType* Bluemod_userConifg = NULL_PTR;

static Bluemod_MainFunctionType Blumod_MainLookup[] =
{
  Bluemod_MainUninitialized,
  Bluemod_MainBooting,
  Bluemod_MainInitializing,
  Bluemod_MainMux };

/**
 * buffer for building the complete command frame
 */
static Bluemod_DataType Bluemod_FrameBuffer[Bluemod_FrameBufferSize] =
{
  0 };

static const Bluemod_CmdLookupEntry Bluemod_CmdLookup[] =
{
  {
    "AT",
    2 },
  {
    "ATI",
    3 },
  {
    "ATE",
    3 },
  {
    "ATA ",
    4 },
  {
    "AT+BMUX=",
    8 },
  {
    "AT+BNAME=",
    9 },
  {
    "AT+BNDDEL=",
    10 },
  {
    "AT+BOAD",
    7 },
  {
    "ATH ",
    4 },
  {
    "RESET",
    5 },
  {
    "ATS",
    3 },
  {
    "AT+BCLASS=",
    10 },
  {
    "AT+BCRYPT=",
    10 },
  {
    "AT+BINQ",
    7 },
  {
    "AT+BPIN",
    7 },
  {
    "AT+BSNAME=",
    10 },
  {
    "AT+LEADINTMIN=",
    14 },
  {
    "AT+LEADINTMAX=",
    14 },
  {
    "AT+LECONINTMIN=",
    15 },
  {
    "AT+LECONINTMAX=",
    15 },
  {
    "AT+LEROLE=",
    10 },
  {
    "AT+LETIO=",
    9 },
  {
    "AT+LEADDATA=",
    12 },
  {
    "AT+LEADE=",
    9 },
  {
    "AT+LEADPAR=",
    9 },
  {
    "AT+LEADATTRIB=",
    12 },
  {
    "AT+LESCDATA=",
    10 } };

/**
 * pre and post fix definitions
 */
static const Ascii_ValueType Bluemod_PostFix[] =
{
  '\r' };

static const Bluemod_IfDataType Bluemod_MuxStart[] =
{
  0xcc };

static const Bluemod_IfDataType Bluemod_AtDelim[] =
{
  0x0D,
  0x0A }; /* CR LF */

static const Bluemod_IfDataType Bluemod_MuxAtChannel[] =
{
  0xff };

static const Bluemod_CmdDataType Bluemod_CmdDataEmpty =
{
  0,
  0u };
static const Bluemod_CmdDataType Bluemod_CmdDataDisable =
{
  "0",
  1u };
static const Bluemod_CmdDataType Bluemod_CmdDataEnable =
{
  "1",
  1u };
static const Bluemod_CmdDataType Bluemod_CmdDataSetCallAceptance =
{
  "0=1",
  3u };

static const Ascii_ValueType Bluemod_ResponseOk[] = "OK";
static const Ascii_ValueType Bluemod_ResponseRing[] = "RING";
static const Ascii_ValueType Bluemod_ResponseConnect[] = "CONNECT";
static const Ascii_ValueType Bluemod_ResponseNoCarrier[] = "NO CARRIER";

/****************************************************************
 * User functions
 ***************************************************************/

void Bluemod_Init(Bluemod_ConfigType* config)
{
  /* check user config and parameter */
  if ((Bluemod_userConifg == NULL_PTR) && (config != NULL_PTR))
  {
    /* set user config */
    Bluemod_userConifg = config;
  }
}

void Bluemod_MainFunction(void)
{
  /* local variables */
  uint8_t deviceNumber = 0;

  /* check config */
  if (Bluemod_userConifg != NULL_PTR)
  {
    /* iterate through all devices */
    for (; deviceNumber < Bluemod_userConifg->numberOfDevices; deviceNumber++)
    {
      /* execute main according to device state */
      Blumod_MainLookup[Bluemod_userConifg->deviceConfigs[deviceNumber].state](
        deviceNumber);
    }
  }
}

Bluemod_LengthType Bluemod_GetData(uint8_t const deviceNumber,
                                   Bluemod_ChannelType channel,
                                   Bluemod_DataType* data,
                                   Bluemod_LengthType const length)
{
  /* local variables */
  Bluemod_LengthType written = 0u;
  Bluemod_DeviceConfigType* device;
  Bluemod_LengthType recLength;

  /* TODO: check correct channel */
  UNUSED_PARAM(channel);

  /* check config and input params */
  if (Bluemod_userConifg != NULL_PTR && data != NULL_PTR)
  {
    /* initialize local variables */
    device = &Bluemod_userConifg->deviceConfigs[deviceNumber];

    /* check if data were completely received */
    if (device->receivedData->recState == Bluemod_ReceivedStateComplete)
    {
      /* get length from queue */
      recLength =
        (Bluemod_LengthType) device->receiveQueue.entries[(Ascii_LengthType) (device->receiveQueue.rdIdx
          + (Ascii_LengthType) (device->receivedData->offset
            + Bluemod_ReceivedLengthPos))];

      /* copy data */
      written = Ascii_CopyData(device->receiveQueue.entries,
        (Ascii_LengthType) (device->receiveQueue.rdIdx
          + (Ascii_LengthType) (device->receivedData->offset
            + Bluemod_ReceivedDataPos)), (Ascii_ValueType*) data, 0u,
        recLength);
    }
  }
  return written;
}

void Bluemod_SendData(uint8_t const deviceNumber,
                      Bluemod_ChannelType const channel, Bluemod_DataType* data,
                      Bluemod_LengthType const length)
{
  /* check config and parameter */
  if (Bluemod_userConifg != NULL_PTR && data != NULL_PTR)
  {
    /* send data over interface */
    Bluemod_SendMuxData(deviceNumber, channel, data, length);
  }
}

void Bluemod_AcceptConnection(uint8_t const deviceNumber,
                              Bluemod_CmdDataType cmdData)
{
  /* check config */
  if (Bluemod_userConifg != NULL_PTR)
  {
    /* enqueue command for accepting the connection */
    Bluemod_EnqueueSingleCmd(deviceNumber, Bluemod_CmdAcceptCall, cmdData);

    /* TODO remove test ok command */
    /*Bluemod_EnqueueRetryCmd(deviceNumber, Bluemod_CmdGetOK, Bluemod_CmdDataEmpty);*/
  }
}

void Bluemod_Disconnect(uint8_t const deviceNumber, Bluemod_CmdDataType cmdData)
{
  /* check config */
  if (Bluemod_userConifg != NULL_PTR)
  {
    /* enqueue command for accepting the connection */
    Bluemod_EnqueueSingleCmd(deviceNumber, Bluemod_CmdCloseConnection, cmdData);
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

static void Bluemod_MainUninitialized(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device;
  Bluemod_CmdDataType dataName;

  /* Initialize local variables */
  device = &Bluemod_userConifg->deviceConfigs[deviceNumber];
  dataName.data =
    (Bluemod_CmdDataValueType*) device->bluetoothSettings.deviceName;
  dataName.length = device->bluetoothSettings.deviceNameLength;

  /* set bluetooth state */
  SysState_SetState(device->sysStateChannel, SysState_StateInitializing);

  /* reset bluemod via reset gpio */
  GpioIf_ClrBit(device->bluemodResetPin);

  /* enqueue initial commands */
  Bluemod_EnqueueRetryCmd(deviceNumber, Bluemod_CmdGetOK, Bluemod_CmdDataEmpty);
  Bluemod_EnqueueRetryCmd(deviceNumber, Bluemod_CmdSetName, dataName);
  Bluemod_EnqueueRetryCmd(deviceNumber, Bluemod_CmdSetLocalEcho,
    Bluemod_CmdDataDisable);
  Bluemod_EnqueueRetryCmd(deviceNumber, Bluemod_CmdStatusReg,
    Bluemod_CmdDataSetCallAceptance);
  Bluemod_EnqueueRetryCmd(deviceNumber, Bluemod_CmdSetMux,
    Bluemod_CmdDataEnable);

  /* switch to next state */
  device->state = Bluemod_StateBoot;
}

static void Bluemod_MainBooting(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];

  /* reset bluemod via reset gpio */
  GpioIf_SetBit(device->bluemodResetPin);

  /* check booting timeout */
  if (Stopwatch_ReturnDifference(device->watch, Stopwatch_GetCurrentValue())
    > device->bootTime)
    /* switch to next state */
    device->state = Bluemod_StateInitialising;
}

static void Bluemod_MainInitializing(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Bluemod_CmdQueueType* queue = &device->cmdQueue;
  Bluemod_CmdQueueEntryType* cmdEntry = &queue->commands[queue->readIdx];
  Bluemod_LengthType frameBufferPos;

  /* check current receive/send state */
  switch (device->sendRecvState)
  {
  case Bluemod_SendReceievStateSending:
  {
    /* check if there is an cmd in the queue to send */
    if (queue->space < queue->size)
    {
      /* send command */

      /* build command frame */
      frameBufferPos = 0u;

      /* write command in buffer */
      frameBufferPos = (Bluemod_LengthType) (frameBufferPos
        + Ascii_CopyData(Bluemod_CmdLookup[cmdEntry->cmd].str, 0u,
          (Ascii_ValueType*) Bluemod_FrameBuffer, frameBufferPos,
          Bluemod_CmdLookup[cmdEntry->cmd].length));

      /* check if there are data to be sent */
      if (cmdEntry->data.length > 0u)
      {
        /* write data in buffer */
        frameBufferPos = (Bluemod_LengthType) (frameBufferPos
          + Ascii_CopyData(cmdEntry->data.data, 0u,
            (Ascii_ValueType*) Bluemod_FrameBuffer, frameBufferPos,
            cmdEntry->data.length));
      }

      /* set stopwatch */
      device->watch = Stopwatch_GetCurrentValue();

      /* send command frame */
      Bluemod_SendAt(deviceNumber, Bluemod_FrameBuffer, frameBufferPos);

      /* set receiving state */
      device->sendRecvState = Bluemod_SendReceievStateReceiving;
    }
    else
    {
      /* cmd queue is empty so initialization is finished,
       continue to next state */
      device->state = Bluemod_StateMux;

      /* set async receiving mode for async receiving */
      device->sendRecvState = Bluemod_SendReceievStateReceivingAsync;

      /* set bluetooth state */
      SysState_SetState(device->sysStateChannel, SysState_StateReady);
    }
    break;
  }

  case Bluemod_SendReceievStateReceiving:
  {
    /* check timeout */
    if (Stopwatch_ReturnDifference(device->watch, Stopwatch_GetCurrentValue())
      > device->sendTimeout)
    {
      /* timeout occured return to sending state */
      device->sendRecvState = Bluemod_SendReceievStateSending;

      /* clear input buffer */
      /*device->receiveQueue.rdIdx = device->receiveQueue.wrIdx;
       device->receiveQueue.space = device->receiveQueue.size;*/
    }
    else
    {
      /* read data from if */
      Bluemod_ReadInput(deviceNumber);

      /* process input */
      Bluemod_ParseInputAt(deviceNumber);
    }
    break;
  }
  default:
    break;
  }
}

static void Bluemod_MainMux(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];

  /* check current receiving state */
  switch (device->sendRecvState)
  {
  case Bluemod_SendReceievStateSending:
  {
    Bluemod_ProcessCommands(deviceNumber);
    break;
  }
  case Bluemod_SendReceievStateReceiving:
  {
    /* check timeout */
    if (Stopwatch_ReturnDifference(device->watch, Stopwatch_GetCurrentValue())
      > device->sendTimeout)
    {
      /* timeout occured return to sending state */
      device->sendRecvState = Bluemod_SendReceievStateSending;

      /* clear input buffer */
      device->receiveQueue.rdIdx = device->receiveQueue.wrIdx;
      device->receiveQueue.space = device->receiveQueue.size;
    }
    break;
  }
  default:
    break;
  }

  /* read and process input, neccessary for async receiving */
  Bluemod_ReadInput(deviceNumber);
  Bluemod_ParseInputMux(deviceNumber);
}

static void Bluemod_ProcessCommands(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Bluemod_CmdQueueType* queue = &device->cmdQueue;
  Bluemod_CmdQueueEntryType* cmdEntry;
  Bluemod_LengthType frameBufferPos;

  /* check if there is an command in the queue */
  if (queue->space < queue->size)
  {
    cmdEntry = &queue->commands[queue->readIdx];

    /* check if the command queue entry reached max trycount */
    if (cmdEntry->tryCount < device->bluetoothSettings.retryCount)
    {
      /* send command */

      /* increase try count */
      cmdEntry->tryCount++;

      /* build command frame */
      frameBufferPos = 0u;

      /* write command in buffer */
      frameBufferPos = (Bluemod_LengthType) (frameBufferPos
        + Ascii_CopyData(Bluemod_CmdLookup[cmdEntry->cmd].str, 0u,
          (Ascii_ValueType*) Bluemod_FrameBuffer, frameBufferPos,
          Bluemod_CmdLookup[cmdEntry->cmd].length));

      /* check if there are data to be sent */
      if (cmdEntry->data.length > 0u)
      {
        /* write data in buffer */
        frameBufferPos = (Bluemod_LengthType) (frameBufferPos
          + Ascii_CopyData(cmdEntry->data.data, 0u,
            (Ascii_ValueType*) Bluemod_FrameBuffer, frameBufferPos,
            cmdEntry->data.length));

      }

      /* set stopwatch */
      device->watch = Stopwatch_GetCurrentValue();

      /* send command frame */
      Bluemod_SendMuxCmd(deviceNumber, Bluemod_FrameBuffer, frameBufferPos);

      /* change to receive state */
      device->sendRecvState = Bluemod_SendReceievStateReceiving;
    }
    else
    {
      /* remove command from queue */
      Bluemod_DequeueCmd(deviceNumber);
    }
  }
  else
  {
    /* return to async receive state */
    device->sendRecvState = Bluemod_SendReceievStateReceivingAsync;
  }
}

static void Bluemod_ReadInput(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Ascii_QueueType* recQueue = &device->receiveQueue;
  Ascii_LengthType ret = 0u;
  Ascii_LengthType space = 0u;

  /* Calculate size of readable data */
  space = (Ascii_LengthType) (recQueue->size - recQueue->wrIdx);
  if (recQueue->space < space)
  {
    space = recQueue->space;
  }

  /* read data from bluemod */
  Bluemod_GetFromIf(deviceNumber,
    (Bluemod_IfDataType* )(recQueue->entries + recQueue->wrIdx), space, ret);

  /* update queue controls */
  recQueue->space = (Ascii_LengthType) (recQueue->space - ret);
  recQueue->wrIdx =
    (Ascii_LengthType) ((recQueue->wrIdx + ret) % recQueue->size);
}

static void Bluemod_ParseInputAt(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Ascii_QueueType* recQueue = &device->receiveQueue;
  Ascii_LengthType offset = 0u;
  Ascii_OperationResult result;

  /* initialize offset */
  offset = 0u;

  /* check if start sequence of at response was received*/
  result = Ascii_FindDelimiter(recQueue, &offset,
    (Ascii_ValueType*) Bluemod_AtDelim);

  if (result == Ascii_OperationResultSucceeded)
  {
    /* check if ok was received */
    result = Ascii_FindDelimiter(recQueue, &offset, Bluemod_ResponseOk);
    if (result == Ascii_OperationResultSucceeded)
    {
      /* check if end sequence of at response was received*/
      result = Ascii_FindDelimiter(recQueue, &offset,
        (Ascii_ValueType*) Bluemod_AtDelim);
      if (result == Ascii_OperationResultSucceeded)
      {
        /* remove current cmd from cmd queue */
        Bluemod_DequeueCmd(deviceNumber);

        /* remove data from queue */
        device->receiveQueue.rdIdx =
          (Ascii_LengthType) ((device->receiveQueue.rdIdx + offset)
            % device->receiveQueue.size);
        device->receiveQueue.space =
          (Ascii_LengthType) (device->receiveQueue.space + offset);

        /* return to send state */
        device->sendRecvState = Bluemod_SendReceievStateSending;
      }
    }
  }
}

static void Bluemod_ParseInputMux(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Ascii_QueueType* recQueue = &device->receiveQueue;
  Ascii_OperationResult result;
  Bluemod_LengthType length = 0u;
  Bluemod_LengthType offset = 0u;
  Bluemod_ChannelType channel = 0u;

  /* initialize offset */
  device->receivedData->offset = 0u;
  /* check if start sequence of mux response was received*/
  result = Ascii_FindDelimiter(recQueue, &device->receivedData->offset,
    (Ascii_ValueType*) Bluemod_MuxStart);

  /* set local offset */
  offset = device->receivedData->offset;

  if (result == Ascii_OperationResultSucceeded)
  {
    /* check if channel of the response was received*/
    result = Ascii_CheckLength(recQueue, &offset, sizeof(Bluemod_ChannelType));

    if (result == Ascii_OperationResultSucceeded)
    {
      /* set channel */
      channel =
        (Bluemod_ChannelType) recQueue->entries[(Ascii_LengthType) (recQueue->rdIdx
          + offset - 1)];
      /* check if length of the response was received*/
      result = Ascii_CheckLength(recQueue, &offset, sizeof(Bluemod_LengthType));

      if (result == Ascii_OperationResultSucceeded)
      {
        /* set length */
        length =
          (Bluemod_LengthType) recQueue->entries[(Ascii_LengthType) ((recQueue->rdIdx
            + offset - 1) % recQueue->size)];

        /* check if data were received*/
        result = Ascii_CheckLength(recQueue, &offset, length);

        if (result == Ascii_OperationResultSucceeded)
        {
          /* set state */
          device->receivedData->recState = Bluemod_ReceivedStateComplete;

          /* call channel specific data received function */
          Bluemod_DataReceived(deviceNumber, channel);

          /* clear buffer */
          Bluemod_ClearLastFrame(deviceNumber);
        }
      }
    }
  }
}

static void Bluemod_DataReceivedAt(const uint8_t deviceNumber,
                                   const uint8_t channelNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Ascii_QueueType* recQueue = &device->receiveQueue;
  Ascii_OperationResult result;
  Ascii_LengthType offset;

  /* set offset */
  offset = device->receivedData->offset;

  /* check if an ring event was received */
  result = Ascii_FindDelimiter(recQueue, &offset, Bluemod_ResponseRing);
  if (result == Ascii_OperationResultSucceeded)
  {
    /* check if ring notification is set */
    if (device->ring != NULL_PTR)
      device->ring(deviceNumber);
    return;
  }

  /* set offset */
  offset = device->receivedData->offset;

  /* check if an connect event was received */
  result = Ascii_FindDelimiter(recQueue, &offset, Bluemod_ResponseConnect);
  if (result == Ascii_OperationResultSucceeded)
  {
    /* check if connect notification is set */
    if (device->connect != NULL_PTR)
      device->connect(deviceNumber);
    return;
  }

  /* set offset */
  offset = device->receivedData->offset;

  /* check if an no carrier event was received */
  result = Ascii_FindDelimiter(recQueue, &offset, Bluemod_ResponseNoCarrier);
  if (result == Ascii_OperationResultSucceeded)
  {
    /* check if no carrier notification is set */
    if (device->noCarrier != NULL_PTR)
      device->noCarrier(deviceNumber);
    return;
  }

  /* set offset */
  offset = device->receivedData->offset;

  /* check if an OK event was received */
  result = Ascii_FindDelimiter(recQueue, &offset, Bluemod_ResponseOk);
  if (result == Ascii_OperationResultSucceeded)
  {
    /* remove current cmd from cmd queue */
    Bluemod_DequeueCmd(deviceNumber);

    /* continue in sending state */
    device->sendRecvState = Bluemod_SendReceievStateSending;

    return;
  }
}

static void Bluemod_DataReceivedSpp(const uint8_t deviceNumber,
                                    const uint8_t channelNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];

  /* check if datareceived notification is set */
  if (device->dataReceivedSpp != NULL_PTR)
    device->dataReceivedSpp(deviceNumber, channelNumber);
}

static void Bluemod_DataReceivedBle(const uint8_t deviceNumber,
                                    const uint8_t channelNumber)
{
  /* TODO: future feature */
}

static void Bluemod_SendAt(const uint8_t deviceNumber,
                           const Bluemod_DataType* data,
                           const Bluemod_LengthType length)
{
  /* local variables */
  Bluemod_IfLengthType ret = 0u;

  /* send frame */
  Bluemod_SendToIf(deviceNumber, data, length, ret);

  /* send postfix */
  Bluemod_SendToIf(deviceNumber, Bluemod_PostFix, Bluemod_PostFixLen, ret);

  UNUSED_PARAM(ret);
}

static void Bluemod_SendMuxData(const uint8_t deviceNumber,
                                const Bluemod_DataType channel,
                                const Bluemod_DataType* data,
                                const Bluemod_LengthType length)
{
  /* local variables */
  Bluemod_IfLengthType ret = 0u;

  /* send start */
  Bluemod_SendToIf(deviceNumber, Bluemod_MuxStart, Bluemod_MuxStartLen, ret);

  /* send channel */
  Bluemod_SendToIf(deviceNumber, &channel, sizeof(Bluemod_DataType), ret);

  /* send data length */
  Bluemod_SendToIf(deviceNumber, &length, sizeof(Bluemod_LengthType), ret);

  if (length > 0u)
  {
    /* send inner at data */
    Bluemod_SendToIf(deviceNumber, data, length, ret);
  }

  UNUSED_PARAM(ret);
}

static void Bluemod_SendMuxCmd(const uint8_t deviceNumber,
                               const Bluemod_DataType* data,
                               const Bluemod_LengthType length)
{
  /* local variables */
  Bluemod_IfLengthType ret = 0u;

  Bluemod_IfLengthType completeLength = (Bluemod_IfLengthType) (length
    + Bluemod_PostFixLen);

  /* send start */
  Bluemod_SendToIf(deviceNumber, Bluemod_MuxStart, Bluemod_MuxStartLen, ret);

  /* send channel */
  Bluemod_SendToIf(deviceNumber, Bluemod_MuxAtChannel, Bluemod_MuxAtChannelLen,
    ret);

  /* send data length */
  Bluemod_SendToIf(deviceNumber, &completeLength, sizeof(Bluemod_LengthType),
    ret);

  /* send inner at data */
  Bluemod_SendAt(deviceNumber, data, length);

  UNUSED_PARAM(ret);
}

static void Bluemod_EnqueueRetryCmd(const uint8_t deviceNumber,
                                    const Bluemod_CmdType cmd,
                                    const Bluemod_CmdDataType data)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Bluemod_CmdQueueType* queue = &device->cmdQueue;
  Bluemod_CmdQueueEntryType* cmdEntry = &queue->commands[queue->writeIdx];

  /* check if there is an empty space in the queue */
  if (queue->space > 0u)
  {
    /* set cmd */
    cmdEntry->cmd = cmd;
    cmdEntry->data.data = data.data;
    cmdEntry->data.length = data.length;
    cmdEntry->tryCount = 0u;

    /* decrease space count */
    queue->space--;

    /* increase write index */
    queue->writeIdx++;

    /* reset writeindex */
    if (queue->writeIdx >= queue->size)
      queue->writeIdx = 0u;

    /* set send state */
    device->sendRecvState = Bluemod_SendReceievStateSending;
  }
}

static void Bluemod_EnqueueSingleCmd(const uint8_t deviceNumber,
                                     const Bluemod_CmdType cmd,
                                     const Bluemod_CmdDataType data)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Bluemod_CmdQueueType* queue = &device->cmdQueue;
  Bluemod_CmdQueueEntryType* cmdEntry = &queue->commands[queue->writeIdx];

  /* check if there is an empty space in the queue */
  if (queue->space > 0u)
  {
    /* set cmd */
    cmdEntry->cmd = cmd;
    cmdEntry->data.data = data.data;
    cmdEntry->data.length = data.length;
    /* set trycount to max trycount -1 */
    cmdEntry->tryCount = (uint8_t) (device->bluetoothSettings.retryCount - 1);

    /* decrease space count */
    queue->space--;

    /* increase write index */
    queue->writeIdx++;

    /* reset writeindex */
    if (queue->writeIdx >= queue->size)
      queue->writeIdx = 0u;

    /* set send state */
    device->sendRecvState = Bluemod_SendReceievStateSending;
  }
}

static void Bluemod_ClearLastFrame(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device;
  Bluemod_LengthType length;

  /* initialize local variables */
  device = &Bluemod_userConifg->deviceConfigs[deviceNumber];

  /* check if data were completely received */
  if (device->receivedData->recState == Bluemod_ReceivedStateComplete)
  {
    /* get length from queue */
    length =
      (Bluemod_LengthType) device->receiveQueue.entries[device->receiveQueue.rdIdx
        + (Ascii_LengthType)(device->receivedData->offset + Bluemod_ReceivedLengthPos)];

    /* remove data from receive queue */
    device->receiveQueue.rdIdx = (Ascii_LengthType) ((device->receiveQueue.rdIdx
      + (Ascii_LengthType)(device->receivedData->offset + sizeof(Bluemod_ChannelType)
      + sizeof(Bluemod_LengthType) + length)) % device->receiveQueue.size);

    device->receiveQueue.space = (Ascii_LengthType) (device->receiveQueue.space
      + (Ascii_LengthType)(device->receivedData->offset + sizeof(Bluemod_ChannelType)
      + sizeof(Bluemod_LengthType) + length));

    /* set state */
    device->receivedData->recState = Bluemod_ReceivedStateWaiting;
  }
}

static void Bluemod_DequeueCmd(const uint8_t deviceNumber)
{
  /* local variables */
  Bluemod_DeviceConfigType* device =
    &Bluemod_userConifg->deviceConfigs[deviceNumber];
  Bluemod_CmdQueueType* queue = &device->cmdQueue;

  /* check if there is an cmd to dequeue */
  if (queue->space > 0u)
  {
    /* increase space count */
    queue->space++;

    /* increase read index */
    queue->readIdx++;

    /* reset read index */
    if (queue->readIdx >= queue->size)
      queue->readIdx = 0u;
  }
}
