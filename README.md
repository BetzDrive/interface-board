# Interface Board

This board is intended to connect a chain of 48 VDC RS-485 motor controllers to
the outside world.

## Features

 * USB - RS485 conversion. This is accomplished via the [FT232H](https://www.ftdichip.com/Products/ICs/FT232H.htm) and [ADM2682E](https://www.analog.com/en/products/adm2682e.html)
 IC's. The combination of these chips should provide full electrical isolation
 to the USB host and allow ~10 Mbps throughput to/from RS485.

 * Input power sanitizing. A [LM5066](https://www.ti.com/product/LM5066)
 hotswap controller IC provides sane controlled voltage ramps when the device
 is hot-plugged, and cuts power to the system when it exceeds ~56 volts. An
 input TVS diode starts kicks in around 65 V.

 * Backdrive surge supressing. Sometimes when motors are aggressively
 backdriven by an external power source, significant voltages are generated
 "back" onto the motor bus. The board is equipped to detect this condition and
 dump power through a 10-ohm power resistor. Hysteresis and timing is provided
 by a STM32L071 microcontroller.

## Resources

 * [Schematics in PDF](hardware/blue_interface_board.pdf)
 * KiCAD design files are found in [`hardware`](hardware)
 * fabrication files for rev.A are found in [`hardware/fab`](hardware/fab)

## Renderings

![3d Northwest](/hardware/renderings/3d_northwest_smaller.png)
![3d Southeast](/hardware/renderings/3d_southeast_smaller.png)
![2d](/hardware/renderings/2d_smaller.png)
