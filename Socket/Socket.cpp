#include "Socket.h"

Socket::Socket()
{
	WSADATA wsaData;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	SOCKET ListenSocket = INVALID_SOCKET;
	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP connection options
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	iResult = setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptVal, bOptLen);
	if (iResult == SOCKET_ERROR) {
		printf("setsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}

Socket::~Socket()
{
	// shutdown the send half of the connection since no more data will be sent
	int iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return;
}

int Socket::pull(char * recvbuf, int recvBufLen) {
	int recvbuflen = recvBufLen;
	int recvBytes = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (recvBytes > 0) {
		return recvBytes;
	}
	else if (recvBytes == 0) {
		printf("Connection closing...\n");
		return recvBytes;
	}
	else {
		printf("recv failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return recvBytes;
	}
}

int Socket::push(char * sendbuf, int sendBufLen) {
	int iSendResult = send(ClientSocket, sendbuf, sendBufLen, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return iSendResult;
	}
	return iSendResult;
}


#ifdef DEBUG_SOCKET
#include <string>

const wchar_t * char2wchar(char * in) {
	size_t newsize = strlen(in) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, in, _TRUNCATE);
	return wcstring;
}

int main()
{
	Socket sock;
	char recvBuffer[DEFAULT_BUFLEN];
	int pullRet = 1;
	while (1) {
		pullRet = sock.pull(recvBuffer, DEFAULT_BUFLEN);
		if (pullRet <= 0)
			break;

		const wchar_t * wcstring = char2wchar(recvBuffer);
		wprintf(L"\"%s\"\n", wcstring);
		delete wcstring;

		int pushRet = sock.push(recvBuffer, pullRet);
		if (pushRet == SOCKET_ERROR) {
			break;
		}
	}
	return 0;
}
#endif
