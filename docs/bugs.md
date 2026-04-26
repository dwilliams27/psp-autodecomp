# Known Bugs

Bugs discovered during development. Status reflects 2026-04-25 audit.
Active bugs first; resolved bugs at the bottom for history.

## Active

### 0. Cross-session .o staleness — Makefile lacks header dep tracking

**Found:** 2026-04-25 during Phase 3 Step 2 sanity run.
**Status:** active, narrow workaround in place; root cause unfixed.

The Makefile's `$(BUILD_DIR)/src/%.cpp.o: src/%.cpp` rule has no
header dependency tracking (no `-MMD -MP` or SNC equivalent).
`make build/src/foo.cpp.o` returns "up to date" whenever `.cpp`
mtime ≤ `.o` mtime — even when an included header was modified
since the last compile.

This bites when an agent's session writes to a header that's
classified out-of-scope by the per-session file ledger; the revert
silently invalidates any `.o` whose source includes that header,
but `make` doesn't know. Subsequent `compare_func.py` /
`check_byte_match` calls happily use the stale `.o`, accepting
matches whose current source no longer compiles.

**Workaround landed:** the orchestrator's post-revert re-verify
explicitly removes the `.o` for each surviving matched src before
calling `check_byte_match`, forcing a fresh compile (commit lands
with this bugs.md update). This handles the immediate
within-session case.

**Real fix needed:**
1. Confirm whether SNC supports `-MMD`/`-MD`/`--depend` or similar
   for emitting `.d` dep files. If yes, add it to `CFLAGS`/`ECFLAGS`
   and `-include $(ALL_OBJS:.o=.d)` so make tracks header deps
   properly.
