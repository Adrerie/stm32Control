cmake_minimum_required(VERSION 3.30)
project(stm32ControlCode__ C)

set(CMAKE_C_STANDARD )


# 添加包含目录
include_directories(.)
include_directories(libb64/include)
include_directories(libb64/include/b64)

# 添加库
add_library(b64 STATIC
        ${CMAKE_SOURCE_DIR}/libb64/src/cencode.c
        ${CMAKE_SOURCE_DIR}/libb64/src/cdecode.c
)

# 添加STM32库文件
add_library(stm32f1xx_hal STATIC
        Start/system_stm32f10x.c
        Library/stm32f10x_gpio.c
        Library/stm32f10x_rcc.h

        # 添加其他需要的STM32库文件
)

# 添加可执行文件并链接 b64 和 STM32 库
add_executable(stm32ControlCode__
        User/main.c
        code-deen.c
)

target_link_libraries(stm32ControlCode__ b64 stm32f1xx_hal z)