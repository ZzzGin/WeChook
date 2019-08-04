#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "50526"
#define DEFAULT_BUFLEN 1024

class Socket
{
private:
	SOCKET ClientSocket;

public:
	Socket();
	~Socket();
	int pull(char * recvbuf, int recvBufLen);
	int push(char * sendbuf, int sendBufLen);
};

