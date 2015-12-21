/**
 ******************************************************************************
 * @file    GpioIf.c
 * @author  Bernhard Berger
 * @version V1.0.0
 * @date    05.12.2014
 * @brief   Abstracts the GPIO module.
 *
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, thermocollect
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

#include "GpioIf.h"
#include "stm32f0xx_conf.h"

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

static const GpioIf_ConfigType* GpioIf_userCfg = NULL_PTR;

/****************************************************************
 * User functions
 ***************************************************************/

void GpioIf_Init(const GpioIf_ConfigType* config)
{
  /* Local variables */
  uint8_t port;
  uint8_t pin;
  const GpioIf_PortConfigType* portConfig;
  const GpioIf_PinConfigType* pinConfig;
  GPIO_InitTypeDef gpioPinConfig;
  EXTI_InitTypeDef extiConfig;

  /* Initialize local variables */
  port = 0u;
  pin = 0u;
  portConfig = NULL_PTR;

  assert_param(config != NULL_PTR);
  assert_param(GpioIf_userCfg == NULL_PTR);

  if ((config != NULL_PTR) && (GpioIf_userCfg == NULL_PTR))
  {
    /* Save user configuration */
    GpioIf_userCfg = config;

    /* Iterate through port configurations */
    for (port = 0u; port < config->numberOfPorts; port++)
    {
      /* Get port configuration pointer */
      portConfig = &config->portConfigs[port];
      assert_param(portConfig != NULL_PTR);
      assert_param(portConfig->pinConfigs != NULL_PTR);

      if (portConfig->portUsed)
      {
        /* Enable clock */
        RCC_AHBPeriphClockCmd(portConfig->clockDomain, ENABLE);

        /* Iterate through pin configurations */
        for (pin = 0u; pin < portConfig->numberOfPins; pin++)
        {
          /* Get pin configuration pointer */
          pinConfig = &portConfig->pinConfigs[pin];
          assert_param(pinConfig != NULL_PTR);

          /* Check if pin is used */
          if (pinConfig->pinUsed)
          {
            /* Generate configuration structure for GPIO */
            gpioPinConfig.GPIO_Pin = (uint32_t) (1 << pin);
            gpioPinConfig.GPIO_Mode = pinConfig->mode;
            gpioPinConfig.GPIO_Speed = pinConfig->speed;
            gpioPinConfig.GPIO_OType = pinConfig->outputType;
            gpioPinConfig.GPIO_PuPd = pinConfig->pullUpDownType;

            /* Initialize port */
            GPIO_Init(portConfig->registers, &gpioPinConfig);
            /* Set alternate function, if available */
            GPIO_PinAFConfig(portConfig->registers, pin,
                             pinConfig->alternateFunction);

            /* Initialize ISR if necessary */
            if (pinConfig->isrUsed)
            {
              /* Enable SYSCFG's APB interface clock */
              RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

              /* Connect EXTI to di */
              SYSCFG_EXTILineConfig(port, pin);

              /* Configure di EXTI line */
              extiConfig.EXTI_Line = (1u << pin);
              extiConfig.EXTI_Mode = EXTI_Mode_Interrupt;
              extiConfig.EXTI_Trigger = pinConfig->triggerType;
              extiConfig.EXTI_LineCmd = ENABLE;
              EXTI_Init(&extiConfig);
              /* Clear EXTI line 7 pending bit */
              EXTI_ClearITPendingBit((1u << pin));

              /* Enable di EXTI interrupt */
              NVIC_SetPriority(pinConfig->nvicIrq, 3u); /* Lowest priority */
              NVIC_EnableIRQ(pinConfig->nvicIrq);
            }
          }
        }
      }
    }
  }
}

