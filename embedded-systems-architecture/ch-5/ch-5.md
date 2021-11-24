## Memory Management

- Memory mapping
- The execution stack
- Heap management
- The memory protection unit

### Memory mapping

### Memory model and address space

In the ARM Cortex-M, the total addressable space is divided into 6 macro regions. Depending on their purpose, the regions have different permissions, so that there are areas of memory that can only be accessible for read operations at runtime, or that are not allowed to execute in place. These constraints are implemented in hardware, but might be configurable at runtime on microcontrollers that include an MPU.

- System region
- External peripherals region
- External RAM region
- Built-in peripherals region
- SRAM region
- Code region

### The code region

The lowest 512 MB of the addressing space in a Cortex-M microcontroller is reserved for executable code. The .text and .rodata sections are mapped within this region as they remain constant during the execution of the software. 

### The RAM regions

Internal RAM banks are mapped to addresses in the second 512 MB block, starting at address 0x20000000. External memory banks may be mapped anywhere in the 1 GB region, starting at address 0x60000000. 

### Peripheral-access regions

The 512 MB area following the internal RAM region, starting at address 0x40000000, is reserved for peripherals that are normally built into the microcontroller. 

### The system region

The highest 512 MB of the Cortex-M memory mapping is reserved for accessing system configuration and private control blocks. 

### Order of memory transactions

On ARM CPUs, the memory system does not guarantee that the memory transactions are executed in the same order of the instructions that generate them. The order of memory transactions can be altered to adjust to the characteristics of the hardware, such as the wait states required to access underlying physical memory, or by speculative branch prediction mechanisms implemented at microcode level. To ensure that the previous memory transactions have been executed, the Cortex-M instruction set includes three kinds of barriers:

- **data memory barrier (DMB)**
- **data synchronization barrier (DSB)**
- **instruction synchronization barrier (ISB)**

The DSB is a soft barrier, invoked to ensure that all the pending transactions are executed before the next memory transaction occurs. The DSB is used to actually suspend the execution until all the pending transactions have been executed. The ISB, in addition, also flushes the CPU pipeline and ensures that all the new instructions are fetched again after the memory transactions, thus preventing any side effects caused by the outdated memory content. There are a number of cases where using a barrier is required:

- After updating the VTOR (vector table offset register) to change the address of the IV
- After updating the memory mapping
- During execution of code that modifies itself

### The execution stack

As seen in the previous chapter, a bare-metal application starts executing with an empty stack area. The stack keeps track of the chain of function calls at all times by storing the branching point at each function call, but also serves as temporary storage during function executions. Variables within the local scope of each function are stored inside the stack while the function is executing. 

Placing big objects in the stack, such as communication buffers or long strings, is in general not a good idea, considering that the space for the stack is always very limited. The compiler can be instructed to produce a warning every time the stack space required by a single function exceeds a certain threshold, as, for example, in this code:

```
void function(void)
{
    char buffer[200];
    read_serial_buffer(buffer);
}
```

If compiled with the GCC option `-Wstack-usage=100`, it will produce the following warning:

```
main.c: In function 'function':
main.c:15:6: warning: stack usage is 208 bytes [-Wstack-usage=]
```

While this mechanism is useful to identify local stack overuses, it is not effective to identify all the potential stack overflows in the code, as the function calls may be nested and their stack usage added up. Our function uses 208 bytes of stack whenever it is invoked, 200 to host the buffer local variable in the stack, and 8 additional bytes to store two pointers: the origin of the call in the code section, that is stored as return point, and the frame pointer, which contains the old location of the stack pointer before the call. 

By design, the stack grows every time a function is called and shrinks again when functions return. It is particularly difficult to make estimations about the runtime stack usage, and the use of recursive functions. For this reason, the use of recursion in the code should be avoided whenever possible.

### Stack placement

The initial pointer to the stack area can be selected at boot, by setting the desired memory address in the first word of the IV table, which corresponds to the beginning of the binary image loaded in flash.

Since our STM32F407 provides an additional, tightly coupled, 64 KB memory bank at address 0x10000000, we may want to reserve its lower 16 KB for the execution stack, and keep the rest in a seperate section for later use. The linker script must define the region on top, in the MEMORY block:

```
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 1M
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    CCRAM(rwx) : ORIGIN = 0X10000000, LENGTH = 64K
}
```

Two symbols may now be exported at the end of the file, by assigning constant, pre-defined values:

```
_stack_size = 16 * 1024;
_stack_end = ORIGIN(CCRAM) + _stack_size;
```

The values of _stack_size and _stack_end can be accesses by the application as ordinary C symbols. _stack_end is placed at address 0 when the vector table is initialized to indicate the highest stack address:

```
__attribute__((section(".isr_vector")))
void (* const IV[])(void) = 
{
    (void (*) (void))(&_end_stack),
    isr_reset, // Reset
    isr_fault, // NMI
    isr_fault, // HardFault
    /* More interrupt routines follow */
}
```

### Stack overflows

In other cases, such as when adjacent memory is used for other purposes, the stack pointer might overflow into other segments, with a concrete risk of corrupting other memory areas, with catastrophic consequences including even opening the door to malicious code injections and arbitrary code execution attacks on the target. The best strategy usually consists of assigning adequate stack space at boot, isolating the stack as much as possible from the other memory sections, and checking the stack usage at runtime. 

### Stack painting

An effective way to measure the amount of stack space needed consists of filling the estimated stack space with a well-known pattern.

