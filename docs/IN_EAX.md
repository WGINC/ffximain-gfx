# `in_EAX`: functions whose result carries EAX's entry value

A decision for the owner. Nothing here is decided by the pipeline.

## What is known, and from where

Ghidra prints `in_EAX` where a function's output depends on the value EAX held when it was called, and that
value is no declared parameter. Of frameset6's export (`data/exports/ghidra_frameset.jsonl`), 177 records use
it; in 153 it is the only undeclared register input.

`pipeline/caller_reads.py` (`data/eax_entry_unseen.json`) settles the cases where that value is seen by no code.
hx2cpp starts `in_EAX` at 0 there, and 44 of frameset5's skips pass because of it. The other 101 are still left
out of the tree. The same tool's fixpoint gives the reason for each (per function: the bytes of entry EAX it reads,
whether all its callers are known, the bytes it can hand back unchanged, and the bytes its callers read):

- **79 hand entry EAX bytes back to a caller that reads them.** Most return a byte or a word and leave the rest of
  EAX as it came in: `return CONCAT31((int3)((uint)in_EAX >> 8), 1);` is `mov al, 1; ret`. A caller then reads
  more of EAX than the byte or word that was set. The bytes come from whatever the *caller* held
  in EAX before the call, so the function's result is exact only if those bytes are passed in.
  The entry bytes that reach a reading caller: bytes 1-3 in 46 functions, bytes 2-3 in 22, and all four in 11
  (byte 0 too: on some path the function leaves AL unset).
- **19 read entry EAX themselves**, or through a callee: EAX is an input of their code.
- **3 have callers that are not all known**, because their address is taken (a vtable or a table), so whether
  any caller reads the bytes cannot be settled from the code.

Not known: what the original source's types were, and so whether any caller's use of those bytes was intended. The
code shows only that the bytes reach a caller that reads them.

## The options

1. **Leave them as the game's code** (the current state). The tree calls them through `orig::`, and nothing is
   lost but their reconstruction.
2. **Give the reconstruction an explicit entry-EAX input.** In the gate, the harness passes EAX's entry value as
   a hidden argument. In the game, a 5-byte thunk passes EAX on. This is exact, but it adds a parameter that no
   source declares. It belongs only in the tree's calling layer, never in a class's public method.
3. **Reconstruct each such function together with its callers**, so the value flows through C++ variables. This
   is exact only when every caller is in the tree, which is not the case for most of them today.

## The functions

Hand entry EAX bytes back to callers that read them (byte 0 is AL):
  0x10007830 (callers read bytes 2,3), 0x10008D90 (callers read bytes 1,2,3), 0x100164B0 (callers read bytes 1,2,3), 0x100274B0 (callers read bytes 2,3)
  0x10028BF0 (callers read bytes 0,1,2,3), 0x10036360 (callers read bytes 2,3), 0x10036420 (callers read bytes 2,3), 0x100367F0 (callers read bytes 1,2,3)
  0x10036B90 (callers read bytes 2,3), 0x10038470 (callers read bytes 0,1,2,3), 0x10045920 (callers read bytes 1,2,3), 0x1004E250 (callers read bytes 1,2,3)
  0x10053890 (callers read bytes 0,1,2,3), 0x10053DD0 (callers read bytes 1,2,3), 0x10084330 (callers read bytes 1,2,3), 0x10084350 (callers read bytes 1,2,3)
  0x10084370 (callers read bytes 1,2,3), 0x10086FC0 (callers read bytes 1,2,3), 0x10087110 (callers read bytes 2,3), 0x10087660 (callers read bytes 1,2,3)
  0x1008E130 (callers read bytes 1,2,3), 0x1008E8D0 (callers read bytes 2,3), 0x100A6070 (callers read bytes 1,2,3), 0x100C3020 (callers read bytes 2,3)
  0x100C3030 (callers read bytes 2,3), 0x100D0D70 (callers read bytes 1,2,3), 0x100D12D0 (callers read bytes 1,2,3), 0x100D44B0 (callers read bytes 0,1,2,3)
  0x100D9A70 (callers read bytes 1,2,3), 0x100DC2D0 (callers read bytes 1,2,3), 0x100DC6C0 (callers read bytes 1,2,3), 0x100E0D50 (callers read bytes 1,2,3)
  0x100F0130 (callers read bytes 1,2,3), 0x100F02A0 (callers read bytes 1,2,3), 0x100F2C40 (callers read bytes 1,2,3), 0x100F76D0 (callers read bytes 1,2,3)
  0x100FA940 (callers read bytes 2,3), 0x100FC370 (callers read bytes 1,2,3), 0x100FC3B0 (callers read bytes 1,2,3), 0x10117EE0 (callers read bytes 0,1,2,3)
  0x10118F30 (callers read bytes 0,1,2,3), 0x1011B360 (callers read bytes 2,3), 0x1011D840 (callers read bytes 2,3), 0x10123EE0 (callers read bytes 1,2,3)
  0x10124390 (callers read bytes 2,3), 0x10129C60 (callers read bytes 2,3), 0x10157880 (callers read bytes 2,3), 0x10157890 (callers read bytes 2,3)
  0x101578A0 (callers read bytes 2,3), 0x101578B0 (callers read bytes 2,3), 0x10158AE0 (callers read bytes 1,2,3), 0x10159040 (callers read bytes 0,1,2,3)
  0x10159540 (callers read bytes 1,2,3), 0x101595D0 (callers read bytes 2,3), 0x10159E10 (callers read bytes 1,2,3), 0x1015E6E0 (callers read bytes 0,1,2,3)
  0x1015E790 (callers read bytes 0,1,2,3), 0x1015EDD0 (callers read bytes 1,2,3), 0x101606F0 (callers read bytes 1,2,3), 0x10160700 (callers read bytes 1,2,3)
  0x10176DD0 (callers read bytes 1,2,3), 0x10177770 (callers read bytes 1,2,3), 0x1019CDC0 (callers read bytes 1,2,3), 0x101A33E0 (callers read bytes 2,3)
  0x101AB7D0 (callers read bytes 0,1,2,3), 0x101ABE40 (callers read bytes 1,2,3), 0x101B4450 (callers read bytes 1,2,3), 0x101B6D50 (callers read bytes 1,2,3)
  0x101B6E90 (callers read bytes 1,2,3), 0x101CBDF0 (callers read bytes 2,3), 0x101CC220 (callers read bytes 1,2,3), 0x101FD600 (callers read bytes 1,2,3)
  0x10201F00 (callers read bytes 1,2,3), 0x102031F0 (callers read bytes 1,2,3), 0x1021BCB0 (callers read bytes 1,2,3), 0x1021C100 (callers read bytes 1,2,3)
  0x1021D460 (callers read bytes 0,1,2,3), 0x1021D9A0 (callers read bytes 1,2,3), 0x1025E020 (callers read bytes 2,3)