BOOL GpioIf_GetBit(GpioIf_ChannelType channel)
{
  /* Local variables */
  BOOL ret;
  uint8_t port;
  uint8_t pin;
  const GpioIf_PortConfigType* portConfig;

  /* Check configuration structure */
  assert_param(GpioIf_userCfg != NULL_PTR);

  /* Default return value */
  ret = FALSE;

  /* Calculate port and pin */
  port = (uint8_t) (((uint32_t) channel) / GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(port < GpioIf_userCfg->numberOfPorts);
  pin = (uint8_t) (((uint32_t) channel) % GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(pin < GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);

  /* Get port configuration */
  portConfig = &GpioIf_userCfg->portConfigs[port];
  assert_param(portConfig != NULL_PTR);
  assert_param(pin < portConfig->numberOfPins);

  /* Check if pin is used */
  assert_param(portConfig->pinConfigs[pin].pinUsed);

  /* Depends on the mode which register has to be read */
  switch (portConfig->pinConfigs[pin].mode)
  {
  case GPIO_Mode_IN:
    ret = (BOOL) GPIO_ReadInputDataBit(portConfig->registers,
                                       (uint16_t) (1 << pin));
    break;

  case GPIO_Mode_OUT:
    ret = (BOOL) GPIO_ReadOutputDataBit(portConfig->registers,
                                        (uint16_t) (1 << pin));
    break;

  default:
    /* Wrong pin configuration */
    assert_param(FALSE);
    break;
  }

  return ret;
}

void GpioIf_SetBit(GpioIf_ChannelType channel)
{
  /* Local variables */
  uint8_t port;
  uint8_t pin;
  const GpioIf_PortConfigType* portConfig;

  /* Check configuration structure */
  assert_param(GpioIf_userCfg != NULL_PTR);

  /* Calculate port and pin */
  port = (uint8_t) (((uint32_t) channel) / GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(port < GpioIf_userCfg->numberOfPorts);
  pin = (uint8_t) (((uint32_t) channel) % GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(pin < GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);

  /* Get port configuration */
  portConfig = &GpioIf_userCfg->portConfigs[port];
  assert_param(portConfig != NULL_PTR);
  assert_param(pin < portConfig->numberOfPins);

  /* Check if pin is used */
  assert_param(portConfig->pinConfigs[pin].pinUsed);

  /* Check if the mode is correct */
  switch (portConfig->pinConfigs[pin].mode)
  {
  case GPIO_Mode_OUT:
    GPIO_SetBits(portConfig->registers, (uint16_t) (1 << pin));
    break;

  default:
    /* Wrong pin configuration */
    assert_param(FALSE);
    break;
  }
}

void GpioIf_ClrBit(GpioIf_ChannelType channel)
{
  /* Local variables */
  uint8_t port;
  uint8_t pin;
  const GpioIf_PortConfigType* portConfig;

  /* Check configuration structure */
  assert_param(GpioIf_userCfg != NULL_PTR);

  /* Calculate port and pin */
  port = (uint8_t) (((uint32_t) channel) / GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(port < GpioIf_userCfg->numberOfPorts);
  pin = (uint8_t) (((uint32_t) channel) % GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(pin < GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);

  /* Get port configuration */
  portConfig = &GpioIf_userCfg->portConfigs[port];
  assert_param(portConfig != NULL_PTR);
  assert_param(pin < portConfig->numberOfPins);

  /* Check if pin is used */
  assert_param(portConfig->pinConfigs[pin].pinUsed);

  /* Check if the mode is correct */
  switch (portConfig->pinConfigs[pin].mode)
  {
  case GPIO_Mode_OUT:
    GPIO_ResetBits(portConfig->registers, (uint16_t) (1 << pin));
    break;

  default:
    /* Wrong pin configuration */
    assert_param(FALSE);
    break;
  }
}

void GpioIf_EnterCS(GpioIf_ChannelType channel)
{
  /* Local variables */
  uint8_t port;
  uint8_t pin;
  const GpioIf_PortConfigType* portConfig;

  /* Check configuration structure */
  assert_param(GpioIf_userCfg != NULL_PTR);

  /* Calculate port and pin */
  port = (uint8_t) (((uint32_t) channel) / GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(port < GpioIf_userCfg->numberOfPorts);
  pin = (uint8_t) (((uint32_t) channel) % GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(pin < GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);

  /* Get port configuration */
  portConfig = &GpioIf_userCfg->portConfigs[port];
  assert_param(portConfig != NULL_PTR);
  assert_param(pin < portConfig->numberOfPins);

  /* Check if pin is used */
  assert_param(portConfig->pinConfigs[pin].pinUsed);

  if (portConfig->pinConfigs[pin].isrUsed)
  {
    NVIC_DisableIRQ(portConfig->pinConfigs[pin].nvicIrq);
  }
}

void GpioIf_LeaveCS(GpioIf_ChannelType channel)
{
  /* Local variables */
  uint8_t port;
  uint8_t pin;
  const GpioIf_PortConfigType* portConfig;

  /* Check configuration structure */
  assert_param(GpioIf_userCfg != NULL_PTR);

  /* Calculate port and pin */
  port = (uint8_t) (((uint32_t) channel) / GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(port < GpioIf_userCfg->numberOfPorts);
  pin = (uint8_t) (((uint32_t) channel) % GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);
  assert_param(pin < GPIOIF_NUMBER_OF_PINS_PER_CHANNEL);

  /* Get port configuration */
  portConfig = &GpioIf_userCfg->portConfigs[port];
  assert_param(portConfig != NULL_PTR);
  assert_param(pin < portConfig->numberOfPins);

  /* Check if pin is used */
  assert_param(portConfig->pinConfigs[pin].pinUsed);

  if (portConfig->pinConfigs[pin].isrUsed)
  {
    NVIC_EnableIRQ(portConfig->pinConfigs[pin].nvicIrq);
  }
}

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

void GpioIf_Isr(uint8_t line)
{
  /* Local variables */
  uint8_t i;
  const GpioIf_PinConfigType* pinConfig;

  /* Initialize local variables */
  i = 0u;

  /* Check parameters and configuration */
  if ((line < GPIOIF_NUMBER_OF_PINS_PER_CHANNEL)
    && (GpioIf_userCfg != NULL_PTR))
  {
    /* Iterate through ports and call callback function for ISR handling */
    for (i = 0u; i < GpioIf_userCfg->numberOfPorts; i++)
    {
      /* Get pointer to the pin configuration */
      pinConfig = &GpioIf_userCfg->portConfigs[i].pinConfigs[line];

      /* Check if ISR is used and ISR callback is set */
      if (pinConfig->isrUsed && (pinConfig->isrCallback != NULL_PTR))
      {
        /* Call ISR callback */
        pinConfig->isrCallback();
      }
    }
  }
}

/****************************************************************
 * Static functions
 ***************************************************************/
