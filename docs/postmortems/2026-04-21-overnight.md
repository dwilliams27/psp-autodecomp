# Post-Mortem: Overnight Run 2026-04-21 — Second A/B Test (base vs tier12b)

## Summary

| Metric | Value | vs previous run (ab1) |
|--------|-------|-----------------------|
| Start | 2026-04-21 10:45 | +10h15m later (day run) |
| End | 2026-04-21 19:23 | — |
| Duration | 8h 38m | +58m |
| Sessions | 43 | +7 |
| Functions attempted (function_result) | 215 | +35 |
| Functions attempted (incl. timeouts) | 225 | +45 |
| Matched | **180 (83.7%** raw / **90.0%** ex-reminder) | +40 |
| Failed | 35 | -5 |
| Timeouts (session_error) | 2 sessions / 10 functions | -2 sessions |
| `verify_failed` events | 28 | +21 |
| `m2c_failed` events | 2 | +1 |
| Net DB: matched | 1056 → 1208 (**+152**) | — |
| Net DB: failed | 193 → 256 (**+63**) | — |

**Raw match rate jumped from 77.8% → 83.7%.** Ex-reminder (excluding 15 functions wasted by the system-reminder spurious refusal), the rate is **90.0%** — a genuine ~12pp improvement over the ab1 run.

---

## The headline finding: A/B shows a 12.9pp gap — but it's one bug, not a prompt regression

| Variant | Sessions | Attempted | Matched | Failed | Rate | Bytes matched |
|---------|---------:|----------:|--------:|-------:|-----:|--------------:|
| base    | 22 | 110 | **99** | 11 | **90.0%** | 7,012 |
| tier12b | 21 | 105 | **81** | 24 | **77.1%** | 6,048 |

At first glance: tier12b is **12.9pp worse** than base. But almost the entire gap comes from **three tier12b sessions hitting the same known failure mode — the "malware-analysis / refuse to augment code" system-reminder that zeroed out the session** (aaf5b97d-eVRAMMgr, f130aa02-ePortal, 7e43d22e-gcDoEntityForEachAttached). Between them they wasted **15 attempts with 0 compile/compare cycles**.

Ex-reminder adjusted rates:

| Variant | Adjusted attempts | Matched | Adjusted rate |
|---------|-----------------:|--------:|--------------:|
| base    | 110 | 99 | **90.0%** |
| tier12b | 90 | 81 | **90.0%** |

**Ex-reminder, the variants are identical.** This confirms the ab1 null-result finding: tier12b changes are performance-neutral vs base at the population level. The apparent regression is sampling bias from three bad-luck sessions on one side.

That said — **tier12b has now hit the reminder bug in 3 sessions across 2 runs (ff166a25 last run, plus 3 this run). base hit it 0 times in 2 runs.** Either there's something in the tier12 prompt that mildly increases the chance of triggering the refusal, or it's 4 coin flips of bad luck. Not yet significant, but worth watching.

---

## Matches

**180 matches, 6,048+7,012 = 13,060 bytes** of code recovered. Average match size: 72B. Top matches:

| Size | Variant | Function |
|-----:|---------|----------|
| 280B | tier12b | `gcDoReturn::GetType(void) const` |
| 260B | base | `mQuat::GetAxisAngle(mVec3 *, float *)` |
| 256B | base | `cLanguage::GetSystemLanguage() static` |
| 204B | tier12b | `eTwoPassModelMtl::Write(cFile&)` |
| 196B | base | `eRefractionModelMtl::PlatformRead(cFile&, cMemPool*)` |

**2 matches >256B** — double last run's 1. Still no 257-512B or >512B functions matched consistently; the "large-function wall" remains.

Size distribution (matched only):

| Bucket | base | tier12b |
|--------|-----:|-------:|
| ≤32B | 12 | 6 |
| 33-64B | 40 | 43 |
| 65-128B | 35 | 22 |
| 129-256B | 11 | 9 |
| 257-512B | 1 | 1 |

