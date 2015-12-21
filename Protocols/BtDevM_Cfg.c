/**
 ******************************************************************************
 * @file    BtDevM_Cfg.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    22.04.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "Protocols/BtDevM.h"
#include "Protocols/BtDevM_Cfg.h"
#include "Services/StdDef.h"
#include "Devices/Bluemod.h"
#include "Protocols/TbProt.h"
#include "APP/ClientAbs.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

BtDevM_DeviceEntryType BtDev_DeviceList[BTDEVM_DEVICELISTSIZE] =
{
  {
    0, 0, 0, BtDevM_DevStateEntryUnused, BtDevM_DevEntryInterfaceSpp, 0
  }
};

BtDevM_ConfigType BtDevM_config = 
{
  {
    BtDev_DeviceList,
    0,
    BTDEVM_DEVICELISTSIZE
  },
  BTDEVM_CONNECTION_TIMEOUT,
  BTDEVM_NEWCLIENTCONNECTED,
  BTDEVM_DISCONNECTED,
  BTDEVM_SUBSCRIBE,
  BTDEVM_UNSUBSCRIBE,
  BTDEVM_COMMANDREC,
  BTDEVM_CONFIGREQU,
  BTDEVM_ISALIVE,
  BTDEVM_SYSTSTATE_CHANNEL,
  /* bluemod interface */
  {
    BTDEVM_BLEMOD_GETDATA,
    BTDEVM_BLUEMOD_ACCEPT,
    BTDEVM_BLUEMOD_DISCONNECT
  },
  /* spp interface */
  {
    BTDEVM_SPP_SENDERROR,
    BTDEVM_SPP_SENDDATA,
    BTDEVM_SPP_SENDCONFIG
  },
  /* ble interface */
  {
    0
  }
};

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/
