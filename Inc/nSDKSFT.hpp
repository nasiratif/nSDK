#pragma once
#include <nSDKCommon.hpp>
#include <nSDKFLTExports.hpp>

#include <SoundFilter.h>
#include <SoundError.h>
#include <CfcFile.h>

namespace Filter
{
	// The API you provide for Fusion to interact with your filter
	namespace API
	{
		// Exported as CreateFilter
		CSoundFilter* FUSION_API Create(CInputFile* pf);

		const tchar* FUSION_API GetFilterName();
		dword FUSION_API GetFilterID();
		const tchar** FUSION_API GetFilterExts();
		int32 FUSION_API GetPriority();
		bool32 FUSION_API CanReadFile(CInputFile* pif);
	}
}