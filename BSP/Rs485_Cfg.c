/**
 ******************************************************************************
 * @file    Rs485_Cfg.c
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    09.12.2014
 * @brief   Configuration of Rs485 Module
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#include "Services/StdDef.h"
#include "Rs485.h"
#include "Rs485_Cfg.h"
#include "GpioIf.h"
#include "Services/StdDef.h"

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

static const Rs485_ChannelConfigType Rs485_ChannelConfigs[RS485_NUMBER_OF_CHANNELS] = 
{
  {
    RS485_UARTIF_CHANNEL0,
    RS485_GPIO_CONTROL_PIN0,
    RS485_RX_NOTIFIER0,
    RS485_TX_DRE_NOTIFIER0,
    RS485_TX_COMPLETE_NOTIFIER0
  }
};

const Rs485_ConfigType Rs485_config =
{
  RS485_NUMBER_OF_CHANNELS,
  Rs485_ChannelConfigs
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

