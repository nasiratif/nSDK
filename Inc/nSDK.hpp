#pragma once
#define WIN32_LEAN_AND_MEAN
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <vector>
#include <Windows.h>
#include <strsafe.h>

#define	COXSDK
#define MMFEXT
#include <Ccxhdr.h>
#include <Surface.h>

#ifdef uchar
#undef uchar
#endif

#ifdef ushort
#undef ushort
#endif

#ifdef ulong
#undef ulong
#endif

// TYPEDEFS:
// -----
// 8-bit, unsigned integer
typedef uint8_t uchar, byte, uint8;
// 8-bit, signed integer
typedef int8_t int8;
// 16-bit, signed integer
typedef int16_t int16;
// 16 bit, unsigned integer
typedef uint16_t ushort, word, uint16;
// 32-bit, signed integer
typedef int32_t int32, bool32;
// 32-bit, unsigned integer
typedef unsigned long ulong, dword;
// 32-bit, unsigned integer
typedef uint32_t uint32;
// 64-bit, unsigned integer
typedef uint64_t uint64;
// 64-bit, signed integer
typedef int64_t int64;
// Enough bits to hold a pointer value
typedef uintptr_t uintptr;
// 32-bit, floating-point number
typedef float float32;
// 64-bit, floating-point number
typedef double float64;
// Wide character
typedef wchar_t wchar;
// ASCII/Unicode character (dependong on Unicode define)
#ifdef _UNICODE
typedef wchar tchar;
#else
typedef char tchar;
#endif
// -----

// DEFINES:
// -----
#ifdef _DEBUG
#define EXT_DEBUG
#endif

#ifdef _UNICODE
#define	EXT_DEFAULT_MIN_BUILD 249
#else
#define	EXT_DEFAULT_MIN_BUILD 243
#endif // _UNICODE

// When specifying a string in single quotes, the endianness of the characters will not be what we expect, so this reverses the bytes
#define EXT_FIX_IDENTIFIER(id) ((id & 0xFF) << 24) | ((id & 0xFF00) << 8) | ((id & 0xFF0000) >> 8) | ((id & 0xFF000000) >> 24)

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

#define FUSION_API WINAPI
#define EXT_DLLEXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
// -----

// Forward declarations:
// -----
struct EditData;
struct RunData;
// -----

namespace nSDK
{
	enum ConditionFlags : uint8
	{
		ConditionFlags_Generated = 1,
		ConditionFlags_Negatable = 1 << 1
	};

	enum DataType : uint32
	{
		DataType_Number,
		DataType_String,
		DataType_Color,
		DataType_Filename
	};

	// NB: Some of these types are not officially documented..
	enum PropType : uint16
	{
		PropType_StaticString = PROPTYPE_STATIC,
		PropType_Folder = PROPTYPE_FOLDER,
		PropType_FolderEnd = PROPTYPE_FOLDER_END,
		PropType_EditButton = PROPTYPE_EDITBUTTON,
		PropType_EditString = PROPTYPE_EDIT_STRING,
		PropType_EditNumber = PROPTYPE_EDIT_NUMBER,
		PropType_ComboBox = PROPTYPE_COMBOBOX,
		PropType_Size = PROPTYPE_SIZE,
		PropType_Color = PROPTYPE_COLOR,
		PropType_Checkbox = PROPTYPE_LEFTCHECKBOX,
		PropType_SliderEdit = PROPTYPE_SLIDEREDIT,
		PropType_SpinEdit = PROPTYPE_SPINEDIT,
		PropType_DirectionControl = PROPTYPE_DIRCTRL,
		PropType_Group = PROPTYPE_GROUP,
		PropType_ListButton = PROPTYPE_LISTBTN,
		PropType_FileName = PROPTYPE_FILENAME,
		PropType_Font = PROPTYPE_FONT,
		PropType_Custom = PROPTYPE_CUSTOM,
		PropType_PictureFilename = PROPTYPE_PICTUREFILENAME,
		PropType_ComboBoxButton = PROPTYPE_COMBOBOXBTN,
		PropType_EditFloat = PROPTYPE_EDIT_FLOAT,
		PropType_EditMultiline = PROPTYPE_EDIT_MULTILINE,
		PropType_ImageList = PROPTYPE_IMAGELIST,
		PropType_IconComboBox = PROPTYPE_ICONCOMBOBOX,
		PropType_UrlButton = PROPTYPE_URLBUTTON,
		PropType_DirectoryName = PROPTYPE_DIRECTORYNAME,
		PropType_SpinEditFloat = PROPTYPE_SPINEDITFLOAT,
	};

