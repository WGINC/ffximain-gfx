# Re-evaluation, 17 Oct: everything done, all the data, and where it can go

The owner asked for this from scratch, with nothing assumed. Every statement below names its source, which is
usually a section of docs/HISTORY.md or a log. Anything not established is labelled **not known**. Where an
earlier statement, in the docs or in conversation, does not survive the re-reading, it is marked **corrected**.

## 1. The goals

These are the owner's goals, stated on 6 Oct (ROADMAP.md):
1. a performance patch;
2. a renderer port to Direct3D 11;
3. a UI overhaul.

Later the owner narrowed the first goal (in conversation): "how are we going to get to 60 or higher?".

No frame-time target was ever set as a number. 60 fps means 16.7 ms a frame.

## 2. The machine every measurement was taken on

The sources are the kit's environment log and the owner's own statements (HISTORY 6 Oct, 13 Oct):
- An M5 MacBook Pro running Windows 11 on ARM64 in Parallels (kernel 10.0 build 26200).
- pol.exe is a 32-bit x86 process, so it runs under x86 emulation. The CPUID vendor is "Virtual CPU", with SSE to
  SSE4.2 and no AVX.
- The GPU is the Parallels Display Adapter (prl_umdd10.dll), with 4,092 MB of shared memory and no dedicated
  memory.
- Windows' own d3d8.dll crashes the game. The working path is dgVoodoo 4.8.2.134 with D3D11 output. Ours
  (kit/d3d8on11) has also run in the game since 15–16 Oct.

**Every time in this repository was taken on this one machine.** None of it has been shown to hold on any other
machine (HISTORY 14 Oct, correction 4).

The friend's "hundreds of fps under Vulkan on Linux" comes from a different machine. It runs the game natively as
x86 on a different GPU, through a different D3D8 layer. **Not known:** that machine's hardware, which D3D8 layer it
uses, how its fps is counted, and how its clock is "tied to the fps". Nothing here can be compared with it.

## 3. All the frame-time data (the Port Jeuno plaza, called "Bedau" in the records, unless stated)

| Date, run | Device | Speedfix | Frame | Source |
|---|---|---|---|---|
| 6 Oct, 10016 | dgVoodoo | none | 19.2 ms (scene not recorded; 18–46 ms over the session) | HISTORY 527 |
| 6 Oct, 3736 | dgVoodoo | defaults | 18–21 ms up to frame 6000; 64–79 ms from about frame 6600 (the plaza) | HISTORY 611 |
| 6 Oct, 6156 | dgVoodoo | defaults; A = off | A 70–72 ms, B 66–68 ms | HISTORY 668 |
| 6 Oct, 2020 (session 3) | dgVoodoo | none | 72.7 ms | HISTORY 750 |
| 6 Oct, 12764 (session 1) | dgVoodoo | 4 levers, A = everything off | **A 70.6–74.1 ms, B 33.2–40.2 ms (−50%)** | HISTORY 750 |
| 6 Oct, 12164 (session 4) | dgVoodoo | every lever | first scene (819 draws, place not recorded): p50 17.5–19.4 ms; then 3,450–3,640 draws: 31.5–35 ms | HISTORY 750 |
| 6 Oct, 2264 (session 5) | dgVoodoo | every lever; per-lever A/B | 29.5–45.7 ms by phase | HISTORY 859 |
| 13 Oct, 8948 | dgVoodoo | every lever; A/B of 3 levers | 22.7–31.2 ms by phase | HISTORY 1430 |
| 16 Oct, 2420 (perf60 run 1) | dgVoodoo | every lever | 34.1–34.4 ms | HISTORY 2094 |
| 16 Oct, 3128 (perf60 run 2) | ours, `thread` | every lever | 34.5 ms (filter off), 29.1 ms (filter on) | HISTORY 2094 |

What this table establishes:
- **Corrected:** "frames are flat at 30–35 ms whatever we do" is wrong. Without speedfix the plaza ran at
  about 70 ms. The levers halved it (session 1). The two levers that carried most of that were measured singly:
  `sse_skinning` −14.5 ms and `batch_up_draws`+`lazy_state` −8.2 ms (session 5).
- Since then, with every lever on, the plaza has run at 23–35 ms a frame, depending on the run. That is 29 to 43 fps.
- The lightest scene measured with every lever on (session 4, 819 draws) ran at 17.5–19.4 ms (p50). **No scene in
  any run has been measured at or below 16.7 ms.**
