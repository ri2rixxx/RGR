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

string hyuz_trade()
{
    int64_t n = 1000000007; // Простое число
    int64_t g = 0;          // Основание
    while (true)
    {
        cout << "Ваше простое число N:\n"
             << n << endl;
        cout << "Введите новое число или нажмите Enter чтобы оставить текущее: ";

        string input;
        getline(cin, input);
        if (input.empty())
        {
            break;
        }

        try
        {
            int64_t new_n = stoll(input);
            if (!is_prime(new_n))
            {
                system("clear");
                cout << "Ошибка: число должно быть простым!\n";
                continue;
            }
            n = new_n;
            system("clear");
            break;
        }
        catch (...)
        {
            system("clear");
            cout << "Ошибка: введите корректное число!\n";
        }
    }

    g = random_in_range(2, n - 2);

    while (true)
    {
        cout << "Ваше основание G:\n"
             << g << endl;
        cout << "Введите новое число или нажмите Enter чтобы оставить текущее: ";

        string input;
        getline(cin, input);
        if (input.empty())
        {
            break;
        }

        try
        {
            int64_t new_g = stoll(input);
            if (new_g >= n)
            {
                system("clear");
                cout << "Ошибка: число должно быть меньше N!\n";
                continue;
            }
            g = new_g;
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
            int64_t x = random_in_range(2, 100000000000LL);
            int64_t k = mod_exp(g, x, n);

            cout << "\nВаш закрытый ключ x: " << x << endl;
            cout << "Вычисленный закрытый ключ k: " << k << endl;

            cout << "\nВведите полученное от абонента B значение Y: ";
            int64_t Y;
            cin >> Y;
            cin.ignore();

            int64_t X = mod_exp(Y, x, n);
            cout << "\nОтправьте абоненту B значение X: " << X << endl;

            cout << "\nОбщий секретный ключ: " << k << endl;
            return to_string(k);
        }
        else if (choice == "2")
        {

            int64_t y = random_in_range(2, 100000000000LL);
            int64_t Y = mod_exp(g, y, n);

            cout << "\nВаш закрытый ключ y: " << y << endl;
            cout << "Отправьте абоненту A значение Y: " << Y << endl;

            cout << "\nВведите полученное от абонента A значение X: ";
            int64_t X;
            cin >> X;
            cin.ignore();

            int64_t z = mod_inverse(y, n - 1);
            int64_t k_prime = mod_exp(X, z, n);

            cout << "\nОбщий секретный ключ: " << k_prime << endl;
            return to_string(k_prime);
        }
        else
        {
            cout << "Неверный выбор!\n";
        }
    }
    return "";
}
