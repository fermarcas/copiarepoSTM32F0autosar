/**
 ******************************************************************************
 * @file    SysTimerIf_Cfg.h
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    14.12.2014
 * @brief   Configuration for TimerIf Module
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef SYSTIMERIF_CFG_H
#define SYSTIMERIF_CFG_H

#include "Services/StdDef.h"

/* Generic */
#define SYSTIMERIF_NUMBER_OF_CHANNELS   3u
#define SYSTIMERIF_RESOLUTION           1000u     /* for Milliseconds tick */

/* Channel 0 */
#define SYSTIMERIF_CALLBACK0            Stopwatch_MsTimer
#define SYSTIMERIF_TIMEOUT0             1u        /* in ms */
#define SYSTIMERIF_ACTIVATED0           TRUE

/* Channel 1 */
#define SYSTIMERIF_CALLBACK1            Led_MsTimer
#define SYSTIMERIF_TIMEOUT1             1u        /* in ms */
#define SYSTIMERIF_ACTIVATED1           TRUE

/* Channel 2 */
#define SYSTIMERIF_CALLBACK2            SysState_MsTimer
#define SYSTIMERIF_TIMEOUT2             1u        /* in ms */
#define SYSTIMERIF_ACTIVATED2           TRUE

#endif