- The crowd is not controlled. Runs on different days differ by up to 10 ms in the same spot, so only A/Bs within
  one run are comparisons.

## 4. Every lever A/B, as measured in the game

| Lever | B − A | Spread within a label | Verdict | Source |
|---|---|---|---|---|
| everything (session 1) | −36.0 ms | ≤ 7.0 | gain | HISTORY 750 |
| sse_skinning | −14.51 | 5.96 | gain | session 5 |
| batch_up_draws + lazy_state | −8.16 | 3.37 | gain | session 5 |
| exec_filter (ours) | −5.46 | 4.24 | gain; every on phase faster than every off phase | perf60 run 2 |
| extended_state_cache | −2.53 | 4.25 | within the spread | 13 Oct |
| skip_all_occlusion_readbacks | −1.31 | 3.98 | **not a test** (below) | session 5 |
| skip_actor_occlusion | +0.33 | 4.37 | within the spread | session 5 |
| merge_scenes | +1.17 | 3.48 | within the spread | session 5 |
| skin_merge | +1.36 | 5.29 | within the spread | 13 Oct |
| block_cache | +1.43 | 3.94 | within the spread | 13 Oct |
| precise_pacing | +0.27, +0.88 | 3.5 | no effect: the limiter never waited | perf60 |
| up_ring | +5.3 (one phase each), with corrupted frames | | reverted | run 7388 |
| skip_shadow_pass | corrupted frames | | reverted | run 7388 |

**Corrected: the readback A/B did not test the readback.** In session 5 the log counts every call to the
readback through `sf_readback`, skipped or run, in both arms (sf_main.cpp:236; the hook is in place when the switch
is on at the start). It counted
158 in the first minute and none after that while standing. So that A/B compared two arms with no readbacks in
either. I said in conversation that removing the readback "does not remove the time, the wait moves elsewhere".
That has no source and is withdrawn.

On 16 Oct the samples show the readback's LockRect under 25% of dgVoodoo's render-thread samples and 66.5% of
ours. That was standing in the same plaza, on the new game build. How many readbacks ran a frame that day is not
known: the counter exists only when the switch is on at the start, and it was off. Why the readbacks ran on
16 Oct and not in session 5 is **not known**. The game build, the crowd and `occlusion_stagger=8` all differed.

**Not measured, then:** what the frame is with the readbacks removed while they are actually being requested.

## 5. Where the frame goes now (every lever on, the plaza)

From samples. These are shares of time, not causes.

**dgVoodoo** (perf60 run 1): time in other modules, by the game call under it:

| Game call | Share |
|---|---|
| Readback LockRect | 25.0% |
| SkinAndDrawV2's draws | 18.6% |
| EndScene | 16.6% |
| SetStreamSource | 3.5% |
| Cell quads | 2.9% |

