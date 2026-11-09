#pragma once
#include "TransModule.hpp"

// Define transition info here:
// -----
#define TRANS_NAME_SAMPLE _T("Rotate/Scale Sample Transition")
// Identifier for the transition (limited to 4 chars)
#define TRANS_IDENTIFIER_SAMPLE 'SAMP'
// -----

namespace Transition
{
	class CSample : public CTransitionImpl
	{
	public:
		CSample();
		~CSample();

		CTransitionImpl* MakeInstance() override { return new CSample(); }
		int32 Supports(int32 nFeature, LPARAM lParam) override;
		dword GetID() override { return EXT_FIX_IDENTIFIER(TRANS_IDENTIFIER_SAMPLE); }
		int32 GetName(tchar* buffer, int32 bufferSize) override;
		dword GetParameters(byte* buffer) override;
		void SetParameters(byte* params, dword paramsSize) override;

		HWND CreateSetupDialog(HWND hParent, dword dwReserved) override;
		bool32 EndSetupDialog(bool32 validate) override;
		bool32 FUSION_API ParamsProc(HWND hDlg, uint32 msg, WPARAM wParam, LPARAM lParam) override;

		bool32 StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc) override;
		bool32 StepDrawBlit(cSurface* dest, int32 x, int32 y, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc, BlitMode bm, BlitOp bop, LPARAM lParam, dword dwBlitFlags) override;
		void End() override;
	};
}