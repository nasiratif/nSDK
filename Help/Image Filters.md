# Image Filters

This file documents MMF2/CF2.5 image filters. For additional information, see the README in the Filters/Images folder.

Please note that the image filter template is not currently usable at the moment, it is only a rough draft in it's current state.

## Usage
Image filters are placed in *(Fusion root)\Filters\Images*. They don't have a distinct Runtime variant.

Fusion loads all image filters before it even shows the splash screen. This may be important to note as you may not have enough time to attach a debugger to Fusion before the filter loads.

## Exported Functions

### CreateFilter
`CFilterImpl* FUSION_API Filter::API::Create(dword dwFlags);`

Called when an image filter instance needs to be made.
Return a new instance of your image filter that inherits from `CFilterImpl`.

The `dwFlags` parameter contains information about the copy of Fusion. `dwFlags & 0xF` is the product edition (one of the `PRODUCT_VERSION_XXX` values), and `dwFlags & 0x0100` is a bit-flag which is set if the free edition was used, otherwise cleared.

### GetFilterName
*This is exported as `GetFilterNameW` if compiling for Unicode*

`const tchar* FUSION_API Filter::API::GetFilterName();`

Called at Fusion startup to retrieve the name of your image filter (what is shown in the *Image Filters* tab).

### GetFilterID
`dword FUSION_API Filter::API::GetFilterID();`

Called at Fusion startup to retrieve the identifier of your image filter.

### GetFilterExts
*This is exported as `GetFilterExtsW` if compiling for Unicode*

`const tchar** FUSION_API Filter::API::GetFilterExts();`

Called at Fusion startup to retrieve the file extensions that your filter supports.
This is only used for image file dialogs, the actual detection is to be done in `CanReadFile`, documented below.

The returned pointer must be an array of `const tchar*` strings, with the last element being `NULL` to indicate the end of the array (e.g `{ _T("webp"), _T("webm"), NULL }`).

### GetPriority
`int32 FUSION_API Filter::API::GetPriority();`

Implement this function so Fusion can retrieve the priority of your image filter.
When Fusion plays a sample, it finds a suitable image filter in the order based on their priority value. Lower return values mean your filter is more likely to be looked into first.

This function is optional; if not exported, your filter won't have any explicit priority setting.

The priority values you can return are the following:

- `VERYHIGH` (`0x0000`)
- `HIGH` (`0x1000`)
- `NORMAL` (`0x2000`)
- `LOW` (`0x3000`)
- `VERYLOW` (`0x4000`)

### GetDependencies
*This is exported as `GetDependenciesW` if compiling for Unicode*

`const tchar** FUSION_API Filter::API::GetDependencies();`

Called to retrieve the DLL dependencies of your image filter.
The returned pointer must be an array of `const tchar*` strings, with the last element being `NULL` to indicate the end of the array.

**As of Fusion build R296.9, this function is called but Fusion seems to never actually embed the dependencies, let alone even read from the returned array.**

### CanReadFile
`bool32 FUSION_API Filter::API::CanReadFile(CInputFile* pif);`

Called to determine whether *your* filter is capable of reading the specified file. For example, you could read the header from the input file to check if it matches the image format you support.

If you return `FALSE`, Fusion ignores your filter for that file & looks into other filters instead.