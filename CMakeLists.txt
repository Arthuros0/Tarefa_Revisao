# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Tarefa_revisao C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Tarefa_revisao main.c ssd1306.c joy_button.c matriz_leds.c buzzer.c)

pico_set_program_name(Tarefa_revisao "Tarefa_revisao")
pico_set_program_version(Tarefa_revisao "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Tarefa_revisao 1)
pico_enable_stdio_usb(Tarefa_revisao 1)

pico_generate_pio_header(Tarefa_revisao ${CMAKE_CURRENT_LIST_DIR}/pio_matrix.pio)


# Add the standard library to the build
target_link_libraries(Tarefa_revisao
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(Tarefa_revisao PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Tarefa_revisao 
        hardware_pio
        hardware_adc
        hardware_i2c
        hardware_pwm
        hardware_clocks
        )

pico_add_extra_outputs(Tarefa_revisao)

