# Sound filters

Officially Clickteam has not provided any public sound filter SDKs (to my knowledge anyway), so sound filters are entirely undocumented. However, the public extension SDK has headers relating to sound filters and, along with investigating DLL exports of the built-in sound filters, I got hints as to how a sound filter SDK may have been implemented. I've tested it and it works..

This is not fleshed out, as I haven't completely deciphered how the sound filter APIs work, but I was able to get something playing properly in the template provided here.

See Filter.hpp/cpp comments in the template for more information.

_**PS: I looked into image filters as well but that requires a `CImageImpl` class implementation which is not defined anywhere in the SDK..**_