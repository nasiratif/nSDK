#include "Extension.hpp"

#ifdef EXT_EDITOR
/*

---------- EDITTIME-ONLY FUNCTIONS ----------

*/

void FUSION_API Extension::API::GetObjInfos(mv* mV, EditData* edPtr, tchar* ObjName, tchar* ObjAuthor, tchar* ObjCopyright, tchar* ObjComment, tchar* ObjHttp)
{
#pragma EXT_EXPORT_GETOBJINFOS
	static const size_t length = 255 * sizeof(tchar);
	static const size_t lengthComment = 1024 * sizeof(tchar);

	StringCbCopy(ObjName, length, EXT_NAME);
	StringCbCopy(ObjAuthor, length, EXT_AUTHOR);
	StringCbCopy(ObjCopyright, length, EXT_COPYRIGHT);
	StringCbCopy(ObjComment, lengthComment, EXT_COMMENT);
	StringCbCopy(ObjHttp, length, EXT_HTTP);
}

const tchar* FUSION_API Extension::API::GetHelpFileName()
{
#pragma EXT_EXPORT_GETHELPFILENAME
	return EXT_HELP_FILENAME;
}


/*
int32 FUSION_API Extension::API::MakeIconEx(mv* mV, cSurface* pIconSf, tchar* lpName, OI* oiPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_MAKEICONEX
	// Example: a brown=ish square
	pIconSf->Fill(RGB(64, 24, 24));
	return 0; // success
}
*/


bool32 FUSION_API Extension::API::GetProperties(mv* mV, EditData* edPtr, bool32 bMasterItem)
{
#pragma EXT_EXPORT_GETPROPERTIES
	return nSDK::Exports::GetProperties(mV, edPtr, bMasterItem);
}

void FUSION_API Extension::API::ReleaseProperties(mv* mV, EditData* edPtr, bool32 bMasterItem)
{
#pragma EXT_EXPORT_RELEASEPROPERTIES
}


LPARAM FUSION_API Extension::API::GetPropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_EXPORT_GETPROPCREATEPARAM
	return NULL;
}

void FUSION_API Extension::API::ReleasePropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID, LPARAM lParam)
{
#pragma EXT_EXPORT_RELEASEPROPCREATEPARAM
}


void FUSION_API Extension::API::SetPropValue(mv* mV, EditData* edPtr, uint32 nPropID, CPropValue* lParam)
{
#pragma EXT_EXPORT_SETPROPVALUE
	// Example:
	switch (nPropID)
	{
	case PropID_Images:
		edPtr->nImages = *(word*)((CPropDataValue*)lParam)->m_pData; // first word contains number of images
		break;
	case PropID_Text:
		StringCbCopy(edPtr->szText, sizeof(edPtr->szText), ((CPropStringValue*)lParam)->GetString()); // you might need to use mvReAllocEditData if you need larger string length
		break;
	case PropID_Color:
		edPtr->dwColor = ((CPropDWordValue*)lParam)->m_dwValue;
		break;
	case PropID_Combo:
		edPtr->dwComboIndex = ((CPropDWordValue*)lParam)->m_dwValue;
		break;
	}
	
}

CPropValue* FUSION_API Extension::API::GetPropValue(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_EXPORT_GETPROPVALUE
	// Example:
	switch (nPropID)
	{
	case PropID_Images:
	{
		// For image properties you must fill out a buffer of words where the first word contains the number of images, then the image IDs right after
		auto data = new CPropDataValue((edPtr->nImages + 1) * sizeof(word), NULL);
		auto wImages = (word*)data->m_pData;
		*(wImages++) = edPtr->nImages;
		for (word i = 0; i < edPtr->nImages; ++i)
			*(wImages++) = edPtr->wImages[i];

		return data;
	}
	case PropID_Text:
		return new CPropStringValue(edPtr->szText);
	case PropID_Color:
		return new CPropDWordValue(edPtr->dwColor);
	case PropID_Combo:
		return new CPropDWordValue(edPtr->dwComboIndex);
	case PropID_Copyright:
		return new CPropStringValue(_T("Original SDK \u00A9 Clickteam, nSDK \u00A9 2026 Nasir \"Nassic\" W."));
	}
	
	return NULL;
}

