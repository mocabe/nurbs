function (Test NAME)
    set(EXE_NAME test-${NAME})
    add_executable(${EXE_NAME} ${NAME}.cpp)
    add_test(
        NAME ${EXE_NAME}
        COMMAND $<TARGET_FILE:${EXE_NAME}>
    )
endfunction()

add_subdirectory(2D)
add_subdirectory(util)