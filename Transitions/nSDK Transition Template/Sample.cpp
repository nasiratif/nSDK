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


int32 CSample::GetName(tchar* buffer, int32 bufferSize)
{
	StringCbCopy(buffer, bufferSize, TRANS_SAMPLE_NAME);

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
	// This is a demo transition which just does a rotation & scaling effect:

	auto delta = GetDeltaTime();

	POINT center;
	center.x = dest->GetWidth() / 2;
	center.y = dest->GetHeight() / 2;

	float32 time = (float32)delta / (float32)m_duration; // m_duration is in milliseconds
	time = ElasticOut(time);
	originalImage->Blit(*dest);
	finalImage->BlitEx(
		*dest,
		dest->GetWidth() / 2.0f,
		dest->GetHeight() / 2.0f,
		time,
		time,
		0,
		0,
		dest->GetWidth(),
		dest->GetHeight(),
		&center,
		time * 360.0f
	);
	return FALSE;
}


void CSample::End()
{
	CTransitionImpl::End(); // must be called!
}
