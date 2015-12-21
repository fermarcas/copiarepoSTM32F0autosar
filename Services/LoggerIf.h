/**
 ******************************************************************************
 * @file    LoggerIf.h
 * @author  Gilbert Mitter
 * @version V1.0.0
 * @date    13.01.2015
 * @brief   LoggerIf Header
 ******************************************************************************
 */

#ifndef LOGGERIF_H
#define LOGGERIF_H

#include "Services/StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

typedef uint8_t LoggerIf_IfLengthType;
typedef uint8_t LoggerIf_IfDataType;
typedef uint8_t LoggerIf_IfChannel;

/**
 * A Logger_IfPutDataType
 * @param LoggerIf_IfPutDataType: Function pointed by the 
 * LoggerIf_SendMessage macro
 * @param channel : Channel number for the
 * LoggerIf_IfPutDataType Function
 * @param data : Data typed passed to the function
 * @param length : Length of the passed data type
 */
typedef LoggerIf_IfLengthType (*LoggerIf_IfPutDataType)(
  const uint8_t channel, const LoggerIf_IfDataType* data,
  const LoggerIf_IfLengthType length);

/**
 * The different logging levels as enum
 */
typedef enum
{
  LogLevel_Fatal = 0,
  LogLevel_Error,
  LogLevel_Warning,
  LogLevel_Debug,
  LogLevel_Scope,
  LogLevel_Last
} LoggerIf_LevelType;

/**
 * A LoggerIf config Datatype.
 */
typedef struct
{
  const LoggerIf_LevelType logLevel;
  const LoggerIf_IfPutDataType putDataFunc;
  const LoggerIf_IfChannel channel;
} LoggerIf_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/**
 * Macros for the different logging levels. 
 * Logs the passed message, function path of the file and the line number.
 * @param __FILE__ The name of the current file(This is the path by which
 *        the preprocessor opened the file), as a string literal
 * @param __LINE__ Current line of the source file, as a numerical literal
 * @param message : the message passed to the LogIf_Handler function
 */

/**
 * Logger macro for logging level : fatal
 */
#define LoggerIf_Fatal(message)                                     \
    LoggerIf_Handler(LogLevel_Fatal, __FILE__, __LINE__, message)

/**
 * Logger macro for logging level : error
 */
#define LoggerIf_Error(message)                                     \
    LoggerIf_Handler(LogLevel_Error, __FILE__, __LINE__, message)

/**
 * Logger macro for logging level : message 
 */
#define LoggerIf_Warning(message)                                   \
    LoggerIf_Handler(LogLevel_Warning, __FILE__, __LINE__, message)

/**
 * Logger macro for logging level : debug
 */
#define LoggerIf_Debug(message)                                     \
    LoggerIf_Handler(LogLevel_Debug, __FILE__, __LINE__, message)

/**
 * Logger macro for logging level : info 
 */
#define LoggerIf_Info(message)                                      \
    LoggerIf_Handler(LogLevel_Info, __FILE__, __LINE__, message)

/**
 * Logger macro for logging level : scope 
 */
#define LoggerIf_Scope(message)                                     \
    LoggerIf_Handler(LogLevel_Scope, __FILE__, __LINE__, message)

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * declares a LogIf_Config, will be setup in LogIf_Cfg module.
 */
extern const LoggerIf_ConfigType LoggerIf_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Configures the Logger .
 * @param configuration for initializing the Logger 
 */
void LoggerIf_Init(const LoggerIf_ConfigType* config);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/**
 * Handler for the passed logger messages
 * @param level   : The passed logger level
 * @param file    : Path of the file, where logger gets called
 * @param line    : Current line of the source file, as a numerical literal
 * @param message : Passed message for the logger handler
 */
void LoggerIf_Handler(LoggerIf_LevelType level, const char* file,
                      const uint16_t line, const char* message);

#endif
