# Import the ARM Cortex-M4 math library.
# Make some assumptions about the location of the library and its include directory.
add_library(arm_cortexM4lf_math STATIC IMPORTED)
set_target_properties(arm_cortexM4lf_math PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Middlewares/ST/ARM/DSP/Lib/libarm_cortexM4lf_math.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/Middlewares/ST/ARM/DSP/Inc
)
