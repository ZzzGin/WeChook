#include <vector>
#include "../Hook/Hook_BaseClass.h"
#include "../Hook/Hook_SendMsg.h"

#ifdef TEST_BASETEST

using namespace std;

int main() {
	vector<Hook_BaseClass *> bcv;
	Hook_SendMsg hsm;
	bcv.push_back(&hsm);
	bcv[0]->ejectHook();
	return 0;
}

#endif
