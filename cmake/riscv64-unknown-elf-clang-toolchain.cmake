set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(CMAKE_ASM_COMPILER clang)
set(CMAKE_ASM_COMPILER_TARGET riscv64-unknown-elf)
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wno-unused-command-line-argument")

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET riscv64-unknown-elf)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET riscv64-unknown-elf)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

# Allow override of toolchain root directory
if(NOT DEFINED TOOLCHAIN_ROOT)
    set(TOOLCHAIN_ROOT "/usr/local" CACHE PATH "Toolchain root directory")
    message(WARNING "TOOLCHAIN_ROOT not specified, defaulting to '/usr/local'. "
                    "Set TOOLCHAIN_ROOT explicitly to avoid this warning.")
endif()

add_compile_options(-march=rv64gc -mabi=lp64d --gcc-toolchain=${TOOLCHAIN_ROOT})
add_link_options(-rtlib=libgcc --gcc-toolchain=${TOOLCHAIN_ROOT})

set(CMAKE_SYSROOT ${TOOLCHAIN_ROOT}/riscv64-unknown-elf)
