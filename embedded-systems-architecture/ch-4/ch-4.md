## The Boot-Up Procedure

### The interrupt vector table

The **interrupt vector table**, often abbreviated to **IVT**, is an array of pointers to functions, associated by the CPU to handle specific _exceptions_, such as faults, system service requests from the application, and interrupt requests from peripherals. The IVT is usually located at the beginning of the binary image, and thus stored starting from the lowest address in the flash memory.

The system exceptions that can be handled using seperate service routines in a Cortex-M CPU are:

- Reset
- NMI (Non-Maskable Interrupt)
- Hard fault
- Memory exceptions
- Bus fault
- Usage fault
- Supervisor call
- Debug monitor
- PendSV call
- System tick

### Startup code

In order to boot a workable system, we need to define the interrupt vector, and associate pointers to defined functions. 

```
__attribute__ ((section(".isr_vector")))
void (*const IV[])(void) = 
{
    (void(*) (void)) (END_STACK),
    isr_reset,
    isr_nmi,
    isr_hard_fault,
    isr_mem_fault,
    isr_bus_fault,
    isr_usage,fault,
    0,0,0,0,
    isr_src,
    isr_dbgmon,
    0,
    isr_pendsv,
    isr_systick,
```

From this point on, we define the interrupt lines for the external peripherals, such as:

```
    isr_uart0,
    isr_ethernet,
    // Many more
};
```

The startup code must also include the implementation of every symbol referenced in the array. The handler can be defined as `void` procedures with no arguments, the same format as the signature of the IV:

```
void isr_bus_fault(void) {
    /* Bus Error! */
    while (1);
}
```

### Reset handler

This is a special ISR which does not return, but it rather performs initialization of the .data and .bss sections, and then calls the entry point of the application.The initialization of the .data and .bss sections consists of copying the initial value of the variables in the .data section in flash onto the actual section in RAM where variables are accessed at runtime, and filling the .bss section in RAM with zeros, so that the initial value of static symbols is guaranteed to be zero as per C convention.

Source and destination addresses of the .data and .bss sections in RAM are computed by the linker when generating the binary image, and exported as pointers using the linker script. The implementation of isr_reset may look similar to the following:

```
void isr_reset(void)
{
    unsigned int *src, *dst;
    src = (unsigned int *) &amp;_stored_data;
    dst = (unsigned int *) &amp;_start_data;

    while (dst != (unsigned int *)&amp;_end_data)
    {
        *dst = *src;
        dst++;
        src++;
    }

    dst = &amp;_start_bss;
    while (dst != (unsigned int *)&amp;_end_bss)
    {
        *dst = 0;
        dst++;
    }
    main();
}
```

Once the variables in the .bss and .data section have been initialized, it is finally possible to call the main function, which is the entry point of the application. The application code ensures that main never returns, by implementing an infinite loop.

### Allocating the stack

In order to comply with the **application binary interface (ABI)** of the CPU, it is required to assign space in memory for the execution stack. This can be done in different ways, but usually it is preferable to mark the end of the stack space in the linker script, and associate the stack space to a specific area in RAM not in use by any section.

The address obtained through the END_STACK symbol, exported by the linker script, points to the end of an unused area in RAM. As mentioned earlier, its value must be stored at the beginning of the vector table, at address 0 in our case, just before the IV. The address of the end of the stack has to be constant, and cannot be calculated at runtime, because the IV content is stored in the flash memory and thus cannot be modified later on.

Properly sizing the execution stack in memory is a delicate task that includes the assessment of the whole code base, keeping in mind stack usage from local variables and the depth of the call trace at any time during the execution. The analysis of all the factors related to stack usage and troubleshooting are part of a wider topic that is covered in the next chapter. Our simple startup code provided here has a stack size that is big enough to contain the local variables and the functions call stack, as it is mapped by the linker script as far as possible from the .bss and .data sections. More aspects about the placement of the stack are considered through Chapter 5, Memory management.

### Fault handlers

Fault-related events are triggered by the CPU in the case of execution errors, or policy violations. The CPU is able to detect a number of runtime errors such as:

- Attempting to execute code outside the memory areas marked as executable
- Fetching data or next instructions to execute from an invalid location
- Illegal load or store using an unaligned address
- Division by zero
- Trying to access unavailable coprocessor functionalities
- Attempting to read/write/execute outside the memory areas allowed for the current running mode.

### Memory layout

We integrate the linker script by adding the .isr_vector section at the beginning of .text area, before the rest of the code:

```
.text :
{
    *(.isr_vector)
    *(.text*)
    *(.rodata*)
} > FLASH
```

Defining 