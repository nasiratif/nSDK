# nSDK
nSDK is a Windows-only extension SDK for Clickteam Fusion 2.5. I created this to learn more about the inner workings of Fusion extensions, as well as solve some personal gripes I had with the official SDK. It is based on [Fusion 2.5 SDK](https://www.clickteam.com/extensions-sdks#tabs-2), but the extensions use a brand new Visual Studio project rewritten from the ground up instead of using the official template provided.

## Premise
The entire goal of this SDK is to make some error-prone things (like defining A/C/Es) easier & safer while also keeping abstractions at a minimum to provide the most flexibility and performance possible. Some of the extension exports are implemented by the SDK itself but is still available to every extension so they can add their own behaviors on top if desired.
