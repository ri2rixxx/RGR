#include <cstdio>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include "filework.hpp"
using namespace std;

string get_file(bool encrypt)
{
    FILE *pipe;
    if (encrypt)
    {
        pipe = popen("zenity --file-selection --title=\"Выберите файл для чтения данных\" 2>/dev/null", "r");
    }
    else
    {
        pipe = popen("zenity --file-selection --title=\"Выберите файл для сохранения данных\" 2>/dev/null", "r");
    }
    if (!pipe)
    {
        throw runtime_error("Невозможно выбрать файл");
    }

    char buffer[128];
    string filepath;

    while (fgets(buffer, sizeof(buffer), pipe))
    {
        filepath += buffer;
    }

    int status = pclose(pipe);
    if (status == -1)
    {
        throw runtime_error("Меню выбора файла дало сбой");
    }

    if (!filepath.empty() && filepath.back() == '\n')
    {
        filepath.pop_back();
    }
    return filepath;
}

ostringstream get_content(string file_name)
{
    ifstream file(file_name);
    if (!file.is_open())
    {
        throw runtime_error("Не получилось открыть файл " + file_name);
    }
    ostringstream ss;
    ss << file.rdbuf();
    file.close();
    return ss;
}

void put_content(string file_name, string content)
{
    ofstream file(file_name);
    if (!file.is_open())
    {
        throw runtime_error("Не получилось открыть файл " + file_name);
    }
    file << content;
    file.close();
}
