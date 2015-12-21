/**
 ******************************************************************************
 * @file    GpioIf.h
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    05.12.2014
 * @brief   Abstracts the GPIO module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#ifndef GPIOIF_H
#define GPIOIF_H

#include "Services/StdDef.h"

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_exti.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * Defines the possible channels.
 */
typedef enum
{
  GpioIf_ChannelA00 = 0x00,
  GpioIf_ChannelA01,
  GpioIf_ChannelA02,
  GpioIf_ChannelA03,
  GpioIf_ChannelA04,
  GpioIf_ChannelA05,
  GpioIf_ChannelA06,
  GpioIf_ChannelA07,
  GpioIf_ChannelA08,
  GpioIf_ChannelA09,
  GpioIf_ChannelA10,
  GpioIf_ChannelA11,
  GpioIf_ChannelA12,
  GpioIf_ChannelA13,
  GpioIf_ChannelA14,
  GpioIf_ChannelA15,
  GpioIf_ChannelB00,
  GpioIf_ChannelB01,
  GpioIf_ChannelB02,
  GpioIf_ChannelB03,
  GpioIf_ChannelB04,
  GpioIf_ChannelB05,
  GpioIf_ChannelB06,
  GpioIf_ChannelB07,
  GpioIf_ChannelB08,
  GpioIf_ChannelB09,
  GpioIf_ChannelB10,
  GpioIf_ChannelB11,
  GpioIf_ChannelB12,
  GpioIf_ChannelB13,
  GpioIf_ChannelB14,
  GpioIf_ChannelB15,
  GpioIf_ChannelC00,
  GpioIf_ChannelC01,
  GpioIf_ChannelC02,
  GpioIf_ChannelC03,
  GpioIf_ChannelC04,
  GpioIf_ChannelC05,
  GpioIf_ChannelC06,
  GpioIf_ChannelC07,
  GpioIf_ChannelC08,
  GpioIf_ChannelC09,
  GpioIf_ChannelC10,
  GpioIf_ChannelC11,
  GpioIf_ChannelC12,
  GpioIf_ChannelC13,
  GpioIf_ChannelC14,
  GpioIf_ChannelC15,
  GpioIf_ChannelD00,
  GpioIf_ChannelD01,
  GpioIf_ChannelD02,
  GpioIf_ChannelD03,
  GpioIf_ChannelD04,
  GpioIf_ChannelD05,
  GpioIf_ChannelD06,
  GpioIf_ChannelD07,
  GpioIf_ChannelD08,
  GpioIf_ChannelD09,
  GpioIf_ChannelD10,
  GpioIf_ChannelD11,
  GpioIf_ChannelD12,
  GpioIf_ChannelD13,
  GpioIf_ChannelD14,
  GpioIf_ChannelD15,
} GpioIf_ChannelType;

/* TODO: enum necessary? */
typedef enum
{
  GpioIf_PortA = 0x00,
  GpioIf_PortB,
  GpioIf_PortC,
  GpioIf_PortD,
} GpioIf_PortType;

/**
 * Represents the register type.
 */
typedef GPIO_TypeDef GpioIf_RegisterType;
/**
 * Represents the mode type.
 */
typedef GPIOMode_TypeDef GpioIf_ModeType;
/**
 * Represents the speed type.
 */
typedef GPIOSpeed_TypeDef GpioIf_SpeedType;
/**
 * Represents the output type.
 */
typedef GPIOOType_TypeDef GpioIf_OutputType;
/**
 * Represents the pull up and down type.
 */
typedef GPIOPuPd_TypeDef GpioIf_PullUpDownType;
/**
 * Represents the alternate function type.
 */
typedef uint8_t GpioIf_AlternateFunctionType;
/**
 * Represents the clock domain.
 */
typedef uint32_t GpioIf_ClockDomainType;

/**
 * External interrupt mode.
 */
