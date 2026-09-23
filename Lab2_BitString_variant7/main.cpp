#include "BitString.h" // наш класс

#include <cassert>   // assert для простых проверок
#include <cstdint>   // uint64_t
#include <iostream>  // консольный вывод
#include <sstream>   // проверка оператора ввода

// Та же пара полей с обычным выравниванием.
struct DefaultBitStringLayout
{
    std::uint64_t high; // старшая часть
    std::uint64_t low;  // младшая часть
};

#pragma pack(push, 1)
// Та же пара полей при упаковке по одному байту.
struct PackedBitStringLayout
{
    std::uint64_t high; // старшая часть
    std::uint64_t low;  // младшая часть
};
#pragma pack(pop)

// Небольшой набор автоматических проверок через assert().
void runSelfChecks()
{
    const BitString zero; // конструктор без аргументов
    assert(zero.high() == 0ULL && zero.low() == 0ULL); // проверяем нулевое значение

    const BitString one("1"); // одна единица
    assert(one.countOnes() == 1U); // должен быть один единичный бит

    const BitString all("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"); // все 128 бит равны 1
    assert(all.countOnes() == 128U); // проверяем крайний случай

    const BitString left = one.shiftLeft(64U); // переносим бит в старшую половину
    assert(left.high() == 1ULL && left.low() == 0ULL); // проверяем границу 64 бит

    const BitString back = left.shiftRight(64U); // возвращаем бит обратно
    assert(back.sameBits(one)); // должен получиться исходный объект

    const BitString mask("F"); // маска 1111
    assert(all.contains(mask)); // все единицы маски входят в all
    assert(!mask.contains(all)); // обратное включение неверно

    const BitString a("A"); // 1010 - две единицы
    const BitString b("3"); // 0011 - тоже две единицы
    assert(a == b); // сравнение идет по числу единичных битов
    assert(!a.sameBits(b)); // сами битовые строки при этом разные

    const BitString andResult = BitString("F0") & BitString("3C"); // 0x30
    assert(andResult.sameBits(BitString("30"))); // проверяем AND

    const BitString orResult = BitString("F0") | BitString("0F"); // 0xFF
    assert(orResult.sameBits(BitString("FF"))); // проверяем OR

    const BitString xorResult = BitString("AA") ^ BitString("FF"); // 0x55
    assert(xorResult.sameBits(BitString("55"))); // проверяем XOR
}

