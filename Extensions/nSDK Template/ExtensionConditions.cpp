#include "Extension.hpp"

long FUSION_API Conditions::SameNumbers(RunData* rdPtr, long param1, long param2)
{
	auto p1 = nSDK::ParamToFloat(CNC_GetFloatParameter(rdPtr));
	auto p2 = nSDK::ParamToFloat(CNC_GetFloatParameter(rdPtr));
	return p1 == p2;
}

long FUSION_API Conditions::SameStrings(RunData* rdPtr, long param1, long param2)
{
	auto str1 = nSDK::ParamToString(param1);
	auto str2 = nSDK::ParamToString(param2);
	return _tcscmp(str1, str2) == 0;
}


long FUSION_API Conditions::CheckStringData(RunData* rdPtr, long param1, long param2)
{
	auto str = nSDK::ParamToString(param1);
	auto param = nSDK::ParamToCustom(param2);

	INT ret = 0;
	IsTextUnicode(str, (_tcslen(str) + 1) * sizeof(tchar), &ret);
	switch (param->pextData[0])
	{
	case TRUE: // ASCII16
		return ret == IS_TEXT_UNICODE_ASCII16;
	default:
		return ret == IS_TEXT_UNICODE_STATISTICS;
	}
}
