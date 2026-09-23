#ifndef BITSTRING_H
#define BITSTRING_H

#include <cstdint>   // типы фиксированной длины
#include <iosfwd>    // потоки без лишних подключений
#include <string>    // строки для ввода и вывода

// Класс для работы со 128-битной строкой.
class BitString
{
private:
    std::uint64_t high_; // старшие 64 бита
    std::uint64_t low_;  // младшие 64 бита

    static unsigned int hexDigitValue(char c); // перевод hex-символа в число
    void setFromHex(const std::string& text);   // разбор шестнадцатеричной строки

public:
    // Пустое исключение для неправильной строки.
    class InvalidFormat {};

    BitString() noexcept;                                      // строка из нулей
    BitString(std::uint64_t high, std::uint64_t low) noexcept; // две половины числа
    explicit BitString(const std::string& hexText);            // создание из hex-строки

    std::uint64_t high() const noexcept; // получить старшую часть
    std::uint64_t low() const noexcept;  // получить младшую часть

    BitString& operator&=(const BitString& rhs) noexcept; // AND с присваиванием
    BitString& operator|=(const BitString& rhs) noexcept; // OR с присваиванием
    BitString& operator^=(const BitString& rhs) noexcept; // XOR с присваиванием
    BitString& operator<<=(unsigned int count) noexcept;   // сдвиг влево
    BitString& operator>>=(unsigned int count) noexcept;   // сдвиг вправо

    BitString shiftLeft(unsigned int count) const noexcept;  // сдвиг влево без изменения объекта
    BitString shiftRight(unsigned int count) const noexcept; // сдвиг вправо без изменения объекта
    unsigned int countOnes() const noexcept;                  // количество единичных битов
    bool contains(const BitString& other) const noexcept;     // проверка включения другой строки
    bool sameBits(const BitString& other) const noexcept;     // точное совпадение битов

    std::string toString() const;    // двоичная строка из 128 символов
    std::string toHexString() const; // шестнадцатеричная строка

    friend BitString operator&(const BitString& lhs, const BitString& rhs) noexcept; // AND
    friend BitString operator|(const BitString& lhs, const BitString& rhs) noexcept; // OR
    friend BitString operator^(const BitString& lhs, const BitString& rhs) noexcept; // XOR
    friend BitString operator~(const BitString& value) noexcept;                      // NOT
    friend BitString operator<<(const BitString& value, unsigned int count) noexcept; // сдвиг влево
    friend BitString operator>>(const BitString& value, unsigned int count) noexcept; // сдвиг вправо

    // Сравнение выполняется по количеству единичных битов.
    friend bool operator==(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator!=(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator<(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator<=(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator>(const BitString& lhs, const BitString& rhs) noexcept;
    friend bool operator>=(const BitString& lhs, const BitString& rhs) noexcept;

    friend std::ostream& operator<<(std::ostream& out, const BitString& value); // вывод в hex
    friend std::istream& operator>>(std::istream& in, BitString& value);         // ввод из hex
};

#endif // BITSTRING_H
