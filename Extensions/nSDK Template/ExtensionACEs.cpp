#include "Extension.hpp"

namespace nSDK
{
#ifdef EXT_EDITOR
	// MENUS:
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
				{ 2, ACE_TEXT("Check string data"), true },
			}
		},
	};
	std::vector<ACEMenu> expressionMenus =
	{
		{ 0, ACE_TEXT("Return a random number"), true },
	};
	// -----
#endif

	// DEFINITIONS:
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
				{ DataType_Number, ACE_TEXT("Second number") }
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
		}
	};
	// Note that the only acceptable data types for expressions are Numbers and Strings
	std::vector<ACE> expressionDefinitions =
	{
		{
			0, ACE_TEXT("ReturnRandom("), DataType_Number, Expressions::ReturnRandom,
			{
				{ DataType_String, ACE_TEXT("First string") },
				{ DataType_Number, ACE_TEXT("First number") }
			},
		}
	};
	// -----
}