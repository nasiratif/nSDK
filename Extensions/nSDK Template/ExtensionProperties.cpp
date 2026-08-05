#include "Extension.hpp"

namespace nSDK
{
#ifdef EXT_EDITOR
	// The last parameter passed into each property depends on what type of property it is. For example, a combo box uses that to specify an array of the combo box items
	// See the stock SDK docs for more details
	namespace PropCreationData
	{
		const tchar* combo[] =
		{
			NULL, // Fusion reserved
			_T("First"),
			_T("Second"),
			_T("Last"),
			NULL // Must end with NULL
		};
	}
	using namespace PropCreationData;

	// Make sure to also define your property IDs in Extension.hpp!
	// You'll also need to implement Set/GetPropValue/Check for every property
	// Don't forget UpdateEditStructure as well if you change properties across versions. And if you're migrating from ANSI -> Unicode, see mvIsUnicodeApp/mvGetAppCodePage in stock SDK docs
	// Example properties are used here.
	// Each vector item contains:
	// - Property tab (PropTab_XXX)
	// - "Append to end of properties" flag, if false, properties are appended at the beginning of the tab rather than the end
	// - Properties:
	//		- Property ID (user-defined)
	//		- Property name
	//		- Property description
	//		- Property type (PropType_XXX)
	//		- Property options (depends on the type, see stock SDK docs)
	//		- Creation parameter (depends on the type, see stock SDK docs)
	std::vector<Properties> properties =
	{
		{
			PropTab_General, true,
			{
				{ PropID_Start, _T("Sample Properties"), _T("Sample properties"), PropType_Group },

				{ PropID_Text, _T("Sample text"), _T("Some sample text."), PropType_EditString },
				{ PropID_Color, _T("Sample color"), _T("Sample color."), PropType_Color },
				{ PropID_Combo, _T("Sample combo"), _T("Sample combo box."), PropType_ComboBox, PropOptions_None, combo },
				{ PropID_Check, _T("Sample check box"), _T("Sample check box."), PropType_Checkbox },

				{ PropID_Start, _T("Advanced Properties"), _T("Less common object properties."), PropType_Folder },

				// Image properties are special in that EnumElts (ExtensionGeneral.cpp) needs to be implemented
				// See EnumElts for more details
				{ PropID_Images, _T("Sample image list"), _T("Image list example.\nCan store a maximum of 4 images."), PropType_ImageList },

				{ PropID_Start, _T(""), _T(""), PropType_FolderEnd },
			}
		},
		// Here's an example of adding properties to different tabs!
		{
			PropTab_About, false,
			{
				{ PropID_Start, _T("nSDK Info"), _T("nSDK Information"), PropType_Group },
				{ PropID_Copyright, _T("nSDK Copyright"), _T("nSDK Copyright information"), PropType_StaticString },
				{ PropID_Start, _T("General Info"), _T(""), PropType_Group }
			}
		}
	};
#endif
}