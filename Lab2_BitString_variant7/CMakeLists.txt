cmake_minimum_required(VERSION 3.16) # минимальная версия CMake
project(Lab2_BitString LANGUAGES CXX) # название проекта

set(CMAKE_CXX_STANDARD 14) # стандарт из методички
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++14 обязателен
set(CMAKE_CXX_EXTENSIONS OFF) # без расширений компилятора

add_executable(lab2 # создаем исполняемый файл
    main.cpp # главная функция
    BitString.cpp # реализация класса
)

target_include_directories(lab2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # путь к заголовку

enable_testing() # разрешаем запуск самопроверок через CTest
add_test(NAME Lab2SelfChecks COMMAND lab2) # проверяем программу при помощи встроенных assert()

if(MSVC) # настройки Visual C++
    target_compile_options(lab2 PRIVATE /W4 /UNDEBUG) # предупреждения; assert() оставляем активным и в Release
else() # настройки GCC и Clang
    target_compile_options(lab2 PRIVATE -Wall -Wextra -Wpedantic -UNDEBUG) # предупреждения; assert() оставляем активным и в Release
endif()
