# Template CMakeLists.txt configuration for STM32 MCUs
cmake_minimum_required(VERSION 3.12)

# Edit your MCU information up to the next border
project(demo_GPIO)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(MCU_FAMILY STM32F10x)
set(MCU_MODEL STM32F103xx)

set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb
)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/startup/STM32F103x6.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F103x6.ld)

# ##############################################################################
set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Headers
set(CUBEMX_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/${MCU_FAMILY}_StdPeriph_Driver/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/${MCU_FAMILY}_StdPeriph_Driver/src
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/CMSIS/CM3/DeviceSupport/ST/${MCU_FAMILY}
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/CMSIS/CM3/CoreSupport
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR}
)

# Sources
file(GLOB_RECURSE STM32CUBEMX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/${MCU_FAMILY}_StdPeriph_Driver/src/*.c
)

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/src/*.cpp
    ${PROJECT_DIR}/src/*.c
)

# Executable files
add_executable(${EXECUTABLE}
    ${STM32CUBEMX_SOURCES} 
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT}
)

# Embedded macros(defines)
target_compile_definitions(${EXECUTABLE} PRIVATE
    ${MCU_MODEL}
    USE_STDPERIPH_DRIVER
    STM32F10X_LD
)

# Add header directories (AFTER add_executable !!)
target_include_directories(${EXECUTABLE} PRIVATE
    ${CUBEMX_INCLUDE_DIRECTORIES}
    ${PROJECT_INCLUDE_DIRECTORIES}
)

# Compiler and linker options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.bin
)