**Ours** (perf60 run 2, from the executor's own stats):
- The executor is busy 92–97% of the time. The game thread waits 15–24 ms a frame "for answers": the synchronous
  calls, the readback among them.
- Executor time a frame with the filter on: draws 9.5 ms, uploads 3.7 ms, Present 0.1 ms, "other" 13.4 ms.
- How much of "other" is the readback's wait is **not known**. The build sent on 17 Oct (2c02ad7) logs it
  separately.

The pieces that bear on a floor:
- **Phase 0** (the probes on the owner's machine, HISTORY 6 Oct): a synthetic plaza-shaped frame of 2,500 draws
  costs 7.3 ms through dgVoodoo and 8.7–8.9 ms through D3D11, from x86 and from ARM64 alike (ratio 1.03). The same
  work as 33 draws costs 0.15–0.26 ms.
  - So, on this machine, cost scales with the number of draws.
  - Running the D3D11 calls natively on ARM64 barely changed it in the probe. That is the probe's frame, not the
    game's. The ARM mode (`remote` with d8x_arm64.exe) has **never run in the game**.
- **Readback** (probe, 13 Oct): one readback after 1,600 draws waited 3.40 ms; eight waited 6.26 ms. Eight
  answered a frame late cost 0.18 ms.
- **EIP 0x00010002** (task 34): 66–77% of render-thread samples report it.
  - **Not known** what it is. The reading "the emulator shows this while the thread runs native code or waits" is
    unverified.
  - It does not affect the attributions above, which read return addresses.

What has **never been measured**, and decides where 60 can come from:
1. **The GPU's time a frame.** Present returns in about 0.25 ms. But the readback waits for the GPU, so GPU time
   can hide inside the readback's 25% (and inside EndScene). The scene's render target in trace 6080 is
   3024 × 1898, 5.7 million pixels, drawn on a shared-memory virtual GPU. Whether the GPU alone takes more than
   16.7 ms a frame is **not known**.
2. **The game's own CPU time under emulation, with no rendering at all.** If it is above 16.7 ms in the plaza, no
   renderer work reaches 60 on this machine. **Not known.**
3. **The frame with readbacks really removed** (section 4).

## 6. What each line of work has produced, measured in the owner's game

| Work | In the game | Status |
|---|---|---|
| Reconstruction and gate (774 PASS of 777 in the tree) | swaps run without picture difference (alternating traces) | done; it is the means for source-level fixes, not a speed gain itself |
| xikit, d3dprof, sampler, A/B, fcap | all used in the owner's runs | done |
| speedfix levers | 70 → 34 ms in the plaza | the only measured speed gain to date |
| skin_merge (gated fork) | within the spread | built; no measured gain |
| block_cache, extended_state_cache | within the spread | built; no measured gain |
| d3d8on11 (ours) | runs the game; pictures checked on t8 and replays; plaza 29 ms with exec_filter, about the same as dgVoodoo | working; not yet faster than dgVoodoo |
| ARM mode (d8x_arm64.exe) | never run in the game | built, untested |
| The UI goal | facts gathered (docs/UI.md) | not started as work |

**What went wrong in the method** (my errors):
- perf60 was built around the 60 fps limiter while frames were 30+ ms, so the limiter could not bind. The data in
  hand already said so.
- Lever after lever was measured against a spread of 3.5–5 ms, so anything smaller could never show. Those A/Bs
  could not find gains below about 4 ms, and most results were "within the spread".
- The question that bounds everything was never asked first: what is the floor of this machine, for the GPU and for
  the game's own code? Section 5 lists it. Each round chose a lever from the samples instead.

## 7. Where it can go: three tests that bound it, then the decision

Each test answers a "not known" from section 5. Each is an A/B within one run, standing in the plaza.

| # | Test | Needs | If the result is... | ...then |
|---|---|---|---|---|
| T1 | **Resolution.** The same spot at the usual resolution, then at a much lower resolution (e.g. 1024 × 768) for both the window and the background, in the game's config tool | the owner only; no code | the frame drops a lot | the GPU (fill) is a limit; next, cheaper GPU work and resolution |
| | | | it barely moves | the GPU's fill is not the limit |
| T2 | **The readback, really removed.** `skip_all_occlusion_readbacks=1` at the start and `ab_levers=skip_all_occlusion_readbacks`; the log counts readbacks run in both arms | existing code | the frame drops by more than the spread | the asynchronous readback (task 33) is the next build, on dgVoodoo and on ours |
| | | | it does not drop | the readback's wait was GPU or queue time that the next sync pays instead; T1 and T3 decide |
| T3 | **The game without rendering.** A new speedfix lever, `null_draws`: the draw calls return without drawing, the readbacks skipped as in T2, everything else as usual | a small lever (to build) | the frame stays above 16.7 ms | 60 is out of reach on this machine whatever the renderer does. Speed then means the game's own CPU work, under x86 emulation |
| | | | it falls well below 16.7 ms | the rendering path is the cost, and the port's draw reduction is the way to 60 |

With all three results the plan follows from data. Without them, any next lever is a guess.

**Not proposed, and why:**
- More lever A/Bs, until T1 to T3 say where the floor is.
- The ARM mode as a speed path: phase 0 measured a 3% difference. It can be run once after T3 if T3 says
  rendering is the cost.
- Going above 60: no scene has reached 60 on this machine (section 3). The game's clock above 60 is a reading of
  its code, untested (ROADMAP P4).

## 8. Open, unrelated to speed

- **Exploding vertices on new actors after a while (task 30).** Not known which run showed them, so not known which
  levers were on. The way to settle it: when it happens, Ctrl+Alt+F9 (everything the kit and speedfix add, off,
  live). If they stay, it is not ours. If they go, switch the levers back one at a time.
- **Ours differs from dgVoodoo's picture in 8,689 pixels of a capture**, mostly in the chat log and UI text (HISTORY
  15 Oct). Not found.
