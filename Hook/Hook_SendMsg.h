#pragma once
#include "Hook_BaseClass.h"
#include "Offsets_2_6_8_51.h"

class Hook_SendMsg :
	public Hook_BaseClass
{
public:
	Hook_SendMsg();
	~Hook_SendMsg();
	virtual void injectHook();
	virtual void ejectHook();
	VOID SendTextMessage(wchar_t * wxid, wchar_t * message);
};

