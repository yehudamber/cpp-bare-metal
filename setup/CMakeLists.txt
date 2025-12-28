add_library(setup OBJECT call-main.cpp entry.s exception.cpp new.cpp syscalls.cpp uart.cpp)
target_include_directories(setup INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
