# Transitions

Clickteam once had a Transitions SDK that was available for MMF1. It's no longer available at Clickteam's website, but exists on the Wayback Machine.
It's outdated however; the `CTransition` class definition has changed in newer builds. Via official means, I was able to obtain the up-to-date source code of `cctrans.dll` which is what the template project is based on. It has new functions, namely `StepDrawBlit` (needed for object transitions), `Get/SetTimePos` as well as `Supports` (to determine whether `StepDrawBlit` is supported or not).

See Transitions.md in the Help folder for more details.