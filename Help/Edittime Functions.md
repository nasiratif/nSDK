# Edittime Functions
This file contains brief documentation about some edittime-only functions Fusion calls from your extension. For more details, see the original stock SDK documentation by Clickteam.

### GetObjInfos
`void FUSION_API Extension::API::GetObjInfos(mv* mV, EditData* edPtr, tchar* ObjName, tchar* ObjAuthor, tchar* ObjCopyright, tchar* ObjComment, tchar* ObjHttp)`
Called when Fusion needs to know object metadata. You return the metadata in the `tchar` pointers provided.
The maximum length of the string pointers are `255` characters (including null byte), except for `ObjComment` whose maximum is `1024` (including null byte).

### GetHelpFileName
`const tchar* FUSION_API Extension::API::GetHelpFileName()`
Called when Fusion needs to know the help file of your extension. If you return a pathname relative to Fusion (e.g, `MyExtHelp.chm`), Fusion can automatically load it via it's CHM help file viewer. Otherwise, provide a full path (e.g, `Help\MyExt\MyExtHelp.chm`) to have Fusion simply open that file.

### MakeIconEx
`int32 FUSION_API Extension::API::MakeIconEx(mv* mV, cSurface* pIconSf, tchar* lpName, OI* oiPtr, EditData* edPtr)`
Alternative method of loading the extension icon; you can draw into the surface provided by the `pIconSf` parameter and Fusion will use it as the extension icon instead of what's embedded in resources. The surface is always 32x32.

You can resize the surface, but not that it's beneficial; Fusion will always resize it back to 32x32.

If you want to use this, make sure to remove the `EXO_ICON` & `EXO_IMAGE` resources so Fusion will trigger this function. You must also again draw the icon yourself for the frame editor in `EditorDisplay`!

Side note: here you can also change the name of the extension by writing into `lpName`, but Clickteam discourages that nowadays.

### GetProperties
`bool32 FUSION_API Extension::API::GetProperties(mv* mV, EditData* edPtr, bool32 bMasterItem)`
Called to insert your object properties, using `mvInsertProps`.

### ReleaseProperties
`void FUSION_API Extension::API::ReleaseProperties(mv* mV, EditData* edPtr, bool32 bMasterItem)`
Counterpart of `GetProperties`, this is called right after it. Useful if you allocated memory inside of `GetProperties` and wish to free it now.

### GetPropCreateParam
`LPARAM FUSION_API Extension::API::GetPropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID)`
Called when a property is initialized and its creation parameter is `NULL`.
You could, for example, change the content of a combo-box property according to specific settings in the `EditData` structure.

### ReleasePropCreateParam
`void FUSION_API Extension::API::ReleasePropCreateParam(mv* mV, EditData* edPtr, uint32 nPropID, LPARAM lParam)`
Counterpart of `GetPropCreateParam`, this is called right after it. Useful if you allocated memory inside of `GetPropCreateParam` and wish to free it now.

### SetPropValue
`void FUSION_API Extension::API::SetPropValue(mv* mV, EditData* edPtr, uint32 nPropID, CPropValue* lParam)`
Called when a property is changed.
Here, you modify your `EditData` to set the new property according to `lParam`.

### GetPropValue
`CPropValue* FUSION_API Extension::API::GetPropValue(mv* mV, EditData* edPtr, uint32 nPropID)`
Called when a property needs to be retrieved.
Here, you retrieve a property from `EditData` and return the corresponding `CPropValue` for it.

### SetPropCheck
`void FUSION_API Extension::API::SetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID, bool32 nCheck)`
Ditto as `SetPropValue`, but for checkboxes.

### GetPropCheck
`bool32 FUSION_API Extension::API::GetPropCheck(mv* mV, EditData* edPtr, uint32 nPropID)`
Ditto as `GetPropValue`, but for checkboxes.

