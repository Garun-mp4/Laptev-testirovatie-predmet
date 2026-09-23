cmake_minimum_required(VERSION 3.16) # минимальная версия CMake
project(Lab3_Project2_BitString LANGUAGES CXX) # название проекта

set(CMAKE_CXX_STANDARD 14) # стандарт из методички
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++14 обязателен
set(CMAKE_CXX_EXTENSIONS OFF) # без расширений компилятора

include(FetchContent) # позволяет получить doctest автоматически

# Сначала пробуем найти уже установленный doctest.
find_package(doctest 2.4.11 CONFIG QUIET)

if(NOT doctest_FOUND) # если библиотеки в системе нет
    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(doctest) # скачиваем официальный doctest
endif()

add_executable(lab3_doctest # тестовая программа
    main.cpp # своя главная функция
    testcase.cpp # модульные тесты
    BitString.cpp # рабочий класс
)

target_include_directories(lab3_doctest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # наши заголовки
target_link_libraries(lab3_doctest PRIVATE doctest::doctest) # подключаем doctest

if(MSVC) # Visual C++
    target_compile_options(lab3_doctest PRIVATE /W4) # предупреждения
else() # GCC/Clang
    target_compile_options(lab3_doctest PRIVATE -Wall -Wextra -Wpedantic) # предупреждения
endif()

enable_testing() # включаем CTest
add_test(NAME BitStringDoctest COMMAND lab3_doctest) # регистрируем тест
