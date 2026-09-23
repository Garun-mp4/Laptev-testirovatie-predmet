#include "BitString.h"          // тестируемый класс
#include <doctest/doctest.h>    // система модульного тестирования

#include <sstream>   // проверка потоков
#include <stdexcept> // invalid_argument
#include <string>    // строки

TEST_CASE("Создание объектов BitString")
{
    const BitString zero; // конструктор без аргументов
    CHECK(zero.high() == 0ULL); // high равен нулю
    CHECK(zero.low() == 0ULL);  // low равен нулю

    const BitString parts(1ULL, 2ULL); // конструктор из двух частей
    CHECK(parts.high() == 1ULL); // проверяем high
    CHECK(parts.low() == 2ULL);  // проверяем low

    const BitString text("0xFF"); // конструктор из строки
    CHECK(text.toHexString() == "000000000000000000000000000000FF"); // нормализованный hex

    const BitString copy = text; // копирующий конструктор
    CHECK(copy.sameBits(text)); // биты должны совпасть

    const BitString array[3] = {BitString("1"), BitString("3"), BitString("7")}; // массив объектов
    CHECK(array[0].countOnes() == 1U); // первый элемент
    CHECK(array[1].countOnes() == 2U); // второй элемент
    CHECK(array[2].countOnes() == 3U); // третий элемент
}

TEST_CASE("Побитовые операции BitString")
{
    const BitString a("F0"); // первый операнд
    const BitString b("3C"); // второй операнд

    CHECK((a & b).sameBits(BitString("30"))); // AND
    CHECK((a | b).sameBits(BitString("FC"))); // OR
    CHECK((a ^ b).sameBits(BitString("CC"))); // XOR
    CHECK((~BitString("0")).sameBits(BitString("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"))); // NOT

    BitString value = a; // копия для операций с присваиванием
    value &= b; // AND с присваиванием
    CHECK(value.sameBits(BitString("30"))); // результат &=

    value = a; // восстанавливаем A
    value |= b; // OR с присваиванием
    CHECK(value.sameBits(BitString("FC"))); // результат |=

    value = a; // восстанавливаем A
    value ^= b; // XOR с присваиванием
    CHECK(value.sameBits(BitString("CC"))); // результат ^=
}

TEST_CASE("Сдвиги 128-битной строки")
{
    const BitString one("1"); // младший бит

    CHECK(one.shiftLeft(1U).sameBits(BitString("2"))); // сдвиг на 1
    CHECK(one.shiftLeft(64U).high() == 1ULL); // граница половин
    CHECK(one.shiftLeft(64U).low() == 0ULL);  // low после сдвига
    CHECK(one.shiftLeft(127U).high() == 0x8000000000000000ULL); // старший бит
    CHECK(one.shiftLeft(128U).sameBits(BitString("0"))); // сдвиг за границу

    const BitString top("80000000000000000000000000000000"); // самый старший бит
    CHECK(top.shiftRight(127U).sameBits(BitString("1"))); // сдвиг к младшему биту
    CHECK(top.shiftRight(128U).sameBits(BitString("0"))); // все биты ушли

    BitString value("1"); // проверка <<= и >>=
    value <<= 65U; // сдвиг влево
    CHECK(value.high() == 2ULL); // старшая часть
    CHECK(value.low() == 0ULL);  // младшая часть
    value >>= 65U; // обратный сдвиг
    CHECK(value.sameBits(BitString("1"))); // вернулись к исходному значению
}

TEST_CASE("Подсчет, сравнение и включение")
{
    const BitString a("A"); // две единицы
    const BitString b("3"); // две единицы
    const BitString c("7"); // три единицы

    CHECK(a.countOnes() == 2U); // подсчет единиц
    CHECK(a == b); // равенство по числу единиц
    CHECK(a != c); // неравенство
    CHECK(a < c);  // 2 < 3
    CHECK(c > b);  // 3 > 2
    CHECK(a <= b); // 2 <= 2
    CHECK(c >= b); // 3 >= 2
    CHECK_FALSE(a.sameBits(b)); // сами строки разные

    const BitString container("FF"); // строка-контейнер
    const BitString inside("0F");    // включенная строка
    CHECK(container.contains(inside)); // включение есть
    CHECK_FALSE(inside.contains(container)); // обратного включения нет
}

TEST_CASE("Строковое представление и потоки")
{
    const BitString value("F"); // небольшой пример

    CHECK(value.toString().size() == 128U); // двоичная длина
    CHECK(value.toString().substr(112) == "0000000000001111"); // последние 16 бит
    CHECK(value.toHexString() == "0000000000000000000000000000000F"); // hex

    std::ostringstream out; // поток вывода
    out << value; // operator<<
    CHECK(out.str() == "0x0000000000000000000000000000000F"); // проверяем результат

    std::istringstream in("1234"); // корректный ввод
    BitString read; // объект для чтения
    in >> read; // operator>>
    CHECK(read.sameBits(BitString("1234"))); // значение прочитано

    std::istringstream badIn("ZZ"); // неправильный ввод
    BitString unchanged("AA"); // исходное значение
    badIn >> unchanged; // ввод должен завершиться ошибкой
    CHECK(badIn.fail()); // у потока установлен failbit
    CHECK(unchanged.sameBits(BitString("AA"))); // объект не испорчен
}

TEST_CASE("Стандартные исключения")
{
    CHECK_THROWS_AS(BitString("12XZ"), std::invalid_argument); // неправильный символ
    CHECK_THROWS_AS(BitString(""), std::invalid_argument); // пустая строка
    CHECK_THROWS_AS(BitString("1234567890ABCDEF1234567890ABCDEF0"), std::invalid_argument); // 33 цифры
    CHECK_NOTHROW(BitString("1234567890ABCDEF1234567890ABCDEF")); // ровно 32 цифры допустимы
}
