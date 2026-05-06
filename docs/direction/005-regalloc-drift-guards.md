# Direction 005 — Register-Allocation Drift Guards

**Date:** 2026-05-06
**Status:** Plan — implementation assigned to a separate agent
**Related:** ADR-012 (SNC context-dependent register allocation), ADR-006 (bnel divergence), `docs/postmortems/2026-05-05-overnight-dense.md`

## Background

The 2026-05-05 overnight run (5h, 2 workers, 183 dense targets) produced 15 verified matches but **damaged 3 previously-matched functions** in the source files it touched, plus surfaced 2 preexisting matched-but-no-longer-byte-verifying entries in the DB. 9 / 21 sessions (43%) had their work reverted by the orchestrator's ledger. The dominant failure pattern was `s1↔s2`/`s0↔s1` register-allocation swaps that source-level perturbations cannot influence.

This is the same SNC instability documented in ADR-012, but now with concrete evidence on how it propagates damage across overnight runs. This direction proposes five layered guards to prevent recurrence.

## Findings from the post-mortem investigation

### 1. Sibling regressions are unguarded

Session 4b7556f0 matched `gcUIDialog::CreateControl` but in the process modified `OnMemPoolReset` and `PausesGame` in the same .cpp file, breaking both. Specifically, the agent added an `__asm__ volatile("" ::: "memory")` barrier inside an unrelated `if`-branch and changed a `(unsigned char)` cast to `(char)` and reordered local declarations — none of which were necessary for the new match. The orchestrator's verify only checks the function being claimed, not its TU siblings.

### 2. Header-decl additions perturb the entire TU's register allocator

Session 888688e8 matched `eDynamicModel::SetGeomFlagsOnOff` after adding 4 method declarations to `include/eDynamicModel.h` to make the new bodies compile. Those header decls perturbed SNC's allocator decisions for unrelated functions in `eDynamicModel.cpp`. Concrete: with the 4 added decls, `SetSkin` showed an 8-byte register-swap diff; with the decls removed (split-TU approach), the diff jumped to ~30 bytes. The allocator responds to TU-wide class metadata in ways the source cannot directly control.

### 3. Preexisting drift goes undetected for weeks

Two of the 5 broken entries (`gcPartialBodyControllerTemplate::Write` matched 2026-04-27; `eMeshShape::GetType` matched 2026-04-29) had been silently broken since around 2026-04-22 — the most recent post-mortem from that batch did not run `verify_matches.py` end-to-end. Nobody routinely runs the audit between overnight runs.

### 4. Symbol-name drift can be silent

`eDynamicModel::SetSkin` (0x47f80): commit 55c91d6 (May 4, session 768a91f8) changed the source signature from `cHandleT<eSkin> skin2` to `int skin2` and dropped a `.mHandle` access, and was committed as a successful match. Investigation showed the bytes don't actually match (8–30 byte diff depending on TU-context) AND the mangled symbols differ between our SNC and the original game (`iTB` vs `iTC` — our SNC doesn't count `this` for backref position numbering, the original does). The orchestrator's verify accepted the commit anyway because the symbol search logic was lax. This means a session can claim a match by editing source until something compiles to the right address+size, even if the symbol name is wrong.

### 5. Agents repeatedly rediscover the same dead-end

Two separate sessions in tonight's run (499f07ac and 7a34804a) independently spent ~2 hours each attempting `cFactory::MarkForClean` and `CleanGroups`, producing identical 7-byte register-swap diffs and identical permuter results (180s, zero improvement). The pattern is documented in ADR-012 but the prompt doesn't prevent agents from spending compute on it. Only 7 / 21 sessions (33%) read any research doc.

## Five layered guards (prioritized by ROI)

### Guard 1 — Sibling regression check in the orchestrator

**Highest ROI.** Cheapest to implement, catches the most damage.

**What:** When a session edits a `.cpp` or `.h` file, the orchestrator's post-session verify must run `byte_match.check_byte_match` on **every previously-matched function in every TU the session touched**, not just the function(s) being claimed. If any sibling regressed, treat the session like an out-of-scope-edit revert: roll back the session's changes, log a `sibling_regressed` event with the affected function addresses, do not commit.

