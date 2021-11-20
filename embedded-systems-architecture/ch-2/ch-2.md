## Chapter 2: Work Environment and Workflow Optimization

Building the firmware image of an embedded system relies on a set of tools, which can produce firmware images for specific targets, called a **toolchain**.

### C compiler

GCC: GNU Compiler Collection

The GCC included in the system can compile C code into applications and libraries capable of running on the same architecture as the one of the machine running the compiler.

The GCC compiler takes source code files as input, with the .c extension, and produces object files, with .o extensions, containing the functions and the initial values of the variables, translated from the input source code into machine intstructions. The compiler can be configured to perform additional optimization steps at the end of the compilation, which are specific for the target platform, and insert debug data to facilitate debugging at a later stage.

```
$ gcc -c hello.c
```

This statement will try to compile the C source contained in the `hello.c` file and transform it into machine-specific code, that is stored in the newly created `hello.o` file.

Architecture-specific compilers exists, which provide compilers creating machine instructions for a specific target,  different from the building machine. The process of generating code for a different target is called **cross-compilation**. The cross-compiler runs on a development machine, the host, to produce machine-specific code that can execute on the target.

### Linker

The linker is a tool that can compose an executable program, resolving all the dependencies among the symbols used by the modules, represented by the collection of object files provided as input. The executable that is produced by the linker is an ELF executable. **ELF** stands for **Executable and Linkable Format**, and it is the default standard format for programs, objects, and shared libraries, and even GDB code dumps on many Unix and Unix-like systems.

ELF sections:

- `.text`: Containing the code of the program, accessed in read-only.
- `.rodata`: Containing the value of constants that cannot be altered at runtime.
- `.data`: Containing the values of all the initialized variables of the program, accessible in the read/write mode at runtime.
- `.bss`: A section reserved for uninitialized data, accessible in the read/write mode at runtime.

### Build automation

**Make** is the standard UNIX tool to automate the steps required to create the binary images from the sources, checking the dependencies for each component, and executing the steps in the right order.

Makefiles have a specific syntax to describe rules. Each rule begins with the expected target files, expected as the output of the rule, a colon, and the list of prerequisites, which are the files necessary to execute the rule. A set of recipe items follow, each one describing the actions that Make will execute to create the desired target.

```
target: [prerequisites]
    recipe
    recipe
    ...
```

Makefiles can assign a custom string of text to internal variables while executing. For example the following assignment is used to put the name of two object files into the OBJS variable:

```
OBJS = hello.o world.o
```

A few important variables that are assigned automatically within the rules are the following:

Variable | Meaning
---------|----------
 $(@) | Name of the target for the currently executing rule 
 $(^) | List of all the prerequisites for this rule, without duplicates
 $(+) | List of all the prerequisites for this rule, with duplicates if any
 $(<) | First element in the prerequisites list

These variables are handy to use within the recipe action lines. For example, the recipe to generate `helloworld` ELF file from the two object files can be written as follows:

```
helloworld: $(OBJS)
    gcc -o $(@) $(^)
```

Here is a small list of the most important variables that may be used to alter imlicit rules and recipes:

Variable | Meaning | Default value
---------|----------|---------
 CC | Compiler program | cc
 LD | Linker program | ld
 CFLAGS | Flags passed to the compiler when compiling sources | empty
 LDFLAGS | Flags passed to the compiler in the linking step | empty

 ### Debugger

The default debugging option provided by the GCC suite is called **GDB**, an acronym for the **GNU Debugger**. 

A debug session on a remote target requires an intermediate tool that is configured to translate GDB commands into actual actions on the core CPU, and the related hardware infrastructure, to establish the communication with the core.

### Embedded workflow

Intercepting fatal errors on an embedded target, on the other hand, might be slightly more challenging because of the potential side effect of memory and register corruption, in the absence of virtual addresses[^1] and memory segmentation[^2]. Even if some targets are able to intercept abnormal situations by triggering diagnostic interrupts, such as the Hard Fault handler in Cortex-M, restoring the original context that generated the error is often impossible. 

Finally, developing a distributed embedded system composed of a number of devices running different software images may result in repeating the preceding iterations for each of these devices. Whenever possible, these steps should be eliminated by using the same image and different set configuration parameters on each device, and by implementing parallel firmware upgrade mechanisms. 

[^1]: In computing, a virtual address space (VAS) or address space is the set of ranges of virtual addresses that an operating system makes available to a process. The range of virtual addresses usually starts at a low address and can extend to the highest address allowed by the computer's instruction set architecture and supported by the operating system's pointer size implementation, which can be 4 bytes for 32-bit or 8 bytes for 64-bit OS versions. This provides several benefits, one of which is security through process isolation assuming each process is given a seperate address space. (wikipedia)

