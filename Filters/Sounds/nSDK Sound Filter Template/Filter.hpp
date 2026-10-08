#pragma once
#include <nSDKSFT.hpp>

// Define filter information here:
// -----
#define FLT_IDENTIFIER 'NSDK'
#define FLT_NAME _T("nSDK Sound Filter Template")
// File extensions this filter supports
// Must end with NULL
#define FLT_EXTS { _T("raw"), NULL }
// Priority value for the filter, see GetPriority in the Help folder
#define FLT_PRIORITY VERYLOW
// External DLLs the filter depends on
// Must end with NULL (e.g, { _T("library1.dll"), _T("library2.dll"), NULL })
// See the Help folder for documentation regarding GetDependencies
#define FLT_DEPENDENCIES { NULL }
// -----

namespace Filter
{
	// Sound filter implementation class
	class CCustomSoundFilter : public CSoundFilter
	{
	public:
		CCustomSoundFilter(dword dwFlags);
		~CCustomSoundFilter();

		void Delete();

		int32 Open(CInputFile* pf);
		void Close();

		dword GetLength();
		dword GetPos();
		bool32 SetPos(dword dwPos);
		int32 ReadData(byte* lpDstBuffer, dword dwBufSize, dword* dwRead);

		// If overriding, make sure to call CSoundFilter::SetOutputFormat at the very end!
		// void SetOutputFormat(LPWAVEFORMATEX pStreamFormat) override;
	private:
		// Define your filter data here:
		byte* data = nullptr;
		size_t size = 0;
		size_t pos = 0;
	};
}