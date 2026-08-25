# Transitions

This file documents MMF2/CF2.5 transition extensions. For additional information, see the README in the Transitions folder.

## Usage
Like normal extensions, transitions have distinct editor & runtime versions. Editor versions are placed in *(Fusion root)\Transitions* or *(Fusion root)\Transitions\Unicode*, and runtime versions are placed in *(Fusion root)\Data\Runtime* or *(Fusion root)\Data\Runtime\Unicode*, depending on whether your transition extension is Unicode. In the editor, the transition DLLs are only loaded when you open the *Transition set-up* popup window, and unloaded after.


### Adding a transition
Create a .hpp & .cpp file that implements a transition class that derives from `CTransitionImpl`. There is an example transition, `Sample.hpp` and `Sample.cpp`, in the template to demonstrate this.
Afterwards, in `TransitionAPI.cpp`, declare prototype class objects of your transitions and also store them into the `transitionProtos` array, making them accessible to Fusion.

## Transition Functions

### StepDraw
`virtual bool32 CTransitionImpl::StepDraw(cSurface* dest, cSurface* originalImage, cSurface* finalImage, dword dwFlags, RECT** ppRc);`
Called every frame during the transition. This is where you actually draw the transition.
Parameters are as follows:
- `dest` - the on-screen surface where you draw the transition
- `originalImage` - the image of the frame that you just left, or a solid color if *Background* mode is not used
- `finalImage` - the image of the frame that you are transitioning into
- `dwFlags` - flags, can have `TRFLAG_FADEIN`, `TRFLAG_FADEOUT`, or `TRFLAG_TRANSPARENT` bits set

The original documentation states the following regarding `ppRc`:
> The destination surface doesn't change between 2 calls to StepDraw, so StepDraw can (and should) update only a part of this surface.
> 
> If StepDraw always updates the entire surface, it doesn't need to update the pprc parameter. If this parameter is NULL (default), MMF will copy the whole surface to the screen.
> 
> If StepDraw updates only some areas of the surface, it should allocate and fill a list of the updated rectangles, and pass a pointer to this list to MMF through the pprc parameter. The last element of the list must be an empty rectangle (all members set to 0).
> 
> Note: if StepDraw allocates memory buffers, they must be freed in the End function.

## Exported Functions

### GetModuleType
`dword FUSION_API Transition::API::GetModuleType();`
Return a `DWORD` string indicating the module ID of your transition module. This should be `DWORDSTR('C','C','T','R')`, indicating a transition module.

### GetModuleName
`int32 FUSION_API Transition::API::GetModuleName(tchar* buffer, int32 bufferSize);`
Write the name of your transition module in the `tchar` buffers provided. You must return the string length.

### GetModuleID
`dword FUSION_API Transition::API::GetModuleID();`
Return a `DWORD` string indicating the identifier of your transition module.

### CreateTransition
`CTransition* FUSION_API Transition::API::Create(tchar* reserved, dword transID, byte* params, dword paramsSize);`
Called by Fusion when your transition is to begin. You must create a new `CTransition` object using the respective transition's `CTransitionImpl` class implementation.
If `params` is non-NULL, you must call `Initialize` from your transition implementation to utilize the parameters given in the editor.

### GetTransCount
`int32 FUSION_API Transition::API::GetTransCount();`
Return the number of transitions your module has.

### GetTransName
`int32 FUSION_API Transition::API::GetTransName(int32 index, tchar* buffer, int32 bufferSize);`
Return the name of a specific transition in the `tchar` buffers provided. `index` is the index of your transition.

### GetTransID
`dword FUSION_API Transition::API::GetTransID(int32 index);`
Return the identifier, a `DWORD` string, of a specific transition. `index` is the index of your transition.

### GetTransMode
`dword FUSION_API Transition::API::GetTransMode(int32 index);`
Return the transition mode of a specific transition. This correponds to the `TRMODE_XXX` defines.

### IsUnicode
`bool32 FUSION_API Transition::API::IsUnicode();`
Called by Fusion to determine whether your transition module is Unicode. If so, return `TRUE`, from which you must provide Unicode string buffers. Otherwise, return `FALSE` and provide ANSI strings.