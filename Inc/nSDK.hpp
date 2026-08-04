#pragma once
#include <nSDKCommon.hpp>
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

	// Data types; expressions only use Number/String, everything else is for action/condition parameters
	// See stock SDK docs
	enum DataType : uint32
	{
		DataType_Number = PARAM_EXPRESSION,
		DataType_String = PARAM_EXPSTRING,
		// Parameter value: has PARAMCLICK_DOUBLE flag set for double clicks
		DataType_MouseClick = PARAM_CLICK,
		// Parameter value: COLORREF value
		DataType_Color = PARAM_COLOUR,
		// Example parameter title: "#Title#Choose your image...#Filter#Windows Bitmap Files|*.bmp|All files|*.*|"
		// Parameter value: chosen filename
		DataType_Filename = PARAM_FILENAME2,
		// Uses PARAM_FILENAME (which is deprecated as per stock SDK docs)
		DataType_Filename_Deprecated = PARAM_FILENAME,
		// Parameter value: a JOYSTICK_XXX define
		DataType_JoystickDirection = PARAM_JOYDIRECTION,
		// Parameter value: Windows virtual key code
		DataType_Key = PARAM_KEY,
		// Parameter value: one of the mouse-related Windows virtual key codes
		DataType_MouseKey = PARAM_VMKEY,
		// Parameter value: bitmask where bit 0 is east, bit 8 is north, bit 16 is west, & bit 24 is south
		DataType_Direction = PARAM_NEWDIRECTION,
		// Parameter value: player index, ranging from 0 to 3
		DataType_Player = PARAM_PLAYER,
		// Parameter value: LOWORD contains X position, HIWORD contains Y
		DataType_Position = PARAM_POSITION,
		// Parameter value: speed value, 0 to 100
		DataType_Speed = PARAM_SPEED,
		// Parameter value: integer containing time in 1/1000 of a second
		DataType_Time = PARAM_TIME,
		// Parameter value: pointer to an SRECT structure containing the zone
		DataType_Zone = PARAM_ZONE,
		// Conditions only
		// What you return in your action/condition is the integer to compare against
		DataType_ComparisonNumber = PARAM_COMPARAISON,
		// Conditions only
		// What you return in your action/condition is the string to compare against
		DataType_ComparisonString = PARAM_CMPSTRING,
		// Conditions only (I would assume)
		// What you return in your action/condition is an integer, in milliseconds, to compare against
		DataType_ComparisonTime = PARAM_CMPTIME,
		// Object picker
		DataType_Object = PARAM_OBJECT,
		// PARAM_INKEFFECT comments apparently states that parameter value has LOWORD with effect ID, & HIWORD with effect param
		DataType_InkEffect = PARAM_INKEFFECT
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
		bool insertAfter; // insert before the current properties in the tab, or after?
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
		bool32 FUSION_API GetProperties(mv* mV, EditData* edPtr, bool32 bMasterItem);

		HMENU FUSION_API GetActionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr);
		HMENU FUSION_API GetConditionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr);
		HMENU FUSION_API GetExpressionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr);

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