### EditProp
`bool32 FUSION_API Extension::API::EditProp(mv* mV, EditData* edPtr, uint32 nPropID)`
Called when a `Button/EditButton` property is clicked.

### IsPropEnabled
`bool32 FUSION_API Extension::API::IsPropEnabled(mv* mV, EditData* edPtr, uint32 nPropID)`
Called to determine if a property should be enabled.

### InitParameter
`void FUSION_API Extension::API::InitParameter(mv* mV, int16 code, paramExt* pExt)`
Used for custom Action/Condition parameters.
Called when the parameter needs to be initialized.

### EditParameter
`void FUSION_API Extension::API::EditParameter(mv* mV, int16 code, paramExt* pExt)`
Called for custom Action/Condition parameters when the parameter needs to be edited.
This is where you'd display a dialog box for the user to specify the parameter.

### GetParameterString
`void FUSION_API Extension::API::GetParameterString(mv* mV, int16 code, paramExt* pExt, tchar* pDest, int16 size)`
Called to get the display string of your custom parameter in the action/condition.

### CreateObject
`int32 FUSION_API Extension::API::CreateObject(mv* mV, LO* loPtr, EditData* edPtr)`
Called when a new object instance of your extension is created.
You must initialize your `EditData` here.

### RemoveObject
`void FUSION_API Extension::API::RemoveObject(mv* mV, LO* loPtr, EditData* edPtr, uint16 cpt)`
Called when an object instance of your extension is removed.

### DuplicateObject
`void FUSION_API Extension::API::DuplicateObject(mv* mV, OI* oiPtr, EditData* edPtr)`
Called when an object instance of your extension is cloned (not duplicated, despite the name!).

### PutObject
`void FUSION_API Extension::API::PutObject(mv* mV, LO* loPtr, EditData* edPtr, uint16 cpt)`
Called when a new object instance of your extension is placed onto a frame.

### EditObject
`bool32 FUSION_API Extension::API::EditObject(mv* mV, OI* oiPtr, LO* loPtr, EditData* edPtr)`
Called when your extension object needs to be edited (i.e, is double-clicked in the frame). Some extensions like to display a dialog here.
Return `TRUE` if editing succeeded, otherwise `FALSE`.

### EditorDisplay
`void FUSION_API Extension::API::EditorDisplay(mv* mV, OI* oiPtr, LO* loPtr, EditData* edPtr, RECT* rc)`
Implement this function to do custom drawing in the frame editor. `rc` contains the *absolute* position of the object.
You must blit to the frame surface, which can be retrieved via calling `WinGetSurface((int32)mV->mvIdEditWin)`.

### SetEditSize
`bool32 FUSION_API Extension::API::SetEditSize(mv* mV, EditData* edPtr, int32 cx, int32 cy)`
Required for resizable extensions; if exported, Fusion knows your extension capable of resizing. You must implement `GetObjectRect` in response.

### GetObjectRect
`void FUSION_API Extension::API::GetObjectRect(mv* mV, RECT* rc, LO* loPtr, EditData* edPtr)`
You must modify the `rc` structure returning the rectangle of your object.

Example:
```
rc->right = rc->left + 32; edPtr->swidth
rc->bottom = rc->top + 32; edPtr->sheight
```
### IsTransparent
`bool32 FUSION_API Extension::API::IsTransparent(mv* mV, LO* loPtr, EditData* edPtr, int32 dx, int32 dy);`
Indicates whether the mouse cursor is over a transparent zone of the object.

### PrepareToWriteObject
`void FUSION_API Extension::API::PrepareToWriteObject(mv* mV, EditData* edPtr, OI* adoi)`
Called just before `EditData` is written to the MFA.

### UsesFile
`bool32 FUSION_API Extension::API::UsesFile(mv* mV, tchar* fileName)`
Called when a file is dropped onto the frame.
You should return `TRUE` if you can create a new extension object from the given file.

