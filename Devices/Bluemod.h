/**
 ******************************************************************************
 * @file    Bluemod.c
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    11.03.2015
 * @brief   Abstraction of Stollmann Bluemod + SR
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef BLUEMOD_H
#define BLUEMOD_H

#include "BSP/GpioIf.h"
#include "Services/StdDef.h"
#include "Protocols/Ascii.h"


/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * data type
 */
typedef uint8_t Bluemod_DataType;

typedef uint8_t Bluemod_LengthType;

typedef uint8_t Bluemod_ChannelType;

/**
 * Type definitions for communication inteface
 */
typedef uint8_t Bluemod_IfLengthType;
typedef uint8_t Bluemod_IfDataType;

typedef Bluemod_IfLengthType (*Bluemod_IfPutDataType)(const uint8_t channel,
  const Bluemod_IfDataType* data, const Bluemod_IfLengthType length);
typedef Bluemod_IfLengthType (*Bluemod_IfGetDataType)(const uint8_t channel,
  Bluemod_IfDataType* data, const Bluemod_IfLengthType length);

typedef struct
{
  /**
   * communication if channel number
   */
  const uint8_t ifChannelNumber;

  /**
   * Function pointer to the get data funct of the used interface
   */
  const Bluemod_IfGetDataType ifGetData;
  
  /**
   * Function pointer to the put data funct of the used inteface
   */
  const Bluemod_IfPutDataType ifPutData;
  
} Bluemod_IfType;


/**
 * bluetooth settings
 */
typedef struct
{
  /**
   * Retry count for invalid responses from Bluemod+SR
   */
  const uint8_t retryCount;
  
  /**
   * Pointer to Name of Device (shown over Bluetooth)
   */
  const char* deviceName;
  
  /**
   * length of devicename
   */
  const uint8_t deviceNameLength;
   
} Bluemod_BluetoothSettings;

/**
 * Bluetooth commands
 */
typedef enum {
  Bluemod_CmdGetOK = 0u,
  Bluemod_CmdGetSystemInfo,
  Bluemod_CmdSetLocalEcho,
  Bluemod_CmdAcceptCall,
  Bluemod_CmdSetMux,
  Bluemod_CmdSetName,
  Bluemod_CmdDeleteBond,
  Bluemod_CmdGetOwnAddress,
  Bluemod_CmdCloseConnection,
  Bluemod_CmdReset,
  Bluemod_CmdStatusReg,
  Bluemod_CmdSetBTClass,
  Bluemod_CmdSetEncryption,
  Bluemod_CmdSearchDevices,
  Bluemod_CmdSetPin,
  Bluemod_CmdSetSerialName,
  Bluemod_CmdSetMinAdvertising,
  Bluemod_CmdSetMaxAdvertising,
  Bluemod_CmdSetMinConnection,
  Bluemod_CmdSetMaxConnection,
  Bluemod_CmdSetLERole,
  Bluemod_CmdSetTerminalIO,
  Bluemod_CmdSetAdvData,
  Bluemod_CmdSetAdvertising,
  Bluemod_CmdSetupAdvParameter,
  Bluemod_CmdSetupAdvAttributes,
  Bluemod_CmdSetupDvScanResponse,
} Bluemod_CmdType;

/**
 * Data value type for command data
 */
typedef Ascii_ValueType Bluemod_CmdDataValueType;

/**
 * Data type for command data
 */
typedef uint8_t Bluemod_CmdDataLengthType;

typedef struct
{
  /**
   * Data values
   */
  Bluemod_CmdDataValueType* data;
  
  /**
   * Length of data
   */
  Bluemod_CmdDataLengthType length;
  
} Bluemod_CmdDataType;

/**
 * command queue entry
 */
typedef struct
{
  /**
   * command type
   */
  Bluemod_CmdType cmd;
  
  /**
   * destination channel
   */
  Bluemod_ChannelType channel;
  
  /**
   * command data
   */
  Bluemod_CmdDataType data;
  
  /**
   * number of try
   */
  uint8_t tryCount;
  
} Bluemod_CmdQueueEntryType;

/**
 * length type for command queue
 */
typedef uint8_t Bluemod_CmdQueueLengthType;

/**
 * Command queue for the sent commands
 */
typedef struct
{
  /**
   * command field
   */
  Bluemod_CmdQueueEntryType* commands;
  
  /**
   * read index
   */
  Bluemod_CmdQueueLengthType readIdx;
  
  /**
   * write index
   */
  Bluemod_CmdQueueLengthType writeIdx;
  
  /**
   * number of free spaces
   */
  Bluemod_CmdQueueLengthType space;
  
  /**
   * allocated size
   */
  const Bluemod_CmdQueueLengthType size;
  
} Bluemod_CmdQueueType;

/**
 * Notification type for callbacks
 */
typedef void (*Bluemod_NotificationType)(uint8_t const deviceNumber);

/**
 * Notification type for callbacks with given channel
 */