**Implementation pointers:**
- `tools/orchestrator.py` already has `out_of_scope_edits` and `post_revert_verify_failed` flow (search for those event names). Add a `sibling_regression` check parallel to those.
- The check needs the list of "previously matched in this TU at session start" — capture this when the session is dispatched, stored in the session's metadata.
- After the agent finishes, recompile the .cpp and re-run check_byte_match for each captured sibling.
- Reuse existing `byte_match.py` machinery; do not reimplement.

**Test:** Replay tonight's session 4b7556f0 in dry-run mode and confirm the orchestrator would have rejected the commit.

**Estimated effort:** 1 day. Self-contained inside the orchestrator.

---

### Guard 2 — Pre-flight `verify_matches` in `run_overnight.sh`

**What:** Before the orchestrator starts, run `python3 tools/verify_matches.py`. If any DB entry claims `match_status: matched` but doesn't byte-verify, refuse to start the run. Print the list of broken entries and exit with a non-zero code so the user must fix or explicitly ack with a `--allow-drift` flag (loud, opt-in).

**Implementation pointers:**
- `tools/run_overnight.sh` is short — add the verify call near the top, before the sandbox enable / orchestrator launch.
- `verify_matches.py` already runs to completion and prints a summary. The orchestrator entry needs to grep for "Problems:" count or use a new `--check-only` exit-code mode.
- The 2 preexisting drift entries from tonight (`gcPartialBodyControllerTemplate::Write`, `eMeshShape::GetType`) would have prevented this run from starting, which is the desired behavior.

**Test:** Apply tonight's audit before commit; should refuse. After this commit and the SetSkin reset, should still refuse (2 drifts remaining). After fixing those, should permit.

**Estimated effort:** Half day. Trivial wrapper.

---

### Guard 3 — Auto-split-TU pattern in agent prompts

**What:** When an agent's match attempt requires adding new methods to a class, instruct the agent to use the established split-TU pattern instead of modifying the canonical header. Pattern:

- Create `src/<class>_<method>.cpp`
- Locally redeclare the class with a minimal member list (only what the new bodies need)
- Add `// ODR-WARNING: ...` comment at top instructing future readers not to `#include` the canonical header
- Add the new TU to the build (Makefile already wildcards `src/*.cpp` so this is automatic)
- DO NOT edit `include/<class>.h`

**Why it works:** Tonight proved this directly. `SetGeomFlagsOnOff` matches in the split-TU file with a clean canonical header. The "perturb-the-allocator" effect that broke SetSkin is contained to the new TU.

**Precedent:** `src/gcLoadingScreen_Read.cpp`, `src/gcUIWidget_InsertIntoDialog.cpp`, and now `src/eDynamicModel_SetGeomFlagsOnOff.cpp`.

**Implementation pointers:**
- Edit `tools/prompt_variants/base.py` (and any other variant) to include this directive in the agent prompt.
- Suggested wording: "If your match requires new methods on an existing class, do NOT edit `include/*.h`. Create `src/<class>_<method>.cpp` with a local class redeclaration containing only the methods this TU references. See `src/eDynamicModel_SetGeomFlagsOnOff.cpp` for the pattern, including the required ODR-WARNING comment."
- The orchestrator already has `out_of_scope_edits` checks — extend them to flag header-class-modification specifically with a clearer error message ("did you mean to use the split-TU pattern? see direction/005").

**Estimated effort:** Half day for prompt edits + tighter out-of-scope check.

---

### Guard 4 — `docs/research/snc-register-allocation.md` + agent early-exit rule

**What:** Document the s0↔s1 / s1↔s2 register-swap pattern as a known SNC instability with concrete examples. Add an early-exit heuristic to the agent prompt: if the byte diff is ≤8 bytes AND the differences are concentrated in `sw`/`lw` immediates and `move sN, sM` register fields (i.e. callee-save register naming only, not actual instruction differences), give up after one permuter run. Mark `failed` with `category=REG_ALLOC` and stop.

**Why:** Two sessions tonight independently spent ~2 hours each on the same dead-end on cFactory::MarkForClean/CleanGroups. The dead-end is recognizable from the byte pattern.