### CreateFromFile
`void FUSION_API Extension::API::CreateFromFile(mv* mV, tchar* fileName, EditData* edPtr)`
Creates a new extension object from file.

### GetFilters
`bool32 FUSION_API Extension::API::GetFilters(mv* mV, EditData* edPtr, dword dwFlags, void* pReserved)`
Called when Fusion needs to know if your extension needs to support image/sound filters.
If you need image filter support, return `TRUE` if `dwFlags` has the `GETFILTERS_IMAGES` bit set.
If you need sound filter support, return `TRUE` if `dwFlags` has the `GETFILTERS_SOUNDS` bit set.
Otherwise, return `FALSE`.


## Functions for `OEFLAG_TEXT`

### GetTextCaps
`dword FUSION_API Extension::API::GetTextCaps(mv* mV, EditData* edPtr);`
Called to determine text capabilities of the object under the frame editor.

### SetTextFont
`bool32 FUSION_API Extension::API::SetTextFont(mv* mV, EditData* edPtr, LOGFONT* plf, const tchar* pStyle);`
Called to change the font used by the object. (`pStyle` & `cbSize` are obselete).

### GetTextFont
`bool32 FUSION_API Extension::API::GetTextFont(mv* mV, EditData* edPtr, LOGFONT* plf, tchar* pStyle, uint32 cbSize);`
Called to retrieve the font used by the object. (`pStyle` & `cbSize` are obselete).

### SetTextClr
`void FUSION_API Extension::API::SetTextClr(mv* mV, EditData* edPtr, COLORREF color);`
Called to set the text color of the object.

### GetTextClr
`COLORREF FUSION_API Extension::API::GetTextClr(mv* mV, EditData* edPtr);`
Called to get the text color of the object.

### SetTextAlignment
`void FUSION_API Extension::API::SetTextAlignment(mv* mV, EditData* edPtr, dword dwAlignFlags);`
Called to set the text alignment of the object.
`dwAlignFlags` would have one of the `TEXT_ALIGN_` bits set.

### GetTextAlignment
`dword FUSION_API Extension::API::GetTextAlignment(mv* mV, EditData* edPtr);`
Called to get the text alignment of the object


## A/C/E Functions

### GetActionMenu
`HMENU FUSION_API Extension::API::GetActionMenu(mv* mV, OI* oiPtr, EditData* edPtr);`
Returns the menu to be displayed when choosing an action from this object.
`edPtr` is also available here, you can modify `actionMenus` to change the content of the menu depending on the properties.

### GetConditionMenu
`HMENU FUSION_API Extension::API::GetConditionMenu(mv* mV, OI* oiPtr, EditData* edPtr);`
Returns the menu to be displayed when choosing an condition from this object.
`edPtr` is also available here, you can modify `conditionMenus` to change the content of the menu depending on the properties.

### GetExpressionMenu
`HMENU FUSION_API Extension::API::GetExpressionMenu(mv* mV, OI* oiPtr, EditData* edPtr);`
Returns the menu to be displayed when choosing an expression from this object.
`edPtr` is also available here, you can modify `expressionMenus` to change the content of the menu depending on the properties.


### GetActionCodeFromMenu
`int16 FUSION_API Extension::API::GetActionCodeFromMenu(mv* mV, int16 menuId);`
Returns the action ID from a menu option.
This is how Fusion knows which action ID to choose given the menu option selected.

### GetConditionCodeFromMenu
`int16 FUSION_API Extension::API::GetConditionCodeFromMenu(mv* mV, int16 menuId);`
Returns the condition ID from a menu option.
This is how Fusion knows which condition ID to choose given the menu option selected.

### GetExpressionCodeFromMenu
`int16 FUSION_API Extension::API::GetExpressionCodeFromMenu(mv* mV, int16 menuId);`
Returns the expression ID from a menu option.
This is how Fusion knows which expression ID to choose given the menu option selected.


