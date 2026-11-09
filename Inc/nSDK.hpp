#pragma once
#include <nSDKCommon.hpp>
#include <nSDKExports.hpp>
#include <nSDKProps.hpp>

#include <Ccxhdr.h>
#include <Surface.h>

// DEFINES:
// -----
#ifdef _UNICODE
#define	EXT_DEFAULT_MIN_BUILD 249
#else
#define	EXT_DEFAULT_MIN_BUILD 243
#endif // _UNICODE

#ifdef EXT_EDITOR
#define EXT_MENU_SEPERATOR { NULL, NULL }
#ifdef _UNICODE
#define ACE_TEXT(text) L ## text
#else
#define ACE_TEXT(text) text
#endif // _UNICODE
#else
#define ACE_TEXT(text) NULL
#endif // EXT_EDITOR

// Return the correct pointer to rSpr as indicated in rCom
// You must have the rCom structure declared in RunData!
#define GET_RSPR(rdPtr) (rSpr*)(((uint8*)rdPtr) + rdPtr->rc.rcOffsetSprite)
// Return the correct pointer to rAni as indicated in rCom
// You must have the rCom structure declared in RunData!
#define GET_RANI(rdPtr) (rAni*)(((uint8*)rdPtr) + rdPtr->rc.rcOffsetAnimation)
// -----

// Everything related to your Fusion extension
namespace Extension
{
	struct EditData;
	struct RunData;
	// ^^ Forward declarations
}

namespace nSDK
{
	enum ConditionFlags : uint8
	{
		ConditionFlags_Generated = 1,
		ConditionFlags_Negatable = 1 << 1
	};

	struct ACEParameter
	{
		uint32 type = DataType_Number;
		const tchar* name = _T("");
	};

	// A/C/E (Actions/Conditions/Expressions) description
	struct ACE
	{
		uint16 code = 0;
		const tchar* name = _T("");
		uint8 flags = 0; // ConditionFlags for conditions, or the return type (DataType) for expressions
		void* funcPtr = NULL; // Function for this ACE
		std::vector<ACEParameter> params;
	};

	struct ACEMenu
	{
		uint16 code = 0;
		const tchar* name = _T("");
		bool enabled = false;
		std::vector<ACEMenu> subMenus;
	};

	// EDIT/RUNDATA STUBS:
	// -----
	struct RunDataBase
	{
		// Required
		headerObject rHo;
	};
	struct EditDataBase
	{
		// Required
		extHeader eHeader;
	};
	// -----

#if defined(EXT_EDITOR) && defined(EXT_USE_PNG_ICON)
	// Creates a cSurface ext icon from a PNG resource
	// Use DeleteSurface to free it
	cSurface* FUSION_API CreatePNGIcon(mv* mV, LPCTSTR lpResName);
#endif

	float ParamToFloat(long value);
	const tchar* ParamToString(long value);
	paramExt* ParamToCustom(long value);

	long ReturnString(RunDataBase* rdPtr, const tchar* value, bool copy = true);
	long ReturnFloat(RunDataBase* rdPtr, float value);

#ifdef EXT_EDITOR
	extern std::vector<Properties> properties;

	extern std::vector<ACEMenu> actionMenus;
	extern std::vector<ACEMenu> conditionMenus;
	extern std::vector<ACEMenu> expressionMenus;
#endif

	extern std::vector<ACE> actionDefinitions;
	extern std::vector<ACE> conditionDefinitions;
	extern std::vector<ACE> expressionDefinitions;

	extern void** actionJumps;
	extern void** conditionJumps;
	extern void** expressionJumps;

	extern int16 numActions, numConditions, numExpressions;

	namespace Exports
	{
		int32 FUSION_API Initialize(mv* mV, int32 quiet);
		int32 FUSION_API Free(mv* mV);

#ifdef EXT_EDITOR
		bool32 FUSION_API GetProperties(mv* mV, Extension::EditData* edPtr, bool32 bMasterItem);

		HMENU FUSION_API GetActionMenu(mv* mV, OI* oiPtr, Extension::EditData* edPtr);
		HMENU FUSION_API GetConditionMenu(mv* mV, OI* oiPtr, Extension::EditData* edPtr);
		HMENU FUSION_API GetExpressionMenu(mv* mV, OI* oiPtr, Extension::EditData* edPtr);

		int16 FUSION_API GetActionCodeFromMenu(mv* mV, int16 menuId);
		int16 FUSION_API GetConditionCodeFromMenu(mv* mV, int16 menuId);
		int16 FUSION_API GetExpressionCodeFromMenu(mv* mV, int16 menuId);

