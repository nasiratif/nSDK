#include "Extension.hpp"

// Fill this out if you're implementing debugger support
#ifdef EXT_EDITOR
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
#endif

/*

---------- RUNTIME FUNCTIONS ----------

*/

void FUSION_API Extension::API::StartApp(mv* mV, CRunApp* pApp)
{
#pragma EXT_EXPORT_STARTAPP
	/*
	// Example (store global data):
	auto data = (int32*)mV->mvGetExtUserData(pApp, nSDK::hInst);
	if (data)
		delete[] data;

	data = new int32[500];
	mV->mvSetExtUserData(pApp, mV->mvHInst, data);
	*/
}

void FUSION_API Extension::API::EndApp(mv* mV, CRunApp* pApp)
{
#pragma EXT_EXPORT_ENDAPP
	/*
	// Example (free global data):
	auto data = (int32*)mV->mvGetExtUserData(pApp, nSDK::hInst);
	if (data)
		delete[] data;
	*/
}


void FUSION_API Extension::API::StartFrame(mv* mV, dword dwReserved, int32 nFrameIndex)
{
#pragma EXT_EXPORT_STARTFRAME
}

void FUSION_API Extension::API::EndFrame(mv* mV, dword dwReserved, int32 nFrameIndex)
{
#pragma EXT_EXPORT_ENDFRAME
}


uint16 FUSION_API Extension::API::GetRunObjectDataSize(RunHeader* rhPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_GETRUNOBJECTDATASIZE
	return sizeof(RunData);
}


cSurface* FUSION_API Extension::API::GetRunObjectSurface(RunData* rdPtr)
{
#pragma EXT_EXPORT_GETRUNOBJECTSURFACE
	return NULL;
}

sMask* FUSION_API Extension::API::GetRunObjectCollisionMask(RunData* rdPtr, LPARAM lParam)
{
#pragma EXT_EXPORT_GETRUNOBJECTCOLLISIONMASK
	/*
	// Example:
	if (!(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT)) // Uncomment rs structure in RunData
		return NULL;

	// Transparent? Create mask
	auto pMask = rdPtr->mask; // sMask* mask;
	if (rdPtr->surf)
	{
		auto dwMaskSize = rdPtr->surf->CreateMask(NULL, lParam);
		if (dwMaskSize)
		{
			if (pMask)
				free(pMask);

			pMask = (sMask*)calloc(dwMaskSize, 1); // remember to free this later!
			if (pMask)
			{
				rdPtr->surf->CreateMask(pMask, lParam);
				rdPtr->mask = pMask;
			}
		}
	}

	return pMask;
	*/
	return NULL;
}


int16 FUSION_API Extension::API::CreateRunObject(RunData* rdPtr, EditData* edPtr, createObjectInfo* cobPtr)
{
#pragma EXT_EXPORT_CREATERUNOBJECT
	/*
	// Example (if you're writing a displaying object):
	rdPtr->rHo.hoX = cobPtr->cobX;
	rdPtr->rHo.hoY = cobPtr->cobY;
	rdPtr->rHo.hoImgXSpot = rdPtr->rHo.hoImgYSpot = 0;
	rdPtr->rHo.hoImgWidth = edPtr->swidth;
	rdPtr->rHo.hoImgHeight = edPtr->sheight;
	*/
	return 0; // success
}

int16 FUSION_API Extension::API::DestroyRunObject(RunData* rdPtr, long fast)
{
#pragma EXT_EXPORT_DESTROYRUNOBJECT
	return 0;
}

int16 FUSION_API Extension::API::HandleRunObject(RunData* rdPtr)
{
#pragma EXT_EXPORT_HANDLERUNOBJECT
	return REFLAG_ONESHOT;
}

