/**
 ******************************************************************************
 * @file    CrcIf_Cfg.c
 * @author  Mitter Gilbert
 * @version V1.0.0
 * @date    24.11.2014
 * @brief   Configuration for the CrcIf module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */
 
 #include "Services/StdDef.h"
 #include "CrcIf.h"
 #include "CrcIf_Cfg.h"
 
/****************************************************************
 * User functions
 ***************************************************************/
 
 CrcIf_ConfigType CrcIf_config =
{
   CRCIF_INITVALUE,
   CRCIF_POLYSIZE,
   CRCIF_REVERSE_INPUT,
   CRCIF_REVERSE_OUTPUT
};
