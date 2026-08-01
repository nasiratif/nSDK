#pragma once
#include <nSDKCommon.hpp>
#include <ImageFlt.h>
#include <CfcFile.h>

// Implementation of CFilterImpl for images
class IMGFLTMGR_API CFilterImpl
{
protected:
	CFilterImpl();

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
	LPLOGPALETTE m_pSrcPal;

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
	LPLOGPALETTE m_pDestPal;
	// Source palette of previous image
	LPLOGPALETTE m_pOldSrcPal;
	// Remap table
	byte* m_remaptable;
	// 65K cache table
	byte* m_remapcache;
	// -----
};