We can perform stack painting manually in the reset handler, during memory initialization. To do so, we need to assign an area to paint. In this case it would the last 8 KB of memory up until _end_stack. Once again, while manipulating the stack in the reset_handler function, local variables should not be used. The reset_handler function will store the value of the current stack pointer into the global variable sp:

`static unsigned int sp;`

Within the handler, the following section can be added before invoking main():

```
asm volatile("mrs %0, msp" : "=r"(sp));
dst = ((unsigned int *) (&_end_stack)) - (8192 / sizeof(unsigned int));
while (dst < sp)
{
    *dst = 0xDEADC0DE;
    dst++;
}
```

The first assembly instruction is used to store the current value of the stack pointer to the variable sp, to ensure that the painting stops after painting the area, but only up until the last unused address in the stack.

### Heap management

Safety-critical embedded systems often are designed not to implement any dynamic memory allocation. While this may sound extreme, it minimizes the impact of the most common programming mistakes in the application code, which might lead to catastrophic consequences for the running system.

On the other hand, dynamic memory allocation is a powerful tool, because it gives complete control over the lifetime and the size of the memory blocks. Many third-party libraries designed for embedded devices expect an existing implementation of a dynamic memory allocation. Dynamic memory is managed through a heap structure in memory, by keeping track of the status and the size of each allocation, incrementing the pointer to the next area of free memory and reusing blocks that have been freed if new allocation requests are processed. 

A standard programming interface for heap allocation consists of two basic functions:
```
void *malloc(size_t size);
void free(void *ptr);
```

More complete heap management has support for an additional call, realloc, that would allow us to resize a memory area previously allocated, either in place or by relocating it to a new segment that is large enough to contain an object of the given size:

```
void *realloc(void *ptr, size_t size);
```

[Difference between Heap and Stack](https://www.guru99.com/stack-vs-heap.html#3)

### Custom implementation

A basic implementation, providing dynamic memory starting from the first available address after the end of the .bss section, might represent each block in memory using a preamble, like the following:

```
struct malloc_block {
    unsigned int signature;
    unsigned int size;
}
```

Two different signatures can be assigned to identify valid blocks, and differentiate between blocks still in use versus blocks that have already been freed:

```
#define SIGNATURE_IN_USE (0xAAC0FFEE)
#define SIGNATURE_FREED  (0xFEEDFACE)
#define NULL (((void *)0))
```

The malloc function should keep track of the highest address in the stack. In this example, a static variable is used to mark the current end of the stack. This is set to the start address at the beginning, and will grow every time a new block is allocated:

```
void *malloc(unsigned int size)
{
    static unsigned int *end_heap = 0;
    struct malloc_block *blk;
    char *ret = NULL;
    if (!end_heap) {
        end_heap = &_start_heap;
    }
```

The next two lines ensure that the block requested is 32-bit-aligned, to optimize the access to malloc_block:

```
if ((size >> 2) << 2 != size)
    size = ((size >> 2) + 1) << 2;
```

The malloc function then first looks in the heap for a memory section that has been previously freed:

```
blk = (struct mallock_block *)&_start_heap;
while (blk < end_heap)
{
    if ((blk->signature == SIGNATURE_FREED) && (blk->size <= size))
    {
        ret = ((char *)blk) + sizeof(struct malloc_block);
        return ret;
    }
    blk = ((char *)blk) + sizeof(struct malloc_block) + blk->size;
}
```

If no available slot is found, or if none of them is large enough to satisfy the size required for the allocation, the memory is allocated at the end of the stack and the pointer is updated accordingly:

```
blk = (struct malloc_block *)end_heap;
blk->signature = SIGNATURE_IN_USE;
blk->size = size;
ret = ((char *)end_heap) + sizeof(struct malloc_block);
end_heap = ret + size;
return ret;
}
```

In both cases, the address returned hides the malloc_block control structure that precedes it. The end_heap variable always points to the end of the last block allocated in the heap.

### Using newlib

### Limiting the heap

### Multiple memory pools

Heap allocation mechanisms using seperate pools may be implemented for different reasons, such as ensuring that specific modules or subsystems do not use more memory than the amount that is assigned to them at compile time, or ensuring that allocations with the same size can reuse the same physical space in memory, reducing the impact of fragmentation, or even assigning pre-defined, fixed area in memory for DMA operations with peripherals or network devices. It is possible to delimit the sections for the different pools as usual, by exporting symbols in the linker script. The example below pre-allocates the space in memory for two pools, of 8 KB and 4 KB repsectively, located at the end of the .bss section in RAM:

```
PROVIDE(_start_pool0 = _end_bss);
PROVIDE(_end_pool0 = _start_pool0 + 8 KB);
PROVIDE(_start_pool1 = _end_pool0);
PROVIDE(_end_pool1 = _start_pool1 + 4 KB);
```

### Common heap usage errors

- NULL pointer derefernce
- Double free
- Use after free
- Failure to call free, resulting in memory leaks

### The memory protection unit

The memory protection unit, often referred to as the MPU, is an optional component present in many ARM-based microcontrollers. The MPU is used to seperate sections in memory by setting local permissions and attributes. This mechanism has several uses in real-life scenarios, such as preventing access to the memory when the CPU is running in user mode, or preventing fetching code to execute from the writable locations in RAM. When the MPU is enabled, it enforces the rules by triggering a memory exception interrupt when those are violated. 

While commonly used by operating systems to create process stack separation and enforce privileged access to system memory, the MPU can be useful in a number of other cases, including bare-metal applications. 

### MPU configuration registers

In Cortex-M, the control block region related to MPU configuration is located in the system control block, starting at address 0xE000ED90. 

### Programming the MPU





