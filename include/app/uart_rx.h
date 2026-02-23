/**
 * @file uart_rx.h
 * @brief UART RX interrupt-driven reception for ESP8266 +IPD frames
 *
 * Receives bytes via UART interrupt, parses +IPD,<len>:<payload> frames
 * from the ESP8266, and sends complete payloads into the FreeRTOS
 * MessageBuffer x_message_buffer. The network task blocks on the
 * MessageBuffer and is woken automatically when a new frame is ready.
 *
 * Usage:
 *   1. Call uart_rx_init() once at startup (before RTOS scheduler starts)
 *   2. Use blocking UART for AT commands (init, WiFi, UDP start)
 *   3. Call uart_rx_start() to switch to interrupt-driven reception
 */

#ifndef UART_RX_H
#define UART_RX_H

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"          /* must precede message_buffer.h */
#include "message_buffer.h"

extern MessageBufferHandle_t x_message_buffer;

/*============================================================================
 * Public API
 *============================================================================*/

/**
 * @brief Initialise the UART RX module.
 *
 * Stores the UART handle and resets internal state. Must be called before
 * the RTOS scheduler starts. Does NOT enable interrupts yet.
 *
 * @param huart  HAL UART handle (USART2)
 */
void uart_rx_init(UART_HandleTypeDef* huart);

/**
 * @brief Start interrupt-driven UART reception.
 *
 * Arms the first HAL_UART_Receive_IT call. From this point on, incoming
 * bytes are handled by the ISR and the +IPD state machine. Do NOT use
 * blocking UART receive after calling this.
 */
void uart_rx_start(void);

#endif /* UART_RX_H */
