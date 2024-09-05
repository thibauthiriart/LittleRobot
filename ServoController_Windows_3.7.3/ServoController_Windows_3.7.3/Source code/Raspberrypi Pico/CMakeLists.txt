cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(lsm6ds3trc C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(pico main.c)

pico_add_extra_outputs(pico)

#Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(pico pico_stdlib)

# enable uart output, disable usb output
pico_enable_stdio_usb(pico 0)
pico_enable_stdio_uart(pico 1)
