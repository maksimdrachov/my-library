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













