# Using PNG Icons

nSDK, by default, uses .bmp icons, as is standard in the official Fusion 2.5/MMF2 SDK. However, nSDK allows you to use .png instead. This has the advantage of allowing the use of icons with alpha channel.

To use .png icons, you must modify your extension's Visual Studio project. Follow the steps below:

- add `EXT_USE_PNG_ICON` as a preprecessor definition in the Visual Studio project properties (located in *C/C++ -> Preprocessor*), make sure it's applied to all configurations
- in Visual Studio, delete the `EXO_ICON` & `EXO_IMAGE` resources from `Ext.rc`
- in the project directory, replace the `Ext_Icon.png` file with your own icon (it's dimensions ***MUST*** be exactly 32x32)
- in Visual Studio, open `Ext.rc` and import `Ext_Icon.png` as a resource to it, rename it's *ID* to `IDB_EXO_ICON`, and then set it's *Condition* to `EXT_EDITOR` so it's not included in Runtime MFXs

Afterwards, the `Ext_Icon.bmp` & `Ext_Image.bmp` files can safely be deleted from the project.