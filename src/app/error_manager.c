/**
 * @file error_manager.c
 * @brief Application-level error tracking via a 32-bit bitfield.
 */

#include "app/error_manager.h"

#include "FreeRTOS.h"  // IWYU pragma: keep — must precede all other FreeRTOS headers
#include "task.h"
#include <stdint.h>

#define CALLBACK_ARRAY_SIZE (5)

static void trigger_callbacks(error_id_t id, error_event_t event_type);

/*============================================================================
 * Internal state
 *============================================================================*/

static uint32_t s_error_register = 0U;
static uint8_t s_callback_index = 0U;
static error_event_callback_t s_callbacks[CALLBACK_ARRAY_SIZE];

/*============================================================================
 * API implementation
 *============================================================================*/

void error_set(error_id_t id)
{
    if (id >= ERROR_COUNT) {
        return;
    }
    taskENTER_CRITICAL();
    s_error_register |= (1U << (uint32_t)id);
    taskEXIT_CRITICAL();

    trigger_callbacks(id,ERROR_ADDED);
}

void error_set_from_isr(error_id_t id)
{
    if (id >= ERROR_COUNT) {
        return;
    }
    const UBaseType_t mask = portSET_INTERRUPT_MASK_FROM_ISR();
    s_error_register |= (1U << (uint32_t)id);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);

    trigger_callbacks(id,ERROR_ADDED);
}

void error_reset(error_id_t id)
{
    if (id >= ERROR_COUNT) {
        return;
    }
    taskENTER_CRITICAL();
    s_error_register &= ~(1U << (uint32_t)id);
    taskEXIT_CRITICAL();

    trigger_callbacks(id,ERROR_REMOVED);
}

void error_reset_from_isr(error_id_t id)
{
    if (id >= ERROR_COUNT) {
        return;
    }
    const UBaseType_t mask = portSET_INTERRUPT_MASK_FROM_ISR();
    s_error_register &= ~(1U << (uint32_t)id);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(mask);

    trigger_callbacks(id,ERROR_REMOVED);
}

bool error_is_active(error_id_t id)
{
    if (id >= ERROR_COUNT) {
        return false;
    }
    taskENTER_CRITICAL();
    const bool active = (s_error_register & (1U << (uint32_t)id)) != 0U;
    taskEXIT_CRITICAL();
    return active;
}

uint32_t error_get_all(void)
{
    taskENTER_CRITICAL();
    const uint32_t snapshot = s_error_register;
    taskEXIT_CRITICAL();
    return snapshot;
}

bool error_register_event_callback(error_event_callback_t error_event_callback)
{
    taskENTER_CRITICAL();
    if(s_callback_index>=CALLBACK_ARRAY_SIZE)
    {
        return false;
    }
    s_callbacks[s_callback_index] = error_event_callback;
    s_callback_index++;
    taskEXIT_CRITICAL();

    return true;
}

void trigger_callbacks(error_id_t id, error_event_t event_type)
{
    for(uint8_t i = 0; i<CALLBACK_ARRAY_SIZE; i++)
    {
        if(s_callbacks[i] != NULL)
        {
            (s_callbacks[i])(id,event_type);
        }
    }
}
