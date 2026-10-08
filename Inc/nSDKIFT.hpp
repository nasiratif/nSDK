#pragma once
#include <nSDKFLT.hpp>

#include <ImageFlt.h>

// Implementation of CFilterImpl for images
class IMGFLTMGR_API CFilterImpl
{
protected:
	// Protected constructor: must use static member function to create a CFilterImpl
	CFilterImpl();
public:
	virtual ~CFilterImpl();

	virtual void Delete();

	virtual void Initialize(byte* params = NULL, dword paramsSize = 0L);

	virtual dword GetID();
	virtual const tchar* GetName();
	virtual dword GetFilterColorCaps();
	virtual	dword GetVersion();

	// Save customization
	virtual void SetCompressionLevel(int32 nLevel);

	virtual bool32 CanRead(CInputFile* pf);
	virtual	bool32 DoesSupportImages();
	virtual	bool32 DoesSupportAnimations();
	virtual bool32 CanSave();
	virtual bool32 CanSaveAnim();

	virtual int32 GetPictureInfo(CInputFile* pf);

	virtual int32 Load(CInputFile* pf, byte* pData, int32 pitch, byte* pAlpha, int32 nAlphaPitch);
	virtual int32 Save(COutputFile* pf, byte* pData, int32 nWidth, int32 nHeight, int32 nDepth, int32 nPitch, LOGPALETTE* pPal, byte* pAlpha, int32 nAlphaPitch);

	virtual void OnCloseInputFile(CInputFile* pf);
	virtual void OnCloseOutputFile(COutputFile* pf);

	virtual int32 GetWidth();
	virtual int32 GetHeight();
	virtual int32 GetPitch();
	virtual int32 GetDepth();
	virtual dword GetDataSize();
	virtual LOGPALETTE* GetPalette();
	virtual bool32 GetTransparentColor(COLORREF* pTranspColor);
	virtual bool32 ContainsAlphaChannel();

	virtual int32 SetDestinationFormat(int32 destDepth, LOGPALETTE* pDestPal);

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

	virtual int32 SaveAnimationFrame(COutputFile* pf, byte* pPrevData, byte* pData, int32 width, int32 height, int32 pitch, int32 depth, LOGPALETTE* pPal, byte* pPrevAlpha, byte* pAlpha, int32 nAlphaPitch, int32 msFrameDuration, dword dwFlags);

	virtual void AddPreviousFrameDuration(COutputFile* pf, int32 msFrameDuration);
protected:
	void ComputeWidthBytes();

	bool32 AllocatePalette(uint32 nColors);

	bool32 NeedConversion() { return m_bNeedConversion; }
	bool32 PrepareConversion();
	bool32 PrepareRemapTable();
	bool32 ConvertLine(byte* dest, byte* src, int32 width);
protected:
	// Picture info
	// -----
	// Width, in pixels
	int32 m_Width;
	// Height, in pixels
	int32 m_Height;
	// Bits per pixel
	int32 m_Depth;
	// Exact width in bytes
	int32 m_WidthBytes;
	// Pitch (width in bytes + padding)
	int32 m_PicturePitch;
	uint32 m_PixelWidthBytes;
	bool32 m_bAlphaChannel;

	int32 m_nPlanes;
	// Number of pixels per plane
	int32 m_NBitsPerPixelPerPlane;
	// Plane pitch
	int32 m_PlaneWidthBytes;
	// -----

	// Animation info
	// -----
	int32 m_nFrames;
	int32 m_curFrame;
	dword m_msFrameDuration;
	dword m_msAnimDuration;
	// -----

	// Picture palette
	LOGPALETTE* m_pSrcPal;

	// Conversion:
	// -----
	// Conversion required
	bool32 m_bNeedConversion;
	// Change in destination palette
	bool32 m_bDestPalChanged;
	// Destination depth
	int32 m_destDepth;
	// Destination pixel width in bytes
	uint32 m_destPixelWidthBytes;
	// Destination palette
	LOGPALETTE* m_pDestPal;
	// Source palette of previous image
	LOGPALETTE* m_pOldSrcPal;
	// Remap table
	byte* m_remaptable;
	// 65K cache table
	byte* m_remapcache;
	// -----
};

namespace Filter
{
	// The API you provide for Fusion to interact with your filter
	namespace API
	{
		// Exported as CreateFilter
		CFilterImpl* FUSION_API Create(dword dwFlags);

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