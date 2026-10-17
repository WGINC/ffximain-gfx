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

## 3. All the frame-time data, from the raw logs

Every figure below was read from the owner's log folders by `pipeline/run_ledger.py`, which cites each one by file
and line. A run is named by its pid, so `2264/xikit.log:213` is `2026-10-06_104841_2264/xikit.log:213`. The logs
themselves stay out of the repo. Runs marked **HISTORY only** have no log here, so their figures come from HISTORY
alone.

How each column is known:
- **Device:**
  - `4.8.2.134` in the log's `d3d8.dll … version` line is dgVoodoo.
  - `?` means the DLL has no version resource, which is ours (kit/d3d8on11 has no resource file).
  - Ours writes its mode to `d3d8on11.log` (line 2, `executor: …`). That file is rewritten at each start
    (d8_common.cpp:19), so only the last start in a folder keeps it. For every other start, the mode comes from
    the owner's message or the session's README, named in the row.
- **Speedfix:** the `settings (start)` line.
- **Frame:**
  - "per minute" is speedfix's minute means, after the first minute (loading in).
  - "p50" is xikit_timing's median per 600 frames.
  - "phases" are the A/B phase means.
- **Place:** the owner's word, through HISTORY or a message. No log records the place.

| Date, run | Device | Speedfix | Place | Frame | Source |
|---|---|---|---|---|---|
| 6 Oct, 10016 | dgVoodoo (HISTORY 527) | none (HISTORY 527) | not recorded | p50 17.5–42.4 ms; the last row 19.2 ms; the d3dprof figures are HISTORY only | Skink/xikit_timing_10016.csv:3,7,22; its xikit.log is not in the folder |
| 6 Oct, 6080 | not in the log | defaults: batch, lazy, sse, merge not yet in that build | ends at the combat-test spot (HISTORY 552) | per minute 22.4–40.6 ms | Gonked/xikit.log:4,119–165 |
| 6 Oct, 3736 | dgVoodoo | defaults | the plaza from about frame 6600 | 18–21 ms, then 64–79 ms | **HISTORY only** (611) |
| 6 Oct, 6156 | dgVoodoo | defaults; A = off | the plaza | A 70–72, B 66–68 ms | **HISTORY only** (668) |
| 6 Oct, 12764 (session 1) | dgVoodoo | 4 levers on; A = everything the kit adds off | the plaza (HISTORY 750) | **A phases 70.55–74.12 ms, B phases 33.20–40.15 ms** | 12764/xikit_ab_12764_1.csv:3–10; 12764/xikit.log:112,5 |
| 6 Oct, 5180 (session 2) | dgVoodoo | batch, lazy, sse, merge off | the plaza (HISTORY 783) | per minute 62.7–66.5 ms | 5180/xikit.log:34,5,51–95 |
| 6 Oct, 2020 (session 3) | dgVoodoo | none (no speedfix line) | the plaza (HISTORY 750) | the last row 72.7 ms | 2020/xikit_timing_2020.csv:7; 2020/xikit.log:11 |
| 6 Oct, 12164 (session 4) | dgVoodoo | every lever | 819 draws: place not recorded; then 3,450–3,640 draws | p50 17.5–19.4 ms, then 31.5–35 ms | **HISTORY only** (750) |
| 6 Oct, 2264 (session 5 part 1) | dgVoodoo | every lever, one at a time off in A/B | the plaza, standing (HISTORY 851) | phases 27.65–46.40 ms; per minute 28.6–39.9 ms | 2264/xikit.log:111,5,292,278,143–384 |
| 6 Oct, 11952 (session 5 part 2) | dgVoodoo | every lever; profiler on | under session 5's heading; part 2's place not stated | per minute 23.1–35.7 ms | 11952/xikit.log:115,5,147–235 |
| 6 Oct, 7388 | dgVoodoo | up_ring, skip_shadow_pass on | the plaza (HISTORY 972) | two minutes, 19.6 and 22.6 ms; up_ring phases A 20.28, B 25.55 | 7388/xikit.log:114,5,138,151,140,142 |
| 10 Oct, 13172 | dgVoodoo | most levers off; occlusion_async A/B | not recorded | per minute 33.2–46.1 ms; about 4,000 readbacks run a minute | 13172/xikit.log:33,5,54–102,55–82 |
| 13 Oct, 8948 | dgVoodoo | every lever; A/B of 3 | the plaza (HISTORY 1430) | phases 22.70–31.17 ms; per minute 20.0–31.0 ms | 8948/xikit.log:114,6,150,228,146–349 |
| 13 Oct, 6796 | dgVoodoo | every lever | the plaza, then walking (HISTORY 1489) | per minute 23.1–28.5 ms | 6796/xikit.log:116,6,166–251 |
| 14 Oct, 3956 | dgVoodoo | none | not recorded (the capture run, HISTORY 1979) | mean 46.7 ms; p50 up to 52.8 ms | 3956/xikit_timing_3956.csv:9; 3956/xikit.log:13 |
| 14 Oct, 2880 | ours | none | | ended at the start; no frames | 2880/xikit.log:13 |
| 16 Oct, 5560 (up1) | ours, `direct` (the owner) | none | **the house** (the owner) | p50 32.9–35.4 ms in the steady rows | 5560/xikit_timing_5560.csv:8–21; 5560/xikit.log:13 |
| 16 Oct, 7608 (up1) | dgVoodoo | none | **the house** | **p50 17.1–17.8 ms** from frame 4200 | 7608/xikit_timing_7608.csv:8–22; 7608/xikit.log:13 |
| 16 Oct, 5584 (up1) | ours, `remote` (the owner) | none | **the house** | **p50 17.3–17.6 ms** from frame 1200 | 5584/xikit_timing_5584.csv:3–25; 5584/xikit.log:13 |
| 16 Oct, 12120 (up1) | ours, `thread` (the owner; logs_up1/xikit_logs/d3d8on11.log:2) | none | **the house** | **p50 17.2–17.6 ms** from frame 1200 | 12120/xikit_timing_12120.csv:3–34; 12120/xikit.log:13 |
| 16 Oct, 2340 (up1) | DXVK (the owner) | none | | crashed: 5 log lines, no frames | 2340/xikit.log |
| 16 Oct, 3056 | ours, `thread`, profiler on | none | not recorded | mean 38.7 ms (the profiler's cost included; not comparable) | 3056/xikit_timing_3056.csv |
| 16 Oct, 2420 (perf60 run 1) | dgVoodoo | every lever, precise_pacing A/B | the plaza (perf60 README) | phases 32.88–36.40 ms; per minute 31.9–35.6 ms | 2420/xikit.log:115,6,144,167,151–231 |
| 16 Oct, 3128 (perf60 run 2) | ours, `thread` (logs_perf60/xikit_logs/d3d8on11.log:2) | every lever | the plaza | exec_filter phases 26.78–36.92 ms; precise_pacing phases 33.80–37.29 ms | logs_perf60/xikit_logs/d3d8on11.log:74,58; 3128/xikit.log:241,217 |

Excluded: `sfh` (pid 32) is native x86 Windows 10 build 19043 (sfh/xikit.log:107), not the owner's machine.

What this table establishes:
- **Without speedfix, the plaza ran at about 70 ms**: session 1's A phases, session 2's minutes and session 3.
  With the levers, session 1's B phases ran at 33–40 ms. Two levers were measured singly: `sse_skinning` −14.5 ms
  and `batch_up_draws`+`lazy_state` −8.2 ms (section 4).
- **With every lever on, the plaza's minutes ran at 20–40 ms**, depending on the run. That is 25 to 50 fps.
  - Run to run in the same spot, the minutes differ by more than 10 ms (8948 against 3128).
  - The crowd is not controlled, so only A/Bs within one run are comparisons.
- **Corrected: frames do reach the game's limiter, in a light scene.** In the house on 16 Oct, without speedfix,
  dgVoodoo, ours `remote` and ours `thread` all held p50 17.1–17.6 ms.
  - That is the game's own floor. Its limiter allows no frame shorter than 17 ms with whole milliseconds, a tick
    count of 1 and a scale of 1.0 (read from its code, HISTORY 2047). Those values were measured in perf60
    (HISTORY 2107).
  - So in the house, all three wait for the limiter. How far below 17 ms they would go is not measured.
  - Ours `direct` ran at 33–34 ms in the same place: it is the one mode that did not reach the floor there.
  - The earlier "no scene measured at or below 16.7 ms" is true only because the floor is 17 ms. That earlier
    statement did not include these runs, which are in no HISTORY entry; they are named in the owner's message
    of 16 Oct.
- Which rows are in the house is the owner's account ("walked to the house and stayed there"); the logs do not
  record the place. In 7608, rows 4–7 (frames 1200–4200) have p50 28.3–30.5 ms: the walk, by that account (7608/xikit_timing_7608.csv:4–7). In 5584 and
  12120 every row from frame 1200 is at the floor.
- Row 3 (frames 600–1200) is at the floor in almost every run: 17.4–17.7 ms, and 16.67 ms with precise_pacing
  (2420/xikit_timing_2420.csv:3, 3128/xikit_timing_3128.csv:3). What the screen was then is not recorded. The login
  is likely, but it is not known.

## 4. Every lever A/B, as measured in the game

Every row is speedfix's or xikit's own summary line, except exec_filter. The "spread" is the largest difference
between phases of one label, printed on the same line.

| Lever | B − A | Spread | Verdict | Source |
|---|---|---|---|---|
| everything the kit adds (session 1) | −35.98 ms | 6.96 | gain | 12764/xikit.log:208 |
| sse_skinning | −14.51 | 5.96 | gain | 2264/xikit.log:289 |
| batch_up_draws + lazy_state | −8.16 | 3.37 | gain | 2264/xikit.log:368 |
| exec_filter (ours) | −5.46 | 4.24 (frame), 3.63 (executor busy) | gain; every on phase faster than every off phase | logs_perf60/xikit_logs/d3d8on11.log:39–146: 16 phase lines. The −5.46 is the mean of the phase means, computed from them (HISTORY 2124); the device prints no summary |
| extended_state_cache | −2.53 | 4.25 | within the spread | 8948/xikit.log:238 |
| skip_all_occlusion_readbacks | −1.31 | 3.98 | **not a test**: no readback was requested while it ran (below) | 2264/xikit.log:213 |
| skip_actor_occlusion | +0.33 | 4.37 | within the spread | 2264/xikit.log:330 |
| merge_scenes | +1.17 | 3.48 | within the spread | 2264/xikit.log:251 |
| skin_merge | +1.36 | 5.29 | within the spread | 8948/xikit.log:203 |
| block_cache | +1.43 | 3.94 | within the spread | 8948/xikit.log:161 |
| occlusion_async | +4.18 | 15.14 | within the spread; reverted (HISTORY 1068) | 13172/xikit.log:86 |
| precise_pacing | +0.27, +0.88 | 3.52, 3.49 | no effect: the limiter did not bind at 31–36 ms (HISTORY 2113) | 2420/xikit.log:168; 3128/xikit.log:242 |
| up_ring | +5.27 (one phase each: 20.28, 25.55), with corrupted frames | | reverted | 7388/xikit.log:140,142 |
| skip_shadow_pass | corrupted frames | | reverted | HISTORY 972 |

**The readback A/B did not test the readback.** speedfix logs `occlusion readbacks: N run, M skipped` in a minute
only when the count is nonzero (sf_main.cpp:600). In session 5 part 1, with the switch on at the start, the minutes
with readbacks were:

| Minute | Readbacks | Actor occlusion queries run | Source |
|---|---|---|---|
| 1 (loading in) | 158 | 0 | 2264/xikit.log:135,136 |
| 16 | 5,062 | 5,605 | 2264/xikit.log:301,302 |
| 17 | 2,005 | 2,219 | 2264/xikit.log:314,315 |
| 18 | 7,165 | 7,936 | 2264/xikit.log:327,328 |

The readback A/B (group 1) ran its phases from 10:55:47 to 10:58:47 (2264/xikit.log:118,178,212). Those are
minutes 7 to 9 (2264/xikit.log:186,198,210), and none of them has a readback line. So neither arm had readbacks. That conclusion stands.

**Corrected: "none later while standing" (HISTORY 863) is wrong.**
- In part 1, readbacks came back in minutes 16 to 18, the A phases of the skip_actor_occlusion A/B (group 4, from
  line 290), when actor queries ran.
- But part 2 (11952), with skip_actor_occlusion on and every actor query skipped, also had readbacks in minutes 7, 8
  and 10 to 12: 806, 83, 782, 1,790 and 2,602 (11952/xikit.log:192,201,218,227,236; the actor lines below each say
  0 run).
- So actor queries are not the only source of readback requests. What makes the game request them is **not
  known**.
- Runs with readbacks run: 13172 had about 4,000 a minute (13172/xikit.log:55–82); 7388 had 2,699 in its second
  minute (7388/xikit.log:152).

On 16 Oct, the samples show the readback's LockRect under 25% of dgVoodoo's render-thread samples and 66.5% of
ours. That was standing in the plaza on the new game build. How many readbacks ran a frame is not known: the counter
exists only when the switch is on at the start, and it was off. The ceiling session starts with the switch on, so
T2's log will count them.

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
| T3 | **The game without rendering.** A new speedfix lever, `null_draws`: the draw calls return without drawing, the readbacks skipped as in T2, everything else as usual | a small lever (built 17 Oct; session kit/sessions/ceiling) | the frame stays above 16.7 ms | 60 is out of reach on this machine whatever the renderer does. Speed then means the game's own CPU work, under x86 emulation |
| | | | it falls to 16.7 ms (the game's own limiter is the floor: it cannot show how far below) | the rendering path is the cost, and the port's draw reduction is the way to 60 |

With all three results the plan follows from data. Without them, any next lever is a guess.

**Not proposed, and why:**
- More lever A/Bs, until T1 to T3 say where the floor is.
- The ARM mode as a speed path: phase 0 measured a 3% difference. It can be run once after T3 if T3 says
  rendering is the cost.
- Going above 60: the game's own limiter allows no frame shorter than 17 ms (16.67 ms with precise_pacing). In
  the house, three devices sat at that limit (section 3). The game's clock above 60 is a reading of its code,
  untested (ROADMAP P4).

## 8. Open, unrelated to speed

- **Exploding vertices on new actors after a while (task 30).** Not known which run showed them, so not known which
  levers were on. The way to settle it: when it happens, Ctrl+Alt+F9 (everything the kit and speedfix add, off,
  live). If they stay, it is not ours. If they go, switch the levers back one at a time.
- **Ours differs from dgVoodoo's picture in 8,689 pixels of a capture**, mostly in the chat log and UI text (HISTORY
  15 Oct). Not found.