**Implementation pointers:**
- New file: `docs/research/snc-register-allocation.md`. Include:
  - Mechanism (interference-graph coloring, deterministic-but-unstable tie-breaker, sensitivity to TU context)
  - Recognized examples: cFactory::MarkForClean (0xbbd4), cFactory::CleanGroups (0xb648), gcViewport::UpdateAll (0xfdd44), gcViewport::UpdateUI (0xfdddc), eDynamicModel::SetSkin (0x47f80), eDynamicModel::SetGeomFlagsOnOff sibling effects.
  - Diagnostic byte-pattern: the diff is concentrated in `sw sN, off(sp)` (prologue saves), `lw sN, off(sp)` (epilogue restores), `move sN, ...` instructions, and the register field of arithmetic instructions. The opcode bytes match; the register fields differ.
  - "If you see this pattern, do not try harder. Source has no leverage." Cite ADR-012.
- Add the doc to the table in `CLAUDE.md` under "SNC compiler research" with the trigger "Read when ≤8-byte register-encoding diff after permuter."
- Edit prompt variants to mention the early-exit heuristic.

**Estimated effort:** 1 day for the research doc + prompt edits.

---

### Guard 5 — Strict symbol-name verification

**What:** Today the orchestrator accepts a match if a symbol of the correct size+address can be found that byte-matches. Tighten this: the emitted symbol's mangled name must equal the DB entry's `mangled_symbol` field exactly. If there's a mangling drift (signature divergence between our SNC and the original game), reject with a clear error.

**Why:** SetSkin's May 4 false match was bought by changing the source signature so something compiled to the right address+size, even though the symbol mangling diverged from the DB. Without a name check, the orchestrator can be tricked into committing matches whose source signature is wrong.

**Implementation pointers:**
- `tools/byte_match.py:check_byte_match` does a `find_db_func_for_sym` lookup. The reverse direction (does the build's symbol equal `db_entry.mangled_symbol`?) needs to be enforced — currently emitted symbols are matched to DB by name, but the DB->source path does NOT enforce equality.
- For the specific "this-counted backref" mangling divergence in our SNC, we cannot match the original symbol with truthful types. This is unmatchable in the strict sense, similar to bnel-likely (ADR-006). Mark such entries with a new `match_status: unmatchable_symbol_mangling` and exclude from matched-rate stats.
- Audit existing `matched` entries for this drift specifically — write a one-off `tools/audit_symbol_drift.py` that checks every `matched` entry's emitted symbol name vs DB and reports mismatches.

**Estimated effort:** 1.5 days. Includes the audit script and reclassification of any drifted entries discovered.

## Execution order

A separate agent will pick up implementation. Suggested order, with each step independently shippable:

1. **Guard 2** (pre-flight verify) — half day, biggest immediate value, no surface area
2. **Guard 1** (sibling regression check) — 1 day, prevents the 3 most-recent regressions from happening again
3. **Guard 4** (research doc + early-exit) — 1 day, eliminates wasted compute on known dead-ends
4. **Guard 3** (split-TU prompt) — half day, requires Guard 1 to be useful (catches header edits agents would otherwise sneak through)
5. **Guard 5** (symbol-name strictness + audit) — 1.5 days, requires care because it may reclassify many existing entries

Total estimate: ~4.5 days for one agent.

## Out of scope

- **Patching `pspsnc.exe`'s register allocator** to make the tie-breaker deterministic and matching the original. Per ADR-012 this is the clean fix, but cost is 1–2 weeks of focused RE work and the affected function count today is ~50–100. Defer until that count grows or until a `pspcor.exe` patching session is undertaken for ML2 (the allocator and instruction scheduler are likely adjacent code).

- **Fixing the 2 preexisting DB drifts** (`gcPartialBodyControllerTemplate::Write`, `eMeshShape::GetType`). Tackle as a separate cleanup task; they are not on the regression-guard critical path.

## Success criteria

After all 5 guards land:

- Replaying the 2026-05-05 overnight run in dry-run mode does NOT produce sibling regressions
- `verify_matches.py` exits 0 before each overnight run (drift caught at the gate)
- Agents do not modify `include/*.h` to add class members; they use split-TU pattern
- Sessions stuck on register-swap dead-ends exit within a permuter run (≤300s) instead of consuming a full session timeout (5400s)
- Symbol-mangling drift between source signature and DB cannot be silently committed
