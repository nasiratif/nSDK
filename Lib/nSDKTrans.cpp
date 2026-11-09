#include <nSDKTrans.hpp>

using namespace Transition;

/*

---------- TRANSITION IMPLEMENTATION BASE CLASS ----------

*/

CTransitionImpl::CTransitionImpl()
{
	m_nameResID = 0;
	m_dialResID = 0;
	m_hDlg = NULL;
	m_running = FALSE;
	m_starting = FALSE;
}

CTransitionImpl::~CTransitionImpl()
{
	if (m_hDlg != NULL && IsWindow(m_hDlg))
		DestroyWindow(m_hDlg);
	m_hDlg = NULL;
	if (m_running)
		End();
}


void CTransitionImpl::Initialize(byte* params, dword paramsSize)
{
	if (params)
		SetParameters(params, paramsSize);
}

/*

----- GENERAL -----

*/

int32 CTransitionImpl::Supports(int32 nFeature, LPARAM lParam)
{
	return FALSE;
}

dword CTransitionImpl::GetMode()
{
	return 0;
}

int32 CTransitionImpl::GetName(tchar* buffer, int32 bufferSize)
{
	StringCbCopy(buffer, bufferSize, _T("(undefined transition)"));
	return 0;
}

dword CTransitionImpl::GetParameters(byte* buffer)
{
	return 0;
}

void CTransitionImpl::SetParameters(byte* params, dword paramsSize)
{
	// should be overriden
}

/*

----- SETUP -----

*/


HWND CTransitionImpl::CreateSetupDialog(HWND hParent, dword dwReserved)
{
	return NULL; // also should be overriden..
}

bool32 CTransitionImpl::EndSetupDialog(bool32 validate)
{
#ifdef EXT_EDITOR
	// Close dialog
	if (m_hDlg && IsWindow(m_hDlg))
		DestroyWindow(m_hDlg);

	m_hDlg = NULL;
	return TRUE;
#endif
	return TRUE; // dialog can close now
}

bool32 FUSION_API CTransitionImpl::ParamsProc(HWND hDlg, uint32 msg, WPARAM wParam, LPARAM lParam)
{
#ifdef EXT_EDITOR
	switch (msg)
	{
	case WM_INITDIALOG:
		return TRUE;
	}
#endif
	return FALSE;
}

/*

----- RUNTIME -----

*/

bool32 CTransitionImpl::Start(dword durationMS)
{
	m_initTime = timeGetTime();
	m_overflow = FALSE;
	if ((DWORD)m_initTime >= 0x7FFFFFFF)
	{
		m_overflow = TRUE;
		m_initTime -= 0x7FFFFFFF;
	}
	m_duration = durationMS;
	if (m_duration == 0)
		m_duration = 1;
	m_currentTime = m_initTime;
	m_endTime = m_initTime + m_duration;
	m_running = TRUE;
	m_starting = TRUE;
	return TRUE;
}

bool32 CTransitionImpl::StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc)
{
	return FALSE;
}

bool32 CTransitionImpl::StepDrawBlit(cSurface* dest, int32 x, int32 y, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc, BlitMode bm, BlitOp bop, LPARAM lParam, dword dwBlitFlags)
{
	return FALSE;
}

void CTransitionImpl::End()
{
	m_running = FALSE;
}

bool32 CTransitionImpl::IsCompleted()
{
	return m_running ? (timeGetTime() >= m_endTime) : TRUE;
}


int32 CTransitionImpl::GetTimePos()
{
	return (int32)(m_currentTime - m_initTime);
}

void CTransitionImpl::SetTimePos(int32 msTimePos)
{
	m_initTime = (m_currentTime - msTimePos);
	m_endTime = m_initTime + m_duration;
}


void CTransitionImpl::Restart()
{
	m_currentTime = m_endTime;
	m_starting = TRUE;
}

