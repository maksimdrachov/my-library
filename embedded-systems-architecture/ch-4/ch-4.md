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

Defining a read-only section in flash which is dedicated to the vector table is the only strict requirement for our system to boot up properly, as the address of the `isr_reset` function is retrieved by the CPU at boot time from address 0x04 in memory.

Right after the definition for the text and read-only areas in flash, the linker script should export the value of the current address, which is the beginning of the `.data` section stored in flash. This area contains the initial value of all the global and static variables that have been initialized in the code. In the example linker script, the beginning of the .data section is marked by the linker script variable _stored_data, as follows:

`_stored_data = .;`

The data section will eventually be mapped in RAM, but its initialization is done manually in the `isr_reset` function by copying the content from flash to the actual .data region in RAM. The linker script provides a mechanism to separate the **Virtual Memory Address (VMA)** and the **Load Memory Address (LMA)** for a section, using the keyword AT in the definition of the section. If no AT word is specified, the LMA is by default set to the same address as the VMA. In our case, the VMA of the .data region is in RAM, and exported using the _start_data pointer, which will be used by the isr_vector as destination address when copying the values of the symbols stored from flash. The LMA of .data, though, is located in the flash memory, so we set the LMA address to the _stored_data pointer in flash while defining the .data region:

```
.data : AT (_stored_data)
{
    _start_data = .;
    *(.data*)
    . = ALIGN(4);
    _end_data = .;
} > RAM
```

For .bss, there is no LMA, as no data is stored in the image for this section. When including the .bss region, its VMA will automatically be set to the end of the .data section:

```
.bss :
{
    _start_bss = .;
    *(.bss*)
    . = ALIGN(4);
    _end_bss = .;
    _end = .;
} > RAM
```

Finally, in this design, the linker is expected to provide the initial value for the execution stack. Using the highest address in memory is a common choice for a single-threaded application, even though, as discussed in the next chapter, this may cause problems in the case of stack overflow. For this example, however, there is an acceptable solution, and we define the symbol END_STACK by adding the following line to the linker script:

`END_STACK = ORIGIN(RAM) + LENGTH(RAM);`

To better understand where each symbol will be placed in memory, variable definitions can be added to the startup file in different places within the code. This way, we can check the locations where the variables are stored in memory when running the executable in the debugger for the first time.

The section .rodata, which is empty in this minimalist example, is mapped in the flash memory area, in between .text and the data LMA. This is reserved for constant symbols, because constants do not have to be mapped in RAM. It is advisable to enforce the C modifier `const` when defining constant symbols, because RAM is often our most precious resource, and in some cases even sparing a few bytes of writable memory by moving constant symbols to the flash can make the difference in the project development. 

### Building and running the boot code

### The makefile

A very basic makefile to build our startup application describes the final target (image.bin) and the intermediate steps required to build it. 

The typical syntax to define a target in the makefile is:

```
target: dependencies
    recipe
```

A `target` is the name of the output file being built, followed by:. Dependencies are input files, expected to be found, and checked for existence when Make is invoked. If any of the files listed among dependencies is not available, or is newer than the target itself, Make will look for another target that can satisfy that dependency.

The makefile should contain a few configuration variables to describe the toolchain. The assignment operator = allows you to set values for the variables when invoking the make command. 

```
CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy
```

In the same way, we can define our default flags for the compiler and the linker:

```
CFLAGS=-mcpu=cortex-m3 -mthumb -g -ggdb -Wall -Wno-main
LDFLAGS=-T target.ld -gc-sections -nostdlib -Map=image.map
```

When invoked with no arguments, Make builds the first target defined in the makefile, `image.bin`. A new target for `image.bin` can be defined as follows:

```
image.bin: image.elf
    $(OBJCOPY) -O binary $^ $@
```

The variables $@ and $^ will be replaced by the target and the list of dependencies, respectively.

Similarly, we can define the recipe for image.elf, which is the linking step, depending on the compiled object file startup.o, and the linker script.

```
image.elf: startup.o target.ld
    $(LD) $(LDFLAGS) startup.o -o $@
```

In this case we are not using the $^ variable for the list of dependencies, as the recipe includes the linker script in the linker command line using LDFLAGS. 

