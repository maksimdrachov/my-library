## Chapter 1: Exercises

### Ex 1

/

### Ex 2

_The program counter (PC) points to the memory address of the next instruction(s) that the processor load from memory._

1. _How does the PC value change after fetching a 32-bit instruction?_
2. _How does PC change after executing a 16-bit instruction?_

> Each instruction in Cortex-M has either 16 or 32 bits. Typically, the processor increases the program counter by four automatically. The processor retrieves four bytes (i.e., 32 bits) from the instruction memory in one clock cycle, as shown in Figure 1-12. Chapter 13.2 discusses how the processor decodes these 32 bits and finds out whether they represent one 32-bit or two 16-bit instructions.

**PC = PC + 4 after each instruction fetch**

### Ex 3

_The arithmetic and logic unit (ALU) performs integer and logic operations. Implement the logic of a simple ALU that performs 2-bit addition and subtraction. Assume that the operands are A_1A_0 and B_1B_0, and the output is C_1C_0. The operation code O has only one bit, which selects addition or subtraction._

### Ex 4

_A register is the fastest data storage element within a processor. A 32-bit register consists of a set of flip-flops to store 32 bits of information. Design a 4-bit register by using flip-flops and answer the following questions._

1. _How many bits does the address bus have?_
2. _How many bits does the data bus have?_
3. _What else input signals should the register have?_

### Ex 5

_Suppose the memory address in most embedded systems has 32 bits. _

1. _How many unique memory locations can a 32-bit address access?_
2. _Desktops and servers usually have virtual memory, which allows a process to allocate and use a memory space that is larger than the physical memory. Why do not embedded systems use virtual memory usually?_

1. 0x0000 0000 - 0xFFFF FFFF ( 2^32 = 4 294 967 296 )
2. Embedded systems are limited in terms of memory and thus usually do not exceed the limits of using a memory space of 4 GB.

### Ex 6

_What are the advantages and disadvantages of Von Neumann architecture and Harvard architecture?_

> - The Von Neumann architecture is relatively inexpensive and simple.
> - The Harvard architecture allows the processor to access the data memory and the instruction memory concurrently. By contrast , the Von Neumann architecture allows only one memory access at any time instant; the processor either reads an instruction from the instruction memory or accesses data in the data memory. Accordingly, the Harvard architecture often offers faster processing speed at the same clock rate.
> - The Harvard architecture tends to be more energy efficient. Under the same performance requirement, the Harvard architecture often needs lower clock speeds, resulting in lower power consumption rate.
> - In the Harvard architecture, the data bus and the instruction bus may have different widths. For example, digital signal processing processors can leverage this feature to make the instruction bus wider to reduce the number of clock cycles required to load an instruction.

### Ex 7 

_Executable and Linkable Format (ELF) is a standard used for Linux operating systems and many other embedded systems. Compile a C program in Linux into a binary executable and use the **readelf** command to display the information of a binary executable. Identify the data sections and instruction/data sections of a binary executable program._

`sample.c`:

```
#include <stdio.h>
#define NUMBER_1 1
#define NUMBER_2 2

int main(void)
{
    int a,b,c;
    a = NUMBER_1;
    b = NUMBER_2;
    c = a + b;

    printf("Output: %d\r\n", c);

    return 0;
}
```

Compiling:

`gcc sample.c -o sampleOut`

![image-compile](/embedded-systems-with-arm-cortex-m/ch-1/images/image-compile.png)

Running:

`./sampleOut`

![running](/embedded-systems-with-arm-cortex-m/ch-1/images/running.png)

To see the file type of this executable:

`file sampleOutput`

![file-gcc](/embedded-systems-with-arm-cortex-m/ch-1/images/file-gcc-sampleout.png)

This is not the ELF type we wanted, that's cause we need to switch to cross-compiling for ARM architecture using `arm-none-eabi-gcc`.

To compile for ARM: `arm-none-eabi-gcc -c sample.c -o sampleARM -mthumb -mcpu=cortex-m4`

![compile-arm](/embedded-systems-with-arm-cortex-m/ch-1/images/compile-arm.png)

What GCC has done is, it has passed `sample.c` through the following stages of compilation to convert the .c file into a final executable:

- Pre-processing
- Compiling (assembly file creation)
- Assembler (object file creation)
- Linking (links all object files to create an executable)

**Pre-processing**

This is the first stage of compilation in which the source code is processed. The following high-level tasks are done at the pre-processing stage:

- Removal of comments from source code
- Expanding #include files
- Macro expansion
- Conditional compilation

Now let's try to pre-process our sample.c file and look at its output.

`gcc -E sample.c -o sample.i`

From the `sample.i` file, it can be seen that the statement `#include <stdio.h>` has been replaced by the contents of the `stdio.h` file. Macros NUMBER_1 and NUMBER_2 are not present in the output. Besides wherever these macros were used, their values are replaced by macro values. 

In this compilation stage, any line that starts with # is interepreted as a pre-processing command. These commands have their own language format.

**Compilation**

Compilation is converting pre-processed code into assembly code. In this stage, all programming symbols are converted to assembly instructions, which are native to the target processor architecture. This is also the stage where inline assembly code in high-level programming languages is processed. 

Now let's compile our `sample.c` file and produce the equivalent assembly instruction file. 

`gcc -S sample.c -fverbose-asm -o sample.s`

Using this, you can understand how assembly instructions and C programming syntax are related. 

**Assembling**

Assembling is the conversion of assembly instructions to machine code or object code. This is the first stage in which the ELF file is generated. The output file is known as an object file which has an extension .o and has the file format ELF. This file cannot be directly executed, but needs to be linked using Linker, which is the final stage of compilation.

**ELF files and binary utilities**

To understand the final stage of compilation, which is linking, and to visualize how ELF files are structured, we need to understand object files, particularly ELF files. We were able to generate object files in the previous stage, but we were not able to open them. To open this file, we need to understand the structure of an object file. ELF object files contain five kind of information.

- ELF header
- Section header and different sections
- Program header and different program sections
- Object code
- Debugging information

**ELF header**

All ELF object files start with the ELF header. To read the ELF header of our object file, we will use a binary utility tool called readelf. The following is the command to read the ELF header of any object file:

`readelf -h sampleARM`

![readelf](/embedded-systems-with-arm-cortex-m/ch-1/images/readelf.png)

There are two important pieces of information present in the ELF header. One is the ELF program header part and the other is the ELF section header part.

**ELF sections**

When a program is compiled, different things are generated after compilation. 

- Binary executable code
- Code for initialised variables
- Code for uninitialised variables
- Constant strings
- Information about the variable and function names used in the program
- Debugging information

- `.text`: binary executable code
- `.data`: initialised variables
- `.bss`: uninitialised variables
- `.shstrtab`: constant strings
- `.symtab`: variables and function names (symbol table)
- `.debug`: debug information

To see all the sections that are present:

`readelf -S sampleARM`

![sections](/embedded-systems-with-arm-cortex-m/ch-1/images/readelf.png)

To check the code that is present in `.text`:

`objdump -d sampleARM`

![objdump](/embedded-systems-with-arm-cortex-m/ch-1/images/objdump.png)

How does the compiler know where to put each section in the final ELF? This is where the Linker comes in. Linkers use a file known as the linker descriptor file. This contains information about all the memory in the target machine (Flash, RAM, QSPI, SDRAM,...) with its starting address and size. It also contains information about the different sections that should be present in the final ELF file and where each section should be loaded in the target machine. 

Source:

[Understanding ELF, the Executable and Linkable Format](https://www.opensourceforu.com/2020/02/understanding-elf-the-executable-and-linkable-format/)




