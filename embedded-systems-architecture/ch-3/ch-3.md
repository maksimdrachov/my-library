## Architectural Patterns

### Configuration management

- Revision control
- Issue tracking
- Code reviews
- Continuous integration

### Revision control

The latest version in a branch is called **HEAD**.

### Tracking activities

### Code Review

### Continuous integration

As previously mentioned, the test-driven approach is crucial in an embedded environment. Automating the tests is the best way to promptly detect regressions, and defects in general, while the development is ongoing. Using an automation server, such as **Jenkins**, it is possible to plan several actions, or jobs, to run responsively. 

Here are a few examples:

- Unit tests on the development machine
- System validation tests
- Functional tests on a simulated environment
- Funtionanal tests on a physical platform
- Stability tests
- Static code analysis
- Generating documentation
- Tagging, versioning, packaging

### Source code organization

### Hardware abstraction

On our reference target, the support for the hardware components of a generic Cortex-M microcontroller is provided in the form of a library called **Cortex Microcontroller Software Interface Standard (CMSIS)**, distributed by ARM as a reference implementation. Silicon manufacturers derive their specific hardware abstractions by extending CMSIS. An application linked to a target-specific hardware abstraction can access peripherals through its specific API calls, and core MCU functionalities through CMSIS. 

### Middleware

### Application code

### The life cycle of an embedded project

### Defining project steps

Consider having to design an air quality monitor device, with a PM10 air quility serial sensor.

1. Boot a minimal system on the target (empty main loop)
2. Set up a serial port 0 for logging
3. Set up a serial port 1 for the communication to the sensor
4. Set up a timer
5. Write the PM10 sensor driver
6. Application that wakes up every hour and reads from sensor
7. Write flash submodule to store/restore measurements
8. Set up an SPI port to communicate to the radio chip
9. Write the radio driver
10. Implement protocol to communicate with the gateway
11. Every 24 measurements, the application sends daily measurements to the gateway

### Prototyping

As it is part of the specifications, we know that we should prioritize the activities related to the radio communication to allow the hardware to progress on the design, so in this case, the first prototype must:

- Boot a minimal system on the target (empty main loop)
- Set up a serial port 0 for logging
- Set up an SPI port to communicate to the radio chip
- Write the radio driver
- Set up the timer
- Write the main application to test the radio channel (sending raw packets at regular intervals)

### Refactoring

### API and documentation

A clear, readable, and easy-to-understand API is one of the most important features for an embedded system.