void FUSION_API Extension::API::SetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID, bool32 nCheck)
{
#pragma EXT_EXPORT_SETPROPCHECK
	// Example:
	switch (nPropID)
	{
	case PropID_Check:
		edPtr->nCheck = nCheck;
		break;
	}
	
}

bool32 FUSION_API Extension::API::GetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_EXPORT_GETPROPCHECK
	// Example:
	switch (nPropID)
	{
	case PropID_Check:
		return edPtr->nCheck;
	}
	
	return FALSE;
}

bool32 FUSION_API Extension::API::EditProp(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_EXPORT_EDITPROP
	// Example:
	if (nPropID == PropID_Images)
	{
		tchar windowTitle[] = _T("Edit images");

		EditAnimationParams eap;
		eap.m_dwSize = sizeof(EditAnimationParams);
		eap.m_pWindowTitle = windowTitle;
		eap.m_nImages = edPtr->nImages;
		eap.m_nMaxImages = ARRAYSIZE(edPtr->wImages);
		eap.m_nStartIndex = 0;
		eap.m_pImages = edPtr->wImages;
		eap.m_pImageTitles = NULL;
		eap.m_dwOptions = 0;
		eap.m_dwFixedWidth = 128;
		eap.m_dwFixedHeight = 128;
		if (mV->mvEditAnimation(edPtr, &eap, mV->mvHEditWin))
		{
			// Update number of images:
			edPtr->nImages = eap.m_nImages;
			// Refresh the property display
			mvRefreshProp(mV, edPtr, PropID_Images, FALSE);
			// Request Fusion to redraw the extension in the frame editor
			mvInvalidateObject(mV, edPtr);
			return TRUE;
		}
	}
	return FALSE;
}

bool32 FUSION_API Extension::API::IsPropEnabled(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_EXPORT_ISPROPENABLED
	/*
	// Example:
	if (nPropID == PropID_Check)
		// Only enable this property if the first combo box item is selected
		return edPtr->dwComboIndex != 0;
	*/
	return TRUE;
}


void FUSION_API Extension::API::InitParameter(mv* mV, int16 code, paramExt* pExt)
{
#pragma EXT_EXPORT_INITPARAMETER
	// Example:
	if (code == CustomDataType_Example)
	{
		// Although paramExt::pextData is defined to be char[2], the whole pExt buffer is PARAM_EXTMAXSIZE (512) bytes
		// You can store that much data into it, but we will just be using a single byte for this example
		pExt->pextCode = code;
		pExt->pextData[0] = TRUE;
		pExt->pextSize = sizeof(pExt->pextData[0]);
	}
}


void FUSION_API Extension::API::EditParameter(mv* mV, int16 code, paramExt* pExt)
{
#pragma EXT_EXPORT_EDITPARAMETER
	// Example:
	if (code == CustomDataType_Example)
	{
		// Crude example of a dialog box
		if (MessageBox(mV->mvHEditWin, _T("Do you want to test if the string is ASCII16?\nIf not, Unicode will be tested."), _T("Parameter"), MB_ICONINFORMATION | MB_YESNO) == IDYES)
			pExt->pextData[0] = TRUE;
		else
			pExt->pextData[0] = FALSE;

		pExt->pextCode = code;
	}
}

void FUSION_API Extension::API::GetParameterString(mv* mV, int16 code, paramExt* pExt, tchar* pDest, int16 size)
{
#pragma EXT_EXPORT_GETPARAMETERSTRING
	// Example:
	if (code == CustomDataType_Example)
	{
		StringCbCopy(pDest, size, pExt->pextData[0] ? _T("ASCII16") : _T("Unicode"));
	}
}


int32 FUSION_API Extension::API::CreateObject(mv* mV, LO* loPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_CREATEOBJECT
	memset(edPtr->wImages, NULL, sizeof(edPtr->wImages));
	edPtr->nImages = 0;
	StringCbCopy(edPtr->szText, sizeof(edPtr->szText), _T("Editable text"));
	edPtr->dwColor = RGB(100, 200, 255);
	edPtr->nCheck = FALSE;
	edPtr->dwComboIndex = 0;
	return 0;
}