Using -nostdlib ensures that no default C libraries are linked automatically to the project, among those available in the toolchain, that would by default be linked in to produce the executables. This ensures that no symbols are automatically pulled. 

The last step for resolving dependencies is compiling the source code into the object file. This target can have an implicit recipe that is automatically expanded. So to generate the target file startup.o, Make will implicitly look for the file startup.c. The recipe gets implicitly defined as:

`$(CC) -c -o $@ $^ $(CFLAGS)`

By defining a clean target, at any point in time it is possible to start over from a clean slate, by removing the intermediate targets and the final image and running make again. 

```
clean:
    rm -f image.bin image.elf *.o image.map
```

Running `make clean` removes all the intermediate and final targets as instructed in the recipe. 

### Running the application

Once the image is built, we can run it on a real target or using `qemu-system-arm`. Since the application will produce no output while running on the emulator, to investigate more on the actual behaviour of the software it is required to attach a debugger to it. When running the emulator, `qemu-system-arm` must be invoked with the -S option, meaning stop, so that it will not start the execution until the debugger is connected. Since the CFLAGS variable in the previous step contains the -g option, all the symbol names will be kept in the .elf so that the debugger can follow the execution through the code line by line, placing breakpoints and checking the values for the variables. 

### Multiple boot stages

The example proposed here shows the seperation between two boot stages, achieved using the functionalities available in many Cortex-M processors. The only purpose of this simple bootloader is to initialize the system for booting the application in the next stage.

### Bootloader

The first stage bootloader starts up as a normal standalone application. Its IV must be located at the beginning of the flash, and the reset handler initializes the associated .data and .bss memory sections, like in a normal single-stage boot. A partition at the beginning of the flash should be reserved for the bootloader .text and .data sections. To do so, the linker script for the bootloader will only include the beginning of the flash memory, and that of the application will have an offset of the same size. 

The bootloader and the application will be in fact built into two seperate binaries. This way, the two linker scripts can have the same name for sections, and differ only by the description of the FLASH partition in the linker memory. Nevertheless, the method suggested next is only one of the possible configurations: a more complex setup may benefit from exporting the full geometry using start addresses and sizes of all the partitions. 

If we want to reserve 4 KB for the bootloader partition, we can hardcode the FLASH area in the bootloader linker script as follows:

`FLASH (rx): ORIGIN = 0x00000000, LENGTH = 0x00001000`

Similarly, the linker script of the application has an offset in the origin, hardcoded to the size of the bootloader, so that the .text section of the application always starts at the address 0x1000. From the application point of view, the whole FLASH area starts from address 0x00001000:

`FLASH (rx) : ORIGIN = 0x00001000, LENGTH = 0x0003F000`

Bootloader and Application run seperate code, and both can define their own IV, based on the handler that will be used in the respective stage. The simplest example of a working bootloader can be realized by hardcoding the address of the application, and jumping to the entry point, being the reset handler in the IV of the application, stored with offset 4 inside the vector table. 

The application may enforce its own memory layout. At startup it will be able to initialize the new .data and .bss sections according to the new geometry, and even define a new initial stack pointer and IV. The bootloader can obtain these two pointers by reading the first two words of the IV stored at address 0x1000:

```
uint32_t app_end_stack = (*((uint32_t *) (APP_OFFSET)));
void (* app_entry) (void);
app_entry = (void *)(*((uint32_t *) (APP_OFFSET + 4)));
```

Before jumping to the entry point of the application, we want to reset the main execution stack pointer to the end address of the stack. Since MSP is a special-purpose CPU register in the ARMv7-M architecture, it can only be written using the assembly instruction **move special from register (msr)**. The following code is inlined in the bootloader to set the correct application stack pointer to the value stored in flash at the beginning of the application image.

`asm volatile("msr msp, %0" ::"r"(app_end_stack));`

In Cortex-M3 and other, more powerful, 32-bit Cortex-M CPUs, a control register is present within the system control block area, which can be used to specify an offset for the vector table at runtime. This is the **Vector Table Offset Register (VTOR)**, which is located at address 0xE000ED08. Writing the application offset to this register means that, from that moment, the new IV is in place and the interrupt handlers defined in the application will be executed upon exceptions:

```
uint32_t * VTOR = (uint32_t *)0xE000ED08;
*VTOR = (uint32_t *)(APP_OFFSET);
```

