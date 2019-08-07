#include <iostream>
#include <string>
#include "json.hpp"
#include "../Socket/Socket.h"
#include "../MessageDispatcher/MessageDispatcher.h"
#include "../HooksManager/HooksManager.h"
#include "../Hook/Hook_SendMsg.h"
#include "../Hook/Hook_RecvMsg.h"
#include "../Hook/Hook_Eject.h"


void th_MainLoop() {
	Socket sock;
	char recvBuffer[DEFAULT_BUFLEN];

	HooksManager hooksManager;

	Hook_SendMsg hook_sm(&sock);
	Hook_RecvMsg hook_rm(&sock);
	Hook_Eject hook_ej(&sock, &hooksManager);

	hooksManager.addHook(&hook_sm);
	hooksManager.addHook(&hook_rm);
	hooksManager.addHook(&hook_ej);

	MessageDispatcher md;
	md.addEntry("SendMessage", [&hook_sm](MessageDispatcher::DispArg args)->int
	{
		string wxid_str = args[0];
		string msgContent_str = args[1];
		hook_sm.SendStringMessage(wxid_str, msgContent_str);
		return 0;
	});
	md.addEntry("EjectHooks", [&hook_ej](MessageDispatcher::DispArg args)->int {
		hook_ej.EjectHooks();
		return 0;
	});

	hooksManager.injectHooks();

	while (1) {
		int pullRet = sock.pull(recvBuffer, DEFAULT_BUFLEN);
		if (pullRet <= 0)
			break;

		std::string so(recvBuffer);
		auto j = nlohmann::json::parse(so);

		string c = j["cmd"];
		vector<string> a = j["args"];

		md.runInThread(c, a);
	}
	hooksManager.ejectHooks();
	sock.~Socket();
}

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)th_MainLoop, NULL, 0, NULL);
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}

	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
