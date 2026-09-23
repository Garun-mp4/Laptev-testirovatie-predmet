#include "BitString.h" // интерфейс класса

#include <iomanip>  // setw и setfill
#include <ios>      // ios::failbit
#include <sstream>  // строковые потоки

BitString::BitString() noexcept
    : high_(0), low_(0) // обе части обнуляем
{
}

BitString::BitString(std::uint64_t high, std::uint64_t low) noexcept
    : high_(high), low_(low) // сохраняем обе части
{
}

BitString::BitString(const std::string& hexText)
    : high_(0), low_(0) // сначала получаем нулевую строку
{
    setFromHex(hexText); // затем разбираем текст
}

std::uint64_t BitString::high() const noexcept
{
    return high_; // возвращаем старшие 64 бита
}

std::uint64_t BitString::low() const noexcept
{
    return low_; // возвращаем младшие 64 бита
}

unsigned int BitString::hexDigitValue(char c)
{
    if (c >= '0' && c <= '9') // обычная цифра
    {
        return static_cast<unsigned int>(c - '0'); // значение от 0 до 9
    }
    if (c >= 'a' && c <= 'f') // маленькая hex-буква
    {
        return static_cast<unsigned int>(c - 'a' + 10); // значение от 10 до 15
    }
    if (c >= 'A' && c <= 'F') // большая hex-буква
    {
        return static_cast<unsigned int>(c - 'A' + 10); // значение от 10 до 15
    }
    throw InvalidFormat(); // остальные символы недопустимы
}

void BitString::setFromHex(const std::string& text)
{
    std::string value = text; // работаем с копией строки

    if (value.size() >= 2 && value[0] == '0' && (value[1] == 'x' || value[1] == 'X'))
    {
        value.erase(0, 2); // убираем необязательный префикс 0x
    }

    if (value.empty() || value.size() > 32) // 128 бит = максимум 32 hex-цифры
    {
        throw InvalidFormat(); // неправильная длина
    }

    std::uint64_t newHigh = 0; // временная старшая часть
    std::uint64_t newLow = 0;  // временная младшая часть

    for (char c : value) // читаем строку слева направо
    {
        const unsigned int digit = hexDigitValue(c); // получаем значение hex-цифры
        newHigh = (newHigh << 4U) | (newLow >> 60U); // переносим старшие 4 бита low
        newLow = (newLow << 4U) | digit;             // добавляем новую hex-цифру
    }

    high_ = newHigh; // сохраняем результат только после успешного разбора
    low_ = newLow;   // сохраняем младшую часть
}

BitString& BitString::operator&=(const BitString& rhs) noexcept
{
    high_ &= rhs.high_; // AND для старшей половины
    low_ &= rhs.low_;   // AND для младшей половины
    return *this;       // возвращаем текущий объект
}

BitString& BitString::operator|=(const BitString& rhs) noexcept
{
    high_ |= rhs.high_; // OR для старшей половины
    low_ |= rhs.low_;   // OR для младшей половины
    return *this;       // возвращаем текущий объект
}

BitString& BitString::operator^=(const BitString& rhs) noexcept
{
    high_ ^= rhs.high_; // XOR для старшей половины
    low_ ^= rhs.low_;   // XOR для младшей половины
    return *this;       // возвращаем текущий объект
}

BitString& BitString::operator<<=(unsigned int count) noexcept
{
    if (count >= 128U) // все биты уходят за пределы строки
    {
        high_ = 0; // старшая часть становится нулевой
        low_ = 0;  // младшая часть тоже нулевая
    }
    else if (count >= 64U) // младшая часть полностью переходит наверх
    {
        high_ = low_ << (count - 64U); // переносим low в high
        low_ = 0;                      // снизу остаются нули
    }
    else if (count > 0U) // обычный сдвиг от 1 до 63 бит
    {
        high_ = (high_ << count) | (low_ >> (64U - count)); // перенос через границу половин
        low_ <<= count;                                     // сдвигаем младшую часть
    }
    return *this; // count == 0 ничего не меняет
}

BitString& BitString::operator>>=(unsigned int count) noexcept
{
    if (count >= 128U) // все биты уходят за пределы строки
    {
        high_ = 0; // старшая часть становится нулевой
        low_ = 0;  // младшая часть тоже нулевая
    }
    else if (count >= 64U) // старшая часть полностью переходит вниз
    {
        low_ = high_ >> (count - 64U); // переносим high в low
        high_ = 0;                     // сверху остаются нули
    }
    else if (count > 0U) // обычный сдвиг от 1 до 63 бит
    {
        low_ = (low_ >> count) | (high_ << (64U - count)); // перенос через границу половин
        high_ >>= count;                                  // сдвигаем старшую часть
    }
    return *this; // count == 0 ничего не меняет
}

BitString BitString::shiftLeft(unsigned int count) const noexcept
{
    return (*this) << count; // делегируем перегруженному оператору
}

