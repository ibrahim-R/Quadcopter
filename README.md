# STM-Quadcopter

A DIY quadcopter with an STM32F3-Discovery

## Goals

To build a self-stabilizing quadcopter controlled via bluetooth.

## Components

* STM32F3-Discovery
* RN42-I/RM Bluetooth module

### Pin Mapping

| Pin  | Function                       |
| ---  | --------                       |
| PC10 | USART3 TX - Bluetooth comm.    |
| PC11 | USART3 RX - Bluetooth comm.    |
| PC12 | GPIO OUT - Reset Bluetooth     |
| PD0  | GPIO IN - Connection Bluetooth |
| PC6  | PWM - Left Front motor         |
| PC7  | PWM - Left Back motor          |
| PC8  | PWM - Right Back motor         |
| PC9  | PWM - Right Front motor        |

## Communication

### Receiving

The data send from the quadcopter has following formatting:

| Byte  | Name    | Value     |
| ---   | ---     | ---       |
| 0     | Sync    | 0x55      |
| 1     | Sync    | 0x55      |
| 2-5   | Roll    | [-π, π]   |
| 6-9   | Pitch   | [-π, π]   |
| 10-13 | Yaw     | [-π, π]   |
| 14-17 | Height  | [0, 10m]  |
| 18-21 | Battery | [0, 100%] |

### Transmitting

| Byte  | Name     | Value       |
| ---   | ----     | ---         |
| 0-3   | Roll     | [-π/6, π/6] |
| 4-7   | Pitch    | [-π/6, π/6] |
| 8-11  | Throttle | [0, 100%]   |
| 12-15 | Yaw      | [-π, π]     |

## Getting Started

### Compiling and programming
  - Download and install Virtualbox and Vagrant
  - Download the virtual machine `vagrant up && vagrant ssh`
  - In the folder `source`: run `make` to build and `make burn` to program the
    device

### Controllers
  - Connect to the bluetooth device
  - Start the application and select the correct device

## License
See `LICENSE`
