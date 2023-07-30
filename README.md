# Wolke

## Parts

| Reference | Value                | Description                                  |
| --------- | -------------------- | -------------------------------------------- |
| J1        | DSG240 2,54mm        | Terminal for VCC, buttons and status LED     |
| J2        | DSG240 2,54mm        | Terminal for the LED Strip                   |
| Q1-3      | 2N2904               | Transistors for each colour of the LED Strip |
| R1-3      | 10k                  | Pullup Resistors for Button Inputs           |
| R4        | ?                    | Series Resistor for Status LED               |
| R5-7      | ?                    | Basis Resistor for Transistors               |
| U1        | MC78M05 CTG          | 12V 5V Linear Regulator                      |
| U2        | Seeed Studio ESP32S3 | Microcontroller                              |

## Buttons

| Interaction            | Power                | Mode              | Colour        |
| ---------------------- | -------------------- | ----------------- | ------------- |
| Short Press (< 100 ms) | On / Off             | Change Mode       | -             |
| Long Press (> 100 ms)  | Start dim down timer | Change Brightness | Change Colour |

## Modes

- White Light - Status LED ON
- Coloured Light - Status LED ON
- Fading Colours - Status LED pulses
