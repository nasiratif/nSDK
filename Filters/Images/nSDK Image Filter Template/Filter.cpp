#include "Filter.hpp"

using namespace Filter;

/*

---------- FILTER IMPLEMENTATION ----------

*/

CCustomImageFilter::CCustomImageFilter(dword dwFlags)
{

}

void CCustomImageFilter::Delete()
{
	delete this;
}

dword CCustomImageFilter::GetID()
{
	return API::GetFilterID();
}

const tchar* CCustomImageFilter::GetName()
{
	return API::GetFilterName();
}

bool32 CCustomImageFilter::CanRead(CInputFile* pf)
{
	return API::CanReadFile(pf);
}


int32 CCustomImageFilter::GetPictureInfo(CInputFile* pf)
{
	return 0;
}


int32 CCustomImageFilter::Load(CInputFile* pf, byte* pData, int32 pitch, byte* pAlpha, int32 nAlphaPitch)
{
	return 0;
}

int32 CCustomImageFilter::Save(COutputFile* pf, byte* pData, int32 nWidth, int32 nHeight, int32 nDepth, int32 nPitch, LPLOGPALETTE pPal, byte* pAlpha, int32 nAlphaPitch)
{
	return 0;
}