### GetActionTitle
`void FUSION_API Extension::API::GetActionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);`
Outputs the string on the titlebar to be displayed for a specific action parameter.

### GetConditionTitle
`void FUSION_API Extension::API::GetConditionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);`
Outputs the string on the titlebar to be displayed for a specific condition parameter.

### GetExpressionTitle
`void FUSION_API Extension::API::GetExpressionTitle(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);`
The use of this function is not entirely clear..


### GetActionString
`void FUSION_API Extension::API::GetActionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);`
Outputs the string to be displayed for a specific action (e.g, `"Set width to %0"`).

### GetConditionString
`void FUSION_API Extension::API::GetConditionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);`
Outputs the string to be displayed for a specific condition (e.g, `"%o: Is the object resizable?"`).

### GetExpressionString
`void FUSION_API Extension::API::GetExpressionString(mv* mV, int16 code, tchar* strPtr, int16 maxLen);`
Outputs the name of an expression (e.g, `"GetObjectWidth("`).

### GetExpressionParam
`void FUSION_API Extension::API::GetExpressionParam(mv* mV, int16 code, int16 param, tchar* strBuf, int16 maxLen);`
Outputs the name of an expression parameter.


### GetActionInfos
`infosEventsV2* FUSION_API Extension::API::GetActionInfos(mv* mV, int16 code);`
Called to retrieve internal information about a specific action (e.g, number of parameters, parameter info etc).

### GetConditionInfos
`infosEventsV2* FUSION_API Extension::API::GetConditionInfos(mv* mV, int16 code);`
Called to retrieve internal information about a specific condition (e.g, flags, number of parameters, parameter info etc).

### GetExpressionInfos
`infosEventsV2* FUSION_API Extension::API::GetExpressionInfos(mv* mV, int16 code);`
Called to retrieve internal information about a specific expression (e.g, return type, number of parameters, parameter info etc).

## Exporter-Specific Functions

### PrepareAndroidBuild
*This function was found from [DarkEdif](https://github.com/SortaCore/MMF2Exts)*
`void FUSION_API Extension::API::PrepareAndroidBuild(mv* mV, EditData* edPtr, const wchar* androidBuildPath);`
If exported, this is called during Android project building, even if your extension doesn't have an Android runtime version.
You get the build directory of the Android project in `androidBuildPath`, so your extension could modify the build files accordingly.

Also note that `androidBuildPath` is always Unicode even if the extension isn't.


### PrepareHtml5Build
*WARNING: this is an undocumented function!*
`bool32 FUSION_API Extension::API::PrepareHtml5Build(EditData* edPtr, mv* mV, int32 count, int32 buildType, dword flags, const wchar* indexHTMLPath, const wchar* sourceFolderPath, const wchar* mediaFolderPath, const wchar* runtimeHTML5Path, const wchar* sourceFolderName, const wchar* mediaFolderName, const wchar* html5ProjectName, const wchar* jsCompileScriptPath, const wchar* tempSrcFolderPath, int32 appWidth, int32 appHeight);`
If exported, this is called during HTML5 Development/Final Project building, even if your extension doesn't have an HTML5 runtime version.
You're able to retrieve some metadata about the HTML5 build, like various directories of the HTML5 build, and some temp files/folders, so your extension could modify the build files accordingly.
From testing, `buildType` is always set to `28`, which seemingly corresponds to `BUILDTYPE_HTML5FINAL` defined in `Cncy.h`, even if the build type is Development.
Note `indexHTMLPath` is an empty string if the *Generate HTML file* property is unchecked.
`jsCompileScriptPath` & `tempSrcFolderPath` is only valid on HTML5 Final Project, otherwise they're `NULL`.
This function is not officially documented, however I was able to retrieve it's signature thanks to a Clickteam staff member.

Also note that the strings are always Unicode even if the extension isn't.

Return `TRUE` if success, `FALSE` otherwise.