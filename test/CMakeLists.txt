function (Test NAME)
    set(EXE_NAME test-${NAME})
    add_executable(${EXE_NAME} ${NAME}.cpp)
    add_test(
        NAME ${EXE_NAME}
        COMMAND $<TARGET_FILE:${EXE_NAME}>
    )
endfunction()

function (TestCalcOptions NAME)
    set(EXE_NAME_R test-${NAME}-r)
    add_executable(${EXE_NAME_R} ${NAME}.cpp)
    add_test(
        NAME ${EXE_NAME_R}
        COMMAND $<TARGET_FILE:${EXE_NAME_R}>
    )

    set(EXE_NAME_NR test-${NAME}-nr)
    add_executable(${EXE_NAME_NR} ${NAME}.cpp)
    add_test(
        NAME ${EXE_NAME_NR}
        COMMAND $<TARGET_FILE:${EXE_NAME_NR}>
    )
    target_compile_definitions(${EXE_NAME_NR} PRIVATE NURBS_NON_RECURSIVE)

    set(EXE_NAME_NRSA test-${NAME}-nrsa)
    add_executable(${EXE_NAME_NRSA} ${NAME}.cpp)
    add_test(
        NAME ${EXE_NAME_NRSA}
        COMMAND $<TARGET_FILE:${EXE_NAME_NRSA}>
    )
    target_compile_definitions(${EXE_NAME_NRSA} PRIVATE NURBS_NON_RECURSIVE NURBS_ENABLE_STACK_ALLOCATION)
endfunction()

add_subdirectory(2D)
add_subdirectory(3D)
add_subdirectory(util)