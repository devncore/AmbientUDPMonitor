/**
 * @file queue.h  [STUB]
 * @brief Minimal FreeRTOS queue type stub for host-side unit tests.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "portmacro.h"
#include "FreeRTOS.h"

typedef void *QueueHandle_t;

BaseType_t xQueueReceive(QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait);

#endif /* QUEUE_H */