[^2]: Memory segmentation is an operating system memory management technique of division of a computer's primary memory into segments or sections. In a computer system using segmentation, a reference to a memory location includes a value that identifies a segment and an offset (memory location) within that segment. Segments of sections are also used in object files of compiled programs when they are linked together into a program image and when the image is loaded into memory.

### The GCC toolchain

Builing software using a command-line-based toolchain has several advantages, including the possibility of automating the intermediate steps that would build all the modules up from the source code into the final image. This is particularly useful when it is required to program multiple devices in a row, or to automate builds on a continuous integration server. 

In the case of the GNU Arm Embedded Toolchain, the prefix is arm-none-eabi, indicating that the cross-compiler backend is configured to produce objects for ARM, with no specific support for an operating system API, and with the embedded ABI.

### The cross-compiler

The following table lists some of the ARM-specific machine options available on the GCC backend as -m flags:


Option | Description 
---------|----------
 -marm / -mthumb | selects ARM or Thumb instruction sets 
 -march=name | Selects the architecture name within the family (for example, armv7)
 -mtune=name | Selects the CPU name for which GCC is optimized (for example, cortex-m3)
 -mcpu=name | Selects the CPU name for optimizations and architecture (can be used instead of -march and -mtune)

The compiler is in fact capable of creating intermediate assembly code files, instead of compiled and assembled objects, when it is invoked with the -S option.

`arm-none-eabi-gcc -c test.c -mthumb -mcpu=cortex-m4 -S`

The optimization parameter can be provided at the command line to select the desired optimization level:

Option | Description 
---------|----------
-O0 | Do not optimize
-O1 | Optimize for performance
-O2 | Optimize even more
-O3 | Maximum level of performance optimization
-Os | Optimize for size

Another generic GCC command-line option that is often used while debugging and prototyping is the -g flag, which instructs the compiler to keep the debugging-related data in the final object, in order to facilitate the access to functions and variables readable handles while running within the debugger.

To inform the compiler that we are running a bare-metal application, the -ffreestanding command-line option is used. In GCC jargon, a freestanding environment is defined by the possible lack of a standard library in the linking step, and most importantly, this option alerts the compiler that is should note expect to use the main function as the entry point of the program or provide any preamble code before the beginning of the execution. This option is required when compiling code for the embedded platforms, as it enables the boot mechanism described in Chapter 4, The Boot-up Procedure.

For more command-line options: gcc.gnu.org/onlinedocs/

To integrate the cross-compiling toolchain in the automated build using Make, a few changes are required in the makefile.

`CC=arm-none-eabi-gcc`

The custom command-line options required to run the compile options may be exported through the CFLAGS variable:

`CFLAGS=-mthumb -mcpu=cortex-m4 -ffreestanding`

### Compiling the compiler

Binary distributions of the GCC toolchain are available to download for a number of specific targets and host machines. To compile the code for the ARM Cortex-M microprocessors, the arm-none-eabi toolchain is made available for most GNU/Linux distributions. However, in some cases, it might be useful to build the toolchain entirely from the sources. This might be, for example, when the compiler for a certain target does not exist yet or is not shipped in binary format for our favorite development environment. This process is also useful to better understan the various components that are required to build the tools.

Crosstool-NG is an open source project, which consists of a set of scripts aimed to automate the process of creating a toolchain. 

COME BACK TO THIS WITH LINUX

### Linking the executable

Linking is the last step in the creation of the ELF file. The cross-compiling GCC groups all the object files together and resolves the dependencies among symbols. By passing the `-T filename` option at the command line, the linker is asked to replace the default memory layout for the program with a custom script, contained in the filename. 

The linker script is a file containing the description of the memory sectiosn in the target, which need to be known in advance in order for the linker to place the symbols in the correct sections in flash, and instruct the software components about special locations in the memory mapping area that can be referenced in the code. The file is recognizable by its `.ld` extension, and it is written in a specific language. As a rule of thumb, all the symbols from every single compiled object are grouped in the sections of the final executable image.

The script can interact with the C code, exporting symbols defined within the scripts, and following indications provided in the code using GCC-specific attributes associated with symbols. The `__attribute__` keyword is provided by GCC to be put in front of the symbol definition, to activate GCC-specific, non-standard attributes for each symbol. 

Some GCC attributes can be used to communicate to the linker about:

