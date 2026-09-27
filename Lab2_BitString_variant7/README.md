# Лабораторная работа № 2 — вариант 7

Класс `BitString` хранит 128-битную строку в двух полях `std::uint64_t`.

## Быстрый запуск через g++

```bash
g++ -std=c++14 -Wall -Wextra -Wpedantic main.cpp BitString.cpp -o lab2
./lab2
```

## Запуск через CMake

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

CTest запускает программу с набором проверок `assert()`. Они остаются включенными также в конфигурации Release.

Для многоконфигурационных генераторов CMake (например, Visual Studio) параметр `-C Debug` указывает конфигурацию теста. Чтобы просто посмотреть демонстрационный вывод, можно отдельно запустить `build/Debug/lab2.exe` на Windows или `build/lab2` в Linux/MinGW; у других конфигураций имя каталога будет соответственно `Release`.