		void FUSION_API GetActionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);
		void FUSION_API GetConditionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);
		// NB: not sure in what scenario this function is ever called? just rerouting to GetExpressionParam to avoid code duplication
		void FUSION_API GetExpressionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);

		void FUSION_API GetActionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);
		void FUSION_API GetConditionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);
		void FUSION_API GetExpressionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);
		void FUSION_API GetExpressionParam(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);

		infosEventsV2* FUSION_API GetActionInfos(mv* mV, int16 code);
		infosEventsV2* FUSION_API GetConditionInfos(mv* mV, int16 code);
		infosEventsV2* FUSION_API GetExpressionInfos(mv* mV, int16 code);
#endif
	}
}

namespace Extension
{
	// Extension API; the API you provide for Fusion to interact with your extension
	namespace API
	{
		// GENERAL (EDITTIME/RUNTIME) FUNCTIONS:
		// -----
		// *REQUIRED* function to retrieve version information about the extension
		dword FUSION_API GetInfos(int32 info);
		// *REQUIRED*; called to retrieve some information about the extension, like the version, identifier, A/C/E funcs, OEFLAGS etc
		// The name is a misnomer; it's called at edittime as well, hence why this function is in General
		int16 FUSION_API GetRunObjectInfos(mv* mV, kpxRunInfos* infoPtr);

		// Called to retrieve DLL dependencies for this extension
		const tchar** FUSION_API GetDependencies();

		// Called when the extension is loaded into memory
		int32 FUSION_API Initialize(mv* mV, int32 quiet);
		// Counterpart of Initialize
		int32 FUSION_API Free(mv* mV);


		// Called when each object of this extension is loaded into memory
		int32 FUSION_API LoadObject(mv* mV, const tchar* fileName, EditData* edPtr, int32 reserved);
		// Counterpart of LoadObject
		void FUSION_API UnloadObject(mv* mV, EditData* edPtr, int32 reserved);

		// If you change ext properties across versions you must implement this function to migrate the old EditData into the latest EditData you have
		// Fusion calls this if the ext version in the MFA is older than this one
		HGLOBAL FUSION_API UpdateEditStructure(mv* mV, nSDK::EditDataBase* oldEdPtr);

		// Called to relocate filenames in EditData; useful if the MFA file path changes
		void FUSION_API UpdateFileNames(mv* mV, tchar* appName, EditData* edPtr, void (WINAPI* lpfnUpdate)(tchar* appName, tchar* pathname));

		// If you aren't dealing with images, you can safely comment out this function
		// This is called when Fusion needs to enumerate images/fonts being stored in your object
		// See stock SDK docs
		int32 FUSION_API EnumElts(mv* mV, EditData* edPtr, ENUMELTPROC enumProc, ENUMELTPROC undoProc, LPARAM lp1, LPARAM lp2);
		// -----

#ifdef EXT_EDITOR
		// EDITTIME FUNCTIONS:
		// -----
		void FUSION_API GetObjInfos(mv* mV, EditData* edPtr, tchar* ObjName, tchar* ObjAuthor, tchar* ObjCopyright, tchar* ObjComment, tchar* ObjHttp);

		const tchar* FUSION_API GetHelpFileName();

		int32 FUSION_API MakeIconEx(mv* mV, cSurface* pIconSf, tchar* lpName, OI* oiPtr, EditData* edPtr);

		bool32 FUSION_API GetProperties(mv* mV, EditData* edPtr, bool32 bMasterItem);
		void FUSION_API ReleaseProperties(mv* mV, EditData* edPtr, bool32 bMasterItem);

