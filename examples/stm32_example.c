/**
 * STM32 超声波测距示例代码
 * 
 * 开发环境: STM32CubeIDE / Keil
 * MCU: STM32F103C8T6 (可适配其他型号)
 * 
 * 硬件连接:
 * - HC-SR04 Trig -> PA0 (TIM2_CH1)
 * - HC-SR04 Echo -> PA1 (GPIO Input)
 * - UART1 TX -> PA9
 * - UART1 RX -> PA10 (可选)
 * 
 * CubeMX 配置:
 * - UART1: 9600 baud, 8N1
 * - TIM2: 1MHz (1us per tick)
 * - SysTick: 1ms
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

// 外部变量（CubeMX 生成）
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim2;

// 超声波引脚定义
#define TRIG_PORT GPIOA
#define TRIG_PIN  GPIO_PIN_0
#define ECHO_PORT GPIOA
#define ECHO_PIN  GPIO_PIN_1

// 函数声明
float measureDistance(void);
void sendDistanceToPC(float distance);
void delayMicroseconds(uint32_t us);

/**
 * 主循环
 */
void ultrasonicTask(void)
{
    while (1) {
        // 测量距离
        float distance = measureDistance();
        
        // 发送到上位机
        sendDistanceToPC(distance);
        
        // 延时 100ms
        HAL_Delay(100);
    }
}

/**
 * 测量距离（单位：厘米）
 */
float measureDistance(void)
{
    uint32_t startTime, endTime;
    float distance;
    
    // 1. 发送 10us 触发脉冲
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
    delayMicroseconds(2);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delayMicroseconds(10);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
    
    // 2. 等待回波信号上升沿（超时保护）
    uint32_t timeout = HAL_GetTick() + 30;  // 30ms 超时
    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET) {
        if (HAL_GetTick() > timeout) {
            return 0.0f;  // 超时返回 0
        }
    }
    
    // 3. 记录开始时间
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_Base_Start(&htim2);
    startTime = __HAL_TIM_GET_COUNTER(&htim2);
    
    // 4. 等待回波信号下降沿
    timeout = HAL_GetTick() + 30;
    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET) {
        if (HAL_GetTick() > timeout) {
            HAL_TIM_Base_Stop(&htim2);
            return 0.0f;
        }
    }
    
    // 5. 记录结束时间
    endTime = __HAL_TIM_GET_COUNTER(&htim2);
    HAL_TIM_Base_Stop(&htim2);
    
    // 6. 计算距离
    uint32_t duration = endTime - startTime;  // 单位：微秒
    distance = (float)duration * 0.017f;      // 声速 340m/s
    
    // 7. 范围检查
    if (distance < 2.0f || distance > 400.0f) {
        distance = 0.0f;
    }
    
    return distance;
}

/**
 * 发送距离数据到上位机
 */
void sendDistanceToPC(float distance)
{
    char buffer[32];
    
    // 格式 1: 带前缀（推荐）
    sprintf(buffer, "D:%.2f\r\n", distance);
    
    // 格式 2: 纯数字
    // sprintf(buffer, "%.2f\r\n", distance);
    
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
}

/**
 * 微秒级延时（使用 TIM2）
 */
void delayMicroseconds(uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_Base_Start(&htim2);
    
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
    
    HAL_TIM_Base_Stop(&htim2);
}

/**
 * 重定向 printf 到 UART（可选）
 */
#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}
#endif
