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

#ifndef BTDEVM_H
#define BTDEVM_H

#include "Services/StdDef.h"
#include "Ascii.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

typedef uint8_t BtDevM_HandleType;
typedef uint8_t BtDevM_ErrorDataType;
typedef uint8_t BtDevM_LengthType;
typedef uint32_t BtDevM_ConfigLengthType;
typedef int32_t BtDevM_ValueType;

/**
 * Type for the 32 bit tupel of motorgroup id and sensor/actor id
 */
typedef struct
{
  /**
   * id of motor group
   */
  uint16_t motorGroup;

  /**
   * id of specific sensor / actor
   */
  uint16_t id;
} BtDevM_ValueIdType;

/**
 * type for the 32 message id
 */
typedef uint32_t BtDevM_MessageIdType;

/**
 * type for configdata
 */
typedef struct
{
  uint16_t roomId;
  uint16_t motorId;
  uint16_t sensorId;
} BtDevM_ConfigDataType;

/**
 * Data value type for command data
 */
typedef Ascii_ValueType BtDevM_BluemodIfCmdDataValueType;

/**
 * Data type for command data
 */
typedef uint8_t BtDevM_BluemodIfCmdDataLengthType;

/**
 * data types of the bluemodif
 */
typedef uint8_t BtDevM_BluemodIfChannelType;
typedef uint8_t BtDevM_BluemodIfDataType;
typedef uint8_t BtDevM_BluemodIfLengthType;

/**
 * enum for state of entries in device list
 */
typedef enum
{
  BtDevM_DevStateEntryUnused = 0u,
  BtDevM_DevStateEntryConnected
} BtDevM_DevEntryStateType;

/**
 * enum for interface of devices
 */
typedef enum
{
  BtDevM_DevEntryInterfaceSpp = 0u,
  BtDevM_DevEntryInterfaceBle
} BtDevM_DevEntryInterfaceType;

/**
 * Entry for the list of clients
 */
typedef struct
{
  /**
   * device number of underlying bluemod
   */
  uint8_t bluemodDevNumber;
  
  /**
   * channel number of used transport channel within bluemod
   */
  BtDevM_BluemodIfChannelType bluemodChannel;
  
  /**
   * unique handle defining a connection with a device
   */
  BtDevM_HandleType deviceHandle;
  
  /**
   * state of this entry/client
   */
  BtDevM_DevEntryStateType state;
  
  /**
   * interfacetype of client for dispatching to spp/ble
   */
  BtDevM_DevEntryInterfaceType interface;
  
  /**
   * timer value for connection timeout
   */
  uint32_t timer;
} BtDevM_DeviceEntryType;

typedef struct
{
  /**
   * entries for all devices
   */
  BtDevM_DeviceEntryType* devices;

  /**
   * number of current added devices
   */
  uint8_t number;

  /**
   * size of list
   */
  const uint8_t size;

} BtDevM_DeviceListType;

/**
 * type for the get data method of the bluemodif
 */
typedef BtDevM_BluemodIfLengthType (*BtDevM_BluemodIfGetDataType)(
  uint8_t const deviceNumber, BtDevM_BluemodIfChannelType* channel,
  BtDevM_BluemodIfDataType* data, BtDevM_BluemodIfLengthType const length);

/**
 * type for bluemod cmd data
 */
typedef struct
{
  /**
   * Data values
   */
  BtDevM_BluemodIfCmdDataValueType* data;

  /**
   * Length of data
   */
  BtDevM_BluemodIfCmdDataLengthType length;

} BtDevM_BluemodIfCmdDataType;

/**
 * notification type for accept connection function
 */
typedef void (*BtDevM_BluemodIfAcceptConnectionType)(
  uint8_t const deviceNumber, BtDevM_BluemodIfCmdDataType cmdData);

/**
 * notification type for close connection function
 */
typedef void (*BtDevM_BluemodIfCloseConnectionType)(
  uint8_t const deviceNumber, BtDevM_BluemodIfCmdDataType cmdData);

/**
 * function type for send error
 */
typedef void (*BtDevM_SendErrorType)(BtDevM_MessageIdType const * messageId,
                                     BtDevM_ErrorDataType const * errorData,
                                     BtDevM_LengthType const length);

/**
 * function type for send data
 */
typedef void (*BtDevM_SendDataType)(BtDevM_ValueIdType const * id,
                                    BtDevM_ValueType const * data);

/**
 * function type for send configuration
 */
typedef void (*BtDevM_SendConfigType)(BtDevM_MessageIdType const * messageId,
                                      BtDevM_ConfigDataType const * configData,
                                      BtDevM_ConfigLengthType const length);

/**
 * notification type for subscribe data callback
 */
typedef BOOL (*BtDevM_SubscribeNotifyType)(const BtDevM_HandleType handle,
                                           const BtDevM_MessageIdType messageId,
                                           const BtDevM_ValueIdType valueId);

/**
 * notfication type for command received callback
 */