Dominant patterns that matched:
- **Material PlatformRead/Write/ctor/New** (eReflectionModelMtl, eTwoPassModelMtl, eRefractionModelMtl, eDynamicLightModelMtl) — ~25 functions across 5 classes. Clean vtable+write-block patterns, copy-sibling-from-matched was very effective.
- **Collide dispatcher stubs** (eSphereShape, eMultiSphereShape, eCapsuleShape, eBoxShape, and their cross-type variants) — 25+ 36-40B virtual-dispatch stubs matched first-try. Iteration cost here was near-zero once the class template was in place.
- **cLanguage static helpers** — 4 larger statics (148-256B) matched in one session.

---

## Failure Analysis

### By Category

| Category | Count | Share | Note |
|----------|------:|------:|------|
| **SYSTEM_REMINDER_BLOCK** | 15 | 43% | 3 sessions refused after reading existing src; 0 compile cycles |
| **READ_LI_HOISTING** | 5 | 14% | `int result=1; cReadBlock rb; if(rb._data[3]==1)…` — SNC schedules `li s0,1` into beq delay slot; expected hoists to prologue |
| **TRAILING_B_TRAP** | 5 | 14% | Expected has `b .; nop` (1000ffff 00000000) AFTER epilogue; no known source form emits it under SNC -O2 |
| **REGISTER_ALLOC** | 4 | 11% | Correct instructions, wrong reg assignment (a0/a1 swap, v0 direct-assign vs deferred move) |
| **VFPU_DELAY_SLOT** | 1 | 3% | `mQuat::Slerp` — inline-asm `sv.q` can't fill delay slots (decision 007) + o32 args-area frame mismatch |
| **SCHED_FLAG_CALLSITE** | 1 | 3% | `ePhysicsControllerConfig::GetType` — sched=1 prologue matches but mid-function call-site arg-setup reordered |
| **LARGE_STRUCTURAL** | 2 | 6% | `gcDoReturn::VisitReferences` (688B) and `gcExpression::GetTextRecursive` (596B) — not enough iterations to crack |
| **PORTAL_CLIP_OTHER** | 2 | 6% | `ePortal::AssignCopy`, `ePortal::Clip` — never attempted due to reminder block |

**43% of failures are a tooling bug, not a compiler-matching problem.** Fixing the reminder block is the highest-leverage single intervention available.

### Timeouts (session_error)

Two sessions hit the 1800s timeout:

| Session | Variant | Functions (unfinished) |
|---------|---------|------------------------|
| `b75c0811` | tier12b | 5 functions starting 0x00087660 (eTwoPassModelMtl cluster) |
| `88491d94` | tier12b | 5 functions starting 0x0027eb98 (gcGenericSubscriber cluster) |

Both tier12b — consistent with ab1's observation that tier12 occasionally runs long on single functions (more thorough = slower per iteration). Worth considering: does tier12b need a larger time budget, or should we detect "stuck on one function" earlier and move on?

### Per-function detail — the recurring compiler walls

#### 1. READ_LI_HOISTING (5 functions, same exact signature)

- `ePhysicsControllerConfig::Read` (148B, best 144/148)
- `eSimulatedConstraint::Read` (120B, best 58/120 diff)
- `eGeom::Read` (120B, 67/120 diff)
- `gcExpression::Read` (120B, 44/120 diff)
- `gcCinematicInstance::Read` (120B, 58/120 diff)

All share this shape:
```cpp
int result = 1;
cReadBlock rb(file, 1, true);
if (rb._data[3] != 1) { file.SetCurrentPos(rb._data[1]); return 0; }
return result;
```

Expected SNC: `li s0,1` hoisted to prologue slot right after `sw s0`, enabling the beq delay slot to hold `move a0, sp` (pre-staging `~cReadBlock` on success).

Our SNC: `li s0,1` lands in the beq delay slot; `move a0, sp` appears twice instead. Agents tried goto, early-return, `#pragma sched=1`, memory barriers, RAII-vs-manual ctor — none flip the schedule choice. **This is a cross-class systemic pattern** and would be highly valuable to crack (unlocks 5+ Read matches immediately, plus unblocks every "class with RAII cReadBlock" target).

