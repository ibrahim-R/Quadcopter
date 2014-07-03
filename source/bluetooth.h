/**
 * bluetooth.h: Serial communication over Bluetooth
 * via RN-42-I/RM
 * 2014, Zeger Van de Vannet
 */

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_usart.h"
#include "stm32f3_discovery.h"
#include "stm32f30x_misc.h"
#include "controls.h"


extern uint8_t command_bytes[CONTROL_MSG_SIZE];

/**
 * Initialize bluetooth communication
 *
 */
void bluetooth_init(void);


/**
 * Reset the RN42 module
 */
void bluetooth_reset(void);

/**
 * Write data to bluetooth module
 * data - char array with data
 * size - size of the data array
 */
void bluetooth_write(uint8_t* data, int size);