typedef BOOL (*BtDevM_CommandReceivedType)(const BtDevM_HandleType handle,
                                           const BtDevM_ValueIdType valueId,
                                           const BtDevM_ValueType data);

/**
 * notfication type for command received callback
 */
typedef BOOL (*BtDevM_ConfigRequestedType)(
  const BtDevM_HandleType handle, const BtDevM_MessageIdType messageId);

/**
 * notfication type for command received callback
 */
typedef BOOL (*BtDevM_IsAliveType)(const BtDevM_HandleType handle,
                                   const BtDevM_MessageIdType messageId);

/**
 * interface functions for bluemod
 */
typedef struct
{
  /**
   * get data from bluemod
   */
  BtDevM_BluemodIfGetDataType getData;

  /**
   * acceptConnection function pointer
   */
  BtDevM_BluemodIfAcceptConnectionType acceptConnection;

  /**
   * closeConnection function pointer
   */
  BtDevM_BluemodIfCloseConnectionType closeConnection;

} BtDevM_BluemodIftype;

/**
 * Interface functions for ble
 */
typedef struct
{
  BtDevM_SendErrorType sendError;
  BtDevM_SendDataType sendData;
  BtDevM_SendConfigType sendConfig;
} BtDevM_SppIfType;

/**
 * Interface functions for ble
 */
typedef struct
{
  uint8_t test;
  /* TODO future feature */
} BtDevM_BleIfType;

typedef void (*BtDevM_NotificationType)(BtDevM_HandleType const handle);

/**
 * Configuration type for bt device manager
 */
typedef struct
{
  /**
   * list of all connected devices
   */
  BtDevM_DeviceListType devices;

  /**
   * timeout for lost connection
   */
  const uint32_t connectionTimeout;
  
  /**
   * function pointer for the NewClientConnected Callback
   */
  BtDevM_NotificationType newClientConnected;

  /**
   * function pointer for the clientDisconnected Callback
   */
  BtDevM_NotificationType clientDisconnected;

  BtDevM_SubscribeNotifyType subscribe;
  BtDevM_SubscribeNotifyType unsubscribe;
  BtDevM_CommandReceivedType commandReceived;
  BtDevM_ConfigRequestedType configRequested;
  BtDevM_IsAliveType isAlive;

  /**
   * sysstate bluetooth channel
   */
  const uint8_t sysStateChannel;
  
  /**
   * Bluemod interface
   */
  BtDevM_BluemodIftype bluemod;

  /**
   * SPP interface
   */
  BtDevM_SppIfType spp;

  /**
   * ble interface
   */
  BtDevM_BleIfType ble;

} BtDevM_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

extern BtDevM_ConfigType BtDevM_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Init function for bt dev manager
 * @param config pointer to the used config
 */
void BtDevM_Init(BtDevM_ConfigType* config);

/**
 * Main Routine of BtDevM
 */
void BtDevM_Main(void);

/**
 * send error
 * @param handle handle of client device
 * @param messageId message id
 * @param errorData buffer with error data
 * @param length length of error data
 */
void BtDevM_SendError(BtDevM_HandleType const handle,
                      BtDevM_MessageIdType const * messageId,
                      BtDevM_ErrorDataType const * errorData,
                      BtDevM_LengthType const length);

/**
 * send data update
 * @param handle handle of client device
 * @param id id of dataset
 * @param data value of data
 */
void BtDevM_SendData(BtDevM_HandleType const handle,
                     BtDevM_ValueIdType const * id,
                     BtDevM_ValueType const * data);

/**
 * send configuration to client
 * @param handle handle of client device
 * @param messageId message id
 * @param configData buffer with configuration data
 * @param length length of configuration data
 */
void BtDevM_SendConfiguration(BtDevM_HandleType const handle,
                              BtDevM_MessageIdType const * messageId,
                              BtDevM_ConfigDataType const * configData,
                              BtDevM_ConfigLengthType const length);

/**
 * disconnect the defined device
 * @param handle handle of the device which should be disconnected
 */
void BtDevM_DisconnectDevice(BtDevM_HandleType const handle);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void BtDevM_OnRing(const uint8_t btDeviceNumber);
void BtDevM_OnConnect(const uint8_t btDeviceNumber);
void BtDevM_OnNoCarrier(const uint8_t btDeviceNumber);

BOOL BtDevM_OnSppSubscribe(const BtDevM_MessageIdType messageId,
                           const BtDevM_ValueIdType valueId);
BOOL BtDevM_OnSppUnsubscribe(const BtDevM_MessageIdType messageId,
                             const BtDevM_ValueIdType valueId);
BOOL BtDevM_OnSppCommandReceived(const BtDevM_ValueIdType valueId,
                                 const BtDevM_ValueType data);
BOOL BtDevM_OnSppConfigRequested(const BtDevM_MessageIdType messageId);
BOOL BtDevM_OnSppIsAlive(const BtDevM_MessageIdType messageId);

#endif
