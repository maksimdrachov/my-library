## Low-Power Optimizations

Reducing the energy demand of an embedded system is often a tricky process. In fact, all the devices on the board may consume power if not deactivated properly. generating high-frequency clocks is one of the most expensive operations, so CPU and bus clocks should only be enabled when in use.

- System configuration
- Measuring power
- Designing low-power embedded applications

### System configuration

External peripherals are often the most power-demanding components, and thus their power source must be interrupted by the microcontroller when they are not being used.

### Hardware design

In low-power embedded systems, the hardware design must include the possibility to power on/off peripherals using a GPIO pin. This is better done using a line that is normally low, so that it can be pulled down using passive components when the GPIO is not driven by the microcontroller. Mosfets are often used to control the power supplied to external peripherals, using a GPIO signal to control the gate voltage.

Even when peripherals are turned off by interrupting their power source line, smaller currents may leak through other signals connected to them, such as a serial bus or other control signals. The hardware design must be able to detect and identify these leakages in early prototyping stages in order to minimize the energy lost this way.

Additionaly, if the power-saving strategy includes the possibility to put the microprocessor in a deep-sleep operation mode, input signals' logic must be tailored to provide the correct wake-up events to resume the normal operation. Signals that might not be driven while in sleep mode must maintain a known logic value enforced through passive components. 

### Clock management

Internal peripherals and interfaces that are not in use must also stay off. If the platform supports it, clock gating is generally the mechanism used to selectively control the clock source for each peripheral and interface on the system. Each clock line enabled in the system clock gating configuration increases the power usage. Furthermore, the higher the scaling factor applied to generate the CPU clock from a slow oscillator. the higher the energy required by the PLL. The PLL is one of the most power-demanding components of the system, and the power consumed by the CPU is also directly proportional to its clock frequency.

Every change of the system clock requires a reconfiguration of all the clock dividers for all timers and peripherals currently in use. 

Running the system at a lower speed offers other benefits, such as the possibility to decrease the number of wait states required to access the flash memory, and to enable extra low-power features that are only available when the system is not running at full speed.

An embedded platform usually includes low-frequency clock generators, in the kHz range, that may be used as sources for time keeping devices such as watchdogs and **real-time clock (RTC)**. External and internal oscillators can be active during low-power operating modes, and used to implement wake-up strategies. 

### Voltage control

An important aspect that is often neglected is the power consumed by the Schmitt triggers in the digital input logic. When GPIOs are configured as digital input, but not forced to a known logic state through external passive components, they might be floating around the average value, due to the electromagnetic files in the environment. This causes the triggering of the input signal, resulting in a little energy being lost at every change in the logic state. 

### Low power operating modes

In an ARM-based microcontroller, the terminology used for the different low-power modes can be summarized as follows:

- **Normal operation mode**: Active components are selected through clock gating, and the clock is running at the desired frequency. 
- **Sleep mode**: The CPU clock is temporarily suspended, but all the peripherals keep functioning as in normal mode. As long as the CPU is not executing, there is a noticeable, even if marginal, amount of power saved in this mode. Execution can be resumed after receiving an interrupt request. This mode is also referred to as _wait mode_ by some manufacturers.
- **Stop mode**: The CPU clock and the bus clock are disabled. All the peripherals powered by the microcontroller are off. The internal RAM and the CPU registers retain the stored values, because the main voltage regulator stays on. The power consumption drops consistently, but it is still possible to wake up and resume the execution through an external interrupt or event. 
- **Standby mode**: All the voltage regulators are off, and the content of RAM and register is lost. A small amount of power, in the range of a few microwatts, can be required to keep the back circuitry alive during the standby phase. Wake up is then only possible under a few specific conditions, suchas an externally powered RTC, or a 

### Measuring power

The current in use by the target can be measured at any time by connecting an ammeter in series with the device. This mechanism, however, does not show all the oscillations of the values during a time interval, which is why it is often useful to sample the values of the parasitic voltage at the ends of a shunt resistor, using an oscilloscope.

### Development boards

In order to see the effects of the power optimizations, we must exclude the electronics that are not related to the system. Our reference board, for example, the STM32F407-Discovery, has an additional microcontroller that is used to provide a debug interface towards the host, and it is powered using the same USB connector.

On our reference board, the jumper JP1 can be used to open the circuit between the power supply and the microcontroller circuitry. Replacing the jumper with an ammeter connected to the two pins, we can measure the current used by the actual system. In the same way, it is possible to apply a shunt resistor to monitor the current, using the oscilloscope to sample the voltage on the shunt.

### Designing low-power embedded applications

In this section, a few design patterns are proposed to achieve a better energy profile on the target device, by evaluating the power demand of all the components and the states of the system we are about to design. Once we know how to measure the values in the target, and the details about the low-power mode in the selected architecture and microprocessor family, the application can be programmed keeping other parameters in mind, such as the energy efficieny of the software we are writing.

### Replacing busy loops with sleep mode

### Deep sleep during longer inactivity periods

### Power state transitions


