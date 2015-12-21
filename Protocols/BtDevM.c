/**
 ******************************************************************************
 * @file    BtDevM.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    22.04.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "Protocols/BtDevM.h"
#include "Services/Stopwatch.h"
#include "Services/SysState.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

#define BtDevM_ReadBufferSize       128u
#define BtDevM_SendBufferSize       128u

#define BtDevM_RingChannelStart     21u
#define BtDevM_RingChannelEnd       25u

#define BtDevM_ConnectChannelStart     26u
#define BtDevM_ConnectChannelEnd     28u

#define BtDevM_NoCarrierChannelStart  21u
#define BtDevM_NoCarrierChannelEnd  23u

#define BtDevM_ChannelPrefix           "0x"
#define BtDevM_ChannelPrefixLength     2u
#define BtDevM_ChannelLength           2u

/**
 * Makro for getting a new device handle an increase static counter
 */
#define BtDevM_GetNewDevHandle(handle)    \
  (handle) = BtDevM_NextHandle++           

#define BtDevM_InvalidHandle ((BtDevM_HandleType)-1)

/****************************************************************
 * Function declarations
 ***************************************************************/

static BOOL BtDevM_GetHandleInList(BtDevM_HandleType const handle,
                                   BtDevM_DeviceEntryType ** entry);

static void BtDevM_SendDisconnect(uint8_t const btDevNumber,
                                  BtDevM_BluemodIfChannelType const channel);

static uint8_t BtDevM_CastHexNumber(
  BtDevM_BluemodIfCmdDataValueType const * buffer,
  BtDevM_BluemodIfCmdDataLengthType const offset);

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Current configuration
 */
static BtDevM_ConfigType* BtDevM_userConifg = NULL_PTR;

static BtDevM_BluemodIfCmdDataValueType BtDevM_ReadBuffer[BtDevM_ReadBufferSize];
static BtDevM_BluemodIfCmdDataValueType BtDevM_SendBuffer[BtDevM_SendBufferSize];

static BtDevM_BluemodIfCmdDataType BtDevM_SendCmd =
{
  BtDevM_SendBuffer,
  BtDevM_SendBufferSize };

/**
 * next handle for new device
 */
static BtDevM_HandleType BtDevM_NextHandle = 0;

static BtDevM_HandleType BtDevM_SppHandle = BtDevM_InvalidHandle;

/****************************************************************
 * User functions
 ***************************************************************/

void BtDevM_Init(BtDevM_ConfigType* config)
{
  /* check user config and parameter */
  if ((BtDevM_userConifg == NULL_PTR) && (config != NULL_PTR))
  {
    /* set user config */
    BtDevM_userConifg = config;
  }
}

void BtDevM_Main(void)
{
  /* local variables */
  BtDevM_DeviceEntryType* entry;
  uint8_t i;

  /* check user config */
  if (BtDevM_userConifg != NULL_PTR)
  {
    /* check if there are connected clients */
    if (BtDevM_userConifg->devices.number > 0u)
    {
      /* iterate through all clients */
      i = 0u;
      entry = BtDevM_userConifg->devices.devices;
      for (; i < BtDevM_userConifg->devices.size; i++, entry++)
      {
        /* check state of entry */
        if (entry->state == BtDevM_DevStateEntryConnected)
        {
          /* check timeout */
          if (Stopwatch_ReturnDifference(entry->timer,
            Stopwatch_GetCurrentValue()) > BtDevM_userConifg->connectionTimeout)
          {
            /* remove entry */
            entry->state = BtDevM_DevStateEntryUnused;
            BtDevM_userConifg->devices.number--;

            /* check notification */
            if (BtDevM_userConifg->clientDisconnected != NULL_PTR)
              /* notify disconnected client */
              BtDevM_userConifg->clientDisconnected(entry->deviceHandle);

            /* send disconnect to bluemod */
            BtDevM_SendDisconnect(entry->bluemodDevNumber,
              entry->bluemodChannel);
          }
        }
      }
    }
  }
}

