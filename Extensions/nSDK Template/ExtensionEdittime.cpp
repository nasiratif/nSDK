#include "Extension.hpp"

#ifdef EXT_EDITOR
/*

---------- EDITTIME FUNCTIONS ----------

*/

void FUSION_API GetObjInfos(mv* mV, EditData* edPtr, tchar* ObjName, tchar* ObjAuthor, tchar* ObjCopyright, tchar* ObjComment, tchar* ObjHttp)
{
#pragma EXT_DLLEXPORT
	static const size_t length = 255 * sizeof(tchar);
	static const size_t lengthComment = 1024 * sizeof(tchar);

	StringCbCopy(ObjName, length, EXT_NAME);
	StringCbCopy(ObjAuthor, length, EXT_AUTHOR);
	StringCbCopy(ObjCopyright, length, EXT_COPYRIGHT);
	StringCbCopy(ObjComment, lengthComment, EXT_COMMENT);
	StringCbCopy(ObjHttp, length, EXT_HTTP);
}

const tchar* FUSION_API GetHelpFileName()
{
#pragma EXT_DLLEXPORT
	return EXT_HELP_FILENAME;
}


// Called to insert your object properties
bool32 FUSION_API GetProperties(mv* mV, EditData* edPtr, bool32 bMasterItem)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetProperties(mV, edPtr, bMasterItem);
}

// Counterpart of GetProperties
// Called when the properties are removed from the property window
void FUSION_API ReleaseProperties(mv* mV, EditData* edPtr, bool32 bMasterItem)
{
#pragma EXT_DLLEXPORT
}

// Called when a property is initialized and its creation parameter is NULL
// You could, for example, change the content of a combo-box property according to specific settings in the EditData structure
LPARAM FUSION_API GetPropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_DLLEXPORT
	return NULL;
}

// Called after a property was initialized
// Useful if you allocated memory in GetPropCreateParam and wish to free it now
void FUSION_API ReleasePropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID, LPARAM lParam)
{
#pragma EXT_DLLEXPORT
}


// Called when a property is changed
// Here you modify your EditData to set the new property
void FUSION_API SetPropValue(mv* mV, EditData* edPtr, uint32 nPropID, void* lParam)
{
#pragma EXT_DLLEXPORT
	// Gets the pointer to the CPropValue structure
	CPropValue* pValue = (CPropValue*)lParam;

	// Example:
	switch (nPropID)
	{
	case PropID_Images:
		edPtr->nImages = *(LPWORD)((CPropDataValue*)pValue)->m_pData; // first word contains number of images
		break;
	case PropID_Text:
		StringCbCopy(edPtr->szText, sizeof(edPtr->szText), ((CPropStringValue*)pValue)->GetString()); // note this isn't safe! just an example, you might need to use mvReAllocEditData if you need larger string length
		break;
	case PropID_Color:
		edPtr->dwColor = ((CPropDWordValue*)pValue)->m_dwValue;
		break;
	case PropID_Combo:
		edPtr->dwComboIndex = ((CPropDWordValue*)pValue)->m_dwValue;
		break;
	}
	
}

