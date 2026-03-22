cmake_minimum_required(VERSION 3.22)
project(Canny_Metal LANGUAGES C)

add_library(Canny_Metal)
target_include_directories(Canny_Metal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Abstractions/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/CPU/Inc
)
target_sources(Canny_Metal PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Abstractions/Src/cons.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Abstractions/Src/cons_node.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Abstractions/Src/clock64.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Abstractions/Src/registered_opaques.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf_circ.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf_item.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/ring_buf_yield.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/Src/cobs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/CPU/Src/hand_side.c
)