	enum PropOptions : uint32
	{
		PropOptions_None = PROPOPT_NIL,
		// If the property type is a checkbox, this is automatically added if not set
		PropOptions_Checkbox = PROPOPT_CHECKBOX,
		PropOptions_Bold = PROPOPT_BOLD,
		PropOptions_ParamRequired = PROPOPT_PARAMREQUIRED,
		PropOptions_Removable = PROPOPT_REMOVABLE,
		PropOptions_Renamable = PROPOPT_RENAMABLE,
		PropOptions_Movable = PROPOPT_MOVABLE,
		PropOptions_List = PROPOPT_LIST,
		PropOptions_SingleSel = PROPOPT_SINGLESEL,

		// Property-specific options
		// -----
		PropOptions_EditPassword = PROPOPT_EDIT_PASSWORD,
		PropOptions_EditLowercase = PROPOPT_EDIT_LOWERCASE,
		PropOptions_EditUppercase = PROPOPT_EDIT_UPPERCASE,

		PropOptions_ComboSort = PROPOPT_COMBO_SORT
		// -----
	};

	enum PropTab : uint16
	{
		PropTab_General = PROPID_TAB_GENERAL,
		PropTab_Display = PROPID_TAB_DISPLAY,
		PropTab_Window = PROPID_TAB_WINDOW,
		PropTab_SizePos = PROPID_TAB_SIZEPOS,
		PropTab_TextOptions = PROPID_TAB_TEXTOPT,
		PropTab_Movement = PROPID_TAB_MOVEMENT,
		PropTab_RunOptions = PROPID_TAB_RUNOPT,
		PropTab_Values = PROPID_TAB_VALUES,
		PropTab_Events = PROPID_TAB_EVENTS,
		PropTab_About = PROPID_TAB_ABOUT,
		PropTab_Bluray = PROPID_TAB_BLURAY,
		PropTab_iPhone = PROPID_TAB_IPHONE,
		PropTab_Android = PROPID_TAB_ANDROID,
		PropTab_XNA = PROPID_TAB_XNA,
		PropTab_Mac = PROPID_TAB_MAC,
		PropTab_HTML5 = PROPID_TAB_HTML5,
		PropTab_Vita = PROPID_TAB_VITA,
		PropTab_Custom1 = PROPID_TAB_CUSTOM1,
		PropTab_Custom2 = PROPID_TAB_CUSTOM2,
		PropTab_Max = PROPTAB_MAX
	};


	struct Property
	{
		uint32 id = PROPID_EXTITEM_CUSTOM_FIRST;
		const tchar* name = _T("");
		const tchar* info = _T("");
		PropType type = PropType_StaticString;
		uint32 options = PropOptions_None;
		void* creationParam = NULL; // see stock SDK docs
	};

	struct Properties
	{
		PropTab tab = PropTab_General;
		std::vector<Property> props;
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

	long CopyString(RunDataBase* rdPtr, const tchar* str);
	float ParamToFloat(long value);
	const tchar* ParamToString(long value);
	paramExt* ParamToCustom(long value);

	long ReturnString(const tchar* value);
	long ReturnFloat(float value);

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

	extern HINSTANCE hInst;

	namespace Exports
	{
		bool32 WINAPI DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved);

		int32 FUSION_API Initialize(mv* mV, int32 quiet);
		int32 FUSION_API Free(mv* mV);

#ifdef EXT_EDITOR
		bool32 FUSION_API GetProperties(mv* mV, EditData* edPtr, bool32 bMasterItem);

		HMENU FUSION_API GetActionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr);
		HMENU FUSION_API GetConditionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr);
		HMENU FUSION_API GetExpressionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr);

		short FUSION_API GetActionCodeFromMenu(mv* mV, short menuId);
		short FUSION_API GetConditionCodeFromMenu(mv* mV, short menuId);
		short FUSION_API GetExpressionCodeFromMenu(mv* mV, short menuId);

		void FUSION_API GetActionTitle(mv* mV, short code, short param, tchar* strBuf, short maxLen);
		void FUSION_API GetConditionTitle(mv* mV, short code, short param, tchar* strBuf, short maxLen);
		// NB: not sure in what scenario this function is ever called? just rerouting to GetExpressionParam to avoid code duplication
		void FUSION_API GetExpressionTitle(mv* mV, short code, short param, tchar* strBuf, short maxLen);

		void FUSION_API GetActionString(mv* mV, short code, tchar* strPtr, short maxLen);
		void FUSION_API GetConditionString(mv* mV, short code, tchar* strPtr, short maxLen);
		void FUSION_API GetExpressionString(mv* mV, short code, tchar* strPtr, short maxLen);
		void FUSION_API GetExpressionParam(mv* mV, short code, short param, tchar* strBuf, short maxLen);

		LPINFOEVENTSV2 FUSION_API GetActionInfos(mv* mV, short code);
		LPINFOEVENTSV2 FUSION_API GetConditionInfos(mv* mV, short code);
		LPINFOEVENTSV2 FUSION_API GetExpressionInfos(mv* mV, short code);
#endif
	}
}