/**
 * @file watchdog.c
 * @brief IWDG watchdog task implementation.
 *
 * Uses the STM32 Independent Watchdog (IWDG) clocked by the internal LSI
 * oscillator (~32 kHz).  Register access is done directly via the CMSIS
 * IWDG peripheral struct — no HAL IWDG driver is required.
 *
 * Timeout calculation (prescaler /256, LSI = 32 kHz):
 *   tick period = 256 / 32000 Hz = 8 ms
 *   reload      = CONFIG_WATCHDOG_TIMEOUT_MS / 8
 *               = 20000 / 8 = 2500  (fits in 12-bit reload register)
 */

#include "app/watchdog.h"
#include "app/config.h"

#include "cmsis_os.h"
#include "stm32f4xx.h"   /* IWDG peripheral definition */

/*============================================================================
 * IWDG register keys (RM0368 §20.4)
 *============================================================================*/

#define IWDG_KEY_RELOAD   0xAAAAU  /**< Refresh the countdown timer */
#define IWDG_KEY_ENABLE   0xCCCCU  /**< Start the watchdog */
#define IWDG_KEY_WRITE    0x5555U  /**< Unlock PR and RLR for writing */

/*============================================================================
 * Prescaler and reload derived from config
 *
 * Prescaler divider value 6 → /256 → 8 ms per tick at 32 kHz LSI.
 *============================================================================*/

#define IWDG_PRESCALER_DIV256   6U
#define IWDG_RELOAD_VALUE       ((CONFIG_WATCHDOG_TIMEOUT_MS) / 8U)

/*============================================================================
 * Task implementation
 *============================================================================*/

void watchdog_task(void *argument)
{
    (void)argument;

    /* ------------------------------------------------------------------ */
    /* Initialise IWDG                                                      */
    /* ------------------------------------------------------------------ */

    /* Unlock prescaler and reload registers */
    IWDG->KR  = IWDG_KEY_WRITE;
    IWDG->PR  = IWDG_PRESCALER_DIV256;
    IWDG->RLR = IWDG_RELOAD_VALUE;

    /* Wait until the prescaler and reload values are updated */
    while (IWDG->SR != 0U)
    {
        /* SR bits clear once the update completes */
    }

    /* Load the reload register and start the watchdog */
    IWDG->KR = IWDG_KEY_RELOAD;
    IWDG->KR = IWDG_KEY_ENABLE;

    /* ------------------------------------------------------------------ */
    /* Kick loop                                                            */
    /* ------------------------------------------------------------------ */

    for (;;)
    {
        osDelay(CONFIG_WATCHDOG_KICK_MS);
        IWDG->KR = IWDG_KEY_RELOAD;
    }
}
