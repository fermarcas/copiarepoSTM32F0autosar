/**
 ******************************************************************************
 * @file    Rs485_Cfg.h
 * @author  Franz Profelt
 * @version V1.0.0
 * @date    09.12.2014
 * @brief   Configurationheader of Rs485 Module
 *
 *
 * (c) Research Group Embedded Systems, FH OÖ Forschungs und EntwicklungsgmbH
 * You may not use or copy this file except with written permission of FH OÖ
 ******************************************************************************
 */

#ifndef RS485_CFG_H
#define RS485_CFG_H

/* Generic */
#define RS485_NUMBER_OF_CHANNELS      1u

/* Channel 0 */
#define RS485_UARTIF_CHANNEL0         1u
#define RS485_GPIO_CONTROL_PIN0       GpioIf_ChannelA01
#define RS485_RX_NOTIFIER0            NULL_PTR
#define RS485_TX_DRE_NOTIFIER0        NULL_PTR
#define RS485_TX_COMPLETE_NOTIFIER0   NULL_PTR


#endif