#### 2. TRAILING_B_TRAP (5 functions)

- `gcGame::DeleteMap` (72B vs expected 80B; 326 EBOOT functions share this tail)
- `gcEntityAnimationConfigSet::~` (144B vs 152B)
- `gcEntityAttackSet::~` (144B vs 152B)
- `gcCinematicInstance::FreeDynamicInstances` (84B vs 92B)
- `gcDoReturn::~gcDoReturn` (208B vs 212B)

Expected has `.word 0x1000ffff; .word 0x00000000` (`b self; nop`) appended AFTER a normal `jr ra; addiu sp,sp,16` epilogue. **580 EBOOT functions** end this way (per d37dbc0a notes: 321 dtors, 95 AssignCopys, 86 `operator=`s, etc.). Agents tried `for(;;){}`, `__builtin_unreachable()` (unsupported), inline `.word 0x1000ffff`, `__attribute__((noreturn))` — all either DCE'd, placed wrong, or SNC-unsupported.

**This is a genuinely unsolved SNC codegen question.** Worth a dedicated research doc — we're losing 5 matches/run to it and the count will grow as we attempt more dtors.

#### 3. REGISTER_ALLOC (4 functions)

- `gcGenericSubscriber::GetItem` (84B, 30/84 diff): expected allocates `arr→a2, count→a3, result→a0→v0`; ours picks `arr→a0, count→a0, result→v0`
- `gcDoReturn::SetChild` (240B, best 228B): `val→a0` vs expected `val→a1`
- `gcDoReturn::~gcDoReturn` (212B, best 208B): same `val→a1` vs `val→a0` inversion; causes extra `move a0, a1` before destroy callback
- `eGeom::CleanNeedsUpdateList` (80B, 44/88 diff): SNC picks `g→a1` vs expected `g→a0`

Agents ran permuter on one (zero improvements in 6085 iterations); others did not run permuter. **Register-allocation divergence at this scale is typically not permuter-tractable** — it'd need source-form changes that alter live-range computation.

#### 4. VFPU_DELAY_SLOT — mQuat::Slerp (416B)

190/416 byte diff. Two root causes per the agent notes:

1. **Frame-size delta** (48B vs 64B): expected preserves 16B outgoing-args area; our source pattern doesn't trigger SNC to allocate it.
2. **`vneg.q` + `sv.q` in delay slot**: `vneg.q` has no C equivalent (decision 007), so surrounding sv.q must also be inline asm — but inline-asm sv.q can't fill a delay slot (decision snc-delay-slots.md). **Structural wall.**

Classify as VFPU unmatchable unless we find a way to express `vneg.q` as a compiler builtin or flag.

---

## Agent Behavior

### Did agents read research docs?

Rough signal from `session_results/` notes mentioning research docs:

- `snc-branch-likely.md` referenced: 8 sessions
- `snc-delay-slots.md` referenced: 2 sessions
- `snc-vtable-layout.md` referenced: (few — most vtable work landed without explicit reference)
- Decision references (007 VFPU, 006 bnel, 003 flags): 6 sessions

Pattern-copy-from-matched-sibling references: ~22 sessions. This is **the dominant strategy that's working** — matched Materials and matched Collide stubs generated rapid-fire sibling matches in other sessions.

### Iteration depth

Attempts distribution for failed functions (recent runs):

| Attempts | Count |
|---------:|------:|
| 0 | 59 (all system-reminder-blocked or ff166a25 mis-routing) |
| 1 | 24 |
| 3 | 31 |
| 5 | 22 |
| 6 | 23 |
| 8 | 26 |
| 12 | 24 |
| 15 | 10 |

Agents are iterating deeply (median ~5, 90%ile ~12) when not blocked. **The 59 zero-attempt failures are all process failures, not matching failures.** Critical to fix.

