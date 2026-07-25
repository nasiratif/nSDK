#include "Filter.hpp"

/*

NB:
	Sound filters are placed in <Fusion root>\Filters\Sounds
	Sound filters are always assumed to be non-Unicode by Fusion, so you shouldn't compile this filter as Unicode
	If debugging at edittime, note Fusion loads all sound filters before it even shows the splash screen. This may be important if you plan on modifying the exported functions below as you may not have enough time to attach a debugger to Fusion before the filter loads
*/

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
	return 0;
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
		memcpy(lpDstBuffer, data + readPos, *dwRead);

	OutputDebugString(_T("Read data\n"));
	return 0;
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

/*

---------- SOUND FILTER EXPORTS ----------

*/

bool32 WINAPI DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved)
{
	return nSDK::Exports::DllMain(hinstDLL, fdwReason, lpvReserved);
}


// Called when a sound filter instance needs to be made; this is done for every sample that is played using your filter
// Note that this could also be called at edittime (remember that you can play sounds in Data Elements)
CSoundFilter* FUSION_API CreateFilter(CInputFile* pf)
{
#pragma EXT_DLLEXPORT
	return new CCustomSoundFilter;
}


// Called at Fusion startup to retrieve the name of your sound filter (what is shown in the Sound Filters tab)
const tchar* FUSION_API GetFilterName()
{
#pragma EXT_DLLEXPORT
	return FLT_NAME;
}

// Called at Fusion startup to retrieve the identifier of your sound filter
dword FUSION_API GetFilterID()
{
#pragma EXT_DLLEXPORT
	return EXT_FIX_IDENTIFIER(FLT_IDENTIFIER);
}

// Called at Fusion startup to retrieve the file extensions that your filter supports
const tchar** FUSION_API GetFilterExts()
{
#pragma EXT_DLLEXPORT
	static const tchar* exts[] = FLT_EXTS;
	return exts;
}

// Implement this function so Fusion can retrieve the priority of your sound filter
// When Fusion plays a sample, it finds a suitable sound filter in the order based on their priority value. Lower values mean your filter is more likely to be looked into first
// This function is optional; if not exported, your filter won't have any explicit priority setting
dword FUSION_API GetPriority()
{
#pragma EXT_DLLEXPORT
	// Example:
	return MAXDWORD; // lowest priority since we almost always return TRUE in CanReadFile
}


// Called to determine whether your filter is capable of reading the specified file
// You could, for example, read the first 4 bytes and check if it matches the byte signature "fLaC", if we were writing a FLAC filter
// If you return FALSE, Fusion ignroes your filter for that file & looks into other filters
bool32 FUSION_API CanReadFile(CInputFile* pif)
{
#pragma EXT_DLLEXPORT
	// Example:
	if (pif->GetLength() % (sizeof(int16) * 2)) // we assume we're always reading a 16-bit, 44.1hz, 2-channel audio buffer
		return FALSE;

	return TRUE;
}