Read entry EAX themselves:
  0x10008E10, 0x10014200, 0x1001D240, 0x100538E0
  0x10081230, 0x10118260, 0x10118DB0, 0x10118E90
  0x10125170, 0x10158B00, 0x1018FD90, 0x1019CF60
  0x1019D2B0, 0x1021C7C0, 0x102F6BA2, 0x10305B22
  0x10306478, 0x1030774E, 0x103162F7

Callers not all known:
  0x100721F0, 0x10080D00, 0x10120F30

## Reads seen and reads assumed (22 Oct)

`data/eax_inputs.json` now says, for each function, two things about its entry EAX:

- `reads`: the bytes the function or a callee *may* read. This counts code the walk does not follow as reading:
  - a call through a pointer that `vcall_resolve.py` does not settle;
  - a jump the code does not bound;
  - wherever a copy of the bytes goes.

  It is the safe side for what a reconstruction must pass, and the left-out rule and the wrapper check use it.
- `reads_seen`: the bytes an instruction is shown to examine, in the function or in a direct callee:
  - a compare or test;
  - a multiply or divide;
  - a use as an address.

  These are not examinations:
  - copies (`mov`, `push`, `stos`);
  - "upward" arithmetic (add, sub, inc, dec, and, or, xor, shl, lea), where a byte reaches only the same or higher
    bytes;
  - shifts and rotates down, whose bits MSVC usually masks off next (`mov al, [x]; shr eax, 4; and eax, 7`).

  Showing that the bytes reach something would take tracking bits through them, which the walk does not do.

In the gate, a stubbed callee's entry EAX is compared like an argument only in its seen bytes. If the runs agree
on everything else and differ only in bytes the callee is assumed to read, the trial is not a pass. Its verdict is
UNRESOLVED, naming the callee: whether that is a difference is not known.

Of 2,657 functions whose entry EAX is an input, 444 may read it and 16 are seen to: all 15 in the C runtime's
range (helpers that take a value in EAX, `_alloca_probe` and the 64-bit shifts among them), plus 0x10B7193C.

Corrections made on the way, each from the Intel SDM:
- `sbb r, r` gives −CF whatever r held, so it is a write only, like `xor`/`sub r, r`;
- `test r, imm` reads only the bytes the immediate keeps.

The import results also changed: every import's declared return now comes from Microsoft's headers
(`pipeline/sdk_protos.py`, `data/sdk_import_returns.json`). Before, only imports some exported record typed were
known, and `Direct3DCreate8` was treated as handing EAX back unchanged.
