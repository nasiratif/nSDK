#include "Sample.hpp"

using namespace Transition;

// Elastic Out easing function
// Via https://easings.net
float32 ElasticOut(float32 t)
{
	const double c4 = (2 * PI) / 3;
	return (float32)(pow(2, -10 * t) * sin((t * 10 - 0.75) * c4) + 1.0);
}

/*

---------- TRANSITION IMPLEMENTATION ----------

*/

CSample::CSample()
{

}

CSample::~CSample()
{
}


int32 CSample::Supports(int32 nFeature, LPARAM lParam)
{
	if (nFeature == TRF_STEPDRAWBLIT) // this sample supports this feature
		return TRUE;

	return FALSE;
}

int32 CSample::GetName(tchar* buffer, int32 bufferSize)
{
	StringCbCopy(buffer, bufferSize, TRANS_NAME_SAMPLE);

	size_t length = 0;
	StringCbLength(buffer, bufferSize, &length);
	return length;
}

dword CSample::GetParameters(byte* buffer)
{
	return CTransitionImpl::GetParameters(buffer);
}

void CSample::SetParameters(byte* params, dword paramsSize)
{
	CTransitionImpl::SetParameters(params, paramsSize);
}


HWND CSample::CreateSetupDialog(HWND hParent, dword dwReserved)
{
	return CTransitionImpl::CreateSetupDialog(hParent, dwReserved);
}

bool32 CSample::EndSetupDialog(bool32 validate)
{
	return CTransitionImpl::EndSetupDialog(validate);
}

bool32 FUSION_API CSample::ParamsProc(HWND hDlg, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	return CTransitionImpl::ParamsProc(hDlg, msg, wParam, lParam);
}


bool32 CSample::StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc)
{
	// StepDraw is primarily only used in the case that StepDrawBlit is either unavailable (see Supports()), or the transition is playing in the setup preview window.
	// To reduce code duplication, we'll just delegate to StepDrawBlit:
	return StepDrawBlit(dest, 0, 0, originalImage, finalImage, dwFlags, ppRc, BMODE_TRANSP, BOP_COPY, NULL, 0);
}

bool32 CSample::StepDrawBlit(cSurface* dest, int32 x, int32 y, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc, BlitMode bm, BlitOp bop, LPARAM lParam, dword dwBlitFlags)
{
	// This is a demo transition which just does a rotation & scaling effect:

	auto delta = GetDeltaTime();

	POINT center;
	center.x = finalImage->GetWidth() / 2;
	center.y = finalImage->GetHeight() / 2;

	float32 time = (float32)delta / (float32)m_duration; // m_duration is in milliseconds
	time = ElasticOut(time);

	originalImage->Blit(*dest, x, y, BMODE_TRANSP);
	finalImage->BlitEx(
		*dest,
		x + finalImage->GetWidth() / 2.0f, y + finalImage->GetHeight() / 2.0f,
		time, time,
		0, 0,
		finalImage->GetWidth(), finalImage->GetHeight(),
		&center,
		time * 360.0f,
		bm, bop,
		lParam,
		dwBlitFlags
	);
	return TRUE;
}


void CSample::End()
{
	CTransitionImpl::End(); // superclass must be called!
}