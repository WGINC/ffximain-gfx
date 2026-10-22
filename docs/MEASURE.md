# Measuring what the community has not settled (docs/MEASURE.md)

The owner's rule (22 Oct): where the FFXI community has worked something out, trust it; where it is not documented,
or it conflicts with our own names, measure it. `pipeline/community_scan.py`, `community_structs.py` and
`community_reconcile.py` sort every name into adopted, the same, or to measure (`data/community_*.json`). This page
is the measuring: xikit records the values while you do set things in the game, and the recording decides.

**What you need:** the kit built from this commit (`kit/xikit/build.sh`; xikit.dll now has the `[watch]` recorder,
kit/README.md "Recording fields"), injected as usual. Put one block below into `xikit.ini` (it is re-read every 60
frames, so you can switch blocks while the game runs). Before each step press **Ctrl+Alt+F7** (it writes a
`#MARK n` line), then do the step for about five seconds. Push `xikit_logs\\...\\xikit_watch.csv` (and xikit.log)
to the branch, or put them in `requests/`, and say which block it was.

Values are recorded raw (u32 / u16 / u8): floats are decoded afterwards, so nothing depends on a guessed type.

## Block A: your character (the entity)

The entity is the 684-byte object in the table at 0x10480AF0 (Ashita `entitymap`, Windower `types.entities`); your
own one is the slot whose index is the word at 0x10485F7A (the game stores its new entity there, 0x10094943, and
compares entity+0x74, the index, with it). Recorded: the 8 fields where Ashita and Windower say different things
(data/community_judged.json), +0x9C (Ashita `AnimationSpeed`, Windower unsure, DancingMad "walk speed"), the
position vectors around them, and the 7 fields our code uses that neither documents.

```
[watch]
enabled=1
every=1
w1=me_4 0x10480AF0[@0x10485F7A] 0x4 u32
w2=me_8 0x10480AF0[@0x10485F7A] 0x8 u32
w3=me_C 0x10480AF0[@0x10485F7A] 0xC u32
w4=me_24 0x10480AF0[@0x10485F7A] 0x24 u32
w5=me_28 0x10480AF0[@0x10485F7A] 0x28 u32
w6=me_2C 0x10480AF0[@0x10485F7A] 0x2C u32
w7=me_98 0x10480AF0[@0x10485F7A] 0x98 u32
w8=me_9C 0x10480AF0[@0x10485F7A] 0x9C u32
w9=me_15C 0x10480AF0[@0x10485F7A] 0x15C u32
w10=me_E8 0x10480AF0[@0x10485F7A] 0xE8 ptr
w11=me_11C 0x10480AF0[@0x10485F7A] 0x11C u16
w12=me_204 0x10480AF0[@0x10485F7A] 0x204 u32
w13=me_208 0x10480AF0[@0x10485F7A] 0x208 u32
w14=me_20C 0x10480AF0[@0x10485F7A] 0x20C u32
w15=me_210 0x10480AF0[@0x10485F7A] 0x210 u16
w16=me_2A2 0x10480AF0[@0x10485F7A] 0x2A2 u16
w17=me_F6 0x10480AF0[@0x10485F7A] 0xF6 u8
w18=me_178 0x10480AF0[@0x10485F7A] 0x178 u32
w19=me_17C 0x10480AF0[@0x10485F7A] 0x17C u32
w20=me_1BD 0x10480AF0[@0x10485F7A] 0x1BD u8
w21=me_1CC 0x10480AF0[@0x10485F7A] 0x1CC u32
w22=me_202 0x10480AF0[@0x10485F7A] 0x202 u16
w23=me_2A8 0x10480AF0[@0x10485F7A] 0x2A8 u32
```

Steps (mark each with Ctrl+Alt+F7):
1. stand still; 2. walk (toggle walk); 3. run; 4. stop; 5. `/heal` and stand up; 6. Tab-target something, then clear
it; 7. mount (chocobo or mount) and ride, then dismount; 8. an emote (`/wave`); 9. engage a weak target and
disengage (only if safe); 10. if you have a pet or trust, call it; 11. zone into another area; 12. if you can, cast a
fishing line and reel in.