// Called when a property needs to be retrieved
// Here you retrieve a property from EditData and return the corresponding CPropValue for it
void* FUSION_API GetPropValue(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_DLLEXPORT
	
	// Example:
	switch (nPropID)
	{
	case PropID_Images:
	{
		// For image properties you must fill out a buffer of words where the first word contains the number of images, then the image IDs right after
		auto data = new CPropDataValue((edPtr->nImages + 1) * sizeof(WORD), NULL);
		auto wImages = (LPWORD)data->m_pData;
		*(wImages++) = edPtr->nImages;
		for (WORD i = 0; i < edPtr->nImages; ++i)
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

// Ditto as SetPropValue, but for checkboxes
void FUSION_API SetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID, bool32 nCheck)
{
#pragma EXT_DLLEXPORT
	
	// Example:
	switch (nPropID)
	{
	case PropID_Check:
		edPtr->nCheck = nCheck;
		break;
	}
	
}

// Ditto as GetPropValue, but for checkboxes
bool32 FUSION_API GetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_DLLEXPORT
	
	// Example:
	switch (nPropID)
	{
	case PropID_Check:
		return edPtr->nCheck;
	}
	
	return FALSE;
}

// Called when a Button/EditButton property is clicked
bool32 FUSION_API EditProp(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_DLLEXPORT
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

// Called to determine if a property should be enabled
bool32 FUSION_API IsPropEnabled(mv* mV, EditData* edPtr, uint32 nPropID)
{
#pragma EXT_DLLEXPORT
	/*
	// Example:
	if (nPropID == PropID_Check)
		// Only enable this property if the first combo box item is selected
		return edPtr->dwComboIndex != 0;
	*/
	return TRUE;
}


// Used for custom Action/Condition parameters
// Called when the parameter needs to be initialized
void FUSION_API InitParameter(mv* mV, int16 code, paramExt* pExt)
{
#pragma EXT_DLLEXPORT
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

// Called for custom Action/Condition parameters when the parameter needs to be edited
// This is where you'd display a dialog box for the user to specify the parameter
void FUSION_API EditParameter(mv* mV, int16 code, paramExt* pExt)
{
#pragma EXT_DLLEXPORT
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

// Called to get the display string of your custom parameter in the action/condition
void FUSION_API GetParameterString(mv* mV, int16 code, paramExt* pExt, tchar* pDest, int16 size)
{
#pragma EXT_DLLEXPORT
	// Example:
	if (code == CustomDataType_Example)
	{
		StringCbCopy(pDest, size, pExt->pextData[0] ? _T("ASCII16") : _T("Unicode"));
	}
}


// When objeet is created in via the "Create new object" dialog
// Should be used to initialize EditData
int32 FUSION_API CreateObject(mv* mV, fpLevObj loPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	memset(edPtr->wImages, NULL, sizeof(edPtr->wImages));
	edPtr->nImages = 0;
	StringCbCopy(edPtr->szText, sizeof(edPtr->szText), _T("Editable text"));
	edPtr->dwColor = RGB(100, 200, 255);
	edPtr->nCheck = FALSE;
	edPtr->dwComboIndex = 0;
	return 0;
}

// When object is removed
void FUSION_API RemoveObject(mv* mV, fpLevObj loPtr, EditData* edPtr, uint16 cpt)
{
#pragma EXT_DLLEXPORT
}

void FUSION_API DuplicateObject(mv* mV, fpObjInfo oiPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
}

void FUSION_API PutObject(mv* mV, fpLevObj loPtr, EditData* edPtr, uint16 cpt)
{
#pragma EXT_DLLEXPORT
}

// When object is double-clicked
bool32 FUSION_API EditObject(mv* mV, fpObjInfo oiPtr, fpLevObj loPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	return FALSE;
}

/*
// Implement this function to do custom drawing in the frame editor
void FUSION_API EditorDisplay(mv* mV, fpObjInfo oiPtr, fpLevObj loPtr, EditData* edPtr, RECT* rc)
{
#pragma EXT_DLLEXPORT
}
*/

/*
// Required for resizable exts; if exported, Fusion knows the ext is resizable
bool32 FUSION_API SetEditSize(mv* mV, EditData* edPtr, int32 cx, int32 cy)
{
#pragma EXT_DLLEXPORT
	edPtr->swidth = cx;
	edPtr->sheight = cy;
	return TRUE;
}
*/

// You must return the rectangle of the selected instance of the extension object
void FUSION_API GetObjectRect(mv* mV, RECT* rc, fpLevObj loPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	rc->right = rc->left + 32; // edPtr->swidth
	rc->bottom = rc->top + 32; // edPtr->sheight
}


// Indicates whether the mouse cursor is over a transparent zone of the object
bool32 FUSION_API IsTransparent(mv* mV, fpLevObj loPtr, EditData* edPtr, int32 dx, int32 dy)
{
#pragma EXT_DLLEXPORT
	return FALSE;
}


void FUSION_API PrepareToWriteObject(mv* mV, EditData* edPtr, fpObjInfo adoi)
{
#pragma EXT_DLLEXPORT
}


// Called when a file is dropped onto the frame
// You should return TRUE if you can create a new object from the given file
bool32 FUSION_API UsesFile(mv* mV, tchar* fileName)
{
#pragma EXT_DLLEXPORT
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

// Creates a new object from file
void FUSION_API CreateFromFile(mv* mV, tchar* fileName, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	// Example from stock SDK:
	/*
	// Initialize your extension data from the given file
	edPtr->swidth = 32;
	edPtr->sheight = 32;

	// Example: store the filename
	 _tcscpy(edPtr->myFileName, fileName);
	*/
}


// Implement this if your extension uses image/sound filters
bool32 FUSION_API GetFilters(mv* mV, EditData* edPtr, dword dwFlags, void* pReserved)
{
#pragma EXT_DLLEXPORT
	//if (dwFlags & GETFILTERS_IMAGES)
	//	return TRUE;

	//if (dwFlags & GETFILTERS_SOUNDS)
	//	return TRUE;
	return FALSE;
}


// Called to determine text capabilities of the object under the frame editor
dword FUSION_API GetTextCaps(mv* mV, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	return 0; // (TEXT_ALIGN_LEFT | TEXT_ALIGN_HCENTER | TEXT_ALIGN_RIGHT | TEXT_ALIGN_TOP | TEXT_ALIGN_VCENTER | TEXT_ALIGN_BOTTOM | TEXT_FONT | TEXT_COLOR);
}

// Called to change the font used by the object
// (pStyle is obselete)
bool32 FUSION_API SetTextFont(mv* mV, EditData* edPtr, LPLOGFONT plf, const tchar* pStyle)
{
#pragma EXT_DLLEXPORT
	// Example: copy LOGFONT structure to EDITDATA
	// memcpy(&edPtr->m_lf, plf, sizeof(LOGFONT));
	return TRUE;
}

// Returns the font used by the object
// (pStyle & cbSize are obselete)
bool32 FUSION_API GetTextFont(mv* mV, EditData* edPtr, LPLOGFONT plf, tchar* pStyle, uint32 cbSize)
{
#pragma EXT_DLLEXPORT
	// Example: copy LOGFONT structure from EDITDATA
	// memcpy(plf, &edPtr->m_lf, sizeof(LOGFONT));
	return TRUE;
}

// Set the text color of the object
void FUSION_API SetTextClr(mv* mV, EditData* edPtr, COLORREF color)
{
#pragma EXT_DLLEXPORT
	// Example:
	// edPtr->fontColor = color;
}

// Get the text color of the object
COLORREF FUSION_API GetTextClr(mv* mV, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	// Example:
	// return edPtr->fontColor;
	return 0;
}

void FUSION_API SetTextAlignment(mv* mV, EditData* edPtr, dword dwAlignFlags)
{
#pragma EXT_DLLEXPORT
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

dword FUSION_API GetTextAlignment(mv* mV, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
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


// Returns the menu to be displayed when choosing an action from this object
HMENU FUSION_API GetActionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetActionMenu(mV, oiPtr, edPtr);
}

// Returns the menu to be displayed when choosing an condition from this object
HMENU FUSION_API GetConditionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetConditionMenu(mV, oiPtr, edPtr);
}

// Returns the menu to be displayed when choosing an expression from this object
HMENU FUSION_API GetExpressionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetExpressionMenu(mV, oiPtr, edPtr);
}


// Returns the action ID from a menu option
// This is how Fusion knows which action ID to choose given the menu option selected
int16 FUSION_API GetActionCodeFromMenu(mv* mV, int16 menuId)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetActionCodeFromMenu(mV, menuId);
}

// Returns the condition ID from a menu option
// This is how Fusion knows which condition ID to choose given the menu option selected
int16 FUSION_API GetConditionCodeFromMenu(mv* mV, int16 menuId)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetConditionCodeFromMenu(mV, menuId);
}

// Returns the expression ID from a menu option
// This is how Fusion knows which expression ID to choose given the menu option selected
int16 FUSION_API GetExpressionCodeFromMenu(mv* mV, int16 menuId)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetExpressionCodeFromMenu(mV, menuId);
}


// Outputs the string on the titlebar to be displayed for a specific action parameter
void FUSION_API GetActionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetActionTitle(mV, code, param, strBuf, maxLen);
}

