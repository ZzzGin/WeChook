#pragma once
#include <vector>
#include "../Hook/Hook_BaseClass.h"

using namespace std;
class HooksManager
{
private:
	vector<Hook_BaseClass* > hookList;

public:
	HooksManager();
	~HooksManager();
	void addHook(Hook_BaseClass*);
	void injectHooks();
	void ejectHooks();
};

