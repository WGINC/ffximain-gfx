# Working cheaply and fast on this repository (read before any work)

From the owner's usage reports of 27 Oct. These rules come before docs/HANDOFF.md's steps; CLAUDE.md still comes first.

## What the money went on

| Session | Result | Cost | Cache read | Output | API min / wall min |
|---|---|---|---|---|---|
| Lead with up to 5 workers | about a week of work, 83% of a weekly limit | $961 reported | 481.6M | 18.5k | 1,584 / 49 |
| Single session, 27 Oct | 14 functions matched by hand, 4 tools, 1 fix: a whole 5-hour window, 14% weekly | $868 | 168.7M | 4.5k | 1,298 / 118 |

**Almost all the cost is re-reading context.** Every tool call re-sends the whole conversation, so the cost is turns ×
context size. Output is negligible. An API/wall ratio above 1 means requests ran concurrently: subagents.
**Hand matching costs about 1% of a weekly limit per function.** 60% of the frame set at the bytes (#127) is about
1,650 more functions: by hand, out of reach. The sweep put 1,528 functions at the bytes by rules applied in bulk, at no
model cost per function. **So the job is to find rules, not functions.**

## Rules

1. **No subagents, no workers, no Agent tool.** One session, one task.
2. **Short sessions.** One session per item below; end it when the item is committed. Start with only CLAUDE.md, this
   file and the HANDOFF section for the item. Never carry a session past one task: a long context makes every later
   call expensive.
3. **Never read big things into the conversation.** No `cat` of a recompile file, a JSON record file, a gate log or an
   export. Use `grep -n`, `sed -n A,Bp` for a known range, `tail -3`, `python3 -c` that prints a count. Pipe every
   command through `| tail -N` or `| head -N`. Ask scripts for one summary line.
4. **Machines wait, not the model.** Anything over a minute (gate, regen, sweep, reorder.py, export) runs in the
   background with its output to a file; end the turn; read one summary line when it finishes. Never poll in a loop,
   never watch a log.
5. **Batch.** One shell command does the whole mechanical sequence (build, match, count, commit). Do not run one
   command, read it, run the next, when the next does not depend on reading.
6. **Search by script, not by turns.** Trying source variants is a loop for a program (`pipeline/vc6/reorder.py`, a
   variant script), not for the model: write the variants once, let the script compile and score them all
   (`mdiff.py`), read the best score.
7. **Cheaper model and effort for mechanical steps** (applying a known rule, refreshing status files, a regen and its
   gate): Sonnet or Haiku, low effort. Keep the strongest model for reading a new mismatch to its cause.
8. **Commits end with the `Claude-Session:` line only.** No `Co-Authored-By` (it names a model; CLAUDE.md). The 27 Oct
   session's 7 commits carry one; do not repeat it.
9. **Shared code once.** The vc6 shim is pasted into each recompile file (1,100+ lines each). Put it in one header
   (recompile/vc6_shim.h) and keep each file to its function; match.py copies only the source into its build folder,
   so it needs an `/I` to recompile/ added first.

## The work, in the order that gives most per token

1. **Encode the hand-found rules into the sweep (#163).** recompile/d3d_round_118.md, _119.md and _120.md hold
   about 20 source-form rules, each proven on at least one function (rule 8: read a global into a block-scoped local
   before each device call; rule 11: float locals and `f` literals; rule 12: plain arrays instead of the `xi_frame`
   block; rule 15: `v[i++]` order; and the rest). For each: add it to `pipeline/vc6/sweep.py` as a rewrite applied
   only where it keeps the function's meaning (the tree's GATE: PASS source) and only kept where match.py then says
   IDENTICAL. Run the sweep over the whole tree in the background (it has a per-function cache); read the IDENTICAL
   count. One rule can move tens of functions; that is the 60% (#127) path.
2. **A tree round only when there is something to install** (a new export, a generator fix): HANDOFF 3.1a, the
   four `tree_round.py` steps and the live gate in one background script, read once at the end. r14 (27 Oct) was run
   that way.
3. **The owner's kit (HANDOFF 3.2)**: one build script, then the session folder. No gate.
4. **Hand matching only to find a new rule**, on the closest misses (recompile/d3d_round_120.md lists them), and
   stop at the first rule found: encode it (step 1) before matching another function by hand.

## Checks that stay (cheap, and the reason the work is trusted)

`pipeline/vc6/match.py --all` exit 0 after any recompile change; `pipeline/tests/test_rules.py` after any pipeline
change; for a tree, the verdict changes read to their cause before installing. Run them in the same batched command as
the change, and read only their last line.
