# Requests to the IDA session

A Claude Code session on the owner's PC has IDA open on the FFXiMain IDB with its MCP plugin running. It watches
this branch (`claude/gifted-archimedes-cjtg2d`) on origin and picks up requests written here. This folder is the
channel between that session and sessions that have no IDA (cloud sessions cannot message it directly).

## To ask for something
1. Pull first, so you build on any reply already pushed.
2. Add `requests/<topic>.md`: what to produce, where to put it, and the rules it must follow (see
   `02_user_set_types.md` for the shape). Editing an existing request counts as a new ask.
3. Commit and push to this branch. The IDA session polls origin about once a minute and starts on any commit
   that touches `requests/` and was not its own.

## What comes back
- The outputs the request names, committed to this branch.
- A `## Status` section appended to the request file (or, when done, the request file replaced by its report; the
  request stays in git history): `done`, `partial` or `blocked`, the date, what was produced, and anything it could
  not answer from IDA (stated as not known, never filled in).
- A question it needs answered first goes in `requests/<topic>.reply.md`; answer it by editing the request.

The IDA session's own commits carry the trailer `Bridge: ida-session`; that is how it tells them from yours. You
see its replies only after you fetch: nothing notifies you.

## Limits
- It runs only while the owner's PC is on and that session is open.
- Read-only work on the IDB (reading, hashing, exporting) is done as asked. Anything that would modify the IDB, or
  push outside this branch, waits for the owner's approval.
- The IDB, the DLL and other binaries never enter the repo.
