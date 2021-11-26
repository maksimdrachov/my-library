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

### Clock configuration

The configuration of the clocks in Cortex-M mcu happens through the **Reset and Clock Control (RCC)** registers, located at a specific address within the internal peripheral region.

### Clock distribution

Once the bus clock are available, the RCC logic can be programmed to distribute the clock to single peripherals. 

### Enabling the Systick

Once a stable CPU frequency has been set up, we can configure the main timer on the system, the SysTick. 

The system timer that we have configured is the same as that used by real-time operating systems to initiate process switches. In our case, it might be helpful to keep a monotonic system wallclock, measuring the time elapsed since the clock configuration. A minimalist implementation of the interrupt service routine for the system timer could be as follows:

```
volatile unsigned int jiffies = 0;
void isr_systick(void)
{
    ++jiffies;
}
```

This simple function, and the associated global `volatile` variable associated, are sufficient to keep track of the time transparently while the application is running. In fact, the system tick interrupt happens independently, at regular intervals, when the `jiffies` variable is incremented in the interrupt handler, without altering the flow of the main application. What actually happens is that every time the system tick counter reaches 0, the execution is suspended, and the interrupt routine quickly executes. When `isr_systick` returns, the flow of the main application is resumed by restoring exactly the same context of execution stored in memory a moment before the interrupt occured. 

The reason why the system timer variable must be defined and declared everywhere as `volatile` is that its value is supposed to change while executing the application in a way that is independent of the behavior possibly predicted by the compiler for the local context of execution. The keyword `volatile` in this case ensurs that the compiler is forced to produce code that checks the value of the variable every time it is instantiated, by disallowing the use of optimizations based on the false assumption that the variable is not being modified by the local code. 

The shortcut for the assembly instruction **WFI**, short for **wait for interrupt**, is defined. It is used in the main application to keep the CPU inactive until the next interrupt occurs:

`#define WFI() asm volatile ('wfi')`

### Generic timers

we mark the `timer2_ticks` variable as `volatile` so it does not get optimized out by the compiler, since it is never used in the code:

```
void isr_tim2(void)
{
    static volatile uint32_t timer2_ticks = 0;
    TIM2_SR &= ~TIM_SR_UIF;
    timer2_ticks++;
}
```

### General-purpose IO

### Digital output

### PWM

### Digital input

A GPIO configured in input mode detects the logic level of the voltage applied to it. The logic value of all the input pins on a GPIO controller can be read from the **input data register (IDR)**. On the reference board, the pin A0 is connected to the user button, so the status of the button can be read at any time while the application is running.

### Interrupt-based input

Having to proactively read the value of the pin by constantly polling the IDR is not convenient in many cases, where the application is supposed to react to state changes. Microcontrollers usually provide mechanisms to conenct digital input pins to interrupt lines, so that the application can react in real time to events related to the input because the execution is interrupted to execute the associated service routine. 

### Analog input 

Some pins have the possibility to measure the applied voltage dynamically, and assign a discrete number the measured voltage, using an **analog to digital signal converter**, or **ADC**. This is very useful to acquire data from a wide range of sensors, capable of conveying the information as output voltage, or simply using a variable resistor.

### The watchdog

Another common feature in many microcontrollers is the presence of a watchdog timer. A watchdog ensures that the system is not stuck within an endless loop or any other blocking situation within the code. This is particularly useful in bare-metal applications that rely on an event-driven loop, where calls are required not to block, and to return to the main event loop within the allowed amount of time. 

The watchdog must be seen as the very last resort to recover an unresponsive system, by triggering a forced reboot regardless of the current state of the execution in the CPU.

The reference platform provides one independent watchdog timer, with a counter similar to those of the generic timers, with a 12-bit granularity and a prescaler factor. The prescaler of the watchdog, however, is expressed in multiples of 2, and has a range between 4 (represented by the value 0) and 256 (value 6).

Once started, the watchdog cannot be stopped and will run forever, decreasing the counter until it reaches zero, and rebooting the system.

The only way to prevent the system from being rebooted is resetting the timer manually, an operation often referred to as _kicking the watchdog_. A watchdog driver should export a function that allows the application to reset the counter, for example, at the end of each iteration in the main loop.