// Outputs the string on the titlebar to be displayed for a specific condition parameter
void FUSION_API GetConditionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetConditionTitle(mV, code, param, strBuf, maxLen);
}

// The use of this function is not entirely clear..
void FUSION_API GetExpressionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetExpressionTitle(mV, code, param, strBuf, maxLen);
}


// Outputs the string to be displayed for a specific action (e.g, "Set width to %0")
void FUSION_API GetActionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetActionString(mV, code, strPtr, maxLen);
}

// Outputs the string to be displayed for a specific condition (e.g, "%o: Is the object resizable?")
void FUSION_API GetConditionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetConditionString(mV, code, strPtr, maxLen);
}

// Outputs the name of an expression (e.g, "GetObjectWidth(")
void FUSION_API GetExpressionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetExpressionString(mV, code, strPtr, maxLen);
}

// Outputs the name of an expression parameter
void FUSION_API GetExpressionParam(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetExpressionParam(mV, code, param, strBuf, maxLen);
}


// Returns information about a specific action (e.g, number of parameters, parameter info etc)
LPINFOEVENTSV2 FUSION_API GetActionInfos(mv* mV, int16 code)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetActionInfos(mV, code);
}

// Returns information about a specific condition (e.g, flags, number of parameters, parameter info etc)
LPINFOEVENTSV2 FUSION_API GetConditionInfos(mv* mV, int16 code)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetConditionInfos(mV, code);
}

// Returns information about a specific expression (e.g, return type, number of parameters, parameter info etc)
LPINFOEVENTSV2 FUSION_API GetExpressionInfos(mv* mV, int16 code)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::GetExpressionInfos(mV, code);
}
#endif