In Cortex-M, interrupts can be temporarily disabled by using the assembly statement `cpsid i`:

`asm volatile ("cpsid i");`

To enable the interrupt again, the `cpsie i` instruction is used:

`asm volatile ("cpsie i");`

The last action performed by the bootloader in its short life is a direct jump to the reset handler in the application IV. Sicne the function will never return, and a brand new stack space has been just allocated, we force an unconditional jump by setting the value CPU program counter register to start executing from the address of app_entry, that is pointed by the isr_reset:

`asm volatile("mov pc, %0" :: "r" (app_entry));`

### Building the image

The bootloader partition can be filled with zeros upto its size by using the --pad-to option of objcopy when converting from the .elf executable to the binary image. The flash can be reduced by using the 0xFF to fill the padding area, which can be obtained by passing the option --gap-fill=0xFF. The resultant bootloader.bin will be exactly 4096 bytes, so that application image can be concatenated at the end of it. The steps to compose an image containing the two partitions are the following:

```
$ arm-none-eabi-objcopy -O binary --pad-to=4096 --gap-fill=0xFF bootloader.elf bootloader.bin
$ arm-none-eabi-objcopy -O binary app.elf app.bin
$ cat bootloader.bin app.bin > image.bin
```

Looking at the resultant image.bin file with a hexadecimal editor, it should be possible to identify the end of the bootloader within the first partition by recognizing the zero pattern that is used by objdump as padding, and the application code starting at address 0x1000.

By alligning the application offset to the start of a physical page in flash instead, it is even possible to upload the two images in separate steps, allowing you for instance to upgrade the application code, leaving the bootloader partition untouched.

### Debugging a multi-stage system

The separation between two or more stages implies that the symbols of the two executables are linked into different .elf files. Debugging using both sets of symbols is still possible, but the symbols from both .elf files must be loaded in the debugger in two steps. When the debugger is executed using the symbols from the bootloader, by adding the bootloader.elf file as an argument, or using the file command from the GDB command line, the symbols of the bootloader are loaded in the symbol table for the debugging session. To add the symbols from the application .elf file, we can add the corresponding .elf at a later stage, using add-symbol-file.

The directive add-symbol-file, unlike file, ensures that the symbols of a second executable are loaded without overwriting the ones previously loaded, and it requires to specify the address where the .text section is stored in the application .elf file as argument. 

```
> add-symbol-file app.elf 0x1000
add symbol table from file "app.elf" at 
    .text_addr = 0x1000
(y or n) y
Reading symbols from app.elf...done
```

Sharing the same names for sections and symbols between the two executables is legal, as the two executables are self-contained and not linked together. The debugger is aware of duplicate names when we refer to a symbol by its name during debug. For example, if we place a breakpoint on main, and we have correctly loaded the symbols from both executables, the breakpoint will be set on both locations.

### Shared libraries

Suppose that there is a small library providing general-purpose utilities or device drivers, which is in use by both the bootloader and the application. The library can be linked in a dedicated section of the bootloader, and referred to in a latter stage. In our preceding two-stage example, we can safely place the API function pointers in an array starting at address 0x400, which is past the end of the interrupt vector we are currently using. The .utils section is placed in the linker script in between the vector table and the start of .text in the bootloader:

```
.text:
{
    _start_text = .;
    KEEP(*(.isr_vector))
    . = 0x400;
    KEEP(*(.utils))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
    _end_text = .;
} > FLASH
```

The actual function definitions can be placed in a different source file, and linked in the bootloader. What is actually in the .utils section is a table containing the pointers to the actual address of the functions inside the bootloader .text section:

```
__attribute__((section(".utils"), used))
    static void *utils_interface[4] = {
        (void *)utils_open,
        (void *)utils_read,
        (void *)utils_close
    };
```

The application expects to find the function table at the given address:

`static void **utils_interface = (void**) (0x00000400);`

The address of the single stage functions that have been stored in the bootloader are now available, but there is no information about the signature of these functions. For this reason, the application can only access the API properly if the pointers are converted to match the expected function signature. An inline wrapper can then be provided so that the application code can access the function directly. 

```
static inline int utils_read(void *buf, int size) {
    int (*do_read)(void*, int) = (int (*) (void*,int))(utils_interface[2]);
    return do_read(buf, size);
}
```



