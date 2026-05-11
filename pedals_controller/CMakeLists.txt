# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(PedalsController C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

file(GLOB cpp_sources     
     "*.cpp"
     "*.c"
     "simhub/*.cpp"
)

add_executable(PedalsController ${cpp_sources}
                                )

pico_set_program_name(PedalsController "PedalsController")
pico_set_program_version(PedalsController "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(PedalsController 0)
pico_enable_stdio_usb(PedalsController 0)

# Add the standard library to the build
target_link_libraries(PedalsController
        pico_stdlib
        hardware_adc)

# Add the standard include files to the build
target_include_directories(PedalsController PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

include(FetchContent)

# Declare the library you want to download
FetchContent_Declare(
  SPSCQueue
  GIT_REPOSITORY https://github.com/rigtorp/SPSCQueue.git  
)

# Download and make the library available
FetchContent_MakeAvailable(SPSCQueue)


# Add any user requested libraries
target_link_libraries(PedalsController 
        hardware_timer
        hardware_watchdog
        hardware_pwm
        tinyusb_device
        tinyusb_board
        pico_multicore  
        SPSCQueue
        )


pico_enable_stdio_uart(PedalsController 1)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


pico_add_extra_outputs(PedalsController)

