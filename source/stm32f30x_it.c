/**
 * stm32f30x_it.c: Manage interrupt handlers
 */

#include "stm32f30x_it.h"
#include "stm32f30x_exti.h"
#include "controls.h"
#include "remote.h"
#include <string.h>

int i;
extern uint8_t enabled;
static __IO uint32_t TimingDelay;
uint16_t capture = 0;

uint8_t pid_run_flag = 0;

/**
 * User Button
 */
void EXTI0_IRQHandler(void) {
  if ((EXTI_GetITStatus(USER_BUTTON_EXTI_LINE) == SET) && (STM_EVAL_PBGetState(BUTTON_USER) != RESET)) {
    /* Delay */
    for (i = 0; i < 0x7FFFF; i++);

    while (STM_EVAL_PBGetState(BUTTON_USER) != RESET);

    for (i = 0; i < 0x7FFFF; i++);

#ifndef DEBUG
    if (enabled)
      enabled = 0;
    else
      enabled = 1;
#endif

    EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);

  }
}

/**
 * Set LED3 and LED10 on Hard Fault
 */
void HardFault_Handler(void) {
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1) {
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOn(LED10);
  }
}

/**
 * Wait for amount of milliseconds
 * nTime - time to wait in milliseconds
 */
void Delay(__IO uint32_t nTime) {
  TimingDelay = nTime;
  while (TimingDelay != 0);
}

/**
 * Delay Decrementer
 */
void TimingDelay_Decrement(void) {
  if (TimingDelay != 0x00) {
    TimingDelay--;
  }
}

/**
 * SysTick Handler
 */
void SysTick_Handler(void) {
  TimingDelay_Decrement();
}

/**
 * USART3 Interrupt handler
 */
#ifdef BTEN
void USART3_IRQHandler(void) {
  if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
    static uint8_t cnt = 0;
    char t = USART_ReceiveData(USART3);
    if ((t != '\n') && (cnt < CONTROL_MSG_SIZE)) {
      command_bytes[cnt] = t;
      cnt++;
    } else {
      STM_EVAL_LEDOn(LED3);
      command_valid = 0x01;
      // Convert received bytes to command
      controls_format(command_bytes, &command);
      // Reset command bytes
      memset(command_bytes, 0, CONTROL_MSG_SIZE);
      cnt = 0;
    }
  }
}
#endif

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    if (pid_run_flag)
      STM_EVAL_LEDOn(LED10);
    pid_run_flag = 1;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

/**
 * ISR for incoming interrupt IRQ0 from RF module
 */
void EXTI9_5_IRQHandler(void) {
  if (EXTI_GetFlagStatus(EXTI_Line8) != RESET) {
    uint8_t type;
    command.valid = 0;
    command.length = remote_send_byte(SPI_DUMMY_BYTE);
    type = remote_send_byte(SPI_DUMMY_BYTE);
    command.type = (type >> REMOTE_HEADER_RSVD_SIZE) & 0x7;
    remote_read(command.data, (command.length - 2));
    command.valid = 1;
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
}

/**
 * ISR for incoming interrupt IRQ1 from RF module
 */
void EXTI15_10_IRQHandler(void) {
  if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
    // TODO: Process IRQ
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
}
