#pragma once


std::string get_file(bool encrypt);
std::ostringstream get_content(std::string file_name);
void put_content(std::string file_name, std::string content);

