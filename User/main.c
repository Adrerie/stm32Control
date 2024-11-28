#include "ov7670.h"
#include "code-deen.h"
#include "Library/stm32f10x_gpio.h"
#include "Library/stm32f10x_rcc.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Start/stm32f10x.h"

volatile uint32_t uwTick;

void SysTick_Handler(void) {
    uwTick++;
}

uint32_t HAL_GetTick(void) {
    return uwTick;
}

void HAL_InitTick(void) {
    // Configure SysTick to interrupt every 1ms
    if (SysTick_Config(SystemCoreClock / 1000)) {
        // Handle error
        while (1);
    }
}

void HAL_Delay(uint32_t milliseconds) {
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < milliseconds) {
        // Wait until the specified time has passed
    }
}

void process_command(const char *command) {
    if (strcmp(command, "push") == 0) {
        // 启用预定的函数
        printf("Executing push command\n");
        // 这里添加执行 push 命令的代码
    } else {
        printf("Unknown command: %s\n", command);
    }
}

void receive_command() {
    // 模拟接收指令
    const char *received_command = "push"; // 这里替换为实际接收指令的代码
    process_command(received_command);
}

int main(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);

    OV7670_Init();

    while (1) {
        uint8_t image_buffer[320 * 240]; // Buffer for captured image data
        char encoded_data[320 * 240 * 2]; // Buffer for base64 encoded data
        size_t encoded_size;

        // Capture image
        OV7670_CaptureImage(image_buffer);

        // Encode image
        encode_image_to_base64(image_buffer, sizeof(image_buffer), encoded_data, &encoded_size);

        // 发送编码后的数据
        printf("Sending encoded image data: %s\n", encoded_data);
        // 这里添加发送数据的代码

        // 接收指令
        receive_command();

        HAL_Delay(10000); // 每隔10秒上传一次图片
    }
}