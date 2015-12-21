/**
 ******************************************************************************
 * @file    LoggerIf.c
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    13.01.2015
 * @brief   Implements the Logger functions
 ******************************************************************************
 */

#include "Services/LoggerIf.h"
#include "Protocols/Ascii.h"
#include <string.h>

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

#define LOGGERIF_VAL_DELIMITER                                    ":"
#define LOGGERIF_LINE_DELIMITER                                   "\r\n"
#define LOGGERIF_LINE_BUFFER_SIZE	                                6u

/**
 * Sends the Logger Message to the Function specified in the LogIf_userCfg.
 */
#define LoggerIf_SendMessage(message, len)                          \
  if(LoggerIf_userCfg->putDataFunc != NULL_PTR)                     \
  {                                                                 \
    LoggerIf_userCfg->putDataFunc(LoggerIf_userCfg->channel,        \
                                  (LoggerIf_IfDataType*) message,   \
                                  len);                             \
  }

/****************************************************************
 * Function declarations
 ***************************************************************/

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Assign a string to the different Log Levels, need to be adjusted if
 * new Log Levels are added. 
 */
static const char* LoggerIf_logNames[] =
{
  "Fatal",
  "Error",
  "Warning",
  "Debug",
  "Scope" 
};

static char LoggerIf_lineBuffer[LOGGERIF_LINE_BUFFER_SIZE] =
{
  0 };

static const LoggerIf_ConfigType* LoggerIf_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void LoggerIf_Init(const LoggerIf_ConfigType* config)
{
  if ((config != NULL_PTR) && (LoggerIf_userCfg == NULL_PTR))
  {
    LoggerIf_userCfg = config;
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void LoggerIf_Handler(LoggerIf_LevelType level, const char* file,
                      const uint16_t line, const char* message)
{
  if ((LoggerIf_userCfg != NULL_PTR) && (level <= LoggerIf_userCfg->logLevel))
  {
    /* Send the log level */
    LoggerIf_SendMessage(
      LoggerIf_logNames[level],
      (LoggerIf_IfLengthType ) strlen(LoggerIf_logNames[level]));
    /* Send a delimiter */
    LoggerIf_SendMessage(LOGGERIF_VAL_DELIMITER,
                         (LoggerIf_IfLengthType)strlen(LOGGERIF_VAL_DELIMITER));

    /* Send the path of the file */
    LoggerIf_SendMessage(file, (LoggerIf_IfLengthType )strlen(file));
    /* Send a delimiter */
    LoggerIf_SendMessage(LOGGERIF_VAL_DELIMITER,
                         (LoggerIf_IfLengthType)strlen(LOGGERIF_VAL_DELIMITER));

    Ascii_DecToStr(LoggerIf_lineBuffer, LOGGERIF_LINE_BUFFER_SIZE, line, Ascii_NumberBaseDec, 0u);
    LoggerIf_SendMessage(LoggerIf_lineBuffer,
                         (LoggerIf_IfLengthType )strlen(LoggerIf_lineBuffer));
    /* Send a delimiter */
    LoggerIf_SendMessage(LOGGERIF_VAL_DELIMITER,
                         (LoggerIf_IfLengthType)strlen(LOGGERIF_VAL_DELIMITER));

    /* Send the logger message */
    LoggerIf_SendMessage(message, (LoggerIf_IfLengthType )strlen(message));
    /* Send a carriage return and a line feed */
    LoggerIf_SendMessage(LOGGERIF_LINE_DELIMITER,
                         (LoggerIf_IfLengthType)strlen(LOGGERIF_LINE_DELIMITER));
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/