void BtDevM_SendError(BtDevM_HandleType const handle,
                      BtDevM_MessageIdType const * messageId,
                      BtDevM_ErrorDataType const * errorData,
                      BtDevM_LengthType const length)
{
  /* local variables */
  BtDevM_DeviceEntryType* entry;

  /* Initialize local variables */
  entry = NULL_PTR;

  /* check configuration and if handle is within list*/
  if (BtDevM_userConifg != NULL_PTR && BtDevM_GetHandleInList(handle, &entry))
  {
    /* check interface of entry */
    switch (entry->interface)
    {
    case BtDevM_DevEntryInterfaceSpp:
      /* check function pointer */
      if (BtDevM_userConifg->spp.sendError != NULL_PTR)
        BtDevM_userConifg->spp.sendError(messageId, errorData, length);
      break;

    case BtDevM_DevEntryInterfaceBle:
      /* TODO: future feature */
      break;

    default:
      break;
    }
  }
}

void BtDevM_SendData(BtDevM_HandleType const handle,
                     BtDevM_ValueIdType const * id,
                     BtDevM_ValueType const * data)
{
  /* local variables */
  BtDevM_DeviceEntryType* entry;

  /* Initialize local variables */
  entry = NULL_PTR;

  /* check configuration and if handle is within list*/
  if (BtDevM_userConifg != NULL_PTR && BtDevM_GetHandleInList(handle, &entry))
  {
    /* check interface of entry */
    switch (entry->interface)
    {
    case BtDevM_DevEntryInterfaceSpp:
      /* check function pointer */
      if (BtDevM_userConifg->spp.sendData != NULL_PTR)
        BtDevM_userConifg->spp.sendData(id, data);
      break;

    case BtDevM_DevEntryInterfaceBle:
      /* TODO: future feature */
      break;

    default:
      break;
    }
  }
}

void BtDevM_SendConfiguration(BtDevM_HandleType const handle,
                              BtDevM_MessageIdType const * messageId,
                              BtDevM_ConfigDataType const * configData,
                              BtDevM_ConfigLengthType const length)
{
  /* local variables */
  BtDevM_DeviceEntryType* entry;

  /* Initialize local variables */
  entry = NULL_PTR;

  /* check configuration and if handle is within list*/
  if (BtDevM_userConifg != NULL_PTR && BtDevM_GetHandleInList(handle, &entry))
  {
    /* check interface of entry */
    switch (entry->interface)
    {
    case BtDevM_DevEntryInterfaceSpp:
      /* check function pointer */
      if (BtDevM_userConifg->spp.sendConfig != NULL_PTR)
        BtDevM_userConifg->spp.sendConfig(messageId, configData, length);
      break;

    case BtDevM_DevEntryInterfaceBle:
      /* TODO: future feature */
      break;

    default:
      break;
    }
  }
}

