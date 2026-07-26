#pragma once
#include <nSDKSFT.hpp>

// DEFINES:
// Define filter information here:
// -----
#define FLT_IDENTIFIER 'NSDK'
#define FLT_NAME _T("nSDK Sound Filter Template")
// File extensions this filter supports
// Must end with NULL
#define FLT_EXTS { _T("raw"), NULL }
// -----

// This is the sound filter class that you implement for Fusion
class CCustomSoundFilter : public CSoundFilter
{
public:
	~CCustomSoundFilter() override;

	void Delete() override;

	// Called to open a sound file
	// Return SND_OK if success, otherwise failure
	int32 Open(CInputFile* pf) override;
	// Called to close current sound file
	void Close() override;

	dword GetLength() override;
	// Called to retrieve the current read position of the buffer, in bytes
	dword GetPos() override;
	// Called to set the next sample position to play back 
	// dwPos is in total bytes
	bool32 SetPos(dword dwPos) override;
	// Called whenever you need to fill in the sound buffers to Fusion
	// You should implement a streaming mechanicism for this function; e.g, first time this function is called, you get the first block of audio data, in the next call, you get the next audio data right after, and so on
	// lpDstBuffer is the destination audio buffer
	// dwBufSize is the whole size of the buffer, in bytes
	// You write to dwRead the number of bytes that you actually wrote into the buffer. If it's less than dwBufSize, Fusion assumes there's no more data left and either stops the sound after this buffer or loops
	// Return SND_OK if success, otherwise failure
	int32 ReadData(byte* lpDstBuffer, dword dwBufSize, dword* dwRead) override;

	// Called to retrieve the format of your sound data, you must write it to the WAVEFORMATEX provided
	// Note that you must also set m_WaveFormatOut
	void SetOutputFormat(LPWAVEFORMATEX pStreamFormat);
private:
	// Define your filter data here:

	byte* data = nullptr;
	size_t size = 0;
	size_t pos = 0;
};