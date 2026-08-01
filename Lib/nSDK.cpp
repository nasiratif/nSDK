#include "nSDK.hpp"

namespace nSDK
{
#ifdef EXT_EDITOR
	int16* actionInfosEvents = nullptr;
	int16* conditionInfosEvents = nullptr;
	int16* expressionInfosEvents = nullptr;
#endif

	void** actionJumps = nullptr;
	void** conditionJumps = nullptr;
	void** expressionJumps = nullptr;
	int16 numActions, numConditions, numExpressions;

	HINSTANCE hInst;

	// Annoyingly, Initialize is also called on sub-apps, meaning we might do double initialization accidentally, so...
	uint32 refCount = 0;
}

/*

---------- UTILITY FUNCTIONS ----------

*/

#ifdef EXT_EDITOR

HMENU CreateACEMenus(const std::vector<nSDK::ACEMenu>& menus, uint16_t idOffset)
{
	auto hMenu = CreatePopupMenu();
	for (const auto& ace : menus)
	{
		auto enabled = ace.enabled ? MF_ENABLED : MF_DISABLED;
		if (!ace.name)
		{
			AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
		}
		else if (ace.subMenus.empty())
		{
			AppendMenu(hMenu, MF_STRING | enabled, idOffset + ace.code, ace.name);
		}
		else
		{
			auto hSubMenu = CreateACEMenus(ace.subMenus, idOffset);
			AppendMenu(hMenu, MF_POPUP | enabled, (uintptr)hSubMenu, ace.name);
		}
	}
	return hMenu;
}

#endif

/*

---------- MAIN FUNCTIONS ----------

*/

float nSDK::ParamToFloat(long value)
{
	return *((float*)&value);
}

const tchar* nSDK::ParamToString(long value)
{
	return (const tchar*)value;
}

paramExt* nSDK::ParamToCustom(long value)
{
	return (paramExt*)value;
}


long nSDK::ReturnFloat(RunDataBase* rdPtr, float value)
{
	rdPtr->rHo.hoFlags |= HOF_FLOAT; // tell Fusion we are returning a float
	return *((long*)&value);
}

long nSDK::ReturnString(RunDataBase* rdPtr, const tchar* str, bool copy)
{
	rdPtr->rHo.hoFlags |= HOF_STRING; // tell Fusion we are returning a string
	if (!copy)
		return (long)str;

	auto len = _tcslen(str);
	auto dest = (tchar*)callRunTimeFunction(rdPtr, RFUNCTION_GETSTRINGSPACE_EX, NULL, len);
	if (dest)
		memcpy(dest, str, len * sizeof(tchar));
	else
		return (long)_T("");

	dest[len] = _T('\0');
	return (long)dest;
}

/*

---------- EXPORTED FUNCTIONS ----------

*/

bool32 WINAPI nSDK::Exports::DllMain(HINSTANCE hinstDLL, dword fdwReason, void* lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		hInst = hinstDLL;

	return TRUE;
}


