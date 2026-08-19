#include "Extension.hpp"

// DLL entry point
bool32 WINAPI DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved)
{
	return nSDK::Exports::DllMain(hinstDLL, fdwReason, lpvReserved);
}

/*

---------- GENERAL FUNCTIONS (EDITTIME & RUNTIME) ----------

*/


dword FUSION_API Extension::API::GetInfos(int32 info)
{
#pragma EXT_EXPORT_GETINFOS
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

int16 FUSION_API Extension::API::GetRunObjectInfos(mv* mV, kpxRunInfos* infoPtr)
{
#pragma EXT_EXPORT_GETRUNOBJECTINFOS
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


const tchar** FUSION_API Extension::API::GetDependencies()
{
#pragma EXT_EXPORT_GETDEPENDENCIES
	static const tchar* deps[] = EXT_DEPENDENCIES;
	return deps;
}


int32 FUSION_API Extension::API::Initialize(mv* mV, int32 quiet)
{
#pragma EXT_EXPORT_INITIALIZE
	return nSDK::Exports::Initialize(mV, quiet);
}

int32 FUSION_API Extension::API::Free(mv* mV)
{
#pragma EXT_EXPORT_FREE
	return nSDK::Exports::Free(mV);
}


int32 FUSION_API Extension::API::LoadObject(mv* mV, const tchar* fileName, EditData* edPtr, int32 reserved)
{
#pragma EXT_EXPORT_LOADOBJECT
	return 0;
}

void FUSION_API Extension::API::UnloadObject(mv* mV, EditData* edPtr, int32 reserved)
{
#pragma EXT_EXPORT_UNLOADOBJECT
}


HGLOBAL FUSION_API Extension::API::UpdateEditStructure(mv* mV, nSDK::EditDataBase* oldEdPtr)
{
#pragma EXT_EXPORT_UPDATEEDITSTRUCTURE
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


void FUSION_API Extension::API::UpdateFileNames(mv* mV, tchar* appName, EditData* edPtr, void (FUSION_API *lpfnUpdate)(tchar* appName, tchar* pathname))
{
#pragma EXT_EXPORT_UPDATEFILENAMES
	/*
	// Example from stock SDK: update file name
	if (edPtr->eaviName[0] != 0)
		lpfnUpdate(appName, edPtr->eaviName);
	*/
}


int32 FUSION_API Extension::API::EnumElts(mv* mV, EditData* edPtr, ENUMELTPROC enumProc, ENUMELTPROC undoProc, LPARAM lp1, LPARAM lp2)
{
#pragma EXT_EXPORT_ENUMELTS
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