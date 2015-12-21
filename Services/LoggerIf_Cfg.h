/**
 ******************************************************************************
 * @file    LoggerIf_Cfg.h
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    13.01.2015
 * @brief   LoggerIf_Cfg Header
 ******************************************************************************
 */

#ifndef LOGGERIF_CFG_H
#define LOGGERIF_CFG_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/* Defines the Log_Level_Warning as default log level */
#define LOGGERIF_LOG_LEVEL                      LogLevel_Warning

/* Channel number used by LOGGERIF_IF_PUT_FUNC */ 
#define LOGGERIF_IF_CHANNEL								      0u

/* Funtion where logging messages get sent */
#define LOGGERIF_IF_PUT_FUNC                    NULL_PTR

/****************************************************************
 * Global variables
 ***************************************************************/

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/



#endif
