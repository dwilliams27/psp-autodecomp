# Known Bugs

Bugs discovered during development that haven't been fixed yet. Fix these before they bite.

## 1. Orchestrator can mark verified matches as "failed" on retry

**Found:** 2026-04-11 overnight run

When a function is matched and verified in session A, then the same source file is modified by session B (which targets a different function in the same file), session B can overwrite the source, breaking the previously matched function. If session B reports that function as "failed", the database is updated to "failed" even though a verified match existed in an earlier commit.

**Impact:** 3 functions (eAudioChannel::CalcPanning, cRedBlackTree::FindNode, gcCamera::gcEntityFollowState) were verified byte-exact against EBOOT but show as "failed" in functions.json.

**Root cause:** The orchestrator doesn't protect previously-matched functions when a later session writes to the same source file.

**Possible fixes:**
- Before marking a function as "failed", check if it was previously "matched" and re-verify against EBOOT before downgrading
- Use separate source files per function (already mostly done, but some agents put multiple functions in one file)
- Lock matched source files from further modification

## 2. ~20 matched source files are pure assembly with zero C/C++ training data

**Found:** 2026-04-14 pre-commit review

Multiple agents have submitted file-scope `__asm__()` blocks as "matches" — wrapping raw disassembly in `.cpp` files. These produce zero training data value and violate the "C/C++ source only" norm. The orchestrator now rejects new submissions like this, but existing ones remain.

**Affected files:**
- `src/eCompoundShape.cpp` (295 lines, 2 functions)
- `src/cRedBlackTree_FindNode_eCollisionPair.cpp` (187 lines)
- `src/gcEntityFollowState_ctor.cpp` (174 lines)
- `src/eAudioChannel.cpp` (155 lines)
- `src/mQuat.cpp` (136 lines, mQuat::Mult + mBasis::Orthonormalize)
- `src/eCompoundShape_GetEmbedContacts.cpp` (115 lines)
- `src/mBox.cpp` (75 lines)
- `src/mBasis_Orthonormalize.cpp` (75 lines, duplicate of symbol in mQuat.cpp)
- `src/eStaticSkyLight_GetDirectLight.cpp` (67 lines)
- `src/eSimulatedController_GetVelocity.cpp` (64 lines)
- `src/eBipedController_GetVelocity.cpp` (59 lines)
- `src/eMultiSphereShape_GetProjectedMinMax.cpp` (54 lines)
- `src/eCapsuleShape.cpp` (41 lines)
- `src/eHeightmapShape_GetInertialTensor.cpp` (15 lines)
- `src/eHeightmapShape_GetAABB.cpp` (14 lines)

**Impact:** These files are counted as "matched" (inflating the 709 count) but provide zero value for fine-tuning. Some may be reworkable as C++ with minimal inline asm; others (dense VFPU) should be re-marked as `failed`.

**Fix:** Each file needs human triage — determine if it's C++-expressible (rework) or genuinely VFPU-only (mark `failed`). The duplicate mBasis::Orthonormalize in mQuat.cpp will cause link errors if both files are included.


## 3. Substring symbol gate has accepted ~80 wrong-signature matches over the project's history

**Found:** 2026-04-24 while fixing the AMBIGUOUS-overload bug

The verifier's name gate (`tools/byte_match.py:sym_encodes_func`) matches a compiled symbol against a DB entry by class+method substring with a 1–3 character gap, ignoring the parameter-type suffix of SNC mangling. For overloaded methods that compile to identical bytes (common for trivial 28–44 byte stubs that delegate to shared template machinery), this is fine for the AMBIGUOUS rejection case (loud failure, fixed in a separate commit by adding `mangled_symbol` disambiguation).

The deeper issue: when the substring gate yields a **single** byte-matching candidate that happens to be the wrong overload (or a function with reordered/wrong parameter types that still compiles to identical MIPS bytes because all parameters live in registers), it has been silently accepted as a verified match.

**Evidence:** Of 1293 currently-matched DB entries, 80 have a stored `symbol_name` that disagrees with the `.sym` authoritative `mangled_symbol`. Most disagreements are benign:
- `cFile__OnCreated` vs `__0fFcFileJOnCreatedv` (our .o uses safe_name; .sym uses mangled — same function, different naming style)
- `eShape___dtor_eShape_void` vs `__0oGeShapedtv` (our patched-post-splat dtor name vs original `dt` form)

But several are concerning, e.g.:
- `0x00047f80 eDynamicModel::SetSkin(...)`: stored `__0fNeDynamicModelHSetSkin6IcHandleT76FeSkin_iTB6KcTimeValue` vs .sym `...iTC6KcTimeValue` — type back-reference index differs, indicating a different parameter-type sequence.
- `0x0003b4ec eInputMouse::BeginDrag(...)`: stored `__0fLeInputMouseJBeginDragRC6FmVec2iTCT` vs .sym `...RC6FmVec26GeColor6IcHandleT76JeMaterial_T` — substantially different parameter encodings.

These look like real wrong-signature matches: the bytes happened to align under MIPS register-passing, but the C++ signature in our reconstruction differs from the original.

**Impact:** Up to 80 entries in functions.json may be marked "matched" with a source file that doesn't accurately reflect the original function's signature. Training data poisoning risk — a fine-tune would learn incorrect parameter mappings.

**Possible fix:**
- Audit the 80 mismatches, classify each as "naming-style only" (benign) vs "wrong signature" (must re-verify)
- For the wrong-signature subset, revert match status to `failed` and rework the source
- Long-term: tighten the verifier's gate to require exact `mangled_symbol` match for `.cpp` reconstructions (currently only used as an AMBIGUOUS tie-breaker)

**Detection script** (one-shot, addr-list output):
```python
import json, subprocess
db = json.load(open('config/functions.json'))
out = subprocess.run(['mipsel-linux-gnu-nm','--defined-only','extern/extracted_symbols/Game-dvd.sym'],
                    capture_output=True, text=True).stdout
sym_map = {int(p[0],16): p[2] for ln in out.splitlines()
           if (p := ln.split(None, 2)) and len(p)==3 and p[1] in ('T','t','W','w')}
for f in db:
    addr = int(f['address'],16); stored = f.get('symbol_name'); sym = sym_map.get(addr)
    if stored and sym and stored != sym:
        print(f['address'], f['name'], '|', stored, 'vs', sym)
```

## 4. Orchestrator's git_commit_batch absorbs operator-staged changes into session commits

**Found:** 2026-04-24 during fix_plan UI work — codex session was running while operator staged unrelated TUI changes. The orchestrator's per-session commit landed with title "Match 2 functions (session XXXX)" but the diff included `tools/ui/state.py` and `tools/ui/screens/running.py` changes that had nothing to do with the matched functions.

**Mechanism:** `tools/orchestrator.py:git_commit_batch` builds a set of files-to-commit (`matched_files | _session_dirty_paths()`), runs `git add` on each, then does a plain `git commit -m "..."`. Anything else the operator had staged at that moment (or files that became staged via `git add` of a file that overlapped with their work) ends up in the same commit.

**Impact:** silent. The matched-function commit includes drive-by changes the agent didn't make. The commit message lies. Two real cases observed in `4972e53` and `481a283` on `overnight/20260424-201228`. No data loss — the changes are persisted, just attributed wrong.

**Fix:** scope the commit to the explicit file list. Replace `git commit -m MSG` with `git commit -m MSG -- <each path>`, OR `git stash --keep-index` before staging + `git stash pop` after committing to isolate the session's staging area.

**Workaround for operators**: don't stage code changes while an overnight run is active. If you must, `git stash` first.
