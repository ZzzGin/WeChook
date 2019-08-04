#include <iostream>
#include <string>
#include "json.hpp"
#include "../Socket/Socket.h"
#include "../MessageDispatcher/MessageDispatcher.h"

class TestClass {
public:
	int test1(MessageDispatcher::DispArg args) {
		for (auto str : args) {
			cout << "test1: " << str << endl;
			Sleep(1000);
		}
		return 0;
	}
	int test2(MessageDispatcher::DispArg args) {
		for (auto str : args) {
			cout << "test2: " << str << endl;
			Sleep(1000);
		}
		return 0;
	}
};

int main()
{
	Socket sock;
	char recvBuffer[DEFAULT_BUFLEN];

	TestClass tc;

	MessageDispatcher md;
	md.addEntry("test1", [&tc](MessageDispatcher::DispArg args)->int
	{
		return tc.test1(args);
	});
	md.addEntry("test2", [&tc](MessageDispatcher::DispArg args)->int
	{
		return tc.test2(args);
	});

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
	return 0;
}
