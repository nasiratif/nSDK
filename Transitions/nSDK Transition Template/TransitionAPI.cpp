#include "TransitionModule.hpp"

// Include your transitions here:
// -----
#include "Sample.hpp"
// -----

namespace Transition
{
	// Transition prototypes:
	// -----
	CSample proto_Sample;
	// -----

	// Transition prototypes array:
	CTransitionImpl* transitionProtos[] =
	{
		&proto_Sample
	};
}

/*

---------- TRANSITIONS API ----------

*/

CTransition* FUSION_API Transition::API::Create(tchar* reserved, dword transID, byte* params, dword paramsSize)
{
#pragma EXT_EXPORT_CREATETRANSITION
	for (const auto proto : transitionProtos)
	{
		if (proto->GetID() == transID)
		{
			auto inst = proto->MakeInstance();
			if (params)
				inst->Initialize(params, paramsSize);

			return new CTransition(inst);
		}
	}
	return nullptr;
}


dword FUSION_API Transition::API::GetModuleType()
{
#pragma EXT_EXPORT_GETMODULETYPE
	return MODULEID_TRANSITIONS;
}

int32 FUSION_API Transition::API::GetModuleName(tchar* buffer, int32 bufferSize)
{
#pragma EXT_EXPORT_GETMODULENAME
	StringCbCopy(buffer, bufferSize, TRANS_MODULE_NAME);
	return 0;
}

dword FUSION_API Transition::API::GetModuleID()
{
#pragma EXT_EXPORT_GETMODULEID
	return EXT_FIX_IDENTIFIER(TRANS_MODULE_IDENTIFIER);
}

int32 FUSION_API Transition::API::GetTransCount()
{
#pragma EXT_EXPORT_GETTRANSCOUNT
	return ARRAYSIZE(transitionProtos);
}

int32 FUSION_API Transition::API::GetTransName(int32 index, tchar* buffer, int32 bufferSize)
{
#pragma EXT_EXPORT_GETTRANSNAME
	assert(index >= 0 && index < ARRAYSIZE(transitionProtos));
	return transitionProtos[index]->GetName(buffer, bufferSize);
}

dword FUSION_API Transition::API::GetTransID(int32 index)
{
#pragma EXT_EXPORT_GETTRANSID
	assert(index >= 0 && index < ARRAYSIZE(transitionProtos));
	return transitionProtos[index]->GetID();
}

dword FUSION_API Transition::API::GetTransMode(int32 index)
{
#pragma EXT_EXPORT_GETTRANSMODE
	assert(index >= 0 && index < ARRAYSIZE(transitionProtos));
	return transitionProtos[index]->GetMode();
}


bool32 FUSION_API Transition::API::IsUnicode()
{
#pragma EXT_EXPORT_ISUNICODE
#ifdef _UNICODE
	return TRUE;
#else
	return FALSE;
#endif
}