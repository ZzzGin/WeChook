#pragma once
#include "../Socket/Socket.h"
#include "Hook_BaseClass.h"
#include "Offsets_2_6_8_51.h"
#include "../UTF8UnicodeConverter/UTF8UnicodeConverter.h"
#include <string>

class Hook_SendMsg :
	public Hook_BaseClass
{
private:
	Socket* sock;
public:
	Hook_SendMsg(Socket*);
	~Hook_SendMsg();
	virtual void injectHook();
	virtual void ejectHook();
	VOID SendTextMessage(wchar_t *, wchar_t *);
	VOID SendStringMessage(std::string, std::string);
};

