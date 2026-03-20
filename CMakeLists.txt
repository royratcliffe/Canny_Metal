project(Canny_Metal LANGUAGES C)

add_library(Canny_Metal)
target_include_directories(Canny_Metal PUBLIC
    Abstractions/Inc
    Utilities/Inc
    Utilities/CPU/Inc
)
target_sources(Canny_Metal PRIVATE
    Abstractions/Src/clock64.c
    Utilities/Src/cobs.c
    Utilities/CPU/Src/hand_side.c
)
