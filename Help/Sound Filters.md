# Sound Filters

This file documents MMF2/CF2.5 sound filters. For additional information, see the README in the Filters/Sounds folder.

## Usage
Sound filters are placed in *(Fusion root)\Filters\Sounds*. They cannot be Unicode, and also don't have a Runtime variant.

Fusion loads all sound filters before it even shows the splash screen. This may be important to note as you may not have enough time to attach a debugger to Fusion before the filter loads.

## Sound Filter Class Functions

### Delete
`void CCustomSoundFilter:Delete();`

Called to delete this sound filter instance. This is where you should free memory allocated in this instance.

### Open
`int32 CCustomSoundFilter::Open(CInputFile* pf);`

Called when you need to load a sound file. A `CInputFile` object is provided to read the file. Return `SND_OK` to indicate success, otherwise return an appropriate `SND_XXX` error code (see `SoundError.h`).

### Close
`void CCustomSoundFilter::Close();`

Called when the sound file needs to be closed.

### GetLength
`dword CCustomSoundFilter::GetLength();`

Called to retrieve the duration of the whole sound. You must return the entire length of the sound's PCM data, in bytes.

### GetPos
`dword CCustomSoundFilter::GetPos();`

You must return the current streaming position of the sound, in PCM bytes.

### SetPos
`bool32 CCustomSoundFilter::SetPos(dword dwPos);`

Called to set the current streaming position of the sound, in PCM bytes.
Return `TRUE` if success, otherwise `FALSE` for failure.

### ReadData
`int32 CCustomSoundFilter::ReadData(byte* lpDstBuffer, dword dwBufSize, dword* dwRead);`

Called whenever you need to fill in the sound buffers to Fusion. Every call to this function should proceed to read the next chunk of audio data available.
The parameters are as follows:

- `lpDstBuffer` - destination audio buffer
- `dwBufSize` - whole size of the buffer, in bytes
- `dwRead` - number of bytes that you actually wrote into the buffer. If it's less than `dwBufSize`, Fusion assumes there's no more data left and either stops the sound after this buffer finishes playing, or loops

Return `SND_OK` if success, otherwise failure.

### SetOutputFormat
`void CCustomSoundFilter::SetOutputFormat(LPWAVEFORMATEX pStreamFormat);`

Called to retrieve the format of your sound data, you must write it to the `WAVEFORMATEX` provided.
Note that you must also set `m_WaveFormatOut`.

## Exported Functions

### CreateFilter
`CSoundFilter* FUSION_API Filter::API::Create(CInputFile* pf);`

Called when a sound filter instance needs to be made; this is done for every sample that is played using your filter.
Note that this could also be called at edittime (remember that you can play sounds in Data Elements).
Return a new instance of your sound filter that inherits from `CSoundFilter`.

### GetFilterName
`const tchar* FUSION_API Filter::API::GetFilterName();`

Called at Fusion startup to retrieve the name of your sound filter (what is shown in the Sound Filters tab).

### GetFilterID
`dword FUSION_API Filter::API::GetFilterID();`

Called at Fusion startup to retrieve the identifier of your sound filter.

### GetFilterExts
`const tchar** FUSION_API Filter::API::GetFilterExts();`

Called at Fusion startup to retrieve the file extensions that your filter supports.

### GetPriority
*NOTE: I'm not entirely sure about this function's behavior*

`int32 FUSION_API Filter::API::GetPriority();`

Implement this function so Fusion can retrieve the priority of your sound filter.
When Fusion plays a sample, it finds a suitable sound filter in the order based on their priority value. Lower values mean your filter is more likely to be looked into first.
This function is optional; if not exported, your filter won't have any explicit priority setting.

### CanReadFile
`bool32 FUSION_API Filter::API::CanReadFile(CInputFile* pif);`

Called to determine whether *your* filter is capable of reading the specified file. You could, for example, read the first 4 bytes and check if it matches the byte signature `"fLaC"`, if we were writing a FLAC filter.
If you return `FALSE`, Fusion ignores your filter for that file & looks into other filters instead.
