#pragma once
#include "TransitionModule.hpp"

// Define transition info here:
// -----
#define TRANS_SAMPLE_NAME _T("Rotate/Scale Sample Transition")
// Identifier for the transition (limited to 4 chars)
#define TRANS_SAMPLE_IDENTIFIER 'SAMP'
// -----

namespace Transition
{
	class CSample : public CTransitionImpl
	{
	public:
		CSample();
		virtual ~CSample();

		virtual CTransitionImpl* MakeInstance() override { return new CSample(); }
		virtual dword GetID() override { return EXT_FIX_IDENTIFIER(TRANS_SAMPLE_IDENTIFIER); }

		virtual int32 GetName(tchar* buffer, int32 bufferSize);
		virtual dword GetParameters(byte* buffer);
		virtual void SetParameters(byte* params, dword paramsSize);

		virtual HWND CreateSetupDialog(HWND hParent, dword dwReserved);
		virtual bool32 EndSetupDialog(bool32 validate);
		virtual bool32 FUSION_API ParamsProc(HWND hDlg, uint32 msg, WPARAM wParam, LPARAM lParam);

		virtual bool32 StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc);
		virtual void End();
	};
}