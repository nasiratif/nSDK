#pragma once
#include <nSDKSFT.hpp>

// Define filter information here:
// -----
#define FLT_IDENTIFIER 'NSDK'
#define FLT_NAME _T("nSDK Sound Filter Template")
// File extensions this filter supports
// Must end with NULL
#define FLT_EXTS { _T("raw"), NULL }
// -----

namespace Filter
{
	// Sound filter implementation class
	class CCustomSoundFilter : public CSoundFilter
	{
	public:
		~CCustomSoundFilter() override;

		void Delete() override;

		int32 Open(CInputFile* pf) override;
		void Close() override;

		dword GetLength() override;
		dword GetPos() override;
		bool32 SetPos(dword dwPos) override;
		int32 ReadData(byte* lpDstBuffer, dword dwBufSize, dword* dwRead) override;

		void SetOutputFormat(LPWAVEFORMATEX pStreamFormat);
	private:
		// Define your filter data here:
		byte* data = nullptr;
		size_t size = 0;
		size_t pos = 0;
	};
}