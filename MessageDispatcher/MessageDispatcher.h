#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <thread>

using namespace std;

class MessageDispatcher
{
public:
	using DispArg = vector<string>;
	using DispFun = function<int(DispArg)>;
	void addEntry(string, DispFun);
	void runInThread(string, DispArg);

private:
	unordered_map<
		string,
		DispFun
	> dispatcher;
	DispFun dispatch(string);
};

