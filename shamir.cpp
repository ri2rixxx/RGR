#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <cmath>

using namespace std;

// Функция для проверки, является ли число простым
bool is_prime(int64_t n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;

    for (int64_t i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }
    return true;
}

int64_t mod_exp(int64_t base, int64_t exponent, int64_t mod)
{
    int64_t result = 1;
    base = base % mod;

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {
            result = (result * base) % mod;
        }
        exponent = exponent >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Функция для нахождения обратного элемента по модулю (a^(-1) mod m)
int64_t mod_inverse(int64_t a, int64_t m)
{
    a = a % m;
    for (int64_t x = 1; x < m; x++)
    {
        if ((a * x) % m == 1)
        {
            return x;
        }
    }
    throw runtime_error("Обратного элемента не существует");
}

// Генерация случайного числа в диапазоне [min, max]
int64_t random_in_range(int64_t min, int64_t max)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int64_t> dis(min, max);
    return dis(gen);
}

string shamir_trade()
{
    int64_t p = 1000000007; // Простое число

    // Выбор простого числа p
    while (true)
    {
        cout << "Ваше простое число p:\n"
             << p << endl;
        cout << "Введите новое число или нажмите Enter чтобы оставить текущее: ";

        string input;
        getline(cin, input);
        if (input.empty())
        {
            break;
        }

        try
        {
            int64_t new_p = stoll(input);
            if (!is_prime(new_p))
            {
                system("clear");
                cout << "Ошибка: число должно быть простым!\n";
                continue;
            }
            p = new_p;
            system("clear");
            break;
        }
        catch (...)
        {
            system("clear");
            cout << "Ошибка: введите корректное число!\n";
        }
    }

    while (true)
    {
        cout << "Выберите роль:\n";
        cout << "1) Инициатор обмена (Абонент A)\n";
        cout << "2) Ответчик (Абонент B)\n";
        cout << "Ваш выбор: ";

        string choice;
        getline(cin, choice);

        if (choice == "1")
        {
            // Абонент A
            int64_t cA = random_in_range(2, p - 2);
            if (cA % 2 == 0)
                cA -= 1;
            while (!is_prime(cA))
            {
                cA -= 2;
            }
            int64_t dA = mod_inverse(cA, p - 1);

            cout << "\nВаш закрытый ключ cA: " << cA << endl;
            cout << "Ваш закрытый ключ dA: " << dA << endl << endl;
            int64_t M;
            M = random_in_range(2, p - 2);
            cout << "Ваше слово M: " << M << endl << endl;
            int64_t x1;
            x1 = mod_exp(M, cA, p);
            cout << "Отправьте абоненту B x1:" << x1 << endl;

            cout << "\nВведите полученное от абонента B значение x2: ";
            int64_t x2;
            cin >> x2;
            cin.ignore();

            int64_t x3 = mod_exp(x2, dA, p);
            cout << "\nОтправьте абоненту B значение x3: " << x3 << endl;

            cout << "\nОбщий секретный ключ: " << M << endl;
            return to_string(M);
        }
        else if (choice == "2")
        {
            // Абонент B
            int64_t cB = random_in_range(2, p - 2);
            if (cB % 2 == 0)
                cB -= 1;
            while (!is_prime(cB))
            {
                cB -= 2;
            }
            int64_t dB = mod_inverse(cB, p - 1);

            cout << "\nВаш закрытый ключ cB: " << cB << endl;
            cout << "Ваш закрытый ключ dB: " << dB << endl;

            cout << "\nВведите полученное от абонента A значение x1: ";
            int64_t x1;
            cin >> x1;
            cin.ignore();

            int64_t x2 = mod_exp(x1, cB, p);
            cout << "\nОтправьте абоненту A значение x2: " << x2 << endl;

            cout << "\nВведите полученное от абонента A значение x3: ";
            int64_t x3;
            cin >> x3;
            cin.ignore();

            int64_t M = mod_exp(x3, dB, p);
            cout << "\nОбщий секретный ключ: " << M << endl;
            return to_string(M);
        }
        else
        {
            cout << "Неверный выбор!\n";
        }
    }
    return "";
}
