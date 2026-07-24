#include "Extension.hpp"

// Fill this out if you're implementing debugger support
word DebugTree[] =
{
	// Example:
	// DB_CURRENTSTRING | DB_EDITABLE,
	// DB_CURRENTVALUE | DB_EDITABLE,
	// DB_CURRENTCHECK,
	// DB_CURRENTCOMBO,
	
	// End of table (required)
	DB_END
};

/*

---------- RUNTIME FUNCTIONS ----------

*/

// Called when the Fusion app starts
// Note that "Fusion app" could also mean a sub-app, in which case this function is also called there too
void FUSION_API StartApp(mv* mV, CRunApp* pApp)
{
#pragma EXT_DLLEXPORT
	/*
	// Example (store global data):
	int32* data = (int32*)mV->mvGetExtUserData(pApp, nSDK::hInst);
	if (data)
		delete[] data;

	data = new int32[500];
	mV->mvSetExtUserData(pApp, mV->mvHInst, data);
	*/
}

// Counterpart of StartApp
void FUSION_API EndApp(mv* mV, CRunApp* pApp)
{
#pragma EXT_DLLEXPORT
	/*
	// Example (free global data):
	int32* data = (int32*)mV->mvGetExtUserData(pApp, nSDK::hInst);
	if (data)
		delete[] data;
	*/
}


// Called when the frame starts or restarts
void FUSION_API StartFrame(mv* mV, dword dwReserved, int32 nFrameIndex)
{
#pragma EXT_DLLEXPORT
}

// Called when the frame ends
void FUSION_API EndFrame(mv* mV, dword dwReserved, int32 nFrameIndex)
{
#pragma EXT_DLLEXPORT
}


// Tells Fusion what your RunData size is
uint16 FUSION_API GetRunObjectDataSize(fprh rhPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	return sizeof(RunData);
}


/*
// If you don't wish to implement your own display routine, but you use a cSurface, you can simply pass it to this function and Fusion will handle the blitting automatically to the frame surface; taking effects and sprite position into consideration
LPSURFACE FUSION_API GetRunObjectSurface(RunData* rdPtr)
{
#pragma EXT_DLLEXPORT
	return NULL;
}
*/

/*
// If you're using OEPREFS_FINECOLLISIONS, you need to implement this function to generate the collision mask for your object
LPSMASK FUSION_API GetRunObjectCollisionMask(RunData* rdPtr, LPARAM lParam)
{
#pragma EXT_DLLEXPORT
	// Example:
	if (!(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT)) // Uncomment rs structure in RunData
		return NULL;

	// Transparent? Create mask
	auto pMask = rdPtr->mask; // LPSMASK mask;
	if (rdPtr->surf)
	{
		auto dwMaskSize = rdPtr->surf->CreateMask(NULL, lParam);
		if (dwMaskSize)
		{
			if (pMask)
				free(pMask);

			pMask = (LPSMASK)calloc(dwMaskSize, 1); // remember to free this later!
			if (pMask)
			{
				rdPtr->surf->CreateMask(pMask, lParam);
				rdPtr->mask = pMask;
			}
		}
	}

	return pMask;
}
*/


// Called when the extension object is created
// Should be used to initialize RunData
int16 FUSION_API CreateRunObject(RunData* rdPtr, EditData* edPtr, fpcob cobPtr)
{
#pragma EXT_DLLEXPORT
	/*
	// Example (if you're writing a displaying object):
	rdPtr->rHo.hoX = cobPtr->cobX;
	rdPtr->rHo.hoY = cobPtr->cobY;
	rdPtr->rHo.hoImgXSpot = rdPtr->rHo.hoImgYSpot = 0;
	rdPtr->rHo.hoImgWidth = edPtr->swidth;
	rdPtr->rHo.hoImgHeight = edPtr->sheight;
	*/
	return 0;
}

// Counterpart of CreateRunObject
// Free resources you allocated in RunData
int16 FUSION_API DestroyRunObject(RunData* rdPtr, long fast)
{
#pragma EXT_DLLEXPORT
	return 0;
}

