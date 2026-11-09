#include "Filter.hpp"

/*

---------- SOUND FILTER API ----------

*/

CSoundFilter* FUSION_API Filter::API::Create(dword dwFlags)
{
#pragma FLT_EXPORT_CREATEFILTER
	return new CCustomSoundFilter(dwFlags);
}


const tchar* FUSION_API Filter::API::GetFilterName()
{
#pragma FLT_EXPORT_GETFILTERNAME
	return FLT_NAME;
}

dword FUSION_API Filter::API::GetFilterID()
{
#pragma FLT_EXPORT_GETFILTERID
	return EXT_FIX_IDENTIFIER(FLT_IDENTIFIER);
}

const tchar** FUSION_API Filter::API::GetFilterExts()
{
#pragma FLT_EXPORT_GETFILTEREXTS
	static const tchar* exts[] = FLT_EXTS;
	return exts;
}

dword FUSION_API Filter::API::GetPriority()
{
#pragma FLT_EXPORT_GETPRIORITY
	// Example:
	return FLT_PRIORITY;
}

const tchar** FUSION_API Filter::API::GetDependencies()
{
#pragma FLT_EXPORT_GETDEPENDENCIES
	static const tchar* deps[] = FLT_DEPENDENCIES;
	return deps;
}


bool32 FUSION_API Filter::API::CanReadFile(CInputFile* pif)
{
#pragma FLT_EXPORT_CANREADFILE
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