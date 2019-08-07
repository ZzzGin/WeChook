#pragma once
#include "../Socket/Socket.h"
#include "Hook_BaseClass.h"
#include "../HooksManager/HooksManager.h"

class Hook_Eject :
	public Hook_BaseClass
{
private:
	Socket* sock;
	HooksManager * hookManager;
public:
	Hook_Eject(Socket* , HooksManager*);
	~Hook_Eject();
	virtual void injectHook();
	virtual void ejectHook();
	VOID EjectHooks();
};

