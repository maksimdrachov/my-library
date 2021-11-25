## General-Purpose Peripherals

- The interrupt controller
- System time
- Generic timers
- General-purpose IO
- The watchdog

### The interrupt controller

### System time

### Adjusting the flash wait states

If the initalization code is running from flash, it might be necessary to set the wait state for the flash memory before altering the system clocks. If the mcu runs at high requencies, it might require a few ait states in between two consecutive access operations to persistent memory with XIP capabilities[¹]. Failing to set the correct wait states, and matching the ratio between the CPU speed and the access time of the flash, would most likely result in a hard fault. 

After the wait states are set, it is safe to run the code from the flash after setting the CPU frequency at a higher speed, so we can proceed with the actual clock configuration and distribution to the peripherals.

[¹]: In computer science, execute in place (XIP) is a method of executing programs directly from long-term storage rather than copying it into RAM. It is an extension of using shared memory to reduce the total amount of memory required.

### 