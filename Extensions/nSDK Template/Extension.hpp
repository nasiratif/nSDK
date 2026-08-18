#pragma once
#include <nSDK.hpp>

// General includes can be placed here:
// -----
#include "Resource.h"
// -----

// DEFINES:
// -----
// Define extension information here:
// ---
#define EXT_IDENTIFIER 'NSDK'
#define EXT_NAME _T("nSDK Template")
#define EXT_AUTHOR _T("Nassic :)")
#define EXT_COPYRIGHT _T("\u00A9 Nasir \"Nassic\" W.")
#define EXT_COMMENT _T("nSDK Template extension")
#define EXT_HTTP _T("https://community.clickteam.com/user/36199-nasirfoxx/")
#define EXT_HELP_FILENAME _T("nSDK.chm")
// If your extension depends on specific DLLs then you should define them here and Fusion will embed them automatically into the built app
// The DLL files must be placed in Data\Runtime\Unicode if your ext is unicode, or Data\Runtime if not
// Example: { _T("MyFirstDll.dll"), _T("MySecondDll.dll"), NULL }
// Must end witn NULL
#define EXT_DEPENDENCIES { NULL }
// Version of your extension, you should increment this for every version of your extension
// Important for property migration!
#define EXT_VERSION 1

// Minimum build of Fusion your extension is permitted to run in (e.g, 296)
#define	EXT_MIN_BUILD EXT_DEFAULT_MIN_BUILD
// Set to 1 if MMF2/CF2.5 developer only
#define EXT_DEVONLY 0

// OEFLAGs for this extension (see stock SDK docs)
#define EXT_OEFLAGS 0
// OEPREFs for this extension (see stock SDK docs)
#define EXT_OEPREFS 0
// Window procedure priority; see stock SDK docs
#define EXT_WINDOWPROC_PRIORITY 100
// ---
// -----

namespace Extension
{
	// EDITTIME OBJECT DATA:
	// Variables must be initialized in CreateObject
	// Pointers shouldn't be stored here as this entire structure is serialized to disk (in the MFA)
	// Everything must be plain data only!
	// -----
	struct EditData : nSDK::EditDataBase
	{
		word wImages[4];
		word nImages;
		tchar szText[256];
		COLORREF dwColor;
		bool32 nCheck;
		dword dwComboIndex;
	};

	/*
	// This is an example for UpdateEditStructure; this could have been the EditData of a previous version of this extension
	// It's ideal to keep old versions of your EditData so you can migrate them to the latest EditData you have
	struct EditData_V0 : nSDK::EditDataBase
	{
		tchar szText[256];
		COLORREF dwColor;
		bool32 nCheck;
		dword dwComboIndex;
	};
	*/
	// -----

	// RUNTIME OBJECT DATA:
	// Variables must be initialized in CreateRunObject
	// -----
	struct RunData : nSDK::RunDataBase
	{
		// rCom rc; // OEFLAG_MOVEMENTS or OEFLAG_ANIMATIONS
		// rMvt rm; // OEFLAG_MOVEMENTS
		// rAni ra; // OEFLAG_ANIMATIONS
		// rSpr rs; // OEFLAG_SPRITES
		// rVal rv; // OEFLAG_VALUES
	};
	// -----

	// PROPERTY IDs
	// -----
	enum PropID : uint32
	{
		PropID_Start = PROPID_EXTITEM_CUSTOM_FIRST, // Fusion requires that extension property IDs begin with this

		// Example:
		// -----
		PropID_Images,
		PropID_Text,
		PropID_Color,
		PropID_Combo,
		PropID_Check,
		PropID_Copyright
		// -----
	};
	// -----

	// CUSTOM PARAMETER IDs:
	// -----
	enum CustomDataType
	{
		CustomDataType_Start = PARAM_EXTBASE, // Custom data types must start with this
		CustomDataType_Example
	};
	// -----

	namespace Actions
	{
		// int16 FUSION_API ExampleAction(RunData* rdPtr, long param1, long param2);

		int16 FUSION_API ChangeBGColor(RunData* rdPtr, long param1, long param2);
		int16 FUSION_API ExampleAction2(RunData* rdPtr, long param1, long param2);
	}

	namespace Conditions
	{
		// long FUSION_API ExampleCondition(RunData* rdPtr, long param1, long param2);

		long FUSION_API SameNumbers(RunData* rdPtr, long param1, long param2);
		long FUSION_API SameStrings(RunData* rdPtr, long param1, long param2);

		long FUSION_API CheckStringData(RunData* rdPtr, long param1, long param2);
		long FUSION_API Comparison(RunData* rdPtr, long param1, long param2);
	}

	namespace Expressions
	{
		// long FUSION_API ExampleExpression(RunData* rdPtr, long param);

		long FUSION_API ReturnRandomN(RunData* rdPtr, long param);
		long FUSION_API ReturnRandomS(RunData* rdPtr, long param);
	}
}