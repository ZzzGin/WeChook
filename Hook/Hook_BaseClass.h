#pragma once
#include <Windows.h>
#include "Offsets_2_6_8_51.h"


struct wxStr {
	wchar_t * pStr;
	int strLen;
	int strLen2;
};

class Hook_BaseClass
{
protected:
	DWORD WeChatWinAddr = 0;

public:
	Hook_BaseClass();
	~Hook_BaseClass();
	DWORD getWeChatWinAddr();
	virtual void injectHook() = 0;
	virtual void ejectHook() = 0;
};

