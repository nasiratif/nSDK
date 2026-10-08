#include "Filter.hpp"

using namespace Filter;

/*

---------- FILTER IMPLEMENTATION ----------

*/

CCustomSoundFilter::CCustomSoundFilter(dword dwFlags)
{

}

CCustomSoundFilter::~CCustomSoundFilter() {}


void CCustomSoundFilter::Delete()
{
	Close();
	delete this;
}


int32 CCustomSoundFilter::Open(CInputFile* pf)
{
	Close();

	// Example:
	// For this particular filter, we assume the file is just a 16-bit, 44.1hz, 2-channel raw PCM audio buffer, so we can simply just copy the data into the data provided by ReadData & avoid any sophisticated decoding here
	size = pf->GetLength();
	data = (byte*)malloc(size);
	pf->Read(data, size);

	// We must also set the input wave format:
	m_WaveFormatIn.wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormatIn.nChannels = 2;
	m_WaveFormatIn.wBitsPerSample = 16;
	m_WaveFormatIn.nSamplesPerSec = 44100;
	m_WaveFormatIn.nBlockAlign = (m_WaveFormatIn.wBitsPerSample / 8) * m_WaveFormatIn.nChannels;
	m_WaveFormatIn.nAvgBytesPerSec = m_WaveFormatIn.nSamplesPerSec * m_WaveFormatIn.nBlockAlign;
	m_WaveFormatIn.cbSize = 0;
	return SND_OK;
}

void CCustomSoundFilter::Close()
{
	// Example:
	free(data);
	pos = 0;
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
	if (pos > size) // seeking to EOF should be allowed
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
		memcpy(lpDstBuffer, data + readPos, *dwRead);

	return SND_OK;
}