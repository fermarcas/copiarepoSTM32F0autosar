/**
 ******************************************************************************
 * @file    TbProt_Cfg.c
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    22.04.2015
 *
 * Copyright (c) FH O� Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "TbProt.h"
#include "TbProt_Cfg.h"
#include "Services/StdDef.h"
#include "Devices/Bluemod.h"
#include "Protocols/BtDevM.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

/****************************************************************
 * Function declarations
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

TbProt_ConfigType TbProt_config =
{
  TBPROT_BLUEMOD_DEV_NUM,
  TBPROT_BLUEMOD_GETDATA,
  TBPROT_BLUEMOD_SENDDATA,
  TBPROT_BLUEMOD_SUBSCRIBE,
  TBPROT_BLUEMOD_UNSUBSCRIBE,
  TBPROT_BLUEMOD_COMMAND_RECEIVED,
  TBPROT_BLUEMOD_CONFIG_REQUESTED,
  TBPROT_BLUEMOD_IS_ALIVE
};

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

