#include "UTF8UnicodeConverter.h"

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

char* utf8_encode(wchar_t * wch) {
	std::wstring wch_str(wch);
	std::string ch_str = utf8_encode(wch_str);
	char *ch = (char *)malloc((ch_str.length() + 1) * sizeof(char));
	rsize_t sz = ch_str.length() + 1;
	strcpy_s(ch, sz, ch_str.c_str());
	return ch;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

wchar_t * UTF8ToUnicode(const char* str)
{
	int textlen = 0;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return result;
}

#ifdef DEBUG_UTF8UNICODECONVERTER
int main()
{
	std::string in("你好");
	std::wstring out = utf8_decode(in);
	std::string outFromIn = utf8_encode(out);

	wchar_t in_wch[0x40] = L"你好abc";
	char *out_ch = utf8_encode(in_wch);
	return 0;
}
#endif
