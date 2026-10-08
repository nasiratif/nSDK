#pragma once
#include <nSDKFLT.hpp>

#include <SoundFilter.h>
#include <SoundError.h>

namespace Filter
{
	// The API you provide for Fusion to interact with your filter
	namespace API
	{
		// Exported as CreateFilter
		CSoundFilter* FUSION_API Create(dword dwFlags);

		// May be exported as GetFilterNameW if Unicode
		const tchar* FUSION_API GetFilterName();
		dword FUSION_API GetFilterID();
		// May be exported as GetFilterExtsW if Unicode
		const tchar** FUSION_API GetFilterExts();
		dword FUSION_API GetPriority();
		// May be exported as GetDependenciesW if Unicode
		const tchar** FUSION_API GetDependencies();

		bool32 FUSION_API CanReadFile(CInputFile* pif);
	}
}