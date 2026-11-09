#pragma once
#include "nSDKCommon.hpp"
#include "nSDKTransExports.hpp"

#include <Ccxhdr.h>
#include <Surface.h>

// DEFINES:
// -----
#define MODULEID_TRANSITIONS DWORDSTR('C','C','T','R')

// From MMFTransSDK:
// ---
#define	TRMODE_FADEIN 0x0001	// not used
#define	TRMODE_FADEOUT 0x0002	// not used
#define TRMODE_REFRESHBACKGROUNDWHENCHANGES	0x0004 // refresh the background surface before each call to StepDraw
#define TRMODE_CLEARDEST 0x0008

// StepDraw flags:
#define	TRFLAG_FADEIN 0x0001 // fade-in mode
#define	TRFLAG_FADEOUT 0x0002 // fade-out mode
#define	TRFLAG_TRANSPARENT 0x0004 // transparent mode
#define	TRFLAG_DIRECTTODEST 0x0008 // the original (fade in) or final (fade out) surface contains a copy of the background, it can be ignored (unless the transition affects the background too)
// ---

// Versions:
// ---
#define TRANS_VERSION_1	0x0001
#define TRANS_VERSION_2	0x0002 // current version
// ---
// -----

// Features
enum
{
	TRF_STEPDRAWBLIT = 1
};

// Common directions:
// -----
enum
{
	LEFT_RIGHT = 0,
	RIGHT_LEFT,
	TOP_BOTTOM,
	BOTTOM_TOP
};
enum
{
	CENTER_LEFTRIGHT = 0,
	LEFTRIGHT_CENTER,
	CENTER_TOPBOTTOM,
	TOPBOTTOM_CENTER
};
enum
{
	TOP_LEFT = 0,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTER
};
enum
{
	DIR_HORZ = 0,
	DIR_VERT
};
// -----

// Transition implementation base class
class CTransitionImpl
{
protected:
	CTransitionImpl();
public:
	virtual ~CTransitionImpl();

	void Initialize(byte* params = nullptr, dword paramsSize = 0);

	// General:
	// -----
	virtual CTransitionImpl* MakeInstance() = 0;
	virtual	int32 Supports(int32 nFeature, LPARAM lParam);
	virtual dword GetID() = 0;
	virtual dword GetMode();
	virtual int32 GetName(tchar* buffer, int32 bufferSize);
	virtual dword GetParameters(byte* buffer);
	virtual void SetParameters(byte* params, dword paramsSize);
	// -----
	
	// Setup:
	// -----
	virtual HWND CreateSetupDialog(HWND hParent, dword dwReserved);
	virtual bool32 EndSetupDialog(bool32 validate);
	virtual bool32 FUSION_API ParamsProc(HWND hDlg, uint32 msg, WPARAM wParam, LPARAM lParam);
	// -----

	// Runtime:
	// -----
	// If overriding, this must be called by derived classes!
	virtual bool32 Start(dword durationMS);
	virtual bool32 StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc);
	virtual bool32 StepDrawBlit(cSurface* dest, int32 x, int32 y, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc, BlitMode bm, BlitOp bop, LPARAM lParam, dword dwBlitFlags);
	// If overriding, this must be called by derived classes!
	virtual void End();
	virtual bool32 IsCompleted();

	virtual int32 GetTimePos();
	virtual void SetTimePos(int32 msTimePos);
protected:
	virtual void Restart();
	// Contains a sample delta-time implementation
	dword GetDeltaTime();
	bool32 NeedTempSurface(BlitMode bm, BlitOp bop, LPARAM lParam);
	cSurface* GetTempSurface(cSurface* pObjectSf);
	// -----
protected:
	// Data:
	int32 m_nameResID;
	int32 m_dialResID;
	HWND m_hDlg;

	dword m_initTime;
	dword m_currentTime;
	dword m_endTime;
	dword m_duration;
	bool32 m_overflow;
	bool32 m_running;
	bool32 m_starting;
	cSurface* m_pTempSf;
};

// Transition base class, simply wraps the Impl class and is what you actually send to Fusion
class CTransition
{
public:
	CTransition(CTransitionImpl* pImpl);

	virtual void FUSION_API Delete();

	// Information:
	// -----
	virtual int32 FUSION_API GetVersion();
	virtual int32 FUSION_API GetName(tchar* buffer, int32 bufferSize);
	virtual dword FUSION_API GetID();
	virtual dword FUSION_API GetMode();
	virtual dword FUSION_API GetParameters(byte* buffer);
	// -----

	// Setup:
	// -----
	virtual HWND FUSION_API CreateSetupDialog(HWND hParent, dword dwReserved);
	virtual bool32 FUSION_API EndSetupDialog(bool32 validate);
	// -----

	// Runtime:
	// -----
	virtual bool32 FUSION_API Start(dword durationMS);
	virtual bool32 FUSION_API StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc);
	virtual void FUSION_API End();
	virtual bool32 FUSION_API IsCompleted();

	virtual int32 FUSION_API GetTimePos();
	virtual void FUSION_API SetTimePos(int32 msTimePos);
	
	virtual int32 FUSION_API Supports(int32 nFeature, LPARAM lParam);

	virtual bool32 FUSION_API StepDrawBlit(cSurface* dest, int32 x, int32 y, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc, BlitMode bm, BlitOp bop, LPARAM lParam, dword dwBlitFlags);
	// -----
protected:
	virtual ~CTransition();
private:
	CTransitionImpl* m_pImpl;
};

namespace Transition
{
	namespace API
	{
		// Exported as CreateTransition
		CTransition* FUSION_API Create(tchar* reserved, dword transID, byte* params, dword paramsSize);

		dword FUSION_API GetModuleType();
		int32 FUSION_API GetModuleName(tchar* buffer, int32 bufferSize);
		dword FUSION_API GetModuleID();
		int32 FUSION_API GetTransCount();
		int32 FUSION_API GetTransName(int32 index, tchar* buffer, int32 bufferSize);
		dword FUSION_API GetTransID(int32 index);
		dword FUSION_API GetTransMode(int32 index);

		bool32 FUSION_API IsUnicode();
	}
}