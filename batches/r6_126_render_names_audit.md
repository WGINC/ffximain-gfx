# #126: the low-confidence names on the render path, re-audited (28 Oct)

The render path: the #118 and #119 sets (verification/render_core_status.json, 157 functions). data/names holds 18
names for them; 7 are `low`. Each one's evidence was checked against the game's code (capstone over the function's
extent in the DLL: the calls, their arguments, the device slots by d3d8.h's vtable order).

| Address | Name | Evidence checked | Outcome |
|---|---|---|---|
| 0x10009EA0 | CDx_PushRenderTargetTexture_Flag1 | calls 0x10009F20 with arguments 6 and 7 = 1, 0; 4 and 5 point at one zeroed local; ECX passed on | holds; low (the name rests on 0x10009F20's, whose confidence data/names does not record) |
| 0x10009EE0 | CDx_PushRenderTargetTexture_Flag0 | the same with 0, 0 | holds; low (as above) |
| 0x1000A390 | CDx_EndAndClearBegin | calls 0x100096B0 with 0, 0 first and its four arguments after (ret 0x10) | holds; low (rests on 0x100096B0's name) |
| 0x10033710 | Unit100310A0_Construct | vtable 0x1032A844 stored; 0x1000A6E0 called once; 0x10026E00 twice | holds; low (a constructor of an unnamed class) |
| 0x10076030 | CMoTask_DrawQuadStates | 9 SetRenderState, **16** (not 15) SetTextureStageState, 0x1000CD00 twice; its frame is 0x64 bytes, not 0x5D4 (the decompiler's frame block) | evidence corrected in data/names/020_unit_10074CA0.json; low |
| 0x10076520 | CMoTask_DrawQuadDynVB | 10 SetTextureStageState; the vertex-batch calls 0x10044010, 0x10044130, 0x100441A0, 0x10043F40 | holds; low (the class prefix is not established) |
| 0x1003E910 | YmElem_DrawD3ASegment | one SetRenderState (0xE); 0x1003E9A0, 0x10043F40, 0x10075960 | holds; low (as above) |

No name is raised: each borrows a class or callee name that has no recorded confidence of its own.
