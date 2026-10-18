# xisamp: the native sampler

The render thread's samples, with the x86 position and the machine's native position taken together. On the owner's
machine the game is x86, emulated on ARM64. xikit's `sample=1` runs inside the game, so it sees only the emulated x86
state. There, 64-67% of its samples read EIP 0x00010002, an address in no module (docs/HISTORY.md, 18 Oct).
`xisamp` is a native 64-bit process (ARM64, or x64 on an x64 Windows) that reads both states each sample:
- the x86 one, through `Wow64GetThreadContext`;
- the native one: on ARM64, the emulator, Windows' own ARM64 code or the driver.

- `xisamp.c`: the sampler. Its header comment gives the method and the file format.
- `build.sh`: builds `dist/xisamp_arm64.exe` and `dist/xisamp_x64.exe` (zig cc 0.16.0) and `test/target.exe`.
  `kit/xikit/build.sh` runs it and copies both exes into `kit/xikit/dist/`.
- `read.py`: reads `xikit_native_<pid>.csv` (docs/KIT.md, "Where the time goes"):

      python3 read.py xikit_native_<pid>.csv [--from HH:MM:SS --to HH:MM:SS] [--focus 0x1002A034]

- `test_xisamp.py`: runs the x64 build under 64-bit Wine against `test/target.exe`, an x86 process with a known call
  chain. `kit/xikit/test/test_xikit.py`'s `native` run tests the launch from xikit.

Use: put the two exes in the kit folder, next to `xikit.dll`, and set `sample=2` in `xikit.ini` (`sample_ms`,
`sample_window`). Not tested here: the ARM64 build on ARM64 Windows. In particular its native frame chain, which no
machine here can run.
