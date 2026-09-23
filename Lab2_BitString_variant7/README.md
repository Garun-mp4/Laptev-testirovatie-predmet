# Лабораторная работа № 2 — вариант 7

Класс `BitString` хранит 128-битную строку в двух полях `std::uint64_t`.

## Быстрый запуск через g++

```bash
g++ -std=c++14 -Wall -Wextra -Wpedantic main.cpp BitString.cpp -o lab2
./lab2
```

## Запуск через CMake

```bash
cmake -S . -B build
cmake --build build
./build/lab2
```

На Windows после сборки путь к программе может быть `build/Debug/lab2.exe` или `build/Release/lab2.exe`.
