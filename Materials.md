# Materials

This document lists the hardware components used to build the SenS-Device.

## Core electronics

| Qty. | Component | Specification / part number | Manufacturer / source | Notes |
|---:|---|---|---|---|
| 1 | Microcontroller | Arduino Nano RP2040 Connect | Arduino | Main microcontroller |
| 1 | Transistor | BD437, NPN BJT | — | Switches the vibration motors |
| 1 | Resistor | 220 Ω | — | — |
| 2 | Resistor | 100 kΩ | — | — |
| 1 | Resistor | 1 kΩ | — | — |
| 1 | Push button | COM-00097  | Sparkfun | Manual switch for vibration |
| 2 | Capacitor | 10 µF | — | — |
| 1 | Capacitor | 0.1 µF | — | — |

## Sensors and actuators

| Qty. | Component | Specification / part number | Manufacturer / source | Notes |
|---:|---|---|---|---|
| 2 | Force-sensitive resistor (FSR) | — | — | Used for heel contact detection |
| 2 | Eccentric rotating mass (ERM) vibration motor | 320-105 | Precision Microdrives | 20.4 mm diameter × 25 mm length; nominal voltage 3 V |
| 1 | Transistor heatsink | — | — | Mounted to the BD437 |

## Power

| Qty. | Component | Specification / part number | Manufacturer / source | Notes |
|---:|---|---|---|---|
| 1 | Rechargeable battery | 9 V, 1300 mAh | PAISUE / Shenzhen Maxpower Technology Co., Ltd., Shenzhen, China | Main power supply |
| 1 | Battery connector | 9 V battery connection | — | — |

## Connectors

| Qty. | Component | Specification / part number | Notes |
|---:|---|---|---|
| 1 | BNC connector | Straight, screw connection, product no. 76738 | Used for external synchronization |
| 4 | Female-to-male header connector | 2-pin | Used to mount the Arduino to the perfboard while keeping it removable |
| 2 | Female-to-male header connector | 1-pin | Used for removable Arduino/perfboard connection |
| 1 | Female-to-male header connector | 4-pin | Used for removable Arduino/perfboard connection |
| 1 | Female-to-male header connector | 3-pin | Used for removable Arduino/perfboard connection |
| 4 | Screw terminal / cable connector | Exact type/part number not recorded | — |

## Mechanical hardware

| Qty. | Component | Specification | Notes |
|---:|---|---|---|
| 1 | Screw | M3 | Heatsink mounting |
| 1 | Nut | M3 | Heatsink mounting |

## Reference documentation

- [Arduino Nano RP2040 Connect — official product page](https://store-usa.arduino.cc/collections/interactive-games/products/arduino-nano-rp2040-connect)
- [Precision Microdrives 320-105 — official product page](https://precisionmicrodrives.com/products/4d033395-ce29-4dc2-a324-a3c4ccfe9e42/320-105-20mm-vibration-motor-25mm-type)