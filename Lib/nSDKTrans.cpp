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

int32 CTransitionImpl::GetName(tchar* buffer, int32 bufferSize)
{
	StringCbCopy(buffer, bufferSize, _T("(undefined transition)"));
	return 0;
}

dword CTransitionImpl::GetMode()
{
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

void CTransitionImpl::End()
{
	m_running = FALSE;
}

bool32 CTransitionImpl::IsCompleted()
{
	return m_running ? (timeGetTime() >= m_endTime) : TRUE;
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

/*

---------- TRANSITION BASE CLASS ----------

*/

CTransition::CTransition(CTransitionImpl* pImpl)
{
	m_pImpl = pImpl;
}

CTransition::~CTransition()
{
	if (m_pImpl)
		delete m_pImpl;
}


void FUSION_API CTransition::Delete()
{
	delete this;
}

/*

----- INFOS -----

*/


dword FUSION_API CTransition::GetMode()
{
	return m_pImpl->GetMode();
}

int32 FUSION_API CTransition::GetName(tchar* buffer, int32 bufferSize)
{
	return m_pImpl->GetName(buffer, bufferSize);
}

dword FUSION_API CTransition::GetID()
{
	return m_pImpl->GetID();
}

void FUSION_API CTransition::SetParameters(byte* params, dword paramsSize)
{
	m_pImpl->SetParameters(params, paramsSize);
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


bool32 FUSION_API CTransition::Unknown1()
{
	return FALSE;
}

bool32 FUSION_API CTransition::Unknown2(int32 unknown1, int32 unknown2)
{
	return TRUE;
}

void FUSION_API CTransition::Unknown3(cSurface* dest, int32 reserved, int32 reserved2, cSurface* backgroundSurface, void** ppClassObject, dword dwFlags, void* unknown, int32 reserved3, int32 reserved4, int32 reserved5, int32 reserved6)
{

}