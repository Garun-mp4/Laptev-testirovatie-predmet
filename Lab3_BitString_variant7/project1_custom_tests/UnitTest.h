#ifndef UNITTEST_H
#define UNITTEST_H

#include <cstddef>   // size_t
#include <iostream>  // вывод результатов
#include <string>    // сообщения тестов

// Макрос сохраняет само условие, имя файла и строку.
#define check(condition) dotest((condition), #condition, __FILE__, __LINE__)
// Макрос выводит сообщение об ошибке без повторного увеличения счетчика.
#define error(message) dofail((message), __FILE__, __LINE__)

namespace UnitTest
{
class Test
{
private:
    std::size_t passed_ = 0; // число успешных проверок
    std::size_t failed_ = 0; // число неуспешных проверок

    Test(const Test&) = delete;            // тестовый объект не копируем
    Test(Test&&) = delete;                 // и не перемещаем
    Test& operator=(const Test&) = delete; // присваивание запрещено
    Test& operator=(Test&&) = delete;      // перемещение тоже запрещено

protected:
    void dotest(bool condition, const std::string& message, const char* file, long line)
    {
        if (condition) // проверка прошла
        {
            ++passed_; // считаем успех
        }
        else // проверка не прошла
        {
            ++failed_; // считаем ошибку
            dofail(message, file, line); // печатаем место ошибки
        }
    }

    void dofail(const std::string& message, const char* file, long line) const
    {
        std::cout << "FAILED: " << message << '\n'; // условие теста
        std::cout << "File: " << file << ", line: " << line << '\n'; // место теста
    }

    void success() noexcept
    {
        ++passed_; // ручная успешная проверка
    }

    void fail() noexcept
    {
        ++failed_; // ручная неуспешная проверка
    }

public:
    Test() = default; // обычный конструктор
    virtual ~Test() = default; // виртуальный деструктор

    virtual void run() = 0; // запуск тестов определит наследник

    void report() const
    {
        std::cout << "-----\n"; // разделитель
        std::cout << "Testing <BitStringTest>\n"; // имя набора тестов
        std::cout << "All tests = " << (passed_ + failed_) << ";\n"; // всего
        std::cout << "Passed tests = " << passed_ << ";\n"; // успешно
        std::cout << "Failed tests = " << failed_ << ";\n"; // ошибки
        std::cout << "-----\n"; // разделитель
    }
};
} // namespace UnitTest

#endif // UNITTEST_H
