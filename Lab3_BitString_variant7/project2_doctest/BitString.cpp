#include "BitString.h" // интерфейс рабочего класса

#include <iomanip>    // форматирование hex
#include <ios>        // failbit
#include <sstream>    // строковые потоки
#include <stdexcept>  // standard exceptions

BitString::BitString() noexcept
    : high_(0), low_(0) // обе части равны нулю
{
}

BitString::BitString(std::uint64_t high, std::uint64_t low) noexcept
    : high_(high), low_(low) // сохраняем две половины
{
}

BitString::BitString(const std::string& hexText)
    : high_(0), low_(0) // безопасное начальное значение
{
    setFromHex(hexText); // разбираем строку
}

std::uint64_t BitString::high() const noexcept
{
    return high_; // возвращаем старшую часть
}

std::uint64_t BitString::low() const noexcept
{
    return low_; // возвращаем младшую часть
}

unsigned int BitString::hexDigitValue(char c)
{
    if (c >= '0' && c <= '9') // цифра 0-9
    {
        return static_cast<unsigned int>(c - '0'); // числовое значение
    }
    if (c >= 'a' && c <= 'f') // буква a-f
    {
        return static_cast<unsigned int>(c - 'a' + 10); // значение 10-15
    }
    if (c >= 'A' && c <= 'F') // буква A-F
    {
        return static_cast<unsigned int>(c - 'A' + 10); // значение 10-15
    }
    throw std::invalid_argument("BitString: invalid hexadecimal digit"); // стандартное исключение
}

void BitString::setFromHex(const std::string& text)
{
    std::string value = text; // используем копию входной строки

    if (value.size() >= 2 && value[0] == '0' && (value[1] == 'x' || value[1] == 'X'))
    {
        value.erase(0, 2); // убираем 0x
    }

    if (value.empty() || value.size() > 32) // допустимо от 1 до 32 hex-цифр
    {
        throw std::invalid_argument("BitString: hexadecimal string must contain 1..32 digits");
    }

    std::uint64_t newHigh = 0; // временная старшая часть
    std::uint64_t newLow = 0;  // временная младшая часть

    for (char c : value) // читаем все символы
    {
        const unsigned int digit = hexDigitValue(c); // переводим символ
        newHigh = (newHigh << 4U) | (newLow >> 60U); // перенос между половинами
        newLow = (newLow << 4U) | digit;             // добавляем 4 новых бита
    }

    high_ = newHigh; // сохраняем корректный результат
    low_ = newLow;   // сохраняем младшую часть
}

BitString& BitString::operator&=(const BitString& rhs) noexcept
{
    high_ &= rhs.high_; // AND для high
    low_ &= rhs.low_;   // AND для low
    return *this;       // возвращаем объект
}

BitString& BitString::operator|=(const BitString& rhs) noexcept
{
    high_ |= rhs.high_; // OR для high
    low_ |= rhs.low_;   // OR для low
    return *this;       // возвращаем объект
}

BitString& BitString::operator^=(const BitString& rhs) noexcept
{
    high_ ^= rhs.high_; // XOR для high
    low_ ^= rhs.low_;   // XOR для low
    return *this;       // возвращаем объект
}

BitString& BitString::operator<<=(unsigned int count) noexcept
{
    if (count >= 128U) // сдвиг удаляет все биты
    {
        high_ = 0; // обнуляем high
        low_ = 0;  // обнуляем low
    }
    else if (count >= 64U) // перенос целой половины
    {
        high_ = low_ << (count - 64U); // low становится high
        low_ = 0;                      // снизу нули
    }
    else if (count > 0U) // сдвиг 1..63
    {
        high_ = (high_ << count) | (low_ >> (64U - count)); // учитываем переход через 64 бита
        low_ <<= count;                                     // сдвигаем low
    }
    return *this; // возвращаем результат
}

BitString& BitString::operator>>=(unsigned int count) noexcept
{
    if (count >= 128U) // сдвиг удаляет все биты
    {
        high_ = 0; // обнуляем high
        low_ = 0;  // обнуляем low
    }
    else if (count >= 64U) // перенос целой половины
    {
        low_ = high_ >> (count - 64U); // high становится low
        high_ = 0;                     // сверху нули
    }
    else if (count > 0U) // сдвиг 1..63
    {
        low_ = (low_ >> count) | (high_ << (64U - count)); // учитываем переход через 64 бита
        high_ >>= count;                                  // сдвигаем high
    }
    return *this; // возвращаем результат
}

