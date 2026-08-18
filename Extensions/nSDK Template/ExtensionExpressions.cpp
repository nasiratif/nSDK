#include "Extension.hpp"

long FUSION_API Extension::Expressions::ReturnRandomN(RunData* rdPtr, long param)
{
	auto p1 = nSDK::ParamToString(CNC_GetFirstExpressionParameter(rdPtr, param, TYPE_STRING));
	auto p2 = nSDK::ParamToFloat(CNC_GetNextExpressionParameter(rdPtr, param, TYPE_FLOAT));
	auto len = _tcslen(p1);
	return nSDK::ReturnFloat(rdPtr, rand() / 32768.0f * len);
}

long FUSION_API Extension::Expressions::ReturnRandomS(RunData* rdPtr, long param)
{
	auto seed = nSDK::ParamToFloat(CNC_GetFirstExpressionParameter(rdPtr, param, TYPE_FLOAT));

	tchar string[32];

	// Generate random string:
	// -----
	for (int i = 0; i < 32; ++i)
		string[i] = (tchar)((int)(seed + rand() * std::max(seed, 1.0f)) % 128);
	string[31] = '\0';
	// -----

	return nSDK::ReturnString(rdPtr, string);
}
