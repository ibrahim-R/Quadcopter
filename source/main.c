/**
 * Quadrocopter STM32 main.c
 * 2014
 */

#include <string.h>
#include "system.h"


#define GYROBUFFER    (3 * 2)
#define COMPBUFFER    (3 * 2)
#define ACCBUFFER     (3 * 2)
#define BUFFERSIZE    (5 * (sizeof(float)))

#ifndef DEBUG
uint8_t enabled = 0;
#else
uint8_t enabled = 1;
#endif


int main(void) {
  // Initialize system
  quadcopter_init();

  uint8_t stats[BUFFERSIZE];
  uint8_t gyro_data[GYROBUFFER] = {0};
  uint8_t comp_data[COMPBUFFER] = {0};
  uint8_t acc_data[ACCBUFFER] = {0};
  float alt_data = 0;
  float bat_data = 0;
  control_t control = {0};
  sensor_data data;
  pid_output_typedef pid_output = {0};

  while (1) {
    /* Loop and wait for interrupts */

    // Read sensors
    gyroscope_read(gyro_data);
    compass_read(comp_data);
    accelerometer_read(acc_data);
    battery_read(&bat_data);

    sensors_format_data(gyro_data, acc_data, comp_data, alt_data, bat_data, &data);

    memcpy(stats, &data.roll, BUFFERSIZE);

#ifdef SERIAL
    if (serial_connected())
      serial_write(stats, BUFFERSIZE);
#endif
    // TODO: Write stats to RF module
    int j = 0;
    for(j = 0; j < 2e5; j++);
    uint8_t dummy[BUFFERSIZE] = {0};
    dummy[0] = 0xDE;
    dummy[1] = 0xAD;
    dummy[2] = 0xBE;
    dummy[3] = 0xEF;

    dummy[4] = 0xCA;
    dummy[5] = 0xFE;
    dummy[6] = 0xBA;
    dummy[7] = 0xBE;

    dummy[8] = 0x26;
    dummy[9] = 0x02;
    dummy[10] = 0x19;
    dummy[11] = 0x92;

    dummy[12] = 0xAA;
    dummy[13] = 0xBB;
    dummy[14] = 0xCC;
    dummy[15] = 0xDD;

    dummy[16] = 0x11;
    dummy[17] = 0x22;
    dummy[18] = 0x33;
    dummy[19] = 0x44;

    remote_write(dummy, BUFFERSIZE);

    if (enabled) {

      // Process commands into controls
      // and optional other configurations
      process_commands(&control);

      // PID tuning
      /* if (pid_run_flag) { */
      /*   pid_compute(PID_PITCH, data.pitch, control.pitch, &pid_output.pitch); */
      /*   pid_compute(PID_ROLL, data.roll, control.roll, &pid_output.roll); */
      /*   pid_compute(PID_YAW, data.yaw, control.yaw, &pid_output.yaw); */
      /*   pid_run_flag = 0; */
      /*   STM_EVAL_LEDOff(LED10); */
      /* } */

      // Convert throttle input to value in motor PWM control range
      // The throttle has a range of [-100, 100]
      // The PWM range is [2300, 4000], below 2300 is cut-off
      // Throttle range is [-600, 600] around an offset of 2200 = [1600, 2800]
      pid_output.throttle = 6 * control.throttle + 2200;


      // Set motor speeds
      motors_pid_apply(pid_output);

    } else {
      // Animation
    }

    // Reset Watchdog
    IWDG_ReloadCounter();
  }
}

