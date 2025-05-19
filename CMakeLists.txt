cmake_minimum_required(VERSION 3.28.3)
project(c_numerical_methods C)

set(CMAKE_C_STANDARD 99)

add_executable(c_numerical_methods dichotomy.c
        lagrange.c
        gauss-elimination.c
        mmq.c
        trapezoidal-rule.c)
