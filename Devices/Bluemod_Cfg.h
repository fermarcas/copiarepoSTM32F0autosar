/**
 ******************************************************************************
 * @file    Bluemod_Cfg.c
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    11.03.2015
 * @brief   Configuration of Abstraction of Stollmann Bluemod + SR
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef BLUEMOD_CFG_H
#define BLUEMOD_CFG_H

/* Generic */
#define BLUEMOD_NUMBER_OF_DEVICES               1u

/* Device 0 */
#define BLUEMOD_HAL_CHANNEL                     0u
#define BLUEMOD_HAL_GET                         UartIf_GetData
#define BLUEMOD_HAL_PUT                         UartIf_PutData

#define BLUEMOD_RETRY                           5u
#define BLUEMOD_NAME                            "Thermocollect"
#define BLUEMOD_NAME_LEN                        13u

#define BLUEMOD_CMD_QUEUE_SIZE                  16u

#define BLUEMOD_REC_QUEUE_SIZE                  128u

#define BLUEMOD_REC_STATE                       Bluemod_ReceivedStateWaiting

#define BLUEMOD_RING                            NULL_PTR
#define BLUEMOD_CONNECT                         NULL_PTR
#define BLUEMOD_NO_CARRIER                      NULL_PTR

#define BLUEMOD_DATARECEIVED_SPP                NULL_PTR
#define BLUEMOD_DATARECEIVED_BLE                NULL_PTR

#define BLUEMOD_INIT_STATE                      Bluemod_StateUninitialised

#define BLUEMOD_INIT_SEND_RECV_STATE            Bluemod_SendReceievStateSending

#define BLUEMOD_RECEIVE_RESPONSE_TIMEOUT        2000u
#define BLUEMOD_BOOT_TIME                       1200u

#define BLUEMOD_RESET_PIN                       GpioIf_ChannelA05

#define BLUEMOD_SYS_STATE_CHANNEL               1u

#endif