dword CTransitionImpl::GetDeltaTime()
{
	m_currentTime = timeGetTime();
	if (m_overflow)
		m_currentTime -= 0x7FFFFFFF;
	if (m_currentTime > m_endTime)
		m_currentTime = m_endTime;
	return m_currentTime - m_initTime;
}

bool32 CTransitionImpl::NeedTempSurface(BlitMode bm, BlitOp bop, LPARAM lParam)
{
	bool bEffect = false;
	if ((bop & BOP_MASK) != BOP_COPY)
		bEffect = true;
	else if ((bop & BOP_RGBAFILTER) != 0 && lParam != 0xFFFFFFFF)
		bEffect = true;
	return (bEffect || bm != BMODE_OPAQUE);
}

cSurface* CTransitionImpl::GetTempSurface(cSurface* pObjectSf)
{
	if (!m_pTempSf)
	{
		cSurface* pProto;

		m_pTempSf = new cSurface;
		GetSurfacePrototype(&pProto, pObjectSf->GetDepth(), ST_HWA_RTTEXTURE, pObjectSf->GetDriver());
		m_pTempSf->Create(pObjectSf->GetWidth(), pObjectSf->GetHeight(), pProto);
		m_pTempSf->SetTransparentColor(pObjectSf->GetTransparentColor());
		if (pObjectSf->GetDepth() == 8)
			m_pTempSf->SetPalette(pObjectSf->GetPalette());
	}
	return m_pTempSf;
}

/*

---------- TRANSITION BASE CLASS ----------

*/

CTransition::CTransition(CTransitionImpl* pImpl)
{
	m_pImpl = pImpl;
}


void FUSION_API CTransition::Delete()
{
	delete this;
}

/*

----- INFORMATION -----

*/

int32 FUSION_API CTransition::GetVersion()
{
	return TRANS_VERSION_2;
}

int32 FUSION_API CTransition::GetName(tchar* buffer, int32 bufferSize)
{
	return m_pImpl->GetName(buffer, bufferSize);
}

dword FUSION_API CTransition::GetID()
{
	return m_pImpl->GetID();
}

dword FUSION_API CTransition::GetMode()
{
	return m_pImpl->GetMode();
}

dword FUSION_API CTransition::GetParameters(byte* buffer)
{
	return m_pImpl->GetParameters(buffer);
}

/*

----- SETUP -----

*/

HWND FUSION_API CTransition::CreateSetupDialog(HWND hParent, dword dwReserved)
{
	return m_pImpl->CreateSetupDialog(hParent, dwReserved);
}

bool32 FUSION_API CTransition::EndSetupDialog(bool32 validate)
{
	return m_pImpl->EndSetupDialog(validate);
}

/*

----- RUNTIME -----

*/

bool32 FUSION_API CTransition::Start(dword durationMS)
{
	return m_pImpl->Start(durationMS);
}

bool32 FUSION_API CTransition::StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc)
{
	return m_pImpl->StepDraw(dest, originalImage, finalImage, dwFlags, ppRc);
}

void FUSION_API CTransition::End()
{
	return m_pImpl->End();
}

bool32 FUSION_API CTransition::IsCompleted()
{
	return m_pImpl->IsCompleted();
}


int32 FUSION_API CTransition::GetTimePos()
{
	return m_pImpl->GetTimePos();
}

void FUSION_API CTransition::SetTimePos(int32 msTimePos)
{
	m_pImpl->SetTimePos(msTimePos);
}


int32 FUSION_API CTransition::Supports(int32 nFeature, LPARAM lParam)
{
	return m_pImpl->Supports(nFeature, lParam);
}


bool32 FUSION_API CTransition::StepDrawBlit(cSurface* dest, int32 x, int32 y, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc, BlitMode bm, BlitOp bop, LPARAM lParam, dword dwBlitFlags)
{
	return m_pImpl->StepDrawBlit(dest, x, y, originalImage, finalImage, dwFlags, ppRc, bm, bop, lParam, dwBlitFlags);
}


CTransition::~CTransition()
{
	if (m_pImpl)
		delete m_pImpl;
}