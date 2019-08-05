#pragma once
#include "../Socket/Socket.h"
#include "Hook_BaseClass.h"
#include "../UTF8UnicodeConverter/UTF8UnicodeConverter.h"
#include "Offsets_2_6_8_51.h"
#include <stdio.h>

constexpr auto HOOK_LEN = 5;

class Hook_RecvMsg :
	public Hook_BaseClass
{
private:
	BYTE backUpCode[HOOK_LEN] = { 0 };  // backCode
	HANDLE processHandle = 0;			// hWHND
	Socket* sock;
	BYTE backCode[HOOK_LEN] = { 0 };

public:
	Hook_RecvMsg(Socket* sock);
	~Hook_RecvMsg();
	virtual void injectHook();
	virtual void ejectHook();
};