typedef void (*Bluemod_ChannelNotificationType)(uint8_t const deviceNumber,
  Bluemod_ChannelType const channel);

/**
 * Type for different states
 */
typedef enum
{
  Bluemod_StateUninitialised = 0u,//!< Bluemod_StateUninitialised
  Bluemod_StateBoot,
  Bluemod_StateInitialising,      //!< Bluemod_StateInitialising
  Bluemod_StateMux                //!< Bluemod_StateMux
} Bluemod_StateType;

typedef enum
{
  Bluemod_ReceivedStateWaiting = 0u,
  Bluemod_ReceivedStateComplete
} Bluemod_ReceivedState;

typedef struct
{
  /**
   * Mux Received state
   */
  Bluemod_ReceivedState recState;
  
  /**
   * offset within queue, i.e. the index of channel field
   */
  Bluemod_LengthType offset;
  
} Bluemod_ReceivedData;

/**
 * enumeration tpye for current send/receive state of the device
 */
typedef enum
{
  Bluemod_SendReceievStateSending = 0u,
  Bluemod_SendReceievStateReceiving,
  Bluemod_SendReceievStateReceivingAsync
} Bluemod_SendReceiveStateType;

/**
 * device configuration types
 */
typedef struct
{
  /**
   * struct with used interface
   */
  const Bluemod_IfType interface;
  
  /**
   * struct with all settings used for bluetooth connection
   */
  const Bluemod_BluetoothSettings bluetoothSettings;
  
  /**
   * command queue
   */
  Bluemod_CmdQueueType cmdQueue;
  
  /**
   * receive queue
   */
  Ascii_QueueType receiveQueue;
  
  /**
   * received dataset
   */
  Bluemod_ReceivedData* receivedData;
  
  /**
   * Notification for ring event
   */
  Bluemod_NotificationType ring;

  /**
   * Notification for connect event
   */
  Bluemod_NotificationType connect;

  /**
   * Notification for no carrier event
   */
  Bluemod_NotificationType noCarrier;

  /**
   * Notification for received data per spp
   */
  Bluemod_ChannelNotificationType dataReceivedSpp;
  
  /**
   * Notification for received data per ble
   */
  Bluemod_ChannelNotificationType dataReceivedBle;
  
  /**
   * Current bluemod state
   */
  Bluemod_StateType state;
  
  /**
   * current send/receive state of device
   */
  Bluemod_SendReceiveStateType sendRecvState;
  
  /**
   * stopwatch for sending timeout and booting time
   */
  uint32_t watch;
  
  /**
   * timeout for waiting on the response of an command, before resending it
   */
  const uint32_t sendTimeout;
  
  /**
   * boot time of bluemod
   */
  const uint32_t bootTime;
  
  /**
   * bluemodreset pin
   */
  const GpioIf_ChannelType bluemodResetPin;
  
  /**
   * channel of bluetooth system state
   */
  const uint8_t sysStateChannel;
  
} Bluemod_DeviceConfigType;


/**
 * configtype
 */
typedef struct 
{
  /**
   * Number of devices
   */
  const uint8_t numberOfDevices;
  
  /**
   * device Configurations
   */
  Bluemod_DeviceConfigType* deviceConfigs;
  
} Bluemod_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

extern Bluemod_ConfigType Bluemod_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * initialiszes the bluemod module
 * @param config pointer to the configuration struct
 */
void Bluemod_Init(Bluemod_ConfigType* config);

/**
 * Main Rountine for sending commands to bluemod
 */
void Bluemod_MainFunction(void);

/**
 * Get the last received data from the defined device
 * @param deviceNumber the number of the bluetooth device
 * @param channel out param for the according data channel
 * @param data pointer to the data buffer
 * @param length
 * @return number of data written in data
 */
Bluemod_LengthType Bluemod_GetData(uint8_t const deviceNumber,
                                   Bluemod_ChannelType channel,
                                   Bluemod_DataType* data,
                                   Bluemod_LengthType const length);

/**
 * Send data to the given channel
 * @param deviceNumber the number of the bluemod
 * @param channel destination channel
 * @param data pointer to databuffer
 * @param length length of data in buffer
 */
void Bluemod_SendData(uint8_t const deviceNumber,
                      Bluemod_ChannelType const channel,
                      Bluemod_DataType* data,
                      Bluemod_LengthType const length);

/**
 * Accepts an incomming connection with given channel
 * @param deviceNumber number of bluetooth device
 * @param cmdData dataframe with hex coded channelnumber from ring/connect
 *                e.g. "0x01" for channel 1
 */
void Bluemod_AcceptConnection(uint8_t const deviceNumber,
                              Bluemod_CmdDataType cmdData);


/**
 * Disconnects an connection with given channel
 * @param deviceNumber number of bluetooth device
 * @param cmdData dataframe with hex coded channelnumber from ring/connect
 *                e.g. "0x01" for channel 1
 */
void Bluemod_Disconnect(uint8_t const deviceNumber,
                              Bluemod_CmdDataType cmdData);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

#endif
