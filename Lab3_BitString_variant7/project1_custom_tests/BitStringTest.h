#ifndef BITSTRING_TEST_H
#define BITSTRING_TEST_H

#include "BitString.h" // тестируемый класс
#include "UnitTest.h"  // базовый класс Test

#include <sstream>   // тесты ввода и вывода
#include <stdexcept> // invalid_argument
#include <string>    // строки

// Тестовый класс открыто наследуется от Test.
class BitStringTest : public UnitTest::Test
{
private:
    void testCreate()
    {
        const BitString zero; // конструктор без аргументов
        check(zero.high() == 0ULL); // старшая часть нулевая
        check(zero.low() == 0ULL);  // младшая часть нулевая

        const BitString parts(1ULL, 2ULL); // конструктор из двух частей
        check(parts.high() == 1ULL); // проверяем high
        check(parts.low() == 2ULL);  // проверяем low

        const BitString text("0xFF"); // конструктор из строки
        check(text.toHexString() == "000000000000000000000000000000FF"); // нормализованный hex

        const BitString copy = text; // копирование
        check(copy.sameBits(text)); // содержимое должно совпасть

        const BitString array[3] = {BitString("1"), BitString("3"), BitString("7")}; // массив объектов
        check(array[0].countOnes() == 1U); // первый элемент
        check(array[1].countOnes() == 2U); // второй элемент
        check(array[2].countOnes() == 3U); // третий элемент
    }

    void testBitOperations()
    {
        const BitString a("F0"); // 11110000
        const BitString b("3C"); // 00111100

        check((a & b).sameBits(BitString("30"))); // AND
        check((a | b).sameBits(BitString("FC"))); // OR
        check((a ^ b).sameBits(BitString("CC"))); // XOR
        check((~BitString("0")).sameBits(BitString("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"))); // NOT

        BitString value = a; // копия для операций с присваиванием
        value &= b; // AND с присваиванием
        check(value.sameBits(BitString("30"))); // результат &=

        value = a; // восстанавливаем A
        value |= b; // OR с присваиванием
        check(value.sameBits(BitString("FC"))); // результат |=

        value = a; // восстанавливаем A
        value ^= b; // XOR с присваиванием
        check(value.sameBits(BitString("CC"))); // результат ^=
    }

    void testShifts()
    {
        const BitString one("1"); // один младший бит
        check(one.shiftLeft(1U).sameBits(BitString("2"))); // сдвиг на 1
        check(one.shiftLeft(64U).high() == 1ULL); // переход через границу 64 бит
        check(one.shiftLeft(64U).low() == 0ULL);  // младшая часть после перехода
        check(one.shiftLeft(127U).high() == 0x8000000000000000ULL); // самый старший бит
        check(one.shiftLeft(128U).sameBits(BitString("0"))); // полный выход за границу

        const BitString top("80000000000000000000000000000000"); // старший бит
        check(top.shiftRight(127U).sameBits(BitString("1"))); // сдвиг к младшему биту
        check(top.shiftRight(128U).sameBits(BitString("0"))); // сдвиг за пределы

        BitString value("1"); // проверяем операторы с присваиванием
        value <<= 65U; // сдвиг влево на 65
        check(value.high() == 2ULL && value.low() == 0ULL); // ожидаем 2 в high
        value >>= 65U; // возвращаем обратно
        check(value.sameBits(BitString("1"))); // исходное значение
    }

    void testCountCompareAndContains()
    {
        const BitString a("A"); // 1010 - две единицы
        const BitString b("3"); // 0011 - две единицы
        const BitString c("7"); // 0111 - три единицы

        check(a.countOnes() == 2U); // countOnes
        check(a == b); // равенство по количеству единиц
        check(a != c); // неравенство по количеству единиц
        check(a < c);  // 2 < 3
        check(c > b);  // 3 > 2
        check(a <= b); // 2 <= 2
        check(c >= b); // 3 >= 2
        check(!a.sameBits(b)); // точное содержимое разное

        const BitString container("FF"); // восемь младших единиц
        const BitString inside("0F");    // четыре младших единицы
        check(container.contains(inside)); // inside включена в container
        check(!inside.contains(container)); // обратное неверно
    }

    void testStringsAndStreams()
    {
        const BitString value("F"); // небольшой пример
        check(value.toString().size() == 128U); // двоичная строка имеет 128 символов
        check(value.toString().substr(112) == "0000000000001111"); // последние 16 бит
        check(value.toHexString() == "0000000000000000000000000000000F"); // hex

        std::ostringstream out; // поток вывода
        out << value; // используем operator<<
        check(out.str() == "0x0000000000000000000000000000000F"); // проверяем текст

        std::istringstream in("1234"); // поток ввода
        BitString read; // объект для чтения
        in >> read; // используем operator>>
        check(read.sameBits(BitString("1234"))); // проверяем результат

        std::istringstream badIn("ZZ"); // неправильный ввод
        BitString unchanged("AA"); // значение до чтения
        badIn >> unchanged; // поток должен перейти в fail
        check(badIn.fail()); // проверяем состояние потока
        check(unchanged.sameBits(BitString("AA"))); // объект не изменился
    }

    void testException()
    {
        try
        {
            const BitString wrong("12XZ"); // неправильный символ
            (void)wrong; // явно отмечаем объект как использованный
            fail(); // исключение не было выброшено
            error("std::invalid_argument was expected"); // поясняем ошибку
        }
        catch (const std::invalid_argument&)
        {
            success(); // получили правильное стандартное исключение
        }

        try
        {
            const BitString tooLong("1234567890ABCDEF1234567890ABCDEF0"); // 33 hex-цифры
            (void)tooLong; // убираем предупреждение о неиспользовании
            fail(); // исключение не было выброшено
            error("std::invalid_argument was expected for long string"); // пояснение
        }
        catch (const std::invalid_argument&)
        {
            success(); // длинная строка правильно отклонена
        }
    }

public:
    void run() override
    {
        testCreate(); // проверка конструкторов и массива
        testBitOperations(); // проверка AND, OR, XOR, NOT
        testShifts(); // проверка сдвигов
        testCountCompareAndContains(); // подсчет, сравнение, включение
        testStringsAndStreams(); // строки и потоки
        testException(); // стандартные исключения
    }
};

#endif // BITSTRING_TEST_H
