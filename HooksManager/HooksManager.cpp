#include "HooksManager.h"



HooksManager::HooksManager()
{
}


HooksManager::~HooksManager()
{
}

void HooksManager::addHook(Hook_BaseClass * hook) {
	hookList.push_back(hook);
}

void HooksManager::injectHooks() {
	for (auto h : hookList) {
		h->injectHook();
	}
}

void HooksManager::ejectHooks() {
	for (auto h : hookList) {
		h->ejectHook();
	}
}

#ifdef DEBUG_HOOKSMANAGER

#include "../Hook/Hook_RecvMsg.h"
#include "../Hook/Hook_SendMsg.h"

int main() {
	Hook_RecvMsg rm;
	Hook_SendMsg sm;
	HooksManager hm;
	hm.addHook(&rm);
	hm.addHook(&sm);
	hm.injectHooks();
	hm.ejectHooks();
	return 0;
}
#endif