2. If SNC has no native dep-gen flag, run `cpp` (GCC's preprocessor)
   in a sidecar pass to emit `.d` files, OR have the orchestrator
   rm any `.o` whose source includes a reverted header at the end
   of a session that did out-of-scope reverts. (Approximation:
   conservatively rm `build/src/<Class>*.o` for every reverted
   header's class; correct but over-eager.)

The narrow within-session workaround handles the most-likely
poisoning vector (the just-rewritten match). Cross-session
poisoning where a prior session's revert leaves a stale `.o` that
a later unrelated session's `check_byte_match` happily accepts is
still possible — but harder to trigger and has not been observed.

### 1. Three pre-Phase-1 entries marked "failed" despite verified-byte commits

**Found:** 2026-04-11 overnight run.
**Status:** **partially mitigated** by Phase 1 (commit `9ce49df`); the
mechanism that produced these entries can no longer trigger under
multi-agent. Historical data poisoning persists in the DB.

**Mechanism (now blocked):** session A matched function X in file F
and committed; session B targeted a different function in F,
overwrote the source, reported X as `failed`, and the picker accepted
the downgrade. Phase 1's `class_locks` + per-session file ledger
prevent two sessions from sharing the same `Class.cpp` concurrently,
and the picker no longer overwrites a `matched` row's `match_status`
without a fresh decision specific to that address. The pre-existing
poisoned rows remain because no audit pass has rolled them back.

**DB rows still poisoned (verified 2026-04-25):**
- `0x0001d224 eAudioChannel::CalcPanning(int, const mOCS *, float *) const` → `failed`
- `0x001e5bc4 cRedBlackTree<eCollisionPair, ...>::FindNode(...)` → `failed`
- one or more `gcCamera::gcEntityFollowState::*` rows (specific addr unclear from original bug; spot-check shows `0x000fc640..0x000fc89c` are now `untried`)

**Fix needed:** re-run `tools/byte_match.py check_byte_match` against
each poisoned address. Where the verified bytes still match the
EBOOT slice, flip `match_status` back to `matched` and re-populate
`matched_by_*`. Where the matching src file is missing (FindNode's
`src/cRedBlackTree_FindNode_eCollisionPair.cpp` no longer exists —
see Bug #2), the row stays `failed`/`untried` and a fresh attempt
is needed.

---

### 2. Pure-assembly matched files inherited from before quality gates

**Found:** 2026-04-14 pre-commit review.
**Status:** new submissions are blocked by `validate_source_quality`
in the orchestrator (commit history pre-dates Phase 1). 8 of the
original 15 problem files have already been removed; **7 remain in
the repo as of 2026-04-25** and continue to inflate the matched
count.

**Still in the tree (need triage):**
- `src/eCompoundShape.cpp`
- `src/eAudioChannel.cpp`
- `src/mQuat.cpp` (mQuat::Mult + mBasis::Orthonormalize)
- `src/mBox.cpp`
- `src/eCapsuleShape.cpp`

**Already cleaned up (no action):**
- `src/cRedBlackTree_FindNode_eCollisionPair.cpp`
- `src/gcEntityFollowState_ctor.cpp`
- `src/eCompoundShape_GetEmbedContacts.cpp`
- `src/mBasis_Orthonormalize.cpp`
- `src/eStaticSkyLight_GetDirectLight.cpp`
- `src/eSimulatedController_GetVelocity.cpp`
- `src/eBipedController_GetVelocity.cpp`
- `src/eMultiSphereShape_GetProjectedMinMax.cpp`
- `src/eHeightmapShape_GetInertialTensor.cpp`
- `src/eHeightmapShape_GetAABB.cpp`

**Fix needed:** for each surviving file, decide:
1. **Reworkable** (mostly C++ wrapping a small `__asm__ volatile()`
   for VFPU primitives that have no C equivalent) → rewrite it,
   re-verify byte-exact, keep `matched`.
2. **Genuinely VFPU-only** (100% inline asm, no C++ structure to
   recover) → revert the addr's `match_status` to `failed`, delete
   the src file, leave a `failure_notes` entry recording the
   classification.

Reworking is preferred over reverting because the same source maps
to several matched addresses; reverting one row strands the others.

---

### 3. Symbol-name mismatches between stored DB symbol and authoritative .sym

**Found:** 2026-04-24 while fixing the AMBIGUOUS-overload bug.
**Status:** active. 33 mismatches remain (down from 80 in original
bug) — 19 benign (naming-style only) and **14 concerning** (the
mangled symbol's parameter type sequence differs, indicating the
reconstructed source has the wrong signature even though the bytes
match).

**Why this happens:** the verifier's name gate
(`tools/byte_match.py:sym_encodes_func`) accepts a class+method
substring match with a 1–3 character gap, ignoring the
parameter-type suffix of SNC mangling. For trivial 28–44 byte stubs
that delegate to shared template machinery, two different overloads
can compile to byte-identical MIPS — the gate happily picks
whichever happens to match by name.

**Categorization (2026-04-25 audit):**
- *Benign — naming-style only:* `cFile__OnCreated` vs `__0fFcFileJOnCreatedv`,
  `eShape___dtor_eShape_void` vs `__0oGeShapedtv`, etc. Same
  function, different naming convention (safe-name post-splat patch
  vs canonical SNC mangling). 19 entries.
- *Concerning — different parameter mangling:* the mangled-prefix
  characters disagree on type back-references or primitive codes.
  14 entries.

**Worked example, confirmed by codex 2026-04-25 audit:**
`0x00047f80 eDynamicModel::SetSkin(...)` is byte-correct but the
header at `include/eDynamicModel.h:36` and the source at
`src/eDynamicModel.cpp:87` declare:
```cpp
void SetSkin(cHandleT<eSkin>, int, int, cTimeValue);
```
The `.sym` mangled symbol indicates the third parameter should also
be `cHandleT<eSkin>`, not `int`:
- stored: `__0fNeDynamicModelHSetSkin6IcHandleT76FeSkin_iTB6KcTimeValue`
- .sym:   `__0fNeDynamicModelHSetSkin6IcHandleT76FeSkin_iTC6KcTimeValue`

The `TB` → `TC` shift is a back-reference index difference: in the
.sym version `cTimeValue` references the *third* prior type
(implying an extra `cHandleT<eSkin>` parameter), in our version it
references the *second*. This is exactly the "bytes match, training
data lies" failure the quality system is designed to catch.

**Detection script** (one-shot, addr-list output):
```python
import json, subprocess
db = json.load(open('config/functions.json'))
out = subprocess.run(['mipsel-linux-gnu-nm','--defined-only','extern/extracted_symbols/Game-dvd.sym'],
                    capture_output=True, text=True).stdout
sym_map = {int(p[0],16): p[2] for ln in out.splitlines()
           if (p := ln.split(None, 2)) and len(p)==3 and p[1] in ('T','t','W','w')}
for f in db:
    if f.get('match_status') != 'matched':
        continue
    addr = int(f['address'],16); stored = f.get('symbol_name'); sym = sym_map.get(addr)
    if stored and sym and stored != sym:
        print(f['address'], f['name'], '|', stored, 'vs', sym)
```

**Fix needed:**
- Audit the 14 concerning rows. For each, work out the correct
  signature from the .sym demangling and rewrite the source. Verify
  it still compiles to identical bytes.
- Long-term: tighten the verifier's gate to require exact
  `mangled_symbol` equality on `.cpp` reconstructions where both the
  DB and .sym carry one (currently exact-mangled is only used as an
  AMBIGUOUS-overload tie-breaker).

---

## Resolved

### ~~4. Operator locked out of `config/functions.json` after each overnight run~~

**Found:** 2026-04-25 sanity test for Phase 3.
**Resolved:** same day at the source — `tools/common.py:save_db`
now preserves the existing DB's mode and ownership when atomically
replacing it, falling back to 0644 only when the file doesn't exist
yet.

Root cause was Phase 1's atomic save (commit `9ce49df`):
`tempfile.mkstemp` defaults to mode **0600** (Python's secure-by-
default for tempfiles), and `os.replace` promotes that 0600 tmp to
be the new `config/functions.json` — owned by `autodecomp` from
inside the sandbox. The pre-Phase-1 `save_db` was a plain in-place
`open(path, 'w')` write, which leaves the existing file's
ownership and mode untouched, so the operator could always read.

Other autodecomp-written files (newly-created `src/<Class>.cpp`,
`logs/match_*.jsonl`) were never affected — they go through plain
`open()` and inherit the user's umask 022, ending up `644`. The DB
was the only file that regressed. An earlier fix in
`tools/run_overnight.sh` did a broad `sudo chown -R` of
`config/`, `src/`, `include/`, `logs/` in the cleanup trap; with
the source fix in `save_db`, that broad chown is no longer
necessary and was removed.

---

### ~~5. Orchestrator's git_commit_batch absorbed operator-staged changes~~

**Found:** 2026-04-24 during fix_plan UI work.
**Resolved:** 2026-04-25 by Phase 1 (commit `9ce49df`), Direction #003.

The pre-Phase-1 `git_commit_batch` built a set of files-to-commit
from `matched_files | _session_dirty_paths()`, where
`_session_dirty_paths()` was a global tree-walk of `git status`.
Concurrent sessions and operator edits during a run got absorbed
into whichever session committed next.

Phase 1 replaced `_session_dirty_paths()` with a **per-session file
ledger**: when the picker assigns a batch, it computes the allowed
file set (`{src/<Class>.cpp, include/<Class>.h, src/<Class>_*.cpp}`).
The worker captures `git status --porcelain` after the agent
finishes, partitions dirty files into in-set (the ledger) and
out-of-set (rejected + reverted). Only the ledger gets staged; the
commit message exclusively reflects that session's work.

Tested by `tools/test_phase1_smoke.py` (invariant 3) and the
Phase 3 smoke (per-worktree fan-in verifies cross-tree leakage too).
