# MotorHandler

The motor handler instanciates and owns all of the motors. It receives the torques calculated by `logic` and preprocesses them before forwarding them to the motors.

## Class overview

| Class | Responsibility |
|--------|---|
| `MotorHandler` | Owns the motors. Passes the torques through the following safety checks before forwarding them to the motors: check for errors/overheating, limit max torque, set torque to 0 if motors are disabled and pass torque through moving average. | 
| `MovingAverage` | Implements a moving average with a time window | 
| `IMitModeMotor` | Interface for using Cubemars motors in MIT mode. Contains methods for entering MIT mode, sending a MIT mode command, parsing the motor reply and getting the position, speed, torque, temperature and error of the motor. |
| `CubemarsAK10-9KV100V2` | Implementation of `IMitModeMotor` for a Cubemars V2 motor. |
| `CubemarsAK10-9KV60V3` | Implementation of `IMitModeMotor` for a Cubemars V3 motor. |

![Class diagram of the MotorHandler layer](motorHandler_dclass.svg)

## Dependencies

- [handmade0octopus/ESP32-TWAI-CAN](https://github.com/handmade0octopus/ESP32-TWAI-CAN.git) - Library to send CAN messages with a ESP32

## Troubleshooting

### Motor is unresponsive

- Verify the motor is powered on. The motor CAN transceiver will be inactive unless powered on.


### V2 motor does not execute MIT mode command

- Enter MIT mode using the `enterMode` command before sending requests.

### V2 motor is not sending data

- Verify MIT mode commands are being sent. The V2 motor only sends data after receiving a command.