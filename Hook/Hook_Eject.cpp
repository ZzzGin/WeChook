#include "Hook_Eject.h"



Hook_Eject::Hook_Eject(Socket* s, HooksManager* hm)
{
	sock = s;
	hookManager = hm;
}


Hook_Eject::~Hook_Eject()
{
}

void Hook_Eject::injectHook() {

}

void Hook_Eject::ejectHook() {

}

VOID Hook_Eject::EjectHooks() {
	hookManager->ejectHooks();
	char sendBuffer[0x3000];
	sprintf_s(sendBuffer,
		"{ \"cmd\": \"EjectHooks_R\", \"args\": [\"Success\"] }\0"
	);
	sock->push(sendBuffer, strlen(sendBuffer));
}