int16 FUSION_API Extension::API::DisplayRunObject(RunData* rdPtr)
{
#pragma EXT_EXPORT_DISPLAYRUNOBJECT
	/*
	// Example:
	// Retrieve frame surface:
	auto frameSurf = WinGetSurface((int)rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin);
	// Blit our surface onto the frame
	rdPtr->surf->Blit(*frameSurf, rdPtr->rHo.hoX, rdPtr->rHo.hoY);
	*/
	return 0; // return value is ignored
}


int16 FUSION_API Extension::API::PauseRunObject(RunData* rdPtr)
{
#pragma EXT_EXPORT_PAUSERUNOBJECT
	return 0;
}

int16 FUSION_API Extension::API::ContinueRunObject(RunData* rdPtr)
{
#pragma EXT_EXPORT_CONTINUERUNOBJECT
	return 0;
}


bool32 FUSION_API Extension::API::SaveRunObject(RunData* rdPtr, HANDLE hFile)
{
#pragma EXT_EXPORT_SAVERUNOBJECT
	return FALSE;
}

bool32 FUSION_API Extension::API::LoadRunObject(RunData* rdPtr, HANDLE hFile)
{
#pragma EXT_EXPORT_LOADRUNOBJECT
	return FALSE;
}

/*

------ WINDOW PROC FUNCTIONS (OEFLAG_WINDOWPROC) ------

---- Uncomment these functions if you wish to intercept Fusion's window procedure using OEFLAG_WINDOWPROC

*/

/*
LRESULT FUSION_API Extension::API::WindowProc(RunHeader* rhPtr, HWND hWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
#pragma EXT_EXPORT_WINDOWPROC
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


/*

------ TEXT FUNCTIONS (OEFLAG_TEXT) ------

---- Uncomment these functions if you're using OEFLAG_TEXT

*/

/*
void FUSION_API Extension::API::SetRunObjectFont(RunData* rdPtr, LOGFONT* pLf, RECT* pRc)
{
#pragma EXT_EXPORT_SETRUNOBJECTFONT
	// Example from stock SDK:
	HFONT hFont = CreateFontIndirect(pLf);
	if (hFont != NULL)
	{
		if (rdPtr->m_hFont != 0)
			DeleteObject(rdPtr->m_hFont);
		rdPtr->m_hFont = hFont;
		SendMessage(rdPtr->m_hWnd, WM_SETFONT, (WPARAM)rdPtr->m_hFont, FALSE);
	}
}

void FUSION_API Extension::API::GetRunObjectFont(RunData* rdPtr, LOGFONT* pLf)
{
#pragma EXT_EXPORT_GETRUNOBJECTFONT
	// Example from stock SDK:
	GetObject(rdPtr->m_hFont, sizeof(LOGFONT), pLf);
}


void FUSION_API Extension::API::SetRunObjectTextColor(RunData* rdPtr, COLORREF rgb)
{
#pragma EXT_EXPORT_SETRUNOBJECTTEXTCOLOR
	// Example from stock SDK:
	rdPtr->m_dwColor = rgb;
	InvalidateRect(rdPtr->m_hWnd, NULL, TRUE);
}

COLORREF FUSION_API Extension::API::GetRunObjectTextColor(RunData* rdPtr)
{
#pragma EXT_EXPORT_GETRUNOBJECTTEXTCOLOR
	// Example from stock SDK:
	return rdPtr->m_dwColor;
}
*/


#ifdef EXT_EDITOR
/*

------ FUSION DEBUGGER IMPLEMENTATION ------

*/

word* FUSION_API Extension::API::GetDebugTree(RunData* rdPtr)
{
#pragma EXT_EXPORT_GETDEBUGTREE
	return DebugTree;
}

void FUSION_API Extension::API::GetDebugItem(tchar* pBuffer, RunData* rdPtr, int32 id)
{
#pragma EXT_EXPORT_GETDEBUGITEM
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
}

void FUSION_API Extension::API::EditDebugItem(RunData* rdPtr, int32 id)
{
#pragma EXT_EXPORT_EDITDEBUGITEM
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
}
#endif