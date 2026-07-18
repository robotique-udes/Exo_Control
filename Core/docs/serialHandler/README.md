# SerialHandler

The serial handler offers manual control of the torques by reading commands received from the serial port.

The commands are: <br>
w: Increase the selected motor's torque by 1 <br>
s: Decrease the selected motor's torque by 1 <br>
d: Select the next motor ID <br>
a: Select the previous motor ID <br>
r: Restart the motherboard <br>

## Class overview

| Class | Responsibility |
|--------|---|
| `SerialHandler` | Reads commands from the serial port . | 

![Class diagram of the MotorHandler layer](serialHandler_dclass.png)

## Dependencies

No dependencies.

## Troubleshooting

No known issues.