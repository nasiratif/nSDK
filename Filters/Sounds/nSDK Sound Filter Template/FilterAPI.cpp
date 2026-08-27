#include "Filter.hpp"

/*

---------- SOUND FILTER API ----------

*/

CSoundFilter* FUSION_API Filter::API::Create(CInputFile* pf)
{
#pragma EXT_EXPORT_CREATEFILTER
	return new CCustomSoundFilter;
}


const tchar* FUSION_API Filter::API::GetFilterName()
{
#pragma EXT_EXPORT_GETFILTERNAME
	return FLT_NAME;
}

dword FUSION_API Filter::API::GetFilterID()
{
#pragma EXT_EXPORT_GETFILTERID
	return EXT_FIX_IDENTIFIER(FLT_IDENTIFIER);
}

const tchar** FUSION_API Filter::API::GetFilterExts()
{
#pragma EXT_EXPORT_GETFILTEREXTS
	static const tchar* exts[] = FLT_EXTS;
	return exts;
}

int32 FUSION_API Filter::API::GetPriority()
{
#pragma EXT_EXPORT_GETPRIORITY
	// Example:
	return INT32_MAX; // should be lowest priority; we almost always return TRUE in CanReadFile
}

bool32 FUSION_API Filter::API::CanReadFile(CInputFile* pif)
{
#pragma EXT_EXPORT_CANREADFILE
	// Example:
	if (pif->GetLength() % (sizeof(int16) * 2)) // we assume we're always reading a 16-bit, 44.1hz, 2-channel audio buffer, so we're making sure if the size is a multiple of the bit rate * num channels
		return FALSE;

	return TRUE;
}

/*

---------- ENTRY POINT ----------

*/

bool32 WINAPI DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved)
{
	return nSDK::Exports::DllMain(hinstDLL, fdwReason, lpvReserved);
}