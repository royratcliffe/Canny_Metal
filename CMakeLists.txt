project(Canny_Metal)

add_library(Canny_Metal)
target_include_directories(Canny_Metal PUBLIC
    Abstractions/Inc
    Utilities/CPU/Inc
)
target_sources(Canny_Metal PRIVATE
    Abstractions/Src/clock64.c
    Utilities/CPU/Src/hand_side.c
)