void BtDevM_DisconnectDevice(BtDevM_HandleType const handle)
{
  /* local variables */
  BtDevM_DeviceEntryType* entry;

  /* Initialize local variables */
  entry = NULL_PTR;
  /* check configuration and if handle is in list */
  if (BtDevM_userConifg != NULL_PTR && BtDevM_GetHandleInList(handle, &entry))
  {
    /* remove entry from list */
    BtDevM_userConifg->devices.number--;

    /* set state of entry */
    entry->state = BtDevM_DevStateEntryUnused;

    /* send disconnect to bluemod */
    BtDevM_SendDisconnect(entry->bluemodDevNumber, entry->bluemodChannel);
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void BtDevM_OnRing(const uint8_t btDeviceNumber)
{
  /* local variables */
  BtDevM_BluemodIfChannelType recChannel;
  BtDevM_BluemodIfLengthType length = 0u;
  BtDevM_BluemodIfLengthType i = 0u;
  BtDevM_BluemodIfLengthType j = 0u;
  BtDevM_DeviceListType* devices;

  /* check user config */
  if (BtDevM_userConifg != NULL_PTR)
  {
    /* initialize variables */
    devices = &BtDevM_userConifg->devices;

    /* get data from bluemod for channel id */
    length = BtDevM_userConifg->bluemod.getData(btDeviceNumber, &recChannel,
      (BtDevM_BluemodIfDataType*) BtDevM_ReadBuffer,
      BtDevM_SendBufferSize);

    /* check length of data and if there is space in list */
    if (length >= BtDevM_RingChannelEnd && (devices->number < devices->size))
    {
      /* get channel */
      i = BtDevM_RingChannelStart;
      j = 0;
      for (; i < BtDevM_RingChannelEnd; i++)
        BtDevM_SendCmd.data[j++] =
          (BtDevM_BluemodIfCmdDataValueType) (BtDevM_ReadBuffer[i]);
      BtDevM_SendCmd.length = j;

      /* check function */
      if (BtDevM_userConifg->bluemod.acceptConnection != NULL_PTR)
        /* accept connection of client */
        BtDevM_userConifg->bluemod.acceptConnection(btDeviceNumber,
          BtDevM_SendCmd);
    }
  }
}

void BtDevM_OnConnect(const uint8_t btDeviceNumber)
{
  /* local variables */
  BtDevM_BluemodIfChannelType recChannel;
  BtDevM_BluemodIfLengthType length = 0u;
  BtDevM_DeviceListType* devices;
  BtDevM_DeviceEntryType* entry;
  uint8_t i;

  /* check user config */
  if (BtDevM_userConifg != NULL_PTR)
  {
    /* initialize variables */
    devices = &BtDevM_userConifg->devices;

    /* get data from bluemod for channel id */
    length = BtDevM_userConifg->bluemod.getData(btDeviceNumber, &recChannel,
      (BtDevM_BluemodIfDataType*) BtDevM_ReadBuffer,
      BtDevM_SendBufferSize);

    /* check length of data and if there is space in list */
    if (length >= BtDevM_ConnectChannelEnd && devices->number < devices->size)
    {
      
      /* check if this was the first device */
      if (devices->number == 0u)
        /* set bluetooth state */
        SysState_SetState(BtDevM_userConifg->sysStateChannel, SysState_StateOperating);

      /* get next unsused entry*/
      i = 0u;
      entry = devices->devices;
      while (i < devices->size && entry->state != BtDevM_DevStateEntryUnused)
      {
        entry++;
        i++;
      }

      /* add device */

      /* set state */
      entry->state = BtDevM_DevStateEntryConnected;

      /* set channel */
      entry->bluemodChannel = BtDevM_CastHexNumber(BtDevM_ReadBuffer,
      BtDevM_ConnectChannelStart);

      /* set dev number */
      entry->bluemodDevNumber = btDeviceNumber;

      /* set device handle */
      BtDevM_GetNewDevHandle(entry->deviceHandle);

      /* set device interface */
      entry->interface = BtDevM_DevEntryInterfaceSpp;

      /* set connection timer */
      entry->timer = Stopwatch_GetCurrentValue();

      /* set global spp handle */
      BtDevM_SppHandle = entry->deviceHandle;

      /* increase number of devices */
      devices->number++;

      /* check if notification is set */
      if (BtDevM_userConifg->newClientConnected != NULL_PTR)
        BtDevM_userConifg->newClientConnected(entry->deviceHandle);
    }
  }
}

void BtDevM_OnNoCarrier(const uint8_t btDeviceNumber)
{
  /* local variables */
  BtDevM_BluemodIfChannelType recChannel;
  BtDevM_BluemodIfLengthType length = 0u;
  BtDevM_DeviceListType* devices;
  BtDevM_BluemodIfChannelType channel;
  uint8_t i;

  /* check user config */
  if (BtDevM_userConifg != NULL_PTR)
  {
    /* initialize variables */
    devices = &BtDevM_userConifg->devices;

    /* get data from bluemod for channel id */
    length = BtDevM_userConifg->bluemod.getData(btDeviceNumber, &recChannel,
      (BtDevM_BluemodIfDataType*) BtDevM_ReadBuffer,
      BtDevM_SendBufferSize);

    /* check length of data */
    if (length >= BtDevM_NoCarrierChannelEnd)
    {
      /* get channel from data */
      channel = BtDevM_CastHexNumber(BtDevM_ReadBuffer,
      BtDevM_NoCarrierChannelStart);

      /* check if the device is within list */
      for (i = 0u; i < devices->size; i++)
      {
        if ((devices->devices[i].bluemodDevNumber == btDeviceNumber)
          && (devices->devices[i].state == BtDevM_DevStateEntryConnected)
          && (devices->devices[i].bluemodChannel == channel))
        {
          /* remove device */
          devices->number--;
          devices->devices[i].state = BtDevM_DevStateEntryUnused;

          /* check if this device was spp */
          if (devices->devices[i].deviceHandle == BtDevM_SppHandle)
            BtDevM_SppHandle = BtDevM_InvalidHandle;

          /* check if notification is set */
          if (BtDevM_userConifg->clientDisconnected != NULL_PTR)
            BtDevM_userConifg->clientDisconnected(
              devices->devices[i].deviceHandle);
          
          /* check if this was the last device */
          if (devices->number == 0u)
            /* set bluetooth state */
            SysState_SetState(BtDevM_userConifg->sysStateChannel, SysState_StateReady);
          
          break;
        }
      }
    }
  }
}

BOOL BtDevM_OnSppSubscribe(const BtDevM_MessageIdType messageId,
                           const BtDevM_ValueIdType valueId)
{
  /* check user config, spp handle and callback */
  if ((BtDevM_userConifg != NULL_PTR)
    && (BtDevM_userConifg->subscribe != NULL_PTR)
    && (BtDevM_SppHandle != BtDevM_InvalidHandle))
    /* forward notification with spp handle */
    return BtDevM_userConifg->subscribe(BtDevM_SppHandle, messageId, valueId);
  else
    return FALSE;
}

BOOL BtDevM_OnSppUnsubscribe(const BtDevM_MessageIdType messageId,
                             const BtDevM_ValueIdType valueId)
{
  /* check user config, spp handle and callback */
  if ((BtDevM_userConifg != NULL_PTR)
    && (BtDevM_userConifg->unsubscribe != NULL_PTR)
    && (BtDevM_SppHandle != BtDevM_InvalidHandle))
    /* forward notification with spp handle */
    return BtDevM_userConifg->unsubscribe(BtDevM_SppHandle, messageId, valueId);
  else
    return FALSE;
}

BOOL BtDevM_OnSppCommandReceived(const BtDevM_ValueIdType valueId,
                                 const BtDevM_ValueType data)
{
  /* check user config, spp handle and callback */
  if ((BtDevM_userConifg != NULL_PTR)
    && (BtDevM_userConifg->commandReceived != NULL_PTR)
    && (BtDevM_SppHandle != BtDevM_InvalidHandle))
    /* forward notification with spp handle */
    return BtDevM_userConifg->commandReceived(BtDevM_SppHandle, valueId, data);
  else
    return FALSE;
}

BOOL BtDevM_OnSppConfigRequested(const BtDevM_MessageIdType messageId)
{
  /* check user config, spp handle and callback */
  if ((BtDevM_userConifg != NULL_PTR)
    && (BtDevM_userConifg->subscribe != NULL_PTR)
    && (BtDevM_SppHandle != BtDevM_InvalidHandle))
    /* forward notification with spp handle */
    return BtDevM_userConifg->configRequested(BtDevM_SppHandle, messageId);
  else
    return FALSE;
}

BOOL BtDevM_OnSppIsAlive(const BtDevM_MessageIdType messageId)
{
  /* local variables */
  BtDevM_DeviceEntryType* entry;

  /* get spp device */
  if (BtDevM_GetHandleInList(BtDevM_SppHandle, &entry))
  {
    /* reset timer for spp device */
    entry->timer = Stopwatch_GetCurrentValue();
  }

  /* check user config, spp handle and callback */
  if ((BtDevM_userConifg != NULL_PTR)
    && (BtDevM_userConifg->isAlive != NULL_PTR)
    && (BtDevM_SppHandle != BtDevM_InvalidHandle))
    /* forward notification with spp handle */
    return BtDevM_userConifg->isAlive(BtDevM_SppHandle, messageId);
  else
    return FALSE;
}

/****************************************************************
 * Static functions
 ***************************************************************/

static BOOL BtDevM_GetHandleInList(BtDevM_HandleType const handle,
                                   BtDevM_DeviceEntryType ** entry)
{
  /* local variables */
  BOOL result;
  BtDevM_DeviceEntryType* tmpEntry;
  uint8_t i;

  /* check if handle is within list */
  result = FALSE;
  i = 0u;
  tmpEntry = BtDevM_userConifg->devices.devices;
  for (;
    i < BtDevM_userConifg->devices.size && tmpEntry->deviceHandle != handle;
    i++, tmpEntry++)
    ;

  if (i < BtDevM_userConifg->devices.size)
  {
    *entry = tmpEntry;
    result = TRUE;
  }

  return result;
}

static void BtDevM_SendDisconnect(uint8_t const btDevNumber,
                                  BtDevM_BluemodIfChannelType const channel)
{
  /* set data cmd data for closing connection */
  Ascii_CopyData(BtDevM_ChannelPrefix, 0u, BtDevM_SendBuffer, 0u,
  BtDevM_ChannelPrefixLength);
  Ascii_DecToStr(
    (Ascii_ValueType*) (BtDevM_SendBuffer + BtDevM_ChannelPrefixLength),
    BtDevM_SendBufferSize - BtDevM_ChannelPrefixLength, (int32_t) channel,
    Ascii_NumberBaseHex, BtDevM_ChannelLength);

  /* set length of cmd data */
  BtDevM_SendCmd.length =
    (BtDevM_BluemodIfCmdDataLengthType) (BtDevM_ChannelPrefixLength
      + BtDevM_ChannelLength);

  /* check function */
  if (BtDevM_userConifg->bluemod.closeConnection != NULL_PTR)
    /* send disconnect command */
    BtDevM_userConifg->bluemod.closeConnection(btDevNumber, BtDevM_SendCmd);
}

static uint8_t BtDevM_CastHexNumber(
  BtDevM_BluemodIfCmdDataValueType const * buffer,
  BtDevM_BluemodIfCmdDataLengthType const offset)
{
  /* local variables */
  BtDevM_BluemodIfCmdDataValueType first;
  BtDevM_BluemodIfCmdDataValueType second;

  /* initialize local variables */
  first = buffer[offset];
  second = buffer[offset + 1];

  /* check ranges */
  if (first >= '0' && first <= '9')
    first = (BtDevM_BluemodIfCmdDataValueType) (first - '0');
  else if (first >= 'A' && first <= 'F')
    first = (BtDevM_BluemodIfCmdDataValueType) (first - 'A' + 0xA);
  else if (first >= 'a' && first <= 'f')
    first = (BtDevM_BluemodIfCmdDataValueType) (first - 'a' + 0xA);
  else
    first = 0;

  if (second >= '0' && second <= '9')
    second = (BtDevM_BluemodIfCmdDataValueType) (second - '0');
  else if (first >= 'A' && second <= 'F')
    second = (BtDevM_BluemodIfCmdDataValueType) (second - 'A' + 0xA);
  else if (second >= 'a' && second <= 'f')
    second = (BtDevM_BluemodIfCmdDataValueType) (second - 'a' + 0xA);
  else
    second = 0;

  return (uint8_t) ((first << 8) | second);
}