// Called every Fusion loop, unless you return REFLAG_ONESHOT
// You may return REFLAG_DISPLAY to trigger DisplayRunObject, or 0 for nothing special
int16 FUSION_API HandleRunObject(RunData* rdPtr)
{
#pragma EXT_DLLEXPORT
	return REFLAG_ONESHOT;
}

// If you return REFLAG_DISPLAY in HandleRunObject this routine will run
int16 FUSION_API DisplayRunObject(RunData* rdPtr)
{
#pragma EXT_DLLEXPORT
	return 0;
}


// When Fusion runtime is paused
int16 FUSION_API PauseRunObject(RunData* rdPtr)
{
#pragma EXT_DLLEXPORT
	return 0;
}

// When fusion runtime is resumed
int16 FUSION_API ContinueRunObject(RunData* rdPtr)
{
#pragma EXT_DLLEXPORT
	return 0;
}


// When the extension data needs to be saved to disk (using the frame save action)
bool32 FUSION_API SaveRunObject(RunData* rdPtr, HANDLE hFile)
{
#pragma EXT_DLLEXPORT
	return FALSE;
}

// When the extension data needs to be loaded from disk (using the frame load action)
bool32 FUSION_API LoadRunObject(RunData* rdPtr, HANDLE hFile)
{
#pragma EXT_DLLEXPORT
	return FALSE;
}

/*
// Intercept Fusion's window procedure
// You must export this function if OEFLAG_WINDOWPROC is defined
LRESULT FUSION_API WindowProc(fprh rhPtr, HWND hWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
#pragma EXT_DLLEXPORT
	// Example:
	if (rhPtr->rh2.rh2PauseCompteur != 0) // don't handle our messages if app is paused
		return 0;

	switch (msg)
	{
	case WM_NCCALCSIZE:
		// Remove NC (non-client) area of the window
		return REFLAG_MSGHANDLED;
	}
	return 0;
}
*/


// Tells Fusion your debug tree
LPWORD FUSION_API GetDebugTree(RunData* rdPtr)
{
#pragma EXT_DLLEXPORT
#ifdef EXT_EDITOR
	return DebugTree;
#else
	return NULL;
#endif
}

// Should return the text of a debugger item
void FUSION_API GetDebugItem(tchar* pBuffer, RunData* rdPtr, int32 id)
{
#pragma EXT_DLLEXPORT
#ifdef EXT_EDITOR
	/*
	// Example from stock SDK:
	char temp[DB_BUFFERSIZE];

	switch (id)
	{
	case DB_CURRENTSTRING:
		LoadString(hInstLib, IDS_CURRENTSTRING, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->text);
		break;
	case DB_CURRENTVALUE:
		LoadString(hInstLib, IDS_CURRENTVALUE, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->value);
		break;
	case DB_CURRENTCHECK:
		LoadString(hInstLib, IDS_CURRENTCHECK, temp, DB_BUFFERSIZE);
		if (rdPtr->check)
			wsprintf(pBuffer, temp, _T("TRUE"));
		else
			wsprintf(pBuffer, temp, _T("FALSE"));
		break;
	case DB_CURRENTCOMBO:
		LoadString(hInstLib, IDS_CURRENTCOMBO, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->combo);
		break;
	}
	*/
#endif
}

// When the debug item is to be edited
void FUSION_API EditDebugItem(RunData* rdPtr, int32 id)
{
#pragma EXT_DLLEXPORT
#ifdef EXT_EDITOR
	/*
	// Example from stock SDK:
	switch (id)
	{
	case DB_CURRENTSTRING:
		{
			EditDebugInfo dbi;
			char buffer[256];

			dbi.pText=buffer;
			dbi.lText=TEXT_MAX;
			dbi.pTitle=NULL;

			strcpy(buffer, rdPtr->text);
			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITTEXT, 0, (LPARAM)&dbi);
			if (ret)
				strcpy(rdPtr->text, dbi.pText);
		}
		break;
	case DB_CURRENTVALUE:
		{
			EditDebugInfo dbi;

			dbi.value=rdPtr->value;
			dbi.pTitle=NULL;

			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITINT, 0, (LPARAM)&dbi);
			if (ret)
				rdPtr->value=dbi.value;
		}
		break;
	}
	*/
#endif
}