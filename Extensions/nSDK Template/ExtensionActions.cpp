#include "Extension.hpp"

int16 FUSION_API Actions::ChangeBGColor(RunData* rdPtr, long param1, long param2)
{
	// You probably have to do more than this on Standard display mode (see WinAddZone)
	rdPtr->rHo.hoAdRunHeader->rhFrame->m_hdr.leBackground = (COLORREF)param1;
	return 0;
}

int16 FUSION_API Actions::ExampleAction2(RunData* rdPtr, long param1, long param2)
{
	return 0;
}