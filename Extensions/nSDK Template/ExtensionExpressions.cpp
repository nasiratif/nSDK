#include "Extension.hpp"

long FUSION_API Expressions::ReturnRandom(RunData* rdPtr, long param)
{
	auto p1 = nSDK::ParamToString(CNC_GetFirstExpressionParameter(rdPtr, param, TYPE_STRING));
	auto p2 = nSDK::ParamToFloat(CNC_GetNextExpressionParameter(rdPtr, param, TYPE_FLOAT));
	auto len = _tcslen(p1);

	rdPtr->rHo.hoFlags |= HOF_FLOAT; // tell Fusion we're returning a float
	return nSDK::ReturnFloat(rand() / 32768.0f * len);
}