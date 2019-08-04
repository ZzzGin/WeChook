#include "Hook_BaseClass.h"



Hook_BaseClass::Hook_BaseClass()
{
	WeChatWinAddr = getWeChatWinAddr();
}


Hook_BaseClass::~Hook_BaseClass()
{
}

DWORD Hook_BaseClass::getWeChatWinAddr() {
	HMODULE wechatWinDllAddr = LoadLibrary(L"WeChatWin.dll");
	return (DWORD)wechatWinDllAddr;
}
