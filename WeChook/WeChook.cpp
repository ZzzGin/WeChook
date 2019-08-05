#include <iostream>
#include <string>
#include "json.hpp"
#include "../Socket/Socket.h"
#include "../MessageDispatcher/MessageDispatcher.h"
#include "../HooksManager/HooksManager.h"
#include "../Hook/Hook_SendMsg.h"
#include "../Hook/Hook_RecvMsg.h"

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

void th_MainLoop() {
	Socket sock;
	char recvBuffer[DEFAULT_BUFLEN];

	Hook_SendMsg hook_sm;
	Hook_RecvMsg hook_rm(&sock);

	HooksManager hooksManager;
	hooksManager.addHook(&hook_sm);
	hooksManager.addHook(&hook_rm);

	MessageDispatcher md;
	md.addEntry("SendMessage", [&hook_sm](MessageDispatcher::DispArg args)->int
	{
		string wxid_str = args[0];
		string msgContent_str = args[1];
		char wxid_ch[0x50];
		strcpy_s(wxid_ch, wxid_str.c_str());
		char msgContent_ch[0x1000];
		strcpy_s(msgContent_ch, msgContent_str.c_str());
		wchar_t* wxid_wc = UTF8ToUnicode(wxid_ch);
		wchar_t* msgContent_wc = UTF8ToUnicode(msgContent_ch);
		//wcscpy_s(wxid_wc, char2wchar(wxid_ch));
		//wcscpy_s(msgContent_wc, char2wchar(msgContent_ch));
		hook_sm.SendTextMessage(wxid_wc, msgContent_wc);
		delete wxid_wc;
		delete msgContent_wc;
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

		int pushRet = sock.push(recvBuffer, pullRet);
		if (pushRet == SOCKET_ERROR) {
			break;
		}
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