int32 FUSION_API nSDK::Exports::Initialize(mv* mV, int32 quiet)
{
	if (refCount)
	{
		refCount++;
		return 0;
	}
	else
	{
		refCount++;
	}

#ifdef EXT_EDITOR
	size_t byteSize;
	int16* ie;

	// Create action infos events:
	// -----
	byteSize = 0;
	numActions = 0;
	for (const auto& act : actionDefinitions)
	{
		byteSize += sizeof(infosEventsV2);
		byteSize += act.params.size() * sizeof(int16); // short params[]
		byteSize += act.params.size() * sizeof(int16); // short paramTitle[]
	}
	actionInfosEvents = byteSize ? (int16*)malloc(byteSize) : nullptr;
	ie = actionInfosEvents;
	for (const auto& act : actionDefinitions)
	{
		*(ie++) = act.code; // infosEventsV2::code
		*(ie++) = 0; // infosEventsV2::flags
		*(ie++) = (int16)act.params.size(); // infosEventsV2::nParams
		for (const auto& param : act.params)
		{
			if (param.type >= PARAM_EXTBASE)
			{
				*(ie++) = param.type;
			}
			else
			{
				switch (param.type)
				{
				case DataType_Number:
					*(ie++) = PARAM_EXPRESSION;
					break;
				case DataType_String:
					*(ie++) = PARAM_EXPSTRING;
					break;
				case DataType_Color:
					*(ie++) = PARAM_COLOUR;
					break;
				case DataType_Filename:
					*(ie++) = PARAM_FILENAME;
					break;
				default:
					*(ie++) = NULL;
					break;
				}
			} // infosEventsV2::param[X]
		}
		for (const auto& param : act.params)
			*(ie++) = NULL;  // infosEventsV2::paramTitle[X] (not using that here!)
	}
	// -----

	// Create condition infos events:
	// -----
	byteSize = 0;
	numConditions = 0;
	for (const auto& cnd : conditionDefinitions)
	{
		byteSize += sizeof(infosEventsV2);
		byteSize += cnd.params.size() * sizeof(int16); // short params[]
		byteSize += cnd.params.size() * sizeof(int16); // short paramTitle[]
	}
	conditionInfosEvents = byteSize ? (int16*)malloc(byteSize) : nullptr;
	ie = conditionInfosEvents;
	for (const auto& cnd : conditionDefinitions)
	{
		int16 flags = 0;
		if (!(cnd.flags & ConditionFlags_Generated))
			flags |= EVFLAGS_ALWAYS;
		if (cnd.flags & ConditionFlags_Negatable)
			flags |= EVFLAGS_NOTABLE;

		*(ie++) = cnd.code; // infosEventsV2::code
		*(ie++) = flags; // infosEventsV2::flags
		*(ie++) = (int16)cnd.params.size(); // infosEventsV2::nParams
		for (const auto& param : cnd.params)
		{
			if (param.type >= PARAM_EXTBASE)
			{
				*(ie++) = param.type;
			}
			else
			{
				switch (param.type)
				{
				case DataType_Number:
					*(ie++) = PARAM_EXPRESSION;
					break;
				case DataType_String:
					*(ie++) = PARAM_EXPSTRING;
					break;
				case DataType_Color:
					*(ie++) = PARAM_COLOUR;
					break;
				case DataType_Filename:
					*(ie++) = PARAM_FILENAME;
					break;
				default:
					*(ie++) = NULL;
					break;
				}
			} // infosEventsV2::param[X]
		}
		for (const auto& param : cnd.params)
			*(ie++) = NULL;  // infosEventsV2::paramTitle[X]
	}
	// -----

	// Create expression infos events:
	// -----
	byteSize = 0;
	numExpressions = 0;
	for (const auto& exp : expressionDefinitions)
	{
		byteSize += sizeof(infosEventsV2);
		byteSize += exp.params.size() * sizeof(int16); // short params[]
		byteSize += exp.params.size() * sizeof(int16); // short paramTitle[]
	}
	expressionInfosEvents = byteSize ? (int16*)malloc(byteSize) : nullptr;
	ie = expressionInfosEvents;
	for (const auto& exp : expressionDefinitions)
	{
		*(ie++) = exp.code; // infosEventsV2::code
		*(ie++) = exp.flags == DataType_Number ? 0 : EXPFLAG_STRING; // infosEventsV2::flags
		*(ie++) = (int16)exp.params.size(); // infosEventsV2::nParams
		for (const auto& param : exp.params)
		{
			switch (param.type)
			{
			case DataType_Number:
				*(ie++) = EXPPARAM_LONG;
				break;
			case DataType_String:
				*(ie++) = EXPPARAM_STRING;
				break;
			default:
				*(ie++) = NULL;
				break;
			} // infosEventsV2::param[X]
		}
		for (const auto& param : exp.params)
			*(ie++) = NULL;  // infosEventsV2::paramTitle[X]
	}
	// -----
#endif

	// Get number of ACEs:
	// -----
	numActions = 0;
	for (const auto& act : actionDefinitions)
	{
		if (act.code > numActions)
			numActions = act.code;
	}
	if (!actionDefinitions.empty())
		numActions++;

	numConditions = 0;
	for (const auto& cnd : conditionDefinitions)
	{
		if (cnd.code > numConditions)
			numConditions = cnd.code;
	}
	if (!conditionDefinitions.empty())
		numConditions++;

	numExpressions = 0;
	for (const auto& exp : expressionDefinitions)
	{
		if (exp.code > numExpressions)
			numExpressions = exp.code;
	}
	if (!expressionDefinitions.empty())
		numExpressions++;
	// -----

	size_t size;
	// Write jumps:
	// -----
	size = (numActions + 1) * sizeof(*actionJumps);
	actionJumps = (void**)malloc(size);
	memset(actionJumps, 0x0, size);

	size = (numConditions + 1) * sizeof(*conditionJumps);
	conditionJumps = (void**)malloc(size);
	memset(conditionJumps, 0x0, size);

	size = (numExpressions + 1) * sizeof(*expressionJumps);
	expressionJumps = (void**)malloc(size);
	memset(expressionJumps, 0x0, size);

	for (const auto& act : actionDefinitions)
		actionJumps[act.code] = act.funcPtr;

	for (const auto& cnd : conditionDefinitions)
		conditionJumps[cnd.code] = cnd.funcPtr;

	for (const auto& exp : expressionDefinitions)
		expressionJumps[exp.code] = exp.funcPtr;
	// -----

#ifdef EXT_DEBUG
	// For validity, make sure the A/C/E jumps are valid:
	// -----
	for (int16 i = 0; i < numActions; ++i)
	{
		if (!actionJumps[i])
		{
			tchar msg[256];
			StringCbPrintf(msg, sizeof(msg), _T("NULL action function detected!\nAction ID %u's function pointer is NULL. This could also happen if the action IDs are not contiguous (i.e, there are actions with IDs 0 and 2, but 1 doesn't exist)."), i);
			MessageBox(mV->mvHEditWin, msg, _T("Fatal Error"), MB_ICONERROR);
		}
	}
	for (int16 i = 0; i < numConditions; ++i)
	{
		if (!conditionJumps[i])
		{
			tchar msg[256];
			StringCbPrintf(msg, sizeof(msg), _T("NULL condition function detected!\nCondition ID %u's function pointer is NULL. This could also happen if the condition IDs are not contiguous (i.e, there are conditions with IDs 0 and 2, but 1 doesn't exist)."), i);
			MessageBox(mV->mvHEditWin, msg, _T("Fatal Error"), MB_ICONERROR);
		}
	}
	for (int16 i = 0; i < numExpressions; ++i)
	{
		if (!expressionJumps[i])
		{
			tchar msg[256];
			StringCbPrintf(msg, sizeof(msg), _T("NULL expression function detected!\nExpression ID %u's function pointer is NULL. This could also happen if the expression IDs are not contiguous (i.e, there are expressions with IDs 0 and 2, but 1 doesn't exist)."), i);
			MessageBox(mV->mvHEditWin, msg, _T("Fatal Error"), MB_ICONERROR);
		}
	}
	// -----
#endif

	return 0;
}

