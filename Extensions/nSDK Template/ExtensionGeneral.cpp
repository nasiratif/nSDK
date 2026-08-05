#include "Extension.hpp"

/*

---------- GENERAL FUNCTIONS ----------

*/

// DLL entry point
bool32 WINAPI DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved)
{
	return nSDK::Exports::DllMain(hinstDLL, fdwReason, lpvReserved);
}


// Necessary function to retrieve version information about the extension
dword FUSION_API GetInfos(int32 info)
{
#pragma EXT_DLLEXPORT
	switch (info)
	{
	case KGI_VERSION:
		return EXT_VERSION2;
	case KGI_PLUGIN:
		return EXT_PLUGIN_VERSION1;
	case KGI_PRODUCT:
#if EXT_DEVONLY
		return PRODUCT_VERSION_DEV;
#else
		return PRODUCT_VERSION_STANDARD;
	case KGI_BUILD:
		return EXT_MIN_BUILD;
	case KGI_UNICODE:
#ifdef _UNICODE
		return TRUE;
#else
		return FALSE;
#endif
	default:
		return 0;
#endif
	}
}

// Called to retrieve some information about the extension, like the version, identifier, A/C/E funcs, OEFLAGS etc
// The name is a misnomer; it's called at edittime as well, hence why this function is in General
int16 FUSION_API GetRunObjectInfos(mv* mV, kpxRunInfos* infoPtr)
{
#pragma EXT_DLLEXPORT
	infoPtr->identifier = EXT_FIX_IDENTIFIER(EXT_IDENTIFIER);
	infoPtr->version = EXT_VERSION;

	infoPtr->actions = nSDK::actionJumps;
	infoPtr->conditions = nSDK::conditionJumps;
	infoPtr->expressions = nSDK::expressionJumps;

	infoPtr->numOfActions = nSDK::numActions;
	infoPtr->numOfConditions = nSDK::numConditions;
	infoPtr->numOfExpressions = nSDK::numExpressions;

	infoPtr->editDataSize = sizeof(EditData);
	infoPtr->editFlags = EXT_OEFLAGS;
	infoPtr->editPrefs = EXT_OEPREFS;

	infoPtr->windowProcPriority = EXT_WINDOWPROC_PRIORITY;
	return TRUE;
}


// Called to retrieve DLL dependencies for this extension
const tchar** FUSION_API GetDependencies()
{
#pragma EXT_DLLEXPORT
	static const tchar* deps[] = EXT_DEPENDENCIES;
	return deps;
}


// Called when the extension is loaded into memory
int32 FUSION_API Initialize(mv* mV, int32 quiet)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::Initialize(mV, quiet);
}

// Counterpart of Initialize
int32 FUSION_API Free(mv* mV)
{
#pragma EXT_DLLEXPORT
	return nSDK::Exports::Free(mV);
}


// Called when each object of this extension is loaded into memory
int32 FUSION_API LoadObject(mv* mV, const tchar* fileName, EditData* edPtr, int32 reserved)
{
#pragma EXT_DLLEXPORT
	return 0;
}

// Counterpart of LoadObject
void FUSION_API UnloadObject(mv* mV, EditData* edPtr, int32 reserved)
{
#pragma EXT_DLLEXPORT
}


// If you change ext properties across versions you must implement this function to migrate the old EditData into the latest EditData you have
// Fusion calls this if the ext version in the MFA is older than this one
HGLOBAL FUSION_API UpdateEditStructure(mv* mV, nSDK::EditDataBase* oldEdPtr)
{
#pragma EXT_DLLEXPORT
	/*
	// Example:
	switch (oldEdPtr->eHeader.extVersion)
	{
	case 0:
	{
		auto origEdPtr = (EditData_V0*)oldEdPtr;

		auto hEdPtr = GlobalAlloc(GPTR, sizeof(EditData));
		auto edPtr = (EditData*)GlobalLock(hEdPtr);

		memcpy(&edPtr->eHeader, &origEdPtr->eHeader, sizeof(edPtr->eHeader));
		edPtr->eHeader.extVersion = EXT_VERSION;
		edPtr->eHeader.extSize = sizeof(EditData);

		// Only new thing we added here was the image IDs array:
		memset(edPtr->wImages, 0x0, sizeof(edPtr->wImages));
		// Copy old data:
		StringCbCopy(edPtr->szText, sizeof(edPtr->szText), origEdPtr->szText);
		edPtr->dwColor = origEdPtr->dwColor;
		edPtr->nCheck = origEdPtr->nCheck;
		edPtr->dwComboIndex = origEdPtr->dwComboIndex;

		GlobalUnlock(hEdPtr);
		return hEdPtr;
	}
	}
	*/
	return NULL;
}


// Called to relocate filenames in EditData; useful if the MFA file path changes
void FUSION_API UpdateFileNames(mv* mV, tchar* appName, EditData* edPtr, void (WINAPI* lpfnUpdate)(tchar* appName, tchar* pathname))
{
#pragma EXT_DLLEXPORT
	/*
	// Example from stock SDK: update file name
	if (edPtr->eaviName[0] != 0)
		lpfnUpdate(appName, edPtr->eaviName);
	*/
}


// If you aren't dealing with images, you can safely comment out this function
// This is called when Fusion needs to enumerate images/fonts being stored in your object
// See stock SDK docs
int32 FUSION_API EnumElts(mv* mV, EditData* edPtr, ENUMELTPROC enumProc, ENUMELTPROC undoProc, LPARAM lp1, LPARAM lp2)
{
#pragma EXT_DLLEXPORT
	// Example (for images):
	int32 ret = 0;
	for (int32 i = 0; i < edPtr->nImages; ++i)
	{
		ret = enumProc(&edPtr->wImages[i], IMG_TAB, lp1, lp2);
		if (ret != 0)
		{
			for (int32 k = i - 1; k >= 0; --k)
				undoProc(&edPtr->wImages[k], IMG_TAB, lp1, lp2);

			break;
		}
	}

	return ret;
}