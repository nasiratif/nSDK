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

// StepDraw flags:
#define	TRFLAG_FADEIN 0x0001 // fade-in mode
#define	TRFLAG_FADEOUT 0x0002 // fade-out mode
#define	TRFLAG_TRANSPARENT 0x0004 // transparent mode
// ---
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
	virtual dword GetID() = 0;
	virtual int32 GetName(tchar* buffer, int32 bufferSize);
	virtual dword GetMode();
	virtual dword GetParameters(byte* buffer);
	virtual void SetParameters(byte* params, dword paramsSize);
	virtual HWND CreateSetupDialog(HWND hParent, dword dwReserved);
	virtual bool32 EndSetupDialog(bool32 validate);
	virtual bool32 FUSION_API ParamsProc(HWND hDlg, uint32 msg, WPARAM wParam, LPARAM lParam);
	// -----

	// Runtime:
	// -----
	// If overriding, this must be called by derived classes!
	virtual bool32 Start(dword durationMS);
	virtual bool32 StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc);
	// If overriding, this must be called by derived classes!
	virtual void End();
	virtual bool32 IsCompleted();
protected:
	virtual void Restart();
	// Contains a sample delta-time implementation
	dword GetDeltaTime();
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
};

// Transition base class, simply wraps the Impl class and is what you actually send to Fusion
class CTransition
{
public:
	virtual void FUSION_API Delete();

	// Information:
	// -----
	virtual dword FUSION_API GetMode();
	virtual int32 FUSION_API GetName(tchar* buffer, int32 bufferSize);
	virtual dword FUSION_API GetID();
	virtual void FUSION_API SetParameters(byte* params, dword paramsSize);
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
	// -----

public:
	CTransition(CTransitionImpl* pImpl);
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