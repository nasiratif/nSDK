# Transitions

Clickteam once had a Transitions SDK that was available for MMF1. It's no longer available at Clickteam's website, but exists on the Wayback Machine.
Apparently that SDK is still *somewhat* compatible with Fusion 2.5, but the `CTransition` class definition seems to have changed in newer builds, Fusion seems to expect a different vtable, seemingly the member functions in MMF1's SDK are in a contrary order compared to what modern Fusion expects, this led to incorrect functions being called from `CTransition` in my testing. I've done some analysis and fixed it up.

A template project is provided, which also contains a demo transition. 

Curiously a new exported function, `IsUnicode`, is present in modern `cctrans.dll`, but missing in the original SDK. I have this implemented in the template.