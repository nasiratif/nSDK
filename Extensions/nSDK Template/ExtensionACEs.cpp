#include "Extension.hpp"

namespace nSDK
{
#ifdef EXT_EDITOR
	// MENUS:
	// Each vector item contains:
	// - A/C/E ID (code) (ignored for sub-menus)
	// - Title string
	// - Enabled/disabled flag
	// -----
	std::vector<ACEMenu> actionMenus =
	{
		{ 0, ACE_TEXT("Example action: change background color"), true },
		{ 1, ACE_TEXT("Example action 2 (disabled)"), false },
	};
	std::vector<ACEMenu> conditionMenus =
	{
		{ 0, ACE_TEXT("Are two integers the same?"), true },
		{ 1, ACE_TEXT("Are two strings equal?"), true },
		EXT_MENU_SEPERATOR,
		{
			NULL, ACE_TEXT("More:"), true,
			{
				{ 2, ACE_TEXT("Check string data (custom parameter example)"), true },
				{ 3, ACE_TEXT("Perform a comparison against a number"), true },
			}
		},
	};
	std::vector<ACEMenu> expressionMenus =
	{
		{ 0, ACE_TEXT("Return a random number"), true },
		{ 1, ACE_TEXT("Return a random string"), true },
	};
	// -----
#endif

	// DEFINITIONS:
	// Each vector item contains:
	// - A/C/E ID (code) (should match the corresponding menu ID)
	// - Display string
	// - Condition flags (not used in actions/expressions)
	// - Pointer to A/C/E function
	// - Parameters:
	//		- Data type
	//		- Title string
	// -----
	std::vector<ACE> actionDefinitions =
	{
		{
			0, ACE_TEXT("Change frame background color to %0"), NULL, Actions::ChangeBGColor,
			{
				{ DataType_Color, ACE_TEXT("Select color") }
			}
		},
		{
			1, ACE_TEXT("Example action 2"), NULL, Actions::ExampleAction2
		}
	};
	std::vector<ACE> conditionDefinitions =
	{
		{
			0, ACE_TEXT("%o: Are %0 and %1 the same?"), ConditionFlags_Negatable, Conditions::SameNumbers,
			{
				{ DataType_Number, ACE_TEXT("First number") },
				{ DataType_Number, ACE_TEXT("Second number") },
			}
		},
		{
			1, ACE_TEXT("%o: Are %0 and %1 equal?"), ConditionFlags_Negatable, Conditions::SameStrings,
			{
				{ DataType_String, ACE_TEXT("First string") },
				{ DataType_String, ACE_TEXT("Second string") }
			}
		},
		{
			2, ACE_TEXT("%o: Is %0 %1?"), ConditionFlags_Negatable, Conditions::CheckStringData,
			{
				{ DataType_String, ACE_TEXT("String") },
				{ CustomDataType_Example, ACE_TEXT("String data type") }
			}
		},
		{
			3, ACE_TEXT("%o: %0 %1?"), ConditionFlags_Negatable, Conditions::Comparison,
			{
				{ DataType_Number, ACE_TEXT("Number") },
				{ DataType_ComparisonNumber, ACE_TEXT("Compare against..") },
			}
		},
	};
	// Note that the only acceptable data types for expressions are Numbers and Strings
	std::vector<ACE> expressionDefinitions =
	{
		{
			0, ACE_TEXT("ReturnRandomN("), DataType_Number, Expressions::ReturnRandomN,
			{
				{ DataType_String, ACE_TEXT("First string") },
				{ DataType_Number, ACE_TEXT("First number") }
			},
		},
		{
			1, ACE_TEXT("ReturnRandomS("), DataType_String, Expressions::ReturnRandomS,
			{
				{ DataType_Number, ACE_TEXT("Seed") }
			},
		}
	};
	// -----
}