cmake_minimum_required(VERSION 3.16) # минимальная версия CMake
project(Lab3_Project1_BitString LANGUAGES CXX) # название проекта

set(CMAKE_CXX_STANDARD 14) # используем C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON) # стандарт обязателен
set(CMAKE_CXX_EXTENSIONS OFF) # отключаем расширения

add_executable(lab3_custom # исполняемый файл
    main.cpp # главная функция
    BitString.cpp # рабочий класс
)

target_include_directories(lab3_custom PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # наши .h

if(MSVC) # Visual C++
    target_compile_options(lab3_custom PRIVATE /W4) # предупреждения
else() # GCC/Clang
    target_compile_options(lab3_custom PRIVATE -Wall -Wextra -Wpedantic) # предупреждения
endif()
