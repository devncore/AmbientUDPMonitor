/**
 * @file error_manager_stubs.c
 * @brief No-op stubs for error_manager API used in host-side unit tests.
 */

#include "app/error_manager.h"

void  error_set(error_id_t id)              { (void)id; }
void  error_set_from_isr(error_id_t id)     { (void)id; }
void  error_reset(error_id_t id)            { (void)id; }
void  error_reset_from_isr(error_id_t id)   { (void)id; }
bool  error_is_active(error_id_t id)        { (void)id; return false; }
uint32_t error_get_all(void)                { return 0U; }
bool  error_register_event_callback(error_event_callback_t cb) { (void)cb; return true; }
