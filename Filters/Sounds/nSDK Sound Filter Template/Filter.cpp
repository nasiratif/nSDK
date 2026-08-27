#include "Filter.hpp"

using namespace Filter;

/*

---------- FILTER IMPLEMENTATION ----------

*/

CCustomSoundFilter::~CCustomSoundFilter() {}


void CCustomSoundFilter::Delete()
{
	Close();
	delete this;
}


int32 CCustomSoundFilter::Open(CInputFile* pf)
{
	// Example:
	// We assume the file is just a 16-bit, 44.1hz, 2-channel raw PCM audio buffer, so we can simply just copy the data into the data provided by ReadData & avoid any sophisticated decoding here
	size = pf->GetLength();
	data = (byte*)malloc(size);
	pf->Read(data, size);
	return SND_OK;
}

void CCustomSoundFilter::Close()
{
	// Example:
	free(data);
	data = nullptr;
	size = 0;
}


dword CCustomSoundFilter::GetLength()
{
	// Example:
	return size;
}

dword CCustomSoundFilter::GetPos()
{
	// Example:
	return pos;
}

bool32 CCustomSoundFilter::SetPos(dword dwPos)
{
	// Example:
	pos = dwPos;
	if (pos >= size)
	{
		pos = size;
		return FALSE;
	}
	return TRUE;
}

int32 CCustomSoundFilter::ReadData(byte* lpDstBuffer, dword dwBufSize, dword* dwRead)
{
	// Example:
	*dwRead = dwBufSize;

	auto readPos = pos;
	// Is the buffer read range out of bounds? If so, clamp to boundaries
	if (readPos + dwBufSize > size)
	{
		auto delta = (pos + dwBufSize) - size;
		*dwRead = (delta < *dwRead) ? *dwRead - delta : 0;
		pos = size;
	}
	else
	{
		// Advance position to next chunk of audio data
		pos += *dwRead;
	}

	if (*dwRead)
	{
		memcpy(lpDstBuffer, data + readPos, *dwRead);
		return SND_OK;
	}
	else
		return SND_CANNOTREADFILE;
}

void CCustomSoundFilter::SetOutputFormat(LPWAVEFORMATEX pStreamFormat)
{
	// Example:
	pStreamFormat->cbSize = 0;
	pStreamFormat->wFormatTag = WAVE_FORMAT_PCM;
	pStreamFormat->nChannels = 2;
	pStreamFormat->wBitsPerSample = 16;
	pStreamFormat->nSamplesPerSec = 44100;
	pStreamFormat->nBlockAlign = (pStreamFormat->wBitsPerSample / CHAR_BIT) * pStreamFormat->nChannels;
	pStreamFormat->nAvgBytesPerSec = pStreamFormat->nSamplesPerSec * pStreamFormat->nBlockAlign;
	m_WaveFormatOut = *pStreamFormat;
}