void FUSION_API Extension::API::RemoveObject(mv* mV, LO* loPtr, EditData* edPtr, uint16 cpt)
{
#pragma EXT_EXPORT_REMOVEOBJECT
}


void FUSION_API Extension::API::DuplicateObject(mv* mV, OI* oiPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_DUPLICATEOBJECT
}

void FUSION_API Extension::API::PutObject(mv* mV, LO* loPtr, EditData* edPtr, uint16 cpt)
{
#pragma EXT_EXPORT_PUTOBJECT
}

bool32 FUSION_API Extension::API::EditObject(mv* mV, OI* oiPtr, LO* loPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_EDITOBJECT
	return FALSE;
}



/*
void FUSION_API Extension::API::EditorDisplay(mv* mV, OI* oiPtr, LO* loPtr, EditData* edPtr, RECT* rc)
{
#pragma EXT_EXPORT_EDITORDISPLAY
	// Example (simply draw ext image):
	cSurface* proto;
	if (GetSurfacePrototype(&proto, 32, ST_MEMORY, SD_DIB))
	{
		auto frameSurf = WinGetSurface((int32)mV->mvIdEditWin);
		if (frameSurf)
		{
			auto width = rc->right - rc->left;
			auto height = rc->bottom - rc->top;

			cSurface iconSurf;
			iconSurf.Create(width, height, proto);
			iconSurf.LoadImage(nSDK::hInst, EXO_IMAGE, LI_REMAP);

			iconSurf.Blit(*frameSurf, rc->left, rc->top, BMODE_TRANSP, BOP_COPY);
		}
	}
}
*/


/*
bool32 FUSION_API Extension::API::SetEditSize(mv* mV, EditData* edPtr, int32 cx, int32 cy)
{
#pragma EXT_EXPORT_SETEDITSIZE
	edPtr->swidth = cx;
	edPtr->sheight = cy;
	return TRUE;
}
*/

void FUSION_API Extension::API::GetObjectRect(mv* mV, RECT* rc, LO* loPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_GETOBJECTRECT
	rc->right = rc->left + 32; // edPtr->swidth
	rc->bottom = rc->top + 32; // edPtr->sheight
}


bool32 FUSION_API Extension::API::IsTransparent(mv* mV, LO* loPtr, EditData* edPtr, int32 dx, int32 dy)
{
#pragma EXT_EXPORT_ISTRANSPARENT
	return FALSE;
}


void FUSION_API Extension::API::PrepareToWriteObject(mv* mV, EditData* edPtr, OI* adoi)
{
#pragma EXT_EXPORT_PREPARETOWRITEOBJECT
}


bool32 FUSION_API Extension::API::UsesFile(mv* mV, tchar* fileName)
{
#pragma EXT_EXPORT_USESFILE
	bool32 r = FALSE;
	/*
	// Example from stock SDK:
	tchar*	ext, npath;

	if ( fileName != NULL )
	{
		if ( (ext=(tchar*)calloc(_MAX_EXT, sizeof(tchar))) != NULL )
		{
			if ( (npath=(tchar*)calloc(_MAX_PATH, sizeof(tchar))) != NULL )
			{
				_tcscpy(npath, fileName);
				_tsplitpath(npath, NULL, NULL, NULL, ext);
				if ( _tcsicmp(ext, _T(".txt")) == 0 )
					r = TRUE;
				free(npath);
			}
			free(ext);
		}
	}
	*/
	return r;
}

void FUSION_API Extension::API::CreateFromFile(mv* mV, tchar* fileName, EditData* edPtr)
{
#pragma EXT_EXPORT_CREATEFROMFILE
	/*
 	// Example from stock SDK:
	// Initialize your extension data from the given file
	edPtr->swidth = 32;
	edPtr->sheight = 32;

	// Example: store the filename
	 _tcscpy(edPtr->myFileName, fileName);
	*/
}


bool32 FUSION_API Extension::API::GetFilters(mv* mV, EditData* edPtr, dword dwFlags, void* pReserved)
{
#pragma EXT_EXPORT_GETFILTERS
	//if (dwFlags & GETFILTERS_IMAGES)
	//	return TRUE;

	//if (dwFlags & GETFILTERS_SOUNDS)
	//	return TRUE;
	return FALSE;
}


dword FUSION_API Extension::API::GetTextCaps(mv* mV, EditData* edPtr)
{
#pragma EXT_EXPORT_GETTEXTCAPS
	return 0; // (TEXT_ALIGN_LEFT | TEXT_ALIGN_HCENTER | TEXT_ALIGN_RIGHT | TEXT_ALIGN_TOP | TEXT_ALIGN_VCENTER | TEXT_ALIGN_BOTTOM | TEXT_FONT | TEXT_COLOR);
}

bool32 FUSION_API Extension::API::SetTextFont(mv* mV, EditData* edPtr, LOGFONT* plf, const tchar* pStyle)
{
#pragma EXT_EXPORT_SETTEXTFONT
	// Example: copy LOGFONT structure to EDITDATA
	// memcpy(&edPtr->m_lf, plf, sizeof(LOGFONT));
	return TRUE;
}

bool32 FUSION_API Extension::API::GetTextFont(mv* mV, EditData* edPtr, LOGFONT* plf, tchar* pStyle, uint32 cbSize)
{
#pragma EXT_EXPORT_GETTEXTFONT
	// Example: copy LOGFONT structure from EDITDATA
	// memcpy(plf, &edPtr->m_lf, sizeof(LOGFONT));
	return TRUE;
}

void FUSION_API Extension::API::SetTextClr(mv* mV, EditData* edPtr, COLORREF color)
{
#pragma EXT_EXPORT_SETTEXTCLR
	// Example:
	// edPtr->fontColor = color;
}

COLORREF FUSION_API Extension::API::GetTextClr(mv* mV, EditData* edPtr)
{
#pragma EXT_EXPORT_GETTEXTCLR
	// Example:
	// return edPtr->fontColor;
	return 0;
}

void FUSION_API Extension::API::SetTextAlignment(mv* mV, EditData* edPtr, dword dwAlignFlags)
{
#pragma EXT_EXPORT_SETTEXTALIGNMENT
	/*
	// Example from stock SDK:
	dword dw = edPtr->eData.dwFlags;

	if ( (dwAlignFlags & TEXT_ALIGN_LEFT) != 0 )
		dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_LEFT;
	if ( (dwAlignFlags & TEXT_ALIGN_HCENTER) != 0 )
		dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_HCENTER;
	if ( (dwAlignFlags & TEXT_ALIGN_RIGHT) != 0 )
		dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_RIGHT;

	if ( (dwAlignFlags & TEXT_ALIGN_TOP) != 0 )
		dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_TOP;
	if ( (dwAlignFlags & TEXT_ALIGN_VCENTER) != 0 )
		dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_VCENTER;
	if ( (dwAlignFlags & TEXT_ALIGN_BOTTOM) != 0 )
		dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_BOTTOM;

	edPtr->eData.dwFlags = dw;
	*/
}

dword FUSION_API Extension::API::GetTextAlignment(mv* mV, EditData* edPtr)
{
#pragma EXT_EXPORT_GETTEXTALIGNMENT
	/*
	// Example from stock SDK:
	if ( (edPtr->eData.dwFlags & ALIGN_LEFT) != 0 )
		dw |= TEXT_ALIGN_LEFT;
	if ( (edPtr->eData.dwFlags & ALIGN_HCENTER) != 0 )
		dw |= TEXT_ALIGN_HCENTER;
	if ( (edPtr->eData.dwFlags & ALIGN_RIGHT) != 0 )
		dw |= TEXT_ALIGN_RIGHT;
	if ( (edPtr->eData.dwFlags & ALIGN_TOP) != 0 )
		dw |= TEXT_ALIGN_TOP;
	if ( (edPtr->eData.dwFlags & ALIGN_VCENTER) != 0 )
		dw |= TEXT_ALIGN_VCENTER;
	if ( (edPtr->eData.dwFlags & ALIGN_BOTTOM) != 0 )
		dw |= TEXT_ALIGN_BOTTOM;
	*/
	return 0;
}


HMENU FUSION_API Extension::API::GetActionMenu(mv* mV, OI* oiPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_GETACTIONMENU
	return nSDK::Exports::GetActionMenu(mV, oiPtr, edPtr);
}

HMENU FUSION_API Extension::API::GetConditionMenu(mv* mV, OI* oiPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_GETCONDITIONMENU
	return nSDK::Exports::GetConditionMenu(mV, oiPtr, edPtr);
}

HMENU FUSION_API Extension::API::GetExpressionMenu(mv* mV, OI* oiPtr, EditData* edPtr)
{
#pragma EXT_EXPORT_GETEXPRESSIONMENU
	return nSDK::Exports::GetExpressionMenu(mV, oiPtr, edPtr);
}


int16 FUSION_API Extension::API::GetActionCodeFromMenu(mv* mV, int16 menuId)
{
#pragma EXT_EXPORT_GETACTIONCODEFROMMENU
	return nSDK::Exports::GetActionCodeFromMenu(mV, menuId);
}

int16 FUSION_API Extension::API::GetConditionCodeFromMenu(mv* mV, int16 menuId)
{
#pragma EXT_EXPORT_GETCONDITIONCODEFROMMENU
	return nSDK::Exports::GetConditionCodeFromMenu(mV, menuId);
}

int16 FUSION_API Extension::API::GetExpressionCodeFromMenu(mv* mV, int16 menuId)
{
#pragma EXT_EXPORT_GETEXPRESSIONCODEFROMMENU
	return nSDK::Exports::GetExpressionCodeFromMenu(mV, menuId);
}


void FUSION_API Extension::API::GetActionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_EXPORT_GETACTIONTITLE
	return nSDK::Exports::GetActionTitle(mV, code, param, strBuf, maxLen);
}

void FUSION_API Extension::API::GetConditionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_EXPORT_GETCONDITIONTITLE
	return nSDK::Exports::GetConditionTitle(mV, code, param, strBuf, maxLen);
}

void FUSION_API Extension::API::GetExpressionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_EXPORT_GETEXPRESSIONTITLE
	return nSDK::Exports::GetExpressionTitle(mV, code, param, strBuf, maxLen);
}


void FUSION_API Extension::API::GetActionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
#pragma EXT_EXPORT_GETACTIONSTRING
	return nSDK::Exports::GetActionString(mV, code, strPtr, maxLen);
}

void FUSION_API Extension::API::GetConditionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
#pragma EXT_EXPORT_GETCONDITIONSTRING
	return nSDK::Exports::GetConditionString(mV, code, strPtr, maxLen);
}

void FUSION_API Extension::API::GetExpressionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
#pragma EXT_EXPORT_GETEXPRESSIONSTRING
	return nSDK::Exports::GetExpressionString(mV, code, strPtr, maxLen);
}

void FUSION_API Extension::API::GetExpressionParam(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_EXPORT_GETEXPRESSIONPARAM
	return nSDK::Exports::GetExpressionParam(mV, code, param, strBuf, maxLen);
}


infosEventsV2* FUSION_API Extension::API::GetActionInfos(mv* mV, int16 code)
{
#pragma EXT_EXPORT_GETACTIONINFOS
	return nSDK::Exports::GetActionInfos(mV, code);
}

infosEventsV2* FUSION_API Extension::API::GetConditionInfos(mv* mV, int16 code)
{
#pragma EXT_EXPORT_GETCONDITIONINFOS
	return nSDK::Exports::GetConditionInfos(mV, code);
}

infosEventsV2* FUSION_API Extension::API::GetExpressionInfos(mv* mV, int16 code)
{
#pragma EXT_EXPORT_GETEXPRESSIONINFOS
	return nSDK::Exports::GetExpressionInfos(mV, code);
}
#endif