/**
 * stm32f30x_it.h
 * Interrupt handlers and timing functions
 */

#include "stm32f3_discovery.h"
#include "stm32f30x_conf.h"
#include "stm32f30x.h"
#include "bluetooth.h"
/**
 * Wait for amount of milliseconds
 * nTime - time to wait in milliseconds
 */
void Delay(__IO uint32_t nTime);

void TimingDelay_Decrement(void);