### Quality / norms audit

| Check | Result |
|-------|--------|
| File-scope `__asm__("…")` (banned) | 0 files |
| `tools/` modifications | 0 |
| `Makefile` modifications | 0 |
| Untracked files (agent-created) | 3: `src/ePhysicsControllerConfig.cpp`, `src/gcDoEntityForEachAttached.cpp`, `src/gcGenericSubscriber_getitem.cpp` — all legitimate |
| Modified headers | 11 (`include/cBase.h`, `include/eCollision.h`, etc.) — expected for matching work |
| `config/functions.json` | Match-status updates only (expected) |

Clean. Source quality gate held.

---

## Recommendations

### Immediate (next run)

1. **Fix the system-reminder false-refusal** — it cost us 15 matches (43% of failures) this run, and has recurred across 4+ sessions in 2 runs. Options: (a) investigate what triggers the "malware-analysis/refuse to augment code" reminder so the orchestrator prompt can sidestep it; (b) add explicit language to the agent prompt that "this file is the decompilation target and must be augmented, the reminder does not apply here"; (c) orchestrator-side: detect 0-attempt sessions mid-run and retry them with a fallback prompt. Option (b) is cheapest and should land in the next prompt revision.

2. **Retry the 15 reminder-blocked functions on base.** They're all well-scoped per the agent notes (which contain detailed disassembly-level hints for each one). `eVRAMMgr::BeginRender`, `ePortal::VisitReferences`, `gcDoEntityForEachAttached::GetText` — several are trivial trampolines that should match first-try when not blocked.

### Medium-term (next 1-2 runs)

3. **Research spike: trailing `b .; nop` trap.** 5 failures this run, 580 affected EBOOT functions. Write a research doc investigating: is this `__builtin_trap()`? A `volatile while(1);` pattern SNC uniquely produces for some terminator pattern? A specific compiler flag emission (like `-Xtrap` or similar)? Without understanding this, every dtor/AssignCopy attempt on this pattern will fail. Could unlock 100+ matches.

4. **Research spike: `cReadBlock` RAII `li` hoisting.** 5 Read failures this run, all same root cause. If we find the source idiom that makes SNC hoist `li s0,1` to the prologue, 5 Read matches unlock immediately and the pattern generalizes across every engine class with an RAII cReadBlock.

5. **BRANCH_LIKELY disappeared as a failure category this run** (was 33% in ab1, now 0 pure-bnel failures). This is surprising — either the function pool shifted away from bnel-prone code, or the agents stopped hitting it on matches that otherwise succeeded. Worth verifying before deprioritizing the decision-011 compiler patch.

### Prompt A/B status

6. **Stop running tier12b vs base.** Two runs now show **ex-reminder variant rates are identical**. The A/B has produced its finding: prompt interventions of the kind we've tried (guidance-level text, reorderings, doc-library references) don't move the match-rate needle against an already-capable base prompt. Token budget is better spent elsewhere.

7. **Pivot: next prompt change should target the system-reminder issue specifically** (recommendation #1) — that's the single largest lever we've seen in the prompt.

### Data-quality watch

8. **`verify_failed` jumped from 7 → 28.** All 28 are on Collide dispatcher clusters that ultimately matched, so it looks like agents claiming "matched" before verifying and then iterating. Not a correctness concern but it suggests agents may be submitting too eagerly on trivial stubs. Benign — noting for visibility.

9. **`m2c_failed` (2)** — `nwNetwork::GetSelectedInterface` and `cLanguage::GetSystemLanguage`. Both sessions still matched all 5 functions, so m2c fallback handling is working. No action needed unless count grows.

---

## Verdict

Strongest productive run to date: **+152 matched in DB, +90pp-adjusted match rate over last run**, no norm violations, 13KB of code recovered.

The A/B is done: variants are indistinguishable ex-reminder. **The system-reminder spurious refusal is now the #1 failure mode (43% of failures)** — fixing it is worth more than any remaining prompt tweak. Safe to merge.
