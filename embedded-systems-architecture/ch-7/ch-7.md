## Local Bus Interfaces

Communication with other systems in the vicinity of the target is enabled by a few protocols. Most microcontrollers designed for embedded systems support the most common interfaces that control and discipline the access to serial lines. 

- Introducing serial communication
- UART-based asynchronuous serial bus
- SPI bus
- I2C bus

### Introducing serial communication

### Clock and symbol synchronization

Serial protocols that don't foresee shared clock lines are called asynchronous.

Symbol synchronization should instead be explicit. As we expect to send and receive information in byte form, the beginning of each eight bit sequence should be marked either through special preamble sequences on the data line, or by turning the clock on and off at the right time. The symbol synchronization strategy is defined by each protocol differently. 

### Bus wiring


Protocol | Number of wires | Clock | Symbol synchronization strategy | Bit order | Communication mode
---------|----------       |---------|---------                      |---------  |---------
 UART-based | Two (data RX/TX) | Asynchronous | Configurable start/stop bit, parity bit | LSB first | One-to-one or one-to-many, single point-to-point communication
 SPI | Three (master to slave data, slave to master data, clock) | Shared through CLK | Clock activation | MSB first of LSB first | One-to-many single master, multiple slave, using additional slave-selection lines
 I2C | Two (serial data and serial clock) | Shared through SCL | Clock activation, with support for clock stretching | MSB first | One-to-many, master/slave with logic addresses, and dynamic multi-master selection
 USB | Two (D+/D- half-duplex differential signal) | Synchronized with a synchronization pattern at the beginning of the data transfer | CRC/end of packet | LSB first | One host/multiple devices with device enumeration
 CAN bus | Two (CAN-Hi and CAN-Low), differential, half duplex | Asynchronous | Start bit, CRC, and end sequence | MSB first | One-to-many multi-master with address-based master arbitration
 Dallas 1-wire | One data wire | Synchronized with the front of the signal | Slave selection before each byte CRC | LSB first | Master/slave with fixed master, slave discovery with 64-bit addresses

 ### Programming the peripherals

 ### UART-based asynchronous serial bus

Historically used for many different purposes, thanks to the simplicity of its asynchronous nature, UART dates back to the origins of computing, and it is still a very popular circuit used in many contexts. Personal computers up to the early 2000s included at least one RS-232 serial port, realized with a UART controller and the tranceivers allowing to operate at higher voltages. Nowadays USB has replaced serial communication on personal computers, but host computers can still access TTL serial buses using USB-UART peripherals. Microcontrollers have one or more pairs of pins that can be associated with an internal UART controller, and connected to a serial bus to configure a bidirectional, asynchronous, full-duplex communication channel toward a device connected to the same bus. 

### Protocol description

Same settings on both sides:

- The bit rate, expressed in bits per second
- The number of data bits in each symbol (typically eight)
- The meaning of parity bit, if present (O is odd, E is even, and N is not present)

Additionally, the sender must be configured to send a number of stop bits at the end of each transmission. This group of settings is often abbreviated into something such as 115200-8-N-1 to indicate, respectively, a 115.2 Kbps serial line with no parity and one stop bit, and a 38400 line with odd parity and two stop bits.

### Programming the controller

### Hello world!

One of the most useful functions when developing an embedded system is to convert one of the available UARTs into a logging port, where debug messages and other information produced during the execution can be read on the host computer using serial-to-USB converter.

### Newlib printf

Writing pre-formatted strings is not the most ideal API to access a serial port to provide debug messages. Application developers would most certainly prefer if the system exposed a standard C printf function. When the toolchain includes an implementation of a standard C library, it usually gives you the possibility to connect the standard output of the main program to a serial interface. Luckily enough, the toolchain in use for the reference platform allows us to link to `newlib` functions. Similar to what we did in Chapter 5, Memory Management, using the malloc and free functions from newlib, we provide a backend function called `_write()`, which gets the output redirected from the string formatted by all the calls to `printf()`. The _write function implemented here will receive all the strings pre-formatted by `printf()`.

### Receiving data

### Interrupt-based input/output

### SPI bus

The **serial peripheral interface** bus, most commonly known as **SPI**, provides a different approach, based on master/slave communication. As the name suggests, the interface was initially designed to control peripherals. 

### Protocol description

The configuration of the SPI transceiver is very flexible. Usually a transceiver on a microcontroller is able to act as master as well as slave. A number of predefined settings must be known in advance and shared between the master and all the slaves on the same bus:

- The clock polarity, indicating whether the clock tick corresponds to a raising or a falling edge of the clock.
- The clock phase, indicating whether the clock idle position is high or low.
- The length of the data packet, any value between 4 and 16 bits
- The bit order, indicating whether the data is transmitted starting from the most significant bit or the least significant bit.

### Programming the transceiver

### SPI transactions

### Interrupt-based SPI transfers

### I2C bus

The third serial communication protocol analyzed in this chapter is I2C. From the communication strategy point of view, this protocol shares some similarities with SPI. However, the default bit rate for I2C communication is much lower, as the protocol privileges lower-power consumption over throughput.

The two signals must be connected to the high-level voltage of the bus (typically 3.3V) using pull-up resistors. 

### Protocol description

### Clock stretching

We have observed that the master is the only one driving the SCL signal during I2C transactions. This is always true, except when the slave is not yet ready to transmit the requested data from the master. In this particular case, the slave may decide to delay the transaction by keeping the clock line pulled low, which results in the transaction being put on hold.

### Multi-master

I2C offers a deterministic mechanism to detect and react to the presence of multiple masters on the bus, which is again based on the electrical property of the SDA line.

Before initiating any communication, the master ensures that the bus is available by sensing the SDA and SCL lines. The way the START condition is designed can already rule out most of the conflicts. Concurrent start conditions can be interrupted whenever the SDA line is sensed low in the initial grace time in between the two edges. This mechanism alone does not prevent two I2C masters to access the channel at the same time, because conflicts are still possible due to the propagation time of the signal across the wire. 