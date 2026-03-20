cmake_minimum_required(VERSION 3.22)
project(Canny_Metal LANGUAGES C)

add_library(Canny_Metal)
target_include_directories(Canny_Metal PUBLIC
    Abstractions/Inc
    Utilities/Inc
    Utilities/CPU/Inc
)
target_sources(Canny_Metal PRIVATE
    Abstractions/Src/cons.c
    Abstractions/Src/cons_node.c
    Abstractions/Src/clock64.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf_circ.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf_item.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf_yield.c
    Utilities/Src/cobs.c
    Utilities/CPU/Src/hand_side.c
)