int main()
{
    std::cout << "Лабораторная работа № 2\n"; // номер работы
    std::cout << "Студент: Сулейманов Гарун Баширович\n"; // разработчик
    std::cout << "Группа: ДИНРБ-41\n"; // учебная группа
    std::cout << "Дисциплина: Тестирование программного обеспечения\n"; // дисциплина
    std::cout << "Вариант 7: класс BitString для 128-битных строк.\n\n"; // условие варианта

    BitString empty; // создание объекта конструктором без аргументов
    BitString parts(0x0123456789ABCDEFULL, 0xFEDCBA9876543210ULL); // создание из двух частей
    BitString fromText("0x0000000000000000000000000000000F"); // создание из строки
    BitString copy = parts; // создание копированием
    BitString array[3] = {BitString("1"), BitString("3"), BitString("F")}; // массив объектов

    std::cout << "Разные способы создания объектов:\n"; // заголовок раздела
    std::cout << "empty    = " << empty << '\n'; // нулевой объект
    std::cout << "parts    = " << parts << '\n'; // объект из двух половин
    std::cout << "fromText = " << fromText << '\n'; // объект из строки
    std::cout << "copy     = " << copy << '\n'; // копия
    std::cout << "array[0..2] = " << array[0] << ", " << array[1] << ", " << array[2] << "\n\n"; // массив

    const BitString a("F0F0F0F0F0F0F0F00F0F0F0F0F0F0F0F"); // первый пример
    const BitString b("FFFF0000FFFF00000000FFFF0000FFFF"); // второй пример

    std::cout << "Побитовые операции:\n"; // показываем основные операции
    std::cout << "A       = " << a << '\n'; // значение A
    std::cout << "B       = " << b << '\n'; // значение B
    std::cout << "A & B   = " << (a & b) << '\n'; // AND
    std::cout << "A | B   = " << (a | b) << '\n'; // OR
    std::cout << "A ^ B   = " << (a ^ b) << '\n'; // XOR
    std::cout << "~A      = " << (~a) << "\n\n"; // NOT

    BitString changed = a; // копия для операций с присваиванием
    changed &= b; // проверяем &=
    std::cout << "После A &= B: " << changed << '\n'; // результат AND
    changed = a; // снова восстанавливаем A
    changed |= b; // проверяем |=
    std::cout << "После A |= B: " << changed << '\n'; // результат OR
    changed = a; // снова восстанавливаем A
    changed ^= b; // проверяем ^=
    std::cout << "После A ^= B: " << changed << "\n\n"; // результат XOR

    const BitString one("1"); // строка с одним единичным битом
    std::cout << "Сдвиги:\n"; // заголовок
    std::cout << "1 << 1   = " << one.shiftLeft(1U) << '\n'; // обычный сдвиг
    std::cout << "1 << 64  = " << one.shiftLeft(64U) << '\n'; // граница двух половин
    std::cout << "1 << 127 = " << one.shiftLeft(127U) << '\n'; // крайний старший бит
    std::cout << "1 << 128 = " << one.shiftLeft(128U) << "\n\n"; // выход за пределы

    std::cout << "Количество единиц в A: " << a.countOnes() << '\n'; // countOnes
    std::cout << "Количество единиц в B: " << b.countOnes() << '\n'; // countOnes
    std::cout << "A == B по числу единиц: " << std::boolalpha << (a == b) << '\n'; // ==
    std::cout << "A < B  по числу единиц: " << (a < b) << '\n'; // <
    std::cout << "A > B  по числу единиц: " << (a > b) << "\n\n"; // >

    const BitString container("FF"); // строка, содержащая 8 младших единиц
    const BitString inside("0F");    // строка, содержащая 4 младших единицы
    std::cout << container << " содержит " << inside << ": " << container.contains(inside) << '\n'; // включение
    std::cout << inside << " содержит " << container << ": " << inside.contains(container) << "\n\n"; // обратная проверка

    std::cout << "toString() для 0xF (последние 16 бит): "
              << fromText.toString().substr(112) << '\n'; // демонстрация двоичного вида
    std::cout << "toHexString(): " << fromText.toHexString() << "\n\n"; // демонстрация hex-строки

    std::istringstream input("1234567890ABCDEF1234567890ABCDEF"); // имитируем ввод пользователя
    BitString readValue; // объект для оператора >>
    input >> readValue; // вызываем перегруженный ввод
    std::cout << "Результат оператора >>: " << readValue << "\n\n"; // показываем результат

    std::cout << "Размеры объектов:\n"; // требование по #pragma pack
    std::cout << "sizeof(BitString) = " << sizeof(BitString) << " байт\n"; // реальный класс
    std::cout << "Без #pragma pack(1) = " << sizeof(DefaultBitStringLayout) << " байт\n"; // обычная структура
    std::cout << "С #pragma pack(1)   = " << sizeof(PackedBitStringLayout) << " байт\n\n"; // упакованная структура

    try
    {
        const BitString wrong("12XZ"); // специально неверная строка
        std::cout << wrong << '\n'; // сюда программа не должна дойти
    }
    catch (const BitString::InvalidFormat&)
    {
        std::cout << "Исключение обработано: неверный формат BitString.\n"; // демонстрация исключения
    }

    runSelfChecks(); // запускаем набор assert-проверок
    std::cout << "Все проверки assert() успешно пройдены.\n"; // итог тестов

    return 0; // нормальное завершение программы
}