BitString BitString::shiftRight(unsigned int count) const noexcept
{
    return (*this) >> count; // делегируем перегруженному оператору
}

unsigned int BitString::countOnes() const noexcept
{
    auto countPart = [](std::uint64_t part) noexcept // считаем биты в одной половине
    {
        unsigned int result = 0; // начальное количество единиц
        while (part != 0U) // пока есть единичные биты
        {
            part &= (part - 1U); // удаляем младший единичный бит
            ++result;            // увеличиваем счетчик
        }
        return result; // возвращаем количество единиц
    };

    return countPart(high_) + countPart(low_); // складываем две половины
}

bool BitString::contains(const BitString& other) const noexcept
{
    const bool highContains = (high_ & other.high_) == other.high_; // все единицы high присутствуют
    const bool lowContains = (low_ & other.low_) == other.low_;     // все единицы low присутствуют
    return highContains && lowContains;                              // обе половины должны входить
}

bool BitString::sameBits(const BitString& other) const noexcept
{
    return high_ == other.high_ && low_ == other.low_; // сравниваем сами биты
}

std::string BitString::toString() const
{
    std::string result; // будущая двоичная строка
    result.reserve(128); // сразу резервируем нужную длину

    for (int bit = 63; bit >= 0; --bit) // сначала старшие 64 бита
    {
        result += ((high_ >> bit) & 1ULL) != 0ULL ? '1' : '0'; // добавляем очередной бит
    }
    for (int bit = 63; bit >= 0; --bit) // затем младшие 64 бита
    {
        result += ((low_ >> bit) & 1ULL) != 0ULL ? '1' : '0'; // добавляем очередной бит
    }

    return result; // возвращаем все 128 символов
}

std::string BitString::toHexString() const
{
    std::ostringstream out; // поток для сборки строки
    out << std::hex << std::uppercase << std::setfill('0') // hex в верхнем регистре
        << std::setw(16) << high_                           // ровно 16 цифр high
        << std::setw(16) << low_;                          // ровно 16 цифр low
    return out.str(); // возвращаем 32 hex-цифры
}

BitString operator&(const BitString& lhs, const BitString& rhs) noexcept
{
    BitString result(lhs); // создаем копию левого операнда
    result &= rhs;         // делегируем оператору &=
    return result;         // исходные объекты не меняются
}

BitString operator|(const BitString& lhs, const BitString& rhs) noexcept
{
    BitString result(lhs); // создаем копию левого операнда
    result |= rhs;         // делегируем оператору |=
    return result;         // возвращаем новый объект
}

BitString operator^(const BitString& lhs, const BitString& rhs) noexcept
{
    BitString result(lhs); // создаем копию левого операнда
    result ^= rhs;         // делегируем оператору ^=
    return result;         // возвращаем новый объект
}

BitString operator~(const BitString& value) noexcept
{
    return BitString(~value.high_, ~value.low_); // инвертируем обе половины
}

BitString operator<<(const BitString& value, unsigned int count) noexcept
{
    BitString result(value); // копируем исходную строку
    result <<= count;        // делегируем оператору <<=
    return result;           // возвращаем результат
}

BitString operator>>(const BitString& value, unsigned int count) noexcept
{
    BitString result(value); // копируем исходную строку
    result >>= count;        // делегируем оператору >>=
    return result;           // возвращаем результат
}

bool operator==(const BitString& lhs, const BitString& rhs) noexcept
{
    return lhs.countOnes() == rhs.countOnes(); // сравниваем количество единиц
}

bool operator!=(const BitString& lhs, const BitString& rhs) noexcept
{
    return !(lhs == rhs); // делегируем оператору ==
}

bool operator<(const BitString& lhs, const BitString& rhs) noexcept
{
    return lhs.countOnes() < rhs.countOnes(); // меньше единичных битов
}

bool operator<=(const BitString& lhs, const BitString& rhs) noexcept
{
    return !(rhs < lhs); // делегируем оператору <
}

bool operator>(const BitString& lhs, const BitString& rhs) noexcept
{
    return rhs < lhs; // меняем операнды местами
}

bool operator>=(const BitString& lhs, const BitString& rhs) noexcept
{
    return !(lhs < rhs); // делегируем оператору <
}

std::ostream& operator<<(std::ostream& out, const BitString& value)
{
    out << "0x" << value.toHexString(); // показываем строку в компактном hex-виде
    return out;                          // возвращаем поток
}

std::istream& operator>>(std::istream& in, BitString& value)
{
    std::string text; // сюда читаем одно слово
    in >> text;       // получаем hex-строку

    if (!in) // если сам поток уже завершился с ошибкой
    {
        return in; // ничего больше не делаем
    }

    try
    {
        BitString temp(text); // сначала создаем временный объект
        value = temp;         // записываем только корректный результат
    }
    catch (const BitString::InvalidFormat&)
    {
        in.setstate(std::ios::failbit); // неправильный ввод отмечаем ошибкой потока
    }

    return in; // возвращаем поток
}
