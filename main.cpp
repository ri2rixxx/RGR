#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include "shamir.hpp"
#include "hill.hpp"
#include "hyuz.hpp"
#include "filework.hpp"

using namespace std;

int main()
{
    try
    {
        while (true)
        {
            string choise_str = "";
            char choise = '\0';
            while (true)
            {
                system("clear");
                cout << "Выберите действие" << endl;
                cout << "1) Обменяться ключами с помощью криптопротокола Хьюза" << endl;
                cout << "2) Обменяться ключами с помощью криптопротокола Шамира" << endl;
                cout << "3) Зашифровать файл шифром Хилла" << endl;
                cout << "4) Расшифровать файл шифром Хилла" << endl;
                cout << "5) Просмотреть содержимое файла" << endl;
                cout << "6) Выйти" << endl;

                getline(cin, choise_str);
                choise = choise_str[0];
                if (choise >= '1' && choise <= '6' && choise_str.size() == 1)
                {
                    break;
                }
            }

            system("clear");
            string key = "";
            string path = "";
            string text = "";
            switch (choise)
            {
            case '1':
                key = hyuz_trade();
                break;
            case '2':
                key = shamir_trade();
                break;
            case '3':
                path = get_file(1);
                text = get_content(path).str();
                cout << "Введите ключ" << endl;
                cin >> key;
                cout << "Началась шифровка..." << endl;
                text = hillCipherEncrypt(text, key);
                path = get_file(0);
                put_content(path, text);
                break;
            case '4':
                path = get_file(1);
                text = get_content(path).str();
                cout << "Введите ключ" << endl;
                cin >> key;
                cout << "Началась расшифровка..." << endl;
                text = hillCipherDecrypt(text, key);
                path = get_file(0);
                put_content(path, text);
                break;
            case '5':
                path = get_file(1);
                text = get_content(path).str();
                cout << text << endl;
                break;
            case '6':
                exit(0);
            default:
                break;
            }
            if (choise == '1' || choise == '2')
            {
                cout << "Нажмите Enter чтобы продолжить" << endl;
                getline(cin, choise_str);

                while (true)
                {
                    system("clear");
                    cout << "В результате обмена вы получили общий ключ " << key << endl;
                    cout << "Хотите зашифровать/расшифровать файл с помощью этого ключа?" << endl;
                    cout << "1) - Зашифровать" << endl;
                    cout << "2) - Расшифровать" << endl;
                    cout << "3) - Не хочу" << endl;

                    getline(cin, choise_str);
                    choise = choise_str[0];
                    if (choise >= '1' && choise <= '3' && choise_str.size() == 1)
                    {
                        break;
                    }
                }
                switch (choise)
                {
                case '1':
                    path = get_file(1);
                    text = get_content(path).str();
                    cout << "Началась шифровка..." << endl;
                    text = hillCipherEncrypt(text, key);
                    path = get_file(0);
                    put_content(path, text);

                    break;
                case '2':
                    path = get_file(1);
                    text = get_content(path).str();
                    cout << "Началась расшифровка..." << endl;
                    text = hillCipherDecrypt(text, key);
                    path = get_file(0);
                    put_content(path, text);
                    break;
                default:
                    break;
                }
            }
            cout << "Процесс завершен" << endl;
            cout << "Нажмите Enter чтобы продолжить" << endl;
            getline(cin, choise_str);
        }
        
        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << "Произошла неизвестная ошибка" << endl;
        return 1;
    }
}