typedef EXTIMode_TypeDef GpioIf_ExternalInterruptModeType;
/**
 * External interrupt trigger type.
 */
typedef EXTITrigger_TypeDef GpioIf_ExternalInterruptTriggerType;
/**
 * Defines a function pointer for the interrupt service routine callback.
 */
typedef void (*GpioIf_IsrCallbackType)(void);
/**
 * Defines the NVIC interrupt type.
 */
typedef IRQn_Type GpioIf_NvicIrqType;

/**
 * Represents one pin configuration.
 */
typedef struct
{
  /* Determines if this channel should be initialized.
   * TRUE means that this pin will be initialized; otherwise FALSE. */
  const BOOL pinUsed;
  /* Pin mode. */
  const GpioIf_ModeType mode;
  /* Pin speed. */
  const GpioIf_SpeedType speed;
  /* Defines the output type. */
  const GpioIf_OutputType outputType;
  /* Defines the pull up and down type. */
  const GpioIf_PullUpDownType pullUpDownType;
  /* Defines the alternate function. */
  const GpioIf_AlternateFunctionType alternateFunction;
  /* Determines if this channel should use the ISR. */
  const BOOL isrUsed;
  /* Defines the NVIC interrupt. */
  const GpioIf_NvicIrqType nvicIrq;
  /* Defines the ISR callback. */
  const GpioIf_IsrCallbackType isrCallback;
  /* Defines the trigger mode. */
  const GpioIf_ExternalInterruptTriggerType triggerType;
} GpioIf_PinConfigType;

/**
 * Represents one port configuration.
 */
typedef struct
{
  /* Determines if this port should be initialized.
   * TRUE means that this pin will be initialized; otherwise FALSE. */
  const BOOL portUsed;
  /* Defines the number of pins per port. */
  const uint8_t numberOfPins;
  /* A pointer to the pin configurations. */
  const GpioIf_PinConfigType* pinConfigs;
  /* The port registers. */
  GpioIf_RegisterType* registers;
  /* The clock domain. */
  const GpioIf_ClockDomainType clockDomain;
} GpioIf_PortConfigType;

/**
 * Represents the GpioIf configuration.
 */
typedef struct
{
  /* Defines the number of ports. */
  const uint8_t numberOfPorts;
  /* A pointer to the port configurations. */
  const GpioIf_PortConfigType* portConfigs;
} GpioIf_ConfigType;

/****************************************************************
 * Macros                                                       *
 ***************************************************************/

/**
 * Defines the number of pins per channel.
 */
#define GPIOIF_NUMBER_OF_PINS_PER_CHANNEL                     16u

/****************************************************************
 * Global variables
 ***************************************************************/

/**
 * Compile time configuration.
 */
extern const GpioIf_ConfigType GpioIf_config;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * Initializes the GpioIf module.
 * @param config A pointer to the configuration.
 */
void GpioIf_Init(const GpioIf_ConfigType* config);

/**
 * Gets the pin state.
 * @param channel The channel of the pin.
 * @return TRUE if the pin is High; otherwise FALSE.
 */
BOOL GpioIf_GetBit(GpioIf_ChannelType channel);

/**
 * Sets one output pin.
 * @param channel The channel which output pin has to be set.
 */
void GpioIf_SetBit(GpioIf_ChannelType channel);

/**
 * Clears one output bin.
 * @param channel The channel which output pin has to be cleared.
 */
void GpioIf_ClrBit(GpioIf_ChannelType channel);

/**
 * Enters a critical section.
 * @param channel The critical channel.
 */
void GpioIf_EnterCS(GpioIf_ChannelType channel);
/**
 * Leaves a critical section.
 * @param channel The critical channel.
 */
void GpioIf_LeaveCS(GpioIf_ChannelType channel);

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/**
 * Interrupt service routine of a specific line.
 * @param line The line which has occurred.
 */
void GpioIf_Isr(uint8_t line);

#endif /* GPIOIF_H */
