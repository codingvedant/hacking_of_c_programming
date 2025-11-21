\# Hacking-of-C — ENPM691 assignments (Vedant Bhalerao)



This repository contains my ENPM691 assignments on low-level memory/ binary exploitation and C programming — lab writeups, source code, and build scripts.



> ⚠️ \*\*Important:\*\* This repository contains exploit code, proof-of-concept binaries, and debugging scripts intended \*\*only\*\* for educational use inside a closed lab environment. Do \*\*not\*\* run these programs on production machines. See `SAFE\_USAGE.md`.



\## Structure

\- `assignments/` — per-homework folders with `src/`, `docs/`, `Makefile`.

\- `makefiles/` — shared build scripts.

\- `misc/` — gdb scripts, helper tools.

\- `docs/` — original PDF writeups (kept for completeness).



\## How to build an assignment (example)

```bash

cd assignments/hw02\_mul\_constants

make      # builds mulby3, mulby7, mulby12

./mulby3

./mulby7

./mulby12



