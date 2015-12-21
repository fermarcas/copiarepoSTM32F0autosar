/**
 ******************************************************************************
 * @file    BtDevM_Cfg.c
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    22.04.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef BTDEVICEMANAGER_CFG_H
#define BTDEVICEMANAGER_CFG_H

#define BTDEVM_DEVICELISTSIZE     8u

#define BTDEVM_CONNECTION_TIMEOUT  UINT32_MAX

#define BTDEVM_NEWCLIENTCONNECTED  NULL_PTR
#define BTDEVM_DISCONNECTED        ClientAbs_OnClientDisconnected

#define BTDEVM_SUBSCRIBE           (BtDevM_SubscribeNotifyType)ClientAbs_OnSubscribe
#define BTDEVM_UNSUBSCRIBE         (BtDevM_SubscribeNotifyType)ClientAbs_OnUnsubscribe
#define BTDEVM_COMMANDREC          (BtDevM_CommandReceivedType)ClientAbs_OnCommandReceived
#define BTDEVM_CONFIGREQU          ClientAbs_OnConfigRequested
#define BTDEVM_ISALIVE             ClientAbs_OnIsAlive

#define BTDEVM_SYSTSTATE_CHANNEL    1u

/* bluemod interface */
#define BTDEVM_BLUEMOD_ACCEPT       (BtDevM_BluemodIfAcceptConnectionType)Bluemod_AcceptConnection
#define BTDEVM_BLUEMOD_DISCONNECT   (BtDevM_BluemodIfCloseConnectionType)Bluemod_Disconnect
#define BTDEVM_BLEMOD_GETDATA       (BtDevM_BluemodIfGetDataType)Bluemod_GetData

/* spp interface */
#define BTDEVM_SPP_SENDERROR        (BtDevM_SendErrorType)TbProt_SendError
#define BTDEVM_SPP_SENDDATA         (BtDevM_SendDataType)TbProt_SendData
#define BTDEVM_SPP_SENDCONFIG       (BtDevM_SendConfigType)TbProt_SendConfiguration

#endif
