#include "Filter.hpp"

/*

NB:
	Image filters are placed in <Fusion root>\Filters\Images
	Image filters are always assumed to be non-Unicode by Fusion, so you shouldn't compile this filter as Unicode
*/

/*

---------- IMAGE FILTER EXPORTS ----------

*/

bool32 WINAPI DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved)
{
	return nSDK::Exports::DllMain(hinstDLL, fdwReason, lpvReserved);
}


// Called when an image filter instance needs to be made
// Note that this could also be called at edittime
CCustomImageFilter* FUSION_API CreateFilter(dword dwFlags)
{
#pragma EXT_DLLEXPORT
	return new CCustomImageFilter;
}


// Called at Fusion startup to retrieve the name of your image filter (what is shown in the Image Filters tab)
const tchar* FUSION_API GetFilterName()
{
#pragma EXT_DLLEXPORT
	return FLT_NAME;
}

// Called at Fusion startup to retrieve the identifier of your image filter
dword FUSION_API GetFilterID()
{
#pragma EXT_DLLEXPORT
	return EXT_FIX_IDENTIFIER(FLT_IDENTIFIER);
}

// Called at Fusion startup to retrieve the file extensions that your filter supports
const tchar** FUSION_API GetFilterExts()
{
#pragma EXT_DLLEXPORT
	static const tchar* exts[] = FLT_EXTS;
	return exts;
}

// Implement this function so Fusion can retrieve the priority of your image filter
// This function is optional; if not exported, your filter won't have any explicit priority setting
int32 FUSION_API GetPriority()
{
#pragma EXT_DLLEXPORT
	// Example:
	return INT32_MAX; // should be lowest priority; we almost always return TRUE in CanReadFile
}


// Called to determine whether your filter is capable of reading the specified file
// You could, for example, read the first 4 bytes and check if it matches the byte signature "PNG", if we were writing our own PNG filter
// If you return FALSE, Fusion ignores your filter for that file & looks into other filters
bool32 FUSION_API CanReadFile(CInputFile* pif)
{
#pragma EXT_DLLEXPORT
	return TRUE;
}

/*

---------- FILTER IMPLEMENTATION ----------

*/

CCustomImageFilter::~CCustomImageFilter()
{

}


void CCustomImageFilter::Delete()
{
	delete this;
}


void CCustomImageFilter::Initialize(byte* params, dword paramsSize)
{

}


dword CCustomImageFilter::GetID()
{
	return GetFilterID();
}

const tchar* CCustomImageFilter::GetName()
{
	return GetFilterName();
}

dword CCustomImageFilter::GetFilterColorCaps()
{
	return 0;
}

dword CCustomImageFilter::GetVersion()
{
	return 0;
}


void CCustomImageFilter::SetCompressionLevel(int32 nLevel)
{

}


bool32 CCustomImageFilter::CanRead(CInputFile* pf)
{
	return CanReadFile(pf);
}

bool32 CCustomImageFilter::DoesSupportImages()
{
	return TRUE;
}

bool32 CCustomImageFilter::DoesSupportAnimations()
{
	return FALSE;
}

bool32 CCustomImageFilter::CanSave()
{
	return FALSE;
}

bool32 CCustomImageFilter::CanSaveAnim()
{
	return FALSE;
}

int32 CCustomImageFilter::GetPictureInfo(CInputFile* pf)
{
	return 0;
}

int32 CCustomImageFilter::Load(CInputFile* pf, byte* pData, int32 pitch, byte* pAlpha, int32 nAlphaPitch)
{
	return 0;
}

int32 CCustomImageFilter::Save(COutputFile* pf, byte* pData, int32 nWidth, int32 nHeight, int32 nDepth, int32 nPitch, LPLOGPALETTE pPal, byte* pAlpha, int32 nAlphaPitch)
{
	return 0;
}

void CCustomImageFilter::OnCloseInputFile(CInputFile* pf)
{

}

void CCustomImageFilter::OnCloseOutputFile(COutputFile* pf)
{

}


int32 CCustomImageFilter::GetWidth()
{
	return m_Width;
}

int32 CCustomImageFilter::GetHeight()
{
	return m_Height;
}

int32 CCustomImageFilter::GetPitch()
{
	return m_PicturePitch;
}

int32 CCustomImageFilter::GetDepth()
{
	return m_Depth;
}

dword CCustomImageFilter::GetDataSize()
{
	return 0;
}

LPLOGPALETTE CCustomImageFilter::GetPalette()
{
	return nullptr;
}

bool32 CCustomImageFilter::GetTransparentColor(COLORREF* pTranspColor)
{
	*pTranspColor = 0;
	return TRUE;
}

bool32 CCustomImageFilter::ContainsAlphaChannel()
{
	return bool32();
}


int32 CCustomImageFilter::SetDestinationFormat(int32 destDepth, LPLOGPALETTE pDestPal)
{
	return 0;
}


bool32 CCustomImageFilter::IsAnimation()
{
	return FALSE;
}

int32 CCustomImageFilter::GetNumberOfFrames()
{
	return 0;
}

int32 CCustomImageFilter::GetCurrentFrame()
{
	return 0;
}

void CCustomImageFilter::GetUpdateRect(RECT* pRc)
{

}

byte* CCustomImageFilter::GetUserInfo()
{
	return nullptr;
}

dword CCustomImageFilter::GetUserInfoSize()
{
	return 0;
}

dword CCustomImageFilter::GetAnimDuration()
{
	return 0;
}

int32 CCustomImageFilter::GetFrameDelay(int32 frameIndex)
{
	return 0;
}

int32 CCustomImageFilter::GetLoopCount()
{
	return 0;
}

int32 CCustomImageFilter::GetLoopFrame()
{
	return 0;
}

void CCustomImageFilter::Restart()
{

}


int32 CCustomImageFilter::CreateAnimation(COutputFile* pf, int32 width, int32 height, int32 depth, int32 nFrames, int32 msFrameDuration, int32 nLoopCount, int32 nLoopFrame, byte* pUserInfo, dword dwUserInfoSize)
{
	return 0;
}


int32 CCustomImageFilter::SaveAnimationFrame(COutputFile* pf, byte* pPrevData, byte* pData, int32 width, int32 height, int32 pitch, int32 depth, LPLOGPALETTE pPal, byte* pPrevAlpha, byte* pAlpha, int32 nAlphaPitch, int32 msFrameDuration, dword dwFlags)
{
	return 0;
}


void CCustomImageFilter::AddPreviousFrameDuration(COutputFile* pf, int32 msFrameDuration)
{

}
