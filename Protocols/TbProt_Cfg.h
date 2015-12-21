/**
 ******************************************************************************
 * @file    TbProt_Cfg.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    22.04.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef TBPROT_CFG_H
#define TBPROT_CFG_H

#define TBPROT_BLUEMOD_DEV_NUM                0
#define TBPROT_BLUEMOD_GETDATA                Bluemod_GetData
#define TBPROT_BLUEMOD_SENDDATA               Bluemod_SendData
#define TBPROT_BLUEMOD_SUBSCRIBE              (TbProt_SubscribeNotifyType)BtDevM_OnSppSubscribe
#define TBPROT_BLUEMOD_UNSUBSCRIBE            (TbProt_SubscribeNotifyType)BtDevM_OnSppUnsubscribe
#define TBPROT_BLUEMOD_COMMAND_RECEIVED       (TbProt_CommandReceivedType)BtDevM_OnSppCommandReceived
#define TBPROT_BLUEMOD_CONFIG_REQUESTED       BtDevM_OnSppConfigRequested
#define TBPROT_BLUEMOD_IS_ALIVE               BtDevM_OnSppIsAlive

#endif