- Weak symbols, which can be overridden by symbols with the same name
- Symbols to be stored in a specific section in the ELF file, defined in the linker script
- Implicitly used symbols, which prevent the linker from discarding the symbol because it is referred to nowhere in the code. 

The `weak` attribute is used to define weak symbols, which can be overridden anywhere else in the code by another definition with the same name. Consider, for example, the following definition:

`void __attribute__(weak) my_procedure(int x) { /* do nothing */ }`

In this case, the procedure is defined to do nothing, but it is possible to override it anywhere else in the code base by defining it again, using the same name, but this time without the `weak` attribute:

`void my_procedure(int x) { y = x; }`

The linker step ensures that the final executable contains exactly one copy of each defined symbol, which is the one without the attribute, if available. This mechanism introduces the possibility of having several different implementations of the same functionality within the code, which can be altered by including different object files in the linking phase. This is particularly useful when writing code that is portable to different targets, while still maintaining the same abstractions. 

Besides the default sections required in the ELF description, custom sections may be added to store specific symbols, such as functions and variables, at fixed memory addresses. This is useful when storing data at the beginning of a flash page, which might be uploaded to flash at a different time than the software itself. This is the case for target-specific settings in some cases. 

Using the custom GCC attribute `section` when defining a symbol ensures that the symbol ends up at the desired position in the final image. Sections may have custom names, as long as an entry exists in the linker to locate them. The `section` attribute can be added to a symbol definition as follows:

```
const uint8_t
__attribute__((section(".keys")))
private_key[KEY_SIZE] = {0};
```

In this example, the array is placed in the .keys section, which requires its own entry in the linker script as well.

It is considered good practice to have the linker discard the unused symbols in the final image, especially when using third-party libraries that are not completely utilized by the embedded application. This can be done in GCC using the linker garbage collector, activated via the -gc-sections command-line option. If this flag is provided, the sections that are unused in the code are automatically discarded, and the unused symbols will in fact be kept out of the final image. 

To prevent the linker from discarding symbols associated with a particular section, the `used` attribute marks the symbol as implicitly used by the program. Multiple attributes can be listed in the same declaration, separated by commas, as follows:

```
const uint8_t
__attribute__((used,section(".keys")))
private_key[KEY_SIZE] = {0};
```

In this example, the attributes indicate both that the `private_key` array belong to the `.keys` section, and that it must not be discarded by the linker garbage collector, because it is marked as used. 

A simple linker script for an embedded target defines at least the two sections relative to RAM and FLASH mapping, and exports some predefined symbols to instruct the assembler of the toolchain about the memory areas. A bare-metal system based on the GNU toolchain usually starts with a memory section, describing the mapping of the two different areas in the system, such as:

```
MEMORY {
    FLASH(rx)   : ORIGIN = 0x00000000, LENGTH=256k
    RAW(rwx)    : ORIGIN = 0x20000000, LENGTH=64k
}
```

The preceding code snippet describes two memory areas used in the system. The first block is 256k mapped to FLASH, with the r and x flasgs, indicating that the area is accessible for read and execute operations. This enforces the read-only attribute of the whole area, and ensures that no variant sections are placed there. RAM, on the other hand, can be accessed in write mode directly, which means that variables are going to be place in a section within that area. In this specific example, the target maps the FLASH at the beginning of the address space, while the RAM is mapped starting at 512 MB. each target has its own address space mapping and flash/RAM size, so the linker script is definitely target-specific. 

As mentoined earlies in this chapter, the .text and .rodata ELF sections can only be accessed for reading, so they can safely be stored in the FLASH area, since they will not be modified while the target is running. On the other hand, both `.data` and `.bss` must be mapped in RAM to ensure that they are modifiable. 

Additional custom sections can be added in the script, in the case where it is necessary to store additional sections at a specific location in memory. The linker script can also export symbols related to a specific position in memory, or to the length of dynamically sized sections in memory, which can be referred to as external symbols and accessed in the C source code. 

The second block of statements in the linker script is called sections, and contains the allocation of the sections in specific positions of the defined memory areas. The . symbol, when associated with a variable in the script, represents the current position in the area, which is filled progressively from the lower addresses available. Each section must specify the area where is has to be mapped. The following example, though still incomplete to run the binary executable, shows how the different sections can be deployed using the linker script. The `.text` and `.rodata` sections are mapped in the flash memory:

```
SECTIONS
{
    /* Text section (code and read-only data) */
    .text:
{
    . = ALIGN(4);
    _start_text = .;
    *(.text*) /* code */
    . = ALIGN(4);
    _end_text = .;
    *(.rodata*) /* read only data */
    . = ALIGN(4);
    _end_rodata = .;
} > FLASH
```

