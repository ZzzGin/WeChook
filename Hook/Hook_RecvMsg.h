#pragma once
#include "Hook_BaseClass.h"
#include "Offsets_2_6_8_51.h"
#include <stdio.h>

constexpr auto HOOK_LEN = 5;

class Hook_RecvMsg :
	public Hook_BaseClass
{
private:
	BYTE backUpCode[HOOK_LEN] = { 0 };  // backCode
	HANDLE processHandle = 0;			// hWHND

public:
	Hook_RecvMsg();
	~Hook_RecvMsg();
	virtual void injectHook();
	virtual void ejectHook();
};