		LPARAM FUSION_API GetPropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID);
		void FUSION_API ReleasePropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID, LPARAM lParam);

		void FUSION_API SetPropValue(mv* mV, EditData* edPtr, uint32 nPropID, CPropValue* lParam);
		CPropValue* FUSION_API GetPropValue(mv* mV, EditData* edPtr, uint32 nPropID);

		void FUSION_API SetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID, bool32 nCheck);
		bool32 FUSION_API GetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID);

		bool32 FUSION_API EditProp(mv* mV, EditData* edPtr, uint32 nPropID);

		bool32 FUSION_API IsPropEnabled(mv* mV, EditData* edPtr, uint32 nPropID);

		void FUSION_API InitParameter(mv* mV, int16 code, paramExt* pExt);
		void FUSION_API EditParameter(mv* mV, int16 code, paramExt* pExt);
		void FUSION_API GetParameterString(mv* mV, int16 code, paramExt* pExt, tchar* pDest, int16 size);

		int32 FUSION_API CreateObject(mv* mV, LO* loPtr, EditData* edPtr);
		void FUSION_API RemoveObject(mv* mV, LO* loPtr, EditData* edPtr, uint16 cpt);
		void FUSION_API DuplicateObject(mv* mV, OI* oiPtr, EditData* edPtr);
		void FUSION_API PutObject(mv* mV, LO* loPtr, EditData* edPtr, uint16 cpt);
		bool32 FUSION_API EditObject(mv* mV, OI* oiPtr, LO* loPtr, EditData* edPtr);

		void FUSION_API EditorDisplay(mv* mV, OI* oiPtr, LO* loPtr, EditData* edPtr, RECT* rc);

		bool32 FUSION_API SetEditSize(mv* mV, EditData* edPtr, int32 cx, int32 cy);
		void FUSION_API GetObjectRect(mv* mV, RECT* rc, LO* loPtr, EditData* edPtr);

		bool32 FUSION_API IsTransparent(mv* mV, LO* loPtr, EditData* edPtr, int32 dx, int32 dy);

		void FUSION_API PrepareToWriteObject(mv* mV, EditData* edPtr, OI* adoi);

		bool32 FUSION_API UsesFile(mv* mV, tchar* fileName);
		void FUSION_API CreateFromFile(mv* mV, tchar* fileName, EditData* edPtr);

		bool32 FUSION_API GetFilters(mv* mV, EditData* edPtr, dword dwFlags, void* pReserved);


		// TEXT FUNCTIONS (OEFLAG_TEXT):
		// These functions can be discarded if you're not using OEFLAG_TEXT:
		// ---
		dword FUSION_API GetTextCaps(mv* mV, EditData* edPtr);

		bool32 FUSION_API SetTextFont(mv* mV, EditData* edPtr, LOGFONT* plf, const tchar* pStyle);
		bool32 FUSION_API GetTextFont(mv* mV, EditData* edPtr, LOGFONT* plf, tchar* pStyle, uint32 cbSize);

		void FUSION_API SetTextClr(mv* mV, EditData* edPtr, COLORREF color);
		COLORREF FUSION_API GetTextClr(mv* mV, EditData* edPtr);

		void FUSION_API SetTextAlignment(mv* mV, EditData* edPtr, dword dwAlignFlags);
		dword FUSION_API GetTextAlignment(mv* mV, EditData* edPtr);
		// ---

		HMENU FUSION_API GetActionMenu(mv* mV, OI* oiPtr, EditData* edPtr);
		HMENU FUSION_API GetConditionMenu(mv* mV, OI* oiPtr, EditData* edPtr);
		HMENU FUSION_API GetExpressionMenu(mv* mV, OI* oiPtr, EditData* edPtr);

		int16 FUSION_API GetActionCodeFromMenu(mv* mV, int16 menuId);
		int16 FUSION_API GetConditionCodeFromMenu(mv* mV, int16 menuId);
		int16 FUSION_API GetExpressionCodeFromMenu(mv* mV, int16 menuId);

		void FUSION_API GetActionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);
		void FUSION_API GetConditionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);
		void FUSION_API GetExpressionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);

		void FUSION_API GetActionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);
		void FUSION_API GetConditionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);
		void FUSION_API GetExpressionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);
		void FUSION_API GetExpressionParam(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);

		infosEventsV2* FUSION_API GetActionInfos(mv* mV, int16 code);
		infosEventsV2* FUSION_API GetConditionInfos(mv* mV, int16 code);
		infosEventsV2* FUSION_API GetExpressionInfos(mv* mV, int16 code);


		bool32 FUSION_API PushBuild(EditData* edPtr, mv* mV, int32 count, dword buildType, dword flags);
		bool32 FUSION_API PopBuild(EditData* edPtr, mv* mV, int32 count, dword buildType, dword flags);

		// EXPORTER-SPECIFIC FUNCTIONS:
		// ---
		void FUSION_API PrepareAndroidBuild(mv* mV, EditData* edPtr, const wchar* androidBuildPath);
		// (not officially documented):
		// ---
		bool32 FUSION_API PrepareUWPBuild(mv* mV, EditData* edPtr, int32 buildType, const wchar* uwpBuildPath, const wchar* uwpProjectName);
		bool32 FUSION_API PrepareFlexBuild(mv* mV, EditData* edPtr, const wchar* flexBuildPath);
		
		bool32 FUSION_API PrepareHtml5Build(EditData* edPtr, mv* mV, int32 count, int32 buildType, dword flags, const wchar* indexHTMLPath, const wchar* sourceFolderPath, const wchar* mediaFolderPath, const wchar* runtimeHTML5Path, const wchar* sourceFolderName, const wchar* mediaFolderName, const wchar* html5ProjectName, const wchar* jsCompileScriptPath, const wchar* tempSrcFolderPath, int32 appWidth, int32 appHeight);
		// ---
		// -----
