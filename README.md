# C++ for Bare-Metal

An experiment in using C++ for bare-metal development.

## Background

I was reading [Uros Popovic](https://github.com/popovicu)'s [blog posts](https://popovicu.com/tags/bare-metal/) on bare-metal development using assembly, C, and Rust, and asked myself: "can I do it with C++ too?"

Unlike C, C++ is typically considered unsuitable for bare-metal development, as some of its core features (like exceptions and dynamic memory allocation) require runtime support. However, I managed to get it to work pretty well.

## Implementation details

### Printing through UART (`src/uart.hpp`, `src/uart.cpp`)

The function `UART::putc` prints a single character to the UART using memory-mapped registers.
For more complex printing, `std::format_to` could theoretically be used, but unfortunately it isn't available in free-standing mode, so we have to implement integer printing ourselves.

### Exiting (`src/syscalls.cpp`)

We use newlib's mechanism of providing "system calls" for the standard library to call from its functions. In our case, we provide a `_exit` function that uses a memory-mapped register to signal QEMU to exit.

### Dynamic memory allocation (`src/new.cpp`, `src/syscalls.cpp`)

Here too we provide a custom "system call" for newlib; the `_sbrk` function simply keeps a pointer to the end of the heap (initialized from the linker-provided symbol `_end`) and moves it as requested.

To ensure `new` and `delete` expressions use newlib's `malloc` and `free`, we define replacement `operator new` and `operator delete` functions in `src/new.cpp`.

### Exception handling (`_initExceptionHandling` in `src/init.c`)

Because we have no runtime support, we have to register the exception handling frame from within the program. To do this, we instruct the linker to place the `__eh_frame_start` at the start of the `.eh_frame` section and to put a null terminator at the end of that section, so we can register it using libgcc's `__register_frame` function.

This initialization is done in `_initExceptionHandling`, which is called from the assembly entry point (`_start`) before any C++ code is executed.

### Global constructors and destructors (`_init` in `src/init.c`)

To support global object construction and destruction, we implement the `_init` function that calls `__libc_init_array` to run global constructors and registers `__libc_fini_array` with `atexit` to handle global destructors during program termination (this is what the runtime-provided `_start` function would do if we had a runtime).
It seems like we need the `-fno-use-cxa-atexit` compiler flag to make this work (otherwise the linker complains about undefined references to `__dso_handle`).

### `_runtimeMain` (`src/runtime-main.cpp`)

In normal C++, the runtime support code is responsible for exiting when `main` returns. Moreover, it is responsible for catching any exception thrown outside of any `try` block, telling the user about it and calling `std::terminate`.

So in our case, we have to handle these tasks ourselves. This is the purpose of `_runtimeMain`; it is called from the assembly entry point (`_start`), wraps the call to `applicationMain` with a `try` block, prints (via UART) an appropriate message when `applicationMain` returns, either normally or with an exception, and finally calls `std::exit`.

### Assembly initialization (`src/entry.s`)

The assembly entry point (`_start`) is responsible for initializing the stack from the linker-provided `_stack_top` symbol, calling `_initBSS` to clear the BSS region (although it seems like we don't really need to on QEMU), activating the FPU (via the `mstatus` register), calling `_initExceptionHandling` to register the exception handling frame, calling `_init` to handle global constructors, and finally calling `_runtimeMain`.

### Linker script (`link.ld`)

The linker script is based on ld's default script, with the following changes:

- We define the memory region to begin at `0x80000000`, the beginning of the physical RAM on QEMU's virt machine
- We define the stack region as the final 64K of the RAM
- We add the necessary symbols for the stack and `.eh_frame` sections

## Building and Executing

### Prerequisites

- CMake 4.1 or higher
- A GCC toolchain for _riscv64-unknown-elf_ with architecture _rv64gc_ and ABI _lp64d_, including newlib as libc and libstdc++
  - The libraries should be built with `-mcmodel=medany`
  - If using the provided GCC toolchain/presets, the compilers should be available in the PATH as `riscv64-unknown-elf-gcc` and `riscv64-unknown-elf-g++`
  - I built the toolchain from the [RISC-V GNU Compiler Toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain.git) repository by running `./configure` with `--with-cmodel=medany` and then `sudo make`
- If building with Clang, the Clang compiler is needed in addition to the GCC toolchain
- QEMU for RISC-V (`qemu-system-riscv64`)

### Building

`CMakePresets.json` provides the following presets:

- `debug`
- `release`
- `clang-debug`
- `clang-release`

To build with one of those presets, run:

```bash
cmake --preset <preset-name>
cmake --build --preset <preset-name>
```

When using `clang-debug` or `clang-release`, you need to specify the _riscv64-unknown-elf_ toolchain installation path using the `TOOLCHAIN_ROOT` CMake variable. For example, if you built the toolchain from the [RISC-V GNU Compiler Toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain.git) repository with `--prefix=/opt/riscv`, you should run:

```bash
cmake --preset <preset-name> -DTOOLCHAIN_ROOT=/opt/riscv
cmake --build --preset <preset-name>
```

If you don't provide the `TOOLCHAIN_ROOT` variable, it will default to `/usr/local`.

You can of course try to build with your own configuration or even toolchain. `.gitignore` includes `cmake/*-toolchain.cmake`, so you can create your own toolchain files without adding them to the repository.
To be able to use the `exec.sh` script, the build directory should be `build/<name>`.

### Executing

To execute the program on a QEMU virtual machine, run:

```bash
./exec.sh <preset-or-build-dir-name>
```

## Acknowledgments

The inspiration for this project came from [Uros Popovic](https://github.com/popovicu), as described above, and many details were taken from [his blog posts](https://popovicu.com/tags/bare-metal/).