int32 FUSION_API nSDK::Exports::Free(mv* mV)
{
	if (--refCount == 0)
	{
#ifdef EXT_EDITOR
		free(actionInfosEvents);
		free(conditionInfosEvents);
		free(expressionInfosEvents);
#endif
		free(actionJumps);
		free(conditionJumps);
		free(expressionJumps);
	}
	return 0;
}


#ifdef EXT_EDITOR
/*

------ EDITTIME FUNCTIONS ------

*/

bool32 FUSION_API nSDK::Exports::GetProperties(mv* mV, EditData* edPtr, bool32 bMasterItem)
{
	if (properties.empty())
		return FALSE;

	for (const auto& props : properties)
	{
		std::vector<PropData> propDatas;
		if (!props.props.empty())
		{
			propDatas.reserve(props.props.size() + 1);
			for (const auto& prop : props.props)
			{
				auto& propData = propDatas.emplace_back();
				propData.dwID = prop.id;
				propData.sName = (uintptr)prop.name;
				propData.sInfo = (uintptr)prop.info;
				propData.lType = (uintptr)prop.type;
				propData.dwOptions = prop.options;
				if (propData.lType == PropType_Checkbox)
					propData.dwOptions |= PropOptions_Checkbox; // NB: seems redundant but necessary? UB seems to occur if this isn't set on a checkbox property.. but what even is the purpose of this option really?

				propData.lCreateParam = (LPARAM)prop.creationParam;
			}
			auto& end = propDatas.emplace_back();
			memset(&end, NULL, sizeof(end)); // Fusion expects the PropData array to end with all zeros

			mvInsertProps(mV, edPtr, propDatas.data(), props.tab, props.insertAfter);
		}
	}
	return TRUE;
}


