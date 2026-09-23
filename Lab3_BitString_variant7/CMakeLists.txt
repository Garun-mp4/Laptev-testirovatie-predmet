cmake_minimum_required(VERSION 3.16) # минимальная версия CMake
project(Lab3_BitString LANGUAGES CXX) # общий проект лабораторной

# Каждый подпроект можно также собирать отдельно.
add_subdirectory(project1_custom_tests) # собственный класс Test
add_subdirectory(project2_doctest) # тесты doctest
