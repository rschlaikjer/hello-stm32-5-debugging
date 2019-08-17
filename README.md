# STM32 Programming with opencm3: Part 5

This repository contains the complete companion code
to accompany
[this blog post](https://rhye.org/post/stm32-with-opencm3-5-fault-handlers/)
which works through using debugging faults on STM32 series MCUs.

### Building

To build and test the code in this repo, you will need CMake and the GNU ARM
toolchain.

To build:

```
mkdir build
cmake ../
make
````

To flash the interrupt fault demo code:
```
make interrupt_fault_example_flash
```

To flash the memory fault demo code:
```
make memory_fault_example_flash
```