HMENU FUSION_API nSDK::Exports::GetActionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr)
{
	return CreateACEMenus(actionMenus, 25000);
}

HMENU FUSION_API nSDK::Exports::GetConditionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr)
{
	return CreateACEMenus(conditionMenus, 26000);
}

HMENU FUSION_API nSDK::Exports::GetExpressionMenu(mv* mV, fpObjInfo oiPtr, EditData* edPtr)
{
	return CreateACEMenus(expressionMenus, 27000);
}


int16 FUSION_API nSDK::Exports::GetActionCodeFromMenu(mv* mV, int16 menuId)
{
	return menuId - 25000;
}

int16 FUSION_API nSDK::Exports::GetConditionCodeFromMenu(mv* mV, int16 menuId)
{
	return menuId - 26000;
}

int16 FUSION_API nSDK::Exports::GetExpressionCodeFromMenu(mv* mV, int16 menuId)
{
	return menuId - 27000;
}


void FUSION_API nSDK::Exports::GetActionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
	for (const auto& act : actionDefinitions)
	{
		if (act.code == code && param >= 0 && param < (int16)act.params.size())
		{
			StringCbCopy(strBuf, maxLen * sizeof(tchar), act.params[param].name);
			break;
		}
	}
}

void FUSION_API nSDK::Exports::GetConditionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
	for (const auto& cnd : conditionDefinitions)
	{
		if (cnd.code == code && param >= 0 && param < (int16)cnd.params.size())
		{
			StringCbCopy(strBuf, maxLen * sizeof(tchar), cnd.params[param].name);
			break;
		}
	}
}

void FUSION_API nSDK::Exports::GetExpressionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
	GetExpressionParam(mV, code, param, strBuf, maxLen);
}


void FUSION_API nSDK::Exports::GetActionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
	for (const auto& act : actionDefinitions)
	{
		if (act.code == code)
		{
			StringCbCopy(strPtr, maxLen * sizeof(tchar), act.name);
			break;
		}
	}
}

void FUSION_API nSDK::Exports::GetConditionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
	for (const auto& cnd : conditionDefinitions)
	{
		if (cnd.code == code)
		{
			StringCbCopy(strPtr, maxLen * sizeof(tchar), cnd.name);
			break;
		}
	}
}

void FUSION_API nSDK::Exports::GetExpressionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen)
{
	for (const auto& exp : expressionDefinitions)
	{
		if (exp.code == code)
		{
			StringCbCopy(strPtr, maxLen * sizeof(tchar), exp.name);
			break;
		}
	}
}

void FUSION_API nSDK::Exports::GetExpressionParam(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen)
{
	for (const auto& exp : expressionDefinitions)
	{
		if (exp.code == code && param >= 0 && param < (int16)exp.params.size())
		{
			StringCbCopy(strBuf, maxLen * sizeof(tchar), exp.params[param].name);
			break;
		}
	}
}


infosEventsV2* FUSION_API nSDK::Exports::GetActionInfos(mv* mV, int16 code)
{
	auto ie = (infosEventsV2*)actionInfosEvents;
	if (!ie)
		return nullptr;

	while (ie->code != code)
		ie = (infosEventsV2*)(((byte*)ie + sizeof(*ie)) + (ie->nParams * sizeof(int16) * 2));
	return ie;
}

infosEventsV2* FUSION_API nSDK::Exports::GetConditionInfos(mv* mV, int16 code)
{
	auto ie = (infosEventsV2*)conditionInfosEvents;
	if (!ie)
		return nullptr;

	while (ie->code != code)
		ie = (infosEventsV2*)(((byte*)ie + sizeof(*ie)) + (ie->nParams * sizeof(int16) * 2));
	return ie;
}

infosEventsV2* FUSION_API nSDK::Exports::GetExpressionInfos(mv* mV, int16 code)
{
	auto ie = (infosEventsV2*)expressionInfosEvents;
	if (!ie)
		return nullptr;

	while (ie->code != code)
		ie = (infosEventsV2*)(((byte*)ie + sizeof(*ie)) + (ie->nParams * sizeof(int16) * 2));
	return ie;
}

#endif