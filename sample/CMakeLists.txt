function (Sample NAME)
    set(EXE_NAME test-${NAME})
    add_executable(${EXE_NAME} ${NAME}.cpp)
endfunction()


Sample(basic_usage)
Sample(modify)