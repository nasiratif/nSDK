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
// -----

// Image filter implementation class
class CCustomImageFilter : public CFilterImpl
{
public:
	virtual ~CCustomImageFilter();
	
	virtual void Delete();

	virtual void Initialize(byte* params = nullptr, dword paramsSize = 0);

	// Information:
	// -----
	virtual dword GetID();
	virtual const tchar* GetName();
	virtual dword GetFilterColorCaps();
	virtual dword GetVersion();
	// -----
	
	// Save customization
	virtual void SetCompressionLevel(int32 nLevel);

	virtual bool32 CanRead(CInputFile* pf);
	virtual bool32 DoesSupportImages();
	virtual bool32 DoesSupportAnimations();
	virtual bool32 CanSave();
	virtual bool32 CanSaveAnim();
	virtual int32 GetPictureInfo(CInputFile* pf);
	virtual int32 Load(CInputFile* pf, byte* pData, int32 pitch, byte* pAlpha, int32 nAlphaPitch);
	virtual int32 Save(COutputFile* pf, byte* pData, int32 nWidth, int32 nHeight, int32 nDepth, int32 nPitch, LPLOGPALETTE pPal, byte* pAlpha, int32 nAlphaPitch);
	virtual void OnCloseInputFile(CInputFile* pf);
	virtual void OnCloseOutputFile(COutputFile* pf);

	virtual int32 GetWidth();
	virtual int32 GetHeight();
	virtual int32 GetPitch();
	virtual int32 GetDepth();
	virtual dword GetDataSize();
	virtual LPLOGPALETTE GetPalette();
	virtual bool32 GetTransparentColor(COLORREF* pTranspColor);
	virtual bool32 ContainsAlphaChannel();

	virtual int32 SetDestinationFormat(int32 destDepth, LPLOGPALETTE pDestPal);

	virtual bool32 IsAnimation();
	virtual int32 GetNumberOfFrames();
	virtual int32 GetCurrentFrame();
	virtual void GetUpdateRect(RECT* pRc);
	virtual byte* GetUserInfo();
	virtual dword GetUserInfoSize();
	virtual dword GetAnimDuration();
	virtual int32 GetFrameDelay(int32 frameIndex);
	virtual int32 GetLoopCount();
	virtual int32 GetLoopFrame();
	virtual void Restart();

	// Save animation
	virtual int32 CreateAnimation(COutputFile* pf, int32 width, int32 height, int32 depth,
		int32 nFrames, int32 msFrameDuration,
		int32 nLoopCount, int32 nLoopFrame,
		byte* pUserInfo, dword dwUserInfoSize);

	virtual int32 SaveAnimationFrame(COutputFile* pf, byte* pPrevData, byte* pData, int32 width, int32 height, int32 pitch, int32 depth, LPLOGPALETTE pPal, byte* pPrevAlpha, byte* pAlpha, int32 nAlphaPitch, int32 msFrameDuration, dword dwFlags);

	virtual void AddPreviousFrameDuration(COutputFile* pf, int32 msFrameDuration);
private:
};