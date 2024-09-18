# Template CMakeLists.txt configuration for STM32 MCUs
cmake_minimum_required(VERSION 3.12)

# ##############################################################################
# Edit your MCU information up to the next border
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(demo_RTOS C CXX ASM)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(MCU_FAMILY STM32F10x)
set(MCU_MODEL STM32F103xx)

set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb
    -mno-thumb-interwork
    -mfpu=vfp
    -msoft-float
    -mfix-cortex-m3-ldrd
)

set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb") # importance
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 -mthumb") # importance

enable_language(C CXX ASM)

# Compiler Standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/startup/STM32F103x6.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F103x6.ld)

# ##############################################################################
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

# Header Directories
set(STD_LIB_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/${MCU_FAMILY}_StdPeriph_Driver/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/CMSIS/CM3/DeviceSupport/ST/${MCU_FAMILY}
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/CMSIS/CM3/CoreSupport
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_DIR}
)

# Source Files
file(GLOB_RECURSE STD_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-stdperiph-lib/Libraries/${MCU_FAMILY}_StdPeriph_Driver/src/*.c
)

file(GLOB_RECURSE PROJECT_SOURCES
    ${PROJECT_DIR}/src/*.cpp
    ${PROJECT_DIR}/src/*.c
)

# FreeRTOS Configuration
set(FREERTOS_KERNEL_PATH "./FreeRTOS-Kernel/")

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE "./src")

if (DEFINED FREERTOS_SMP_EXAMPLE AND FREERTOS_SMP_EXAMPLE STREQUAL "1")
    message(STATUS "Building FreeRTOS SMP example")
    add_compile_options(-DconfigNUMBER_OF_CORES=2 -DconfigUSE_PASSIVE_IDLE_HOOK=0)
endif()

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ARM_CM3" CACHE STRING "" FORCE)

add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)

target_compile_options(freertos_kernel PRIVATE
    ${CPU_PARAMETERS}
    -Wall -Wextra -Wundef -Wshadow -Wredundant-decls
    -fno-common -ffunction-sections -fdata-sections -MD
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

# Build the executable
add_executable(${EXECUTABLE}
    ${STD_LIB_SOURCES}
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT}
)

# Compile Definitions (Defines)
target_compile_definitions(${EXECUTABLE} PRIVATE
    ${MCU_MODEL}
    USE_STDPERIPH_DRIVER
    STM32F10X_LD
    HSE_VALUE=8000000
)

# Include Directories
target_include_directories(${EXECUTABLE} PRIVATE
    ${STD_LIB_INCLUDE_DIRECTORIES}
    ${PROJECT_INCLUDE_DIRECTORIES}
)

# Compile and Linker Options
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

target_link_libraries(${EXECUTABLE} PRIVATE freertos_kernel freertos_config)

# Post-build commands
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)