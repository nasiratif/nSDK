#pragma once
#include <nSDKIFT.hpp>

// DEFINES:
// Define filter information here:
// -----
#define FLT_IDENTIFIER 'NSDK'
#define FLT_NAME _T("nSDK Image Filter Template")
// File extensions this filter supports
// Must end with NULL
#define FLT_EXTS { _T("raw"), NULL }
// External DLLs the filter depends on
// Must end with NULL (e.g, { _T("library1.dll"), _T("library2.dll"), NULL })
#define FLT_DEPENDENCIES { NULL }
// -----

namespace Filter
{
	// Image filter implementation class
	class CCustomImageFilter : public CFilterImpl
	{
	public:
		CCustomImageFilter(dword dwFlags);

		void Delete();

		dword GetID();
		const tchar* GetName();

		bool32 CanRead(CInputFile* pf);

		int32 GetPictureInfo(CInputFile* pf);

		int32 Load(CInputFile* pf, byte* pData, int32 pitch, byte* pAlpha, int32 nAlphaPitch);
		int32 Save(COutputFile* pf, byte* pData, int32 nWidth, int32 nHeight, int32 nDepth, int32 nPitch, LPLOGPALETTE pPal, byte* pAlpha, int32 nAlphaPitch);
	private:
		// Filter data
	};
}