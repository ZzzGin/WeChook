#pragma once
#include <iostream>
#include <Windows.h>
#include <string>

std::string utf8_encode(const std::wstring &);
char* utf8_encode(wchar_t *);
std::wstring utf8_decode(const std::string &);
wchar_t * UTF8ToUnicode(const char*);
