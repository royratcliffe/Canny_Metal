add_arm_semihosting_test(TEST_NAME registered_opaques_test
    TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/registered_opaques_test.c
)

add_arm_semihosting_test(TEST_NAME correlate_f32_test
    TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/correlate_f32_test.c
    LINK_LIBRARIES
        arm_cortexM4lf_math
        m
)