BitString BitString::shiftLeft(unsigned int count) const noexcept
{
    return (*this) << count; // используем уже готовый оператор
}

BitString BitString::shiftRight(unsigned int count) const noexcept
{
    return (*this) >> count; // используем уже готовый оператор
}

unsigned int BitString::countOnes() const noexcept
{
    auto countPart = [](std::uint64_t part) noexcept // функция для 64 бит
    {
        unsigned int result = 0; // счетчик
        while (part != 0U) // пока есть единицы
        {
            part &= (part - 1U); // стираем одну единицу
            ++result;            // считаем ее
        }
        return result; // результат для половины
    };

    return countPart(high_) + countPart(low_); // результат для 128 бит
}

bool BitString::contains(const BitString& other) const noexcept
{
    return ((high_ & other.high_) == other.high_) && // проверяем старшую часть
           ((low_ & other.low_) == other.low_);      // проверяем младшую часть
}

bool BitString::sameBits(const BitString& other) const noexcept
{
    return high_ == other.high_ && low_ == other.low_; // точное совпадение
}

std::string BitString::toString() const
{
    std::string result; // строка результата
    result.reserve(128); // заранее резервируем память

    for (int bit = 63; bit >= 0; --bit) // старшая половина
    {
        result += ((high_ >> bit) & 1ULL) != 0ULL ? '1' : '0'; // один символ
    }
    for (int bit = 63; bit >= 0; --bit) // младшая половина
    {
        result += ((low_ >> bit) & 1ULL) != 0ULL ? '1' : '0'; // один символ
    }

    return result; // 128 символов
}

std::string BitString::toHexString() const
{
    std::ostringstream out; // собираем текст
    out << std::hex << std::uppercase << std::setfill('0') // формат hex
        << std::setw(16) << high_                           // 16 цифр high
        << std::setw(16) << low_;                          // 16 цифр low
    return out.str(); // возвращаем строку
}

BitString operator&(const BitString& lhs, const BitString& rhs) noexcept
{
    BitString result(lhs); // копируем lhs
    result &= rhs;         // делегируем &=
    return result;         // возвращаем копию
}

BitString operator|(const BitString& lhs, const BitString& rhs) noexcept
{
    BitString result(lhs); // копируем lhs
    result |= rhs;         // делегируем |=
    return result;         // возвращаем копию
}

BitString operator^(const BitString& lhs, const BitString& rhs) noexcept
{
    BitString result(lhs); // копируем lhs
    result ^= rhs;         // делегируем ^=
    return result;         // возвращаем копию
}

BitString operator~(const BitString& value) noexcept
{
    return BitString(~value.high_, ~value.low_); // инвертируем все 128 бит
}

BitString operator<<(const BitString& value, unsigned int count) noexcept
{
    BitString result(value); // рабочая копия
    result <<= count;        // делегируем <<=
    return result;           // новый объект
}

BitString operator>>(const BitString& value, unsigned int count) noexcept
{
    BitString result(value); // рабочая копия
    result >>= count;        // делегируем >>=
    return result;           // новый объект
}

bool operator==(const BitString& lhs, const BitString& rhs) noexcept
{
    return lhs.countOnes() == rhs.countOnes(); // равны по числу единиц
}

bool operator!=(const BitString& lhs, const BitString& rhs) noexcept
{
    return !(lhs == rhs); // используем ==
}

bool operator<(const BitString& lhs, const BitString& rhs) noexcept
{
    return lhs.countOnes() < rhs.countOnes(); // меньше единиц
}

bool operator<=(const BitString& lhs, const BitString& rhs) noexcept
{
    return !(rhs < lhs); // используем <
}

bool operator>(const BitString& lhs, const BitString& rhs) noexcept
{
    return rhs < lhs; // используем <
}

bool operator>=(const BitString& lhs, const BitString& rhs) noexcept
{
    return !(lhs < rhs); // используем <
}

std::ostream& operator<<(std::ostream& out, const BitString& value)
{
    out << "0x" << value.toHexString(); // выводим 32 hex-цифры
    return out;                          // возвращаем поток
}

std::istream& operator>>(std::istream& in, BitString& value)
{
    std::string text; // входная строка
    in >> text;       // считываем одно значение

    if (!in) // проверяем поток
    {
        return in; // чтение уже не удалось
    }

    try
    {
        BitString temp(text); // проверяем строку конструктором
        value = temp;         // сохраняем корректное значение
    }
    catch (const std::invalid_argument&)
    {
        in.setstate(std::ios::failbit); // неправильный формат отмечаем в потоке
    }

    return in; // возвращаем поток
}
