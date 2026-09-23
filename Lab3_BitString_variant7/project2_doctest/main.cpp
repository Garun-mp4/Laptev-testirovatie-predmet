#define DOCTEST_CONFIG_IMPLEMENT // реализацию doctest размещаем здесь
#include <doctest/doctest.h>     // тестовый фреймворк

#include <iostream> // сведения о работе

int main(int argc, char** argv)
{
    std::cout << "Лабораторная работа № 3, проект 2 (doctest)\n"; // номер проекта
    std::cout << "Студент: Сулейманов Гарун Баширович\n"; // разработчик
    std::cout << "Группа: ДИНРБ-41\n"; // группа
    std::cout << "Дисциплина: Тестирование программного обеспечения\n"; // дисциплина
    std::cout << "Вариант 7: модульные тесты класса BitString.\n\n"; // условие

    doctest::Context context; // создаем контекст doctest
    context.applyCommandLine(argc, argv); // передаем параметры командной строки
    return context.run(); // запускаем все TEST_CASE и возвращаем код результата
}
