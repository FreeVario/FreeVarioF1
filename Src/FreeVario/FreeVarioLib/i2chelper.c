/*
 FreeVario http://FreeVario.org

  Copyright (c), PrimalCode (http://www.primalcode.org)
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>

  i2chelper: resets I2C bus if held busy
  Original code: https://electronics.stackexchange.com/questions/267972/i2c-busy-flag-strange-behaviour/281046#281046

*/

#include "i2chelper.h"



 uint8_t wait_for_gpio_state_timeout(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state, uint32_t timeout)
 {
    uint32_t Tickstart = HAL_GetTick();
    uint8_t ret = true;
    /* Wait until flag is set */
    for(;(state != HAL_GPIO_ReadPin(port, pin)) && (true == ret);)
    {
        /* Check for the timeout */
        if (timeout != HAL_MAX_DELAY)
        {
            if ((timeout == 0U) || ((HAL_GetTick() - Tickstart) > timeout))
            {
                ret = false;
            }
            else
            {
            }
        }
        asm("nop");
    }
    return ret;
}

void I2C_ClearBusyFlagErratum(I2C_Helper_t* i2c, uint32_t timeout)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    I2C_HandleTypeDef* handler = NULL;

    handler = i2c->instance;

    // 1. Clear PE bit.
    CLEAR_BIT(handler->Instance->CR1, I2C_CR1_PE);

    //  2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
    HAL_I2C_DeInit(handler);

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    GPIO_InitStructure.Pin = i2c->sclPin;
    HAL_GPIO_Init(i2c->i2cPort, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = i2c->sdaPin;
    HAL_GPIO_Init(i2c->i2cPort, &GPIO_InitStructure);

    // 3. Check SCL and SDA High level in GPIOx_IDR.
    HAL_GPIO_WritePin(i2c->i2cPort, i2c->sdaPin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(i2c->i2cPort, i2c->sclPin, GPIO_PIN_SET);

    wait_for_gpio_state_timeout(i2c->i2cPort, i2c->sclPin, GPIO_PIN_SET, timeout);
    wait_for_gpio_state_timeout(i2c->i2cPort, i2c->sdaPin, GPIO_PIN_SET, timeout);

    // 4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
    HAL_GPIO_WritePin(i2c->i2cPort, i2c->sdaPin, GPIO_PIN_RESET);

    // 5. Check SDA Low level in GPIOx_IDR.
    wait_for_gpio_state_timeout(i2c->i2cPort, i2c->sdaPin, GPIO_PIN_RESET, timeout);

    // 6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
    HAL_GPIO_WritePin(i2c->i2cPort, i2c->sclPin, GPIO_PIN_RESET);

    // 7. Check SCL Low level in GPIOx_IDR.
    wait_for_gpio_state_timeout(i2c->i2cPort, i2c->sclPin, GPIO_PIN_RESET, timeout);

    // 8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
    HAL_GPIO_WritePin(i2c->i2cPort, i2c->sclPin, GPIO_PIN_SET);

    // 9. Check SCL High level in GPIOx_IDR.
    wait_for_gpio_state_timeout(i2c->i2cPort, i2c->sclPin, GPIO_PIN_SET, timeout);

    // 10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to GPIOx_ODR).
    HAL_GPIO_WritePin(i2c->i2cPort, i2c->sdaPin, GPIO_PIN_SET);

    // 11. Check SDA High level in GPIOx_IDR.
    wait_for_gpio_state_timeout(i2c->i2cPort, i2c->sdaPin, GPIO_PIN_SET, timeout);

    // 12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Alternate = GPIO_AF4_I2C2;

    GPIO_InitStructure.Pin = i2c->sclPin;
    HAL_GPIO_Init(i2c->i2cPort, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = i2c->sdaPin;
    HAL_GPIO_Init(i2c->i2cPort, &GPIO_InitStructure);

    // 13. Set SWRST bit in I2Cx_CR1 register.
    SET_BIT(handler->Instance->CR1, I2C_CR1_SWRST);
    asm("nop");

    /* 14. Clear SWRST bit in I2Cx_CR1 register. */
    CLEAR_BIT(handler->Instance->CR1, I2C_CR1_SWRST);
    asm("nop");

    /* 15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register */
    SET_BIT(handler->Instance->CR1, I2C_CR1_PE);
    asm("nop");

    // Call initialization function.
    HAL_I2C_Init(handler);
}