The modifiable sections are mapped in RAM, with two special cases to notice here.

The AT keyword is used to indicate the load address to the linker, which is the area where the original values of the variables in `.data` are stored, while the actual addresses used in the execution are in a different memory region. More details about the load address and the virtual address for the `.data` section are explained in Chapter 4, The Boot-Up Procedure.

The NOLOAD attribute used for the `.bss` section ensures that no predefined values are stored in the ELF file for this section. Unintialized global and static variables are mapped by the linker in the RAM area, which is allocated by the linker:

```
_stored_data = .;
.data = AT(__stored_data)
{
    . = ALIGN(4);
    _start_data = .;
    *(.data*)
    . = ALIGN(4);
    _end_data = .;
} > RAM

.bss (NOLOAD):
{
    . = ALIGN(4);
    _start_bss = .;
    *(.bss*)
    . = ALIGN(4);
    _end_bss = .;
} > RAM
}
```

The alternative way to force the linker to keep sections in the final executable, avoiding their removal due to the linker garbage collector, is the use of the KEEP instruction to mark sections. Please note that this is an alternative to the `__attribute__((used))` mechanism explained earlier:

```
.keys :
{
    . = ALIGN(4);
    *(.keys*) = .;
    KEEP(*(.keys*));
} > FLASH
```

It is useful, and adivasable in general, to have the linker create a .map file alongside the resultant binary. This is done by appending the -Map=filename option to the link step, such as in:

`$ arm-none-eabi-ld -o image.elf object1.o object2.o -T linker_script.ld -Map=map_file.map`

The map file contains the location and the description of all the symbols, grouped by sections. This is useful to look for the specific location of symbols in the image, as well as for verifying that useful symbols are not accidentaly discarded due to a misconfiguration. 

To exclude the standard C library symbols from the linking process, the `-nostdlib` option can be added to the options passed to the GCC during the linking step.

### Binary format conversion

Despite containing all the compiled symbols in binary format, an ELF file is prefixed with a header that contains a description of the content, and pointers to the position where the sections start within the file. 

A tool in the toolchain, called `objcopy`, converts images from one standard format to others, and what is generally done is a conversion of the ELF into a raw binary image, without altering the symbols. To transform the image from ELF to binary format, invoke:

`arm-none-eabi-objcopy -I elf -O binary image.elf image.bin`

The original ELF file is also useful as the target of other diagnostic tools in the GNU toolchain, such as `nm` and `readelf`, which display the symbols in each module, with their type and relative address within the binary image. 

Furthermore, by using the `objdump` tool on the final image, or even on signle object files, several details about the image can be retrieved, including the visualization of the entire assembly code, using the -d disassemble option:

`arm-none-eabi-objdump -d image.elf`

### Interacting with the target

When started, OpenOCD opens two local TCP server sockets, on preconfigured ports, providing the communication services with the target platform. One socket proviedes an interactive command console that can be accessed through Telnet, while the other is a GDB server, used for remote debugging, as described in the next section.

### Validation

### Functional tests

Writing tests often involves programming the host so it can retrieve information abou the running target while the embedded software is executing, or alongside an ongoing debugging session, while the target executes in between breakpoints. The target can be configured to provide immediate output through a communication interface, such as a UART-based serial port, which can in turn be parsed by the host. It is usually more convenients to write test tools on the host using a higher-level interpreted programming language, to better organize the test cases and easily integrate the parsing of test results using regular expressions. Python, Perl, Ruby, and other languages with similar characteristics, are often a good fit for this purpose, also thanks to the availability of libraries and components designed for collecting and analyzing test results and interacting with continuous integration engines. 

### Hardware tools

Logic analyzers and oscilloscopes

### Testing off-target

Another efficient way to speed up the development is limiting the interaction, as much as possible, with the actual target.

Portions of code that are not CPU-specific can be compiled for the host-machine architecture and run directly, as long as their surroundings are properly abstracted to simulate the real environment. The software to test can be as small as a single function, and in this case a unit test can be written specifically for the development architecture. 

### Emulators

Another valid approach to run the code on the development machine, which is much less invasive for our code base and does not have specific portability requirements, is emulating the whole platform on the host PC. An emulator is a software that can replicate the functionality of an entire system, including its core CPU, memory, and a set of peripherals. 

The limit of the emulation approach is that QEMU can only be used to debug generic features that do not involve interaction with actual hardware.


TO DO:

finish additional exercises on linux