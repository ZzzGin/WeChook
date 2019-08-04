#include "MessageDispatcher.h"

void MessageDispatcher::addEntry(string cmd, 
	DispFun func) {
	dispatcher[cmd] = func;
}

MessageDispatcher::DispFun MessageDispatcher::dispatch(string funcName) {
	return dispatcher[funcName];
}

void MessageDispatcher::runInThread(string funcName,
	MessageDispatcher::DispArg args) {
	std::thread th(dispatch(funcName), args);
	th.detach();
	return;
}

#ifdef DEBUG_MESSAGEDISPATCHER
#include <iostream>
#include <Windows.h>
int test1(MessageDispatcher::DispArg args) {
	for (auto str : args) {
		Sleep(1000);
		cout << "test1: " << str << endl;
	}
	return 0;
}

int test2(MessageDispatcher::DispArg args) {
	for (auto str : args) {
		Sleep(1000);
		cout << "test2: " << str << endl;
	}
	return 0;
}

int main() {
	MessageDispatcher md;
	md.addEntry("test1", test1);
	md.addEntry("test2", test2);
	vector<string> args;
	args.push_back("abc");
	args.push_back("def");
	md.runInThread("test1", args);
	md.runInThread("test2", args);
	Sleep(10000);
	return 0;
}

#endif // DEBUG_MESSAGEDISPATCHER
