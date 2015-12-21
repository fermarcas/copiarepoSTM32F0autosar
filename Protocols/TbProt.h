/**
 ******************************************************************************
 * @file    TbProt.c
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    22.04.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef TBPROT_H
#define TBPROT_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

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
} TbProt_ValueIdType;

/**
 * type for the message id within an frame
 */
typedef uint32_t TbProt_MessageIdType;

/**
 * type for all data values
 */
typedef int32_t TbProt_DataType;

/**
 * type for error data
 */
typedef uint8_t TbProt_ErrorDataType;

/**
 * type for configdata
 */
typedef struct
{
  uint16_t roomId;
  uint16_t motorId;
  uint16_t sensorId;
} TbProt_ConfigDataType;

/**
 * generell length type
 */
typedef uint32_t TbProt_LentghType;

/**
 * notification type of subscribe data callback
 */
typedef BOOL (*TbProt_SubscribeNotifyType)(const TbProt_MessageIdType messageId,
                                           const TbProt_ValueIdType valueId);

/**
 * notfication type for command received callback
 */
typedef BOOL (*TbProt_CommandReceivedType)(const TbProt_ValueIdType valueId,
                                           const TbProt_DataType data);

/**
 * notfication type for command received callback
 */
typedef BOOL (*TbProt_ConfigRequestedType)(
  const TbProt_MessageIdType messageId);

/**
 * notfication type for command received callback
 */
typedef BOOL (*TbProt_IsAliveType)(const TbProt_MessageIdType messageId);

/**
 * data types of the bluemodif
 */
typedef uint8_t TbProt_BluemodIfChannelType;
typedef uint8_t TbProt_BluemodIfDataType;
typedef uint8_t TbProt_BluemodIfLengthType;

/**
 * type for the get data method of the bluemodif
 */
typedef TbProt_BluemodIfLengthType (*TbProt_BluemodIfGetDataType)(
  uint8_t const deviceNumber, TbProt_BluemodIfChannelType channel,
  TbProt_BluemodIfDataType* data, TbProt_BluemodIfLengthType const length);

/**
 * Type for bluemod send data function
 */
typedef void (*TbProt_BluemodIfSendDataType)(
  uint8_t const deviceNumber, TbProt_BluemodIfChannelType const channel,
  TbProt_BluemodIfDataType* data, TbProt_BluemodIfLengthType const length);

/**
 * Configuration Type
 */
typedef struct
{
  /**
   * device number of bluemod
   */
  const uint8_t bluemodDeviceNumber;

  /**
   * function pointer to get data of bluemod
   */
  TbProt_BluemodIfGetDataType getBluemodData;

  /**
   * function pointer to send data of bluemod
   */
  TbProt_BluemodIfSendDataType sendBluemodData;

  /**
   * notification for an subscribe request
   */
  TbProt_SubscribeNotifyType subscribeData;

  /**
   * notification for an unsubscribe request
   */
  TbProt_SubscribeNotifyType unsubscribeData;

  /**
   * Notification for received command
   */
  TbProt_CommandReceivedType commandReceived;

  /**
   * notification for an configuration request
   */
  TbProt_ConfigRequestedType configRequested;

  /**
   * notification for is alive received
   */
  TbProt_IsAliveType isAliveReiceived;

} TbProt_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

extern TbProt_ConfigType TbProt_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Init function
 * @param config pointer to used configuration
 */
void TbProt_Init(TbProt_ConfigType* config);

/**
 * send and error frame to device
 * @param messageId message id
 * @param errorData buffer with error data
 * @param length length of error data
 */
void TbProt_SendError(TbProt_MessageIdType const * messageId,
                      TbProt_ErrorDataType const * errorData,
                      TbProt_LentghType const length);

/**
 * send data to the client
 * @param id pointer to the value id
 * @param data pointer to the data field
 */
void TbProt_SendData(TbProt_ValueIdType const * id,
                     TbProt_DataType const * data);

/**
 * send configuration to device
 * @param messageId message id
 * @param configData buffer with configdata
 * @param length length of configdata
 */
void TbProt_SendConfiguration(TbProt_MessageIdType const * messageId,
                              TbProt_ConfigDataType const * configData,
                              TbProt_LentghType const length);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/
void TbProt_OnDataReceived(uint8_t const bluetoothDeviceNumber,
                           uint8_t const channelNumber);

#endif
