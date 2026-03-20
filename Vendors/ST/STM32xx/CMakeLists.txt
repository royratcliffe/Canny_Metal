cmake_minimum_required(VERSION 3.22)
project(Canny_Metal_STM32xx LANGUAGES C)

include(CMake/arm_cortexM4lf_math.cmake)

add_library(Canny_Metal_STM32xx)
target_include_directories(Canny_Metal_STM32xx PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)
target_sources(Canny_Metal_STM32xx PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/correlate_f32.c
)
target_link_libraries(Canny_Metal_STM32xx PUBLIC
    Canny_Metal
    arm_cortexM4lf_math
    stm32cubemx
)
