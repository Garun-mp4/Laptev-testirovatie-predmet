# ЛР № 3 — проект 1

В проекте используется простой базовый класс `UnitTest::Test` и тестовый класс `BitStringTest`, который открыто наследуется от него.

## Запуск

```bash
g++ -std=c++14 -Wall -Wextra -Wpedantic main.cpp BitString.cpp -o lab3_custom
./lab3_custom
```

Или через CMake:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Тестовый запуск выводит счетчики Passed/Failed и завершается с кодом 1, если хотя бы одна проверка провалилась.
