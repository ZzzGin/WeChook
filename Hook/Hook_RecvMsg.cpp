#include "Hook_RecvMsg.h"

VOID _rm_send2Socket(DWORD esi);
DWORD _rm_cEax = 0;
DWORD _rm_cEcx = 0;
DWORD _rm_cEdx = 0;
DWORD _rm_cEbx = 0;
DWORD _rm_cEsp = 0;
DWORD _rm_cEbp = 0;
DWORD _rm_cEsi = 0;
DWORD _rm_cEdi = 0;
DWORD _rm_retCallAddr = 0;
DWORD _rm_retCallAddr2 = 0;
DWORD _rm_retAddr = 0;
DWORD _rm_WeChatWinAddr = 0;
Socket* _rm_sock = nullptr;

VOID _rm_send2Socket(DWORD esi) {
	DWORD wxidAddr = esi - A_WXID_OFFSET_TO_ESI_NEG;
	DWORD messageAddr = esi - A_MESSAGE_OFFSET_TO_ESI_NEG;
	DWORD wxid2Addr = esi - A_WXID2_OFFSET_TO_ESI_NEG;
	DWORD unknownAddr = esi - A_UNKNOWN_OFFSET_TO_ESI_NEG;
	wchar_t wxid[0x100] = { 0 };
	wchar_t message[0x1000] = { 0 };
	wchar_t wxid2[0x100] = { 0 };
	wchar_t unknown[0x100] = { 0 };
	swprintf_s(wxid, L"%s", (wchar_t*)(*((LPVOID *)wxidAddr)));
	swprintf_s(message, L"%s", (wchar_t*)(*((LPVOID *)messageAddr)));
	swprintf_s(wxid2, L"%s", (wchar_t*)(*((LPVOID *)wxid2Addr)));
	swprintf_s(unknown, L"%s", (wchar_t*)(*((LPVOID *)unknownAddr)));
	char * wxid_c = utf8_encode(wxid);
	char * message_c = utf8_encode(message);
	char * wxid2_c = utf8_encode(wxid2);
	char * unknown_c = utf8_encode(unknown);
	
	// todo: send the message to the socket
	char sendBuffer[0x3000];
	sprintf_s(sendBuffer,
		"{ \"cmd\": \"RecvMessage_R\", \"args\": [\"%s\", \"%s\", \"%s\", \"%s\"] }\0",
		wxid_c, message_c, wxid2_c, unknown_c);
	_rm_sock->push(sendBuffer, strlen(sendBuffer));

	delete wxid_c, message_c, wxid2_c, unknown_c;
}

VOID __declspec(naked) _rm_HookF() {
	__asm {
		mov _rm_cEax, eax
		mov _rm_cEcx, ecx
		mov _rm_cEdx, edx
		mov _rm_cEbx, ebx
		mov _rm_cEsp, esp
		mov _rm_cEbp, ebp
		mov _rm_cEsi, esi
		mov _rm_cEdi, edi
	}
	_rm_send2Socket(_rm_cEsi);
	_rm_retCallAddr = _rm_WeChatWinAddr + A_RETCALL_ADDR_OFFSET;
	_rm_retCallAddr2 = _rm_WeChatWinAddr + A_RETCALL2_ADDR_OFFSET;
	_rm_retAddr = _rm_WeChatWinAddr + A_RET_ADDR_OFFSET;
	__asm {
		mov eax, _rm_cEax
		mov ecx, _rm_cEcx
		mov edx, _rm_cEdx
		mov ebx, _rm_cEbx
		mov esp, _rm_cEsp
		mov ebp, _rm_cEbp
		mov esi, _rm_cEsi
		mov edi, _rm_cEdi
		mov ecx, _rm_retCallAddr
		call _rm_retCallAddr2
		jmp _rm_retAddr
	}
}

Hook_RecvMsg::Hook_RecvMsg(Socket* s)
{
	_rm_WeChatWinAddr = WeChatWinAddr;
	sock = s;
	_rm_sock = s;
}


Hook_RecvMsg::~Hook_RecvMsg()
{

}

void Hook_RecvMsg::injectHook() 
{
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	DWORD hookAddr = WeChatWinAddr + A_HOOK_ADDR_OFFSET;
	
	BYTE jmpCode[HOOK_LEN] = { 0 };
	jmpCode[0] = 0xE9;				// ass code for "jmp"
	*(DWORD *)&jmpCode[1] = (DWORD)&_rm_HookF - hookAddr - HOOK_LEN;

	if (ReadProcessMemory(processHandle, (LPVOID)hookAddr, backCode, HOOK_LEN, NULL) == 0) {
		// fprintf_s(msgFile, "Failed at ReadProcessMemory\n");
		return;
	}

	if (WriteProcessMemory(processHandle, (LPVOID)hookAddr, jmpCode, HOOK_LEN, NULL) == 0) {
		// fprintf_s(msgFile, "Failed at WriteProcessMemory\n");
		return;
	}
}

void Hook_RecvMsg::ejectHook() 
{
	DWORD hookAddr = WeChatWinAddr + A_HOOK_ADDR_OFFSET;
	if (WriteProcessMemory(processHandle, (LPVOID)hookAddr, backCode, HOOK_LEN, NULL) == 0) {
		// fprintf_s(msgFile, "Failed to recover the hook\n");
		return;
	}
}