#endif

		// RUNTIME FUNCTIONS:
		// -----
		// Called when the Fusion app starts
		// Note that "Fusion app" could also mean a sub-app, in which case this function is also called there too
		void FUSION_API StartApp(mv* mV, CRunApp* pApp);
		// Counterpart of StartApp
		void FUSION_API EndApp(mv* mV, CRunApp* pApp);

		// Called when the frame starts or restarts
		void FUSION_API StartFrame(mv* mV, dword dwReserved, int32 nFrameIndex);
		// Called when the frame ends
		void FUSION_API EndFrame(mv* mV, dword dwReserved, int32 nFrameIndex);

		// Tells Fusion what your RunData size is
		uint16 FUSION_API GetRunObjectDataSize(RunHeader* rhPtr, EditData* edPtr);

		// If you don't wish to implement your own display routine, but you use a cSurface, you can simply pass it to this function and Fusion will handle the blitting automatically to the frame surface; taking effects and sprite position into consideration
		cSurface* FUSION_API GetRunObjectSurface(RunData* rdPtr);
		// If you're using OEPREFS_FINECOLLISIONS, you need to implement this function to generate the collision mask for your object
		sMask* FUSION_API GetRunObjectCollisionMask(RunData* rdPtr, LPARAM lParam);

		// Called when the extension object is created
		// Should be used to initialize RunData
		// If you return an error code here (other than 0), DestroyRunObject is invoked
		int16 FUSION_API CreateRunObject(RunData* rdPtr, EditData* edPtr, createObjectInfo* cobPtr);
		// Counterpart of CreateRunObject
		// Free resources you allocated in RunData
		// For the 'fast' parameter, see stock SDK docs
		int16 FUSION_API DestroyRunObject(RunData* rdPtr, long fast);
		// *REQUIRED*; called every Fusion loop, unless you return REFLAG_ONESHOT
		// You may return REFLAG_DISPLAY to trigger DisplayRunObject, or 0 to simply trigger this function next frame
		int16 FUSION_API HandleRunObject(RunData* rdPtr);
		// If you return REFLAG_DISPLAY in HandleRunObject, this function will run
		// Most common use case is to blit a cSurface onto the frame surface (see example below), although GetRunObjectSurface, defined far above, is meant to do this for you automatically
		// Note that this function will never be triggered if you use OEFLAG_ANIMATIONS!
		int16 FUSION_API DisplayRunObject(RunData* rdPtr);

		// When Fusion runtime is paused
		int16 FUSION_API PauseRunObject(RunData* rdPtr);
		// When Fusion runtime is resumed
		int16 FUSION_API ContinueRunObject(RunData* rdPtr);

		// When the extension data needs to be saved to disk (using the frame save action)
		bool32 FUSION_API SaveRunObject(RunData* rdPtr, HANDLE hFile);
		// When the extension data needs to be loaded from disk (using the frame load action)
		bool32 FUSION_API LoadRunObject(RunData* rdPtr, HANDLE hFile);

		// WINDOW PROC FUNCTIONS (OEFLAG_WINDOWPROC):
		// Remove/ignore these functions if you aren't using OEFLAG_WINDOWPROC
		// ---
		// Get the RunData pointer from an HWND (must have been subclassed with RFUNCTION_SUBCLASSWINDOW, see stock SDK docs)
		inline RunData* GetRdPtr(HWND hWnd, RunHeader* rhPtr) { return (RunData*)GetProp(hWnd, (LPCTSTR)rhPtr->rh4.rh4AtomRd); }

		LRESULT FUSION_API WindowProc(RunHeader* rhPtr, HWND hWnd, uint32 msg, WPARAM wParam, LPARAM lParam);
		// ---

		// TEXT FUNCTIONS (OEFLAG_TEXT):
		// Remove/ignore these functions if you aren't using OEFLAG_TEXT
		// ---
		// Set the extension object's font
		void FUSION_API SetRunObjectFont(RunData* rdPtr, LOGFONT* pLf, RECT* pRc);
		// Return the extension object's font
		void FUSION_API GetRunObjectFont(RunData* rdPtr, LOGFONT* pLf);

		// Set the extension object's text color
		void FUSION_API SetRunObjectTextColor(RunData* rdPtr, COLORREF rgb);
		// Return the extension object's text color
		COLORREF FUSION_API GetRunObjectTextColor(RunData* rdPtr);
		// ---

#ifdef EXT_EDITOR
		// FUSION DEBUGGER IMPLEMENTATION:
		// ---
		// Tells Fusion your debug tree
		word* FUSION_API GetDebugTree(RunData* rdPtr);
		// Should return the text of a debugger item
		void FUSION_API GetDebugItem(tchar* pBuffer, RunData* rdPtr, int32 id);
		// When the debug item is to be edited
		void FUSION_API EditDebugItem(RunData* rdPtr, int32 id);
		// ---
#endif
		// -----
	}
}

// Compatibility (namely for TigsExt):
// -----
typedef Extension::EditData* LPEDATA;
typedef Extension::RunData* LPRDATA;
// -----