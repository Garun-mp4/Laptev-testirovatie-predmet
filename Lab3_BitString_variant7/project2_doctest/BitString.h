#ifndef BITSTRING_H
#define BITSTRING_H

#include <cstdint>   // целые типы нужной длины
#include <iosfwd>    // объявления потоков
#include <string>    // std::string

// Рабочий класс для 128-битной строки.
class BitString
{
private:
    std::uint64_t high_; // старшие 64 бита
    std::uint64_t low_;  // младшие 64 бита

    static unsigned int hexDigitValue(char c); // перевод одной hex-цифры
    void setFromHex(const std::string& text);   // разбор строки

public:
    BitString() noexcept;                                      // нулевая строка
    BitString(std::uint64_t high, std::uint64_t low) noexcept; // две половины
    explicit BitString(const std::string& hexText);            // строка в hex

    std::uint64_t high() const noexcept; // старшая часть
    std::uint64_t low() const noexcept;  // младшая часть

    BitString& operator&=(const BitString& rhs) noexcept; // AND
    BitString& operator|=(const BitString& rhs) noexcept; // OR
    BitString& operator^=(const BitString& rhs) noexcept; // XOR
    BitString& operator<<=(unsigned int count) noexcept;   // сдвиг влево
    BitString& operator>>=(unsigned int count) noexcept;   // сдвиг вправо

    BitString shiftLeft(unsigned int count) const noexcept;  // копия со сдвигом влево
    BitString shiftRight(unsigned int count) const noexcept; // копия со сдвигом вправо
    unsigned int countOnes() const noexcept;                  // число единичных битов
    bool contains(const BitString& other) const noexcept;     // проверка включения
    bool sameBits(const BitString& other) const noexcept;     // точное совпадение

    std::string toString() const;    // 128 символов 0/1
    std::string toHexString() const; // 32 hex-цифры

    friend BitString operator&(const BitString& lhs, const BitString& rhs) noexcept;
    friend BitString operator|(const BitString& lhs, const BitString& rhs) noexcept;
    friend BitString operator^(const BitString& lhs, const BitString& rhs) noexcept;
    friend BitString operator~(const BitString& value) noexcept;
    friend BitString operator<<(const BitString& value, unsigned int count) noexcept;
    friend BitString operator>>(const BitString& value, unsigned int count) noexcept;

    // Сравнения выполняются по количеству единичных битов.
    friend bool operator==(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator!=(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator<(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator<=(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator>(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator>=(const BitString& lhs, const BitString& rhs) noexcept;

    friend std::ostream& operator<<(std::ostream& out, const BitString& value); // вывод
    friend std::istream& operator>>(std::istream& in, BitString& value);         // ввод
};

#endif // BITSTRING_H
