#include <nSDKIFT.hpp>

namespace nSDK
{
	HINSTANCE hInst;
}

/*

---------- EXPORTED FUNCTIONS ----------

*/

bool32 WINAPI nSDK::Exports::DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		hInst = hinstDLL;

	return TRUE;
}