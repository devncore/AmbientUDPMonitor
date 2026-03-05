/**
 * @file watchdog.h
 * @brief IWDG watchdog task — periodically refreshes the hardware watchdog.
 *
 * The task initialises the STM32 Independent Watchdog (IWDG) on first run
 * and then refreshes it every CONFIG_WATCHDOG_KICK_MS milliseconds.
 * If any task blocks the scheduler longer than CONFIG_WATCHDOG_TIMEOUT_MS,
 * the watchdog fires and resets the MCU.
 */

#ifndef APP_WATCHDOG_H
#define APP_WATCHDOG_H

/**
 * @brief Watchdog task entry point (osThreadFunc_t).
 *
 * Initialises the IWDG peripheral and enters a loop that refreshes the
 * watchdog counter every CONFIG_WATCHDOG_KICK_MS milliseconds.
 *
 * @param argument  Unused (pass NULL).
 */
void watchdog_task(void *argument);

#endif /* APP_WATCHDOG_H */
