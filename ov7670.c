#include "ov7670.h"
#include "Library/stm32f10x_i2c.h"
#include "Library/stm32f10x_rcc.h"
#include "Library/stm32f10x_gpio.h"
#include "libb64/include/b64/cencode.h"
#include <stdlib.h>
#include <string.h>

void OV7670_I2C_Init() {
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000; // 100kHz

    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

void OV7670_WriteRegister(uint8_t reg, uint8_t value) {
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, 0x42, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, reg);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

    I2C_SendData(I2C1, value);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}

void OV7670_Init(void) {
    OV7670_I2C_Init();

    // Example register configuration
    OV7670_WriteRegister(0x12, 0x80); // Reset register
    HAL_Delay(100);

    OV7670_WriteRegister(0x12, 0x14); // Set QVGA resolution
    OV7670_WriteRegister(0x11, 0x01); // Set clock prescaler
    // Add more register configurations as needed
}

void OV7670_CaptureAndEncodeImage(char *encoded_data, size_t *encoded_size) {
    // Capture image data
    uint8_t image_buffer[320 * 240]; // Example buffer size for QVGA resolution
    // Capture image data into image_buffer (implementation depends on your hardware setup)

    // Encode image to base64
    base64_encodestate state;
    base64_init_encodestate(&state);
    *encoded_size = base64_encode_block((const char *)image_buffer, sizeof(image_buffer), encoded_data, &state);
    *encoded_size += base64_encode_blockend(encoded_data + *encoded_size, &state);
}