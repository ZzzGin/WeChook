#pragma once
#include <iostream>
#include <Windows.h>
#include <string>

std::string utf8_encode(const std::wstring &wstr);
char* utf8_encode(wchar_t * wch);
std::wstring utf8_decode(const std::string &str);