## Block B: graphics and frame time

The camera object (`g_pCamera`, Windower `graphics.render`: frame-rate divisor at +0x30, aspect +0x2F0, zoom +0x2F8;
our Camera_GetFrameScale reads +0x28), Windower's graphics globals (animation frame rate, aspect option, clipping
planes, footstep effects), and the CDx fields our graphics code uses that the community does not document.

```
[watch]
enabled=1
every=1
w1=cam_28 0x104568FC 0,0x28 u32
w2=cam_30 0x104568FC 0,0x30 u32
w3=cam_2F0 0x104568FC 0,0x2F0 u32
w4=cam_2F8 0x104568FC 0,0x2F8 u32
w5=g_anim_framerate 0x10456C00 - u32
w6=g_aspect_option 0x10456BE8 - u32
w7=g_clip_entity 0x10456AA8 - u32
w8=g_clip_map 0x10456AB8 - u32
w9=g_footsteps 0x10456A70 - u8
w10=cdx_158 0x1045666C 0,0x158 u32
w11=cdx_15C 0x1045666C 0,0x15C u32
w12=cdx_160 0x1045666C 0,0x160 u32
w13=cdx_174 0x1045666C 0,0x174 u32
w14=cdx_814 0x1045666C 0,0x814 u32
w15=cdx_818 0x1045666C 0,0x818 u32
w16=cdx_81C 0x1045666C 0,0x81C u32
w17=cdx_820 0x1045666C 0,0x820 u32
w18=cdx_824 0x1045666C 0,0x824 u32
w19=cdx_830 0x1045666C 0,0x830 u32
w20=cdx_834 0x1045666C 0,0x834 u32
w21=cdx_908 0x1045666C 0,0x908 u8
w22=cdx_909 0x1045666C 0,0x909 u8
w23=cdx_90A 0x1045666C 0,0x90A u16
w24=cdx_90C 0x1045666C 0,0x90C u16
w25=cdx_90E 0x1045666C 0,0x90E u16
w26=cdx_910 0x1045666C 0,0x910 u8
w27=cdx_911 0x1045666C 0,0x911 u8
w28=cdx_912 0x1045666C 0,0x912 u8
w29=cdx_913 0x1045666C 0,0x913 u8
w30=cdx_914 0x1045666C 0,0x914 u8
w31=cdx_915 0x1045666C 0,0x915 u16
w32=cdx_918 0x1045666C 0,0x918 u32
w33=cdx_91C 0x1045666C 0,0x91C u32
w34=cdx_920 0x1045666C 0,0x920 u8
w35=cdx_921 0x1045666C 0,0x921 u16
w36=cdx_924 0x1045666C 0,0x924 u32
w37=cdx_928 0x1045666C 0,0x928 u32
w38=cdx_92C 0x1045666C 0,0x92C u8
w39=cdx_92D 0x1045666C 0,0x92D u8
w40=cdx_92E 0x1045666C 0,0x92E u8
w41=cdx_92F 0x1045666C 0,0x92F u8
w42=cdx_930 0x1045666C 0,0x930 u8
w43=cdx_931 0x1045666C 0,0x931 u16
w44=cdx_934 0x1045666C 0,0x934 u32
w45=cdx_938 0x1045666C 0,0x938 u32
w46=cdx_A00 0x1045666C 0,0xA00 u32
w47=cdx_A04 0x1045666C 0,0xA04 u32
w48=cdx_A4C 0x1045666C 0,0xA4C u32
```

Steps: 1. stand still; 2. zoom the camera in and out (mouse wheel); 3. rotate the camera; 4. open the config menu and
change the frame-rate / graphics options you have (footstep effects, aspect/window size, draw distance), one at a
time, a mark each; 5. open and close the map; 6. switch window focus away and back; 7. zone; 8. a cut scene if one is
at hand.

## What happens with the recordings

`pipeline/measure_read.py` (to be written against the first recording) lines the values up with the marks: a field
that follows the steps the way a name says (a speed that rises when you run, a flag that sets when you mount) settles
that name; one that never changes in any step is reported as such, not named. Each result goes into
data/community_judged.json as measured, with the recording it came from.
