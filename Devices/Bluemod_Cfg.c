/**
 ******************************************************************************
 * @file    Bluemod_Cfg.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    11.03.2015
 * @brief   Configuration of Abstraction of Stollmann Bluemod + SR
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "Bluemod.h"
#include "Bluemod_Cfg.h"
#include "Services/StdDef.h"
#include "BSP/UartIf.h"
#include "Bluemod.h"
#include "Protocols/BtDevM.h"
#include "Protocols/TbProt.h"
#include "BSP/GpioIf.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

Bluemod_CmdQueueEntryType Bluemod_CmdQueueField[BLUEMOD_CMD_QUEUE_SIZE];
Ascii_ValueType Bluemod_ReceiveQueueField[BLUEMOD_REC_QUEUE_SIZE];

Bluemod_ReceivedData Bluemod_ReceivedDataSet = 
{
  BLUEMOD_REC_STATE,
  0
};

Bluemod_DeviceConfigType Bluemod_DeviceConfigs[BLUEMOD_NUMBER_OF_DEVICES] =
{
  {
    /* interface */
    {
      BLUEMOD_HAL_CHANNEL,
      BLUEMOD_HAL_GET,
      BLUEMOD_HAL_PUT
    },
    /* bluetooth settings */
    {
      BLUEMOD_RETRY,
      BLUEMOD_NAME,
      BLUEMOD_NAME_LEN
    },
    /* command queue */
    {
      Bluemod_CmdQueueField,
      0u, 0u,
      BLUEMOD_CMD_QUEUE_SIZE,
      BLUEMOD_CMD_QUEUE_SIZE
    },
    /* ascii queue */
    {
      Bluemod_ReceiveQueueField,
      0u, 0u,
      BLUEMOD_REC_QUEUE_SIZE,
      BLUEMOD_REC_QUEUE_SIZE
    },
    /* received dataset */
    &Bluemod_ReceivedDataSet,
    /* Notifications */
    BLUEMOD_RING,
    BLUEMOD_CONNECT,
    BLUEMOD_NO_CARRIER,
    BLUEMOD_DATARECEIVED_SPP,
    BLUEMOD_DATARECEIVED_BLE,
    BLUEMOD_INIT_STATE,
    BLUEMOD_INIT_SEND_RECV_STATE,
    0u,
    BLUEMOD_RECEIVE_RESPONSE_TIMEOUT,
    BLUEMOD_BOOT_TIME,
    BLUEMOD_RESET_PIN,
    BLUEMOD_SYS_STATE_CHANNEL
  }
};

Bluemod_ConfigType Bluemod_config = 
{
  1u,
  Bluemod_DeviceConfigs
};

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/
