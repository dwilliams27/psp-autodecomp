# Research TODOs

Open research spikes worth taking on to unlock classes of failures. Each entry: the pattern, observed impact (approximate function count affected or failures-per-run), what's been tried, candidate next steps. Convert to a full `docs/research/snc-*.md` doc once understood.

---

## 1. Trailing `b .; nop` trap after epilogue

**Pattern:** Expected function ends with a normal `jr ra; addiu sp,sp,N` epilogue, then 8 extra bytes: `.word 0x1000ffff; .word 0x00000000` (= `b self; nop`). SNC under -O2 on plain C/C++ does not emit this trailer.

**Scale:** **~580 EBOOT functions** end with this pattern (per 2026-04-21 run notes):
- 321 destructors
- 95 `AssignCopy`s
- 86 `operator=`s
- 9 `SetSize`s
- 11 `Read`s
- plus various

**Impact:** 5+ match-blockers per overnight run. Will grow as we attempt more dtors/AssignCopy targets. Currently blocks every dtor on a class whose original emitted this trailer.

**Failures observed (2026-04-21 run):**
- `gcGame::DeleteMap` (72B vs expected 80B — trailer is the whole 8B diff)
- `gcEntityAnimationConfigSet::~dtor` (144B vs 152B)
- `gcEntityAttackSet::~dtor` (144B vs 152B)
- `gcCinematicInstance::FreeDynamicInstances` (84B vs 92B)
- `gcDoReturn::~gcDoReturn` (208B vs 212B)

**What's been tried (agents, 2026-04-21):**
- `for(;;){}` at function end — emits the trap but branches to trap from an earlier offset (breaks 1/N byte)
- `__builtin_unreachable()` — unsupported by SNC
- Inline `__asm__(".word 0x1000ffff\n.word 0x00000000\n")` — compiler inserts BEFORE epilogue, not after
- `__attribute__((noreturn))` on inner calls — no effect on function-end layout
- `.set noreorder; b 1b; .set reorder` — same placement problem
- `goto end; end: return; for(;;){}` — DCE eliminates the infinite loop

**Candidate next steps:**
1. **Hypothesis: SN Systems-specific safety epilogue flag.** Check SNC manual / pspsnc.exe `--help` for a trap-after-return flag. Something like `-Xtrap`, `-Xterminator`, `-Xsafety`.
2. **Hypothesis: It's a compiler-emitted dead-code filler for alignment.** Check: does the next function always start at 16-byte alignment? If yes, the trap may be compiler-inserted padding. If no, it's a semantic emission.
3. **Hypothesis: It comes from a specific source construct** — maybe an explicit `PSP_ASSERT()`, `assert(false)`, `HALT()` macro, or similar that ViciousEngine defined. Search for any engine-specific panic/halt macros in the decompiled Sony SDK headers (`extern/include/`) and see if any of those expand to `__trap()` under SNC.
4. **Experiment: try `volatile int *p = 0; *p = 0;`** — a forced null-deref at function end. Some compilers emit a trap sequence for this.
5. **Cross-reference:** pick one function where this trailer is a 100% match-blocker (e.g. `gcEntityAttackSet::~` — body matches 144/144 bytes, ONLY the 8-byte trailer diffs) and run permuter variants specifically on function-end constructs.

**Success criterion:** one working source pattern that makes SNC -O2 emit the 8-byte trailer after a normal epilogue. Unlocks 100+ match candidates immediately.

---

## 2. `cReadBlock` RAII `li` hoisting

**Pattern:** Engine `Read()` functions use an RAII `cReadBlock` to read a magic-tagged block from a file. The source shape appears to be:

```cpp
int result = 1;
cReadBlock rb(file, 1, true);
if (rb._data[3] != 1) {
    file.SetCurrentPos(rb._data[1]);
    return 0;
}
return result;
```

Expected SNC schedules `li s0, 1` (initializing `result`) in the **prologue** — immediately after `sw s0, 20(sp)` and before the `cReadBlock` ctor call args are marshalled. This makes the beq delay slot available for `move a0, sp` (pre-staging `~cReadBlock()` on the success path).

Our SNC schedules `li s0, 1` into the **beq delay slot**, causing `move a0, sp` to appear twice (once in each branch target).

**Scale:** Every Read() method on every `cFile`-serializable class uses this idiom. Estimated **100+ Read functions** across the engine.

**Impact:** 5 Read match-blockers in the 2026-04-21 run alone — all same root cause, all same diff shape, all ~50% byte mismatch despite identical IR.

**Failures observed (2026-04-21 run):**
- `ePhysicsControllerConfig::Read` (148B, best 144B 4-short)
- `eSimulatedConstraint::Read` (120B, 58/120 diff)
- `eGeom::Read` (120B, 67/120 diff)
- `gcExpression::Read` (120B, 44/120 diff with permuter)
- `gcCinematicInstance::Read` (120B, 58/120 diff)

**What's been tried (agents, 2026-04-21):**
- `int result = 1; … return result;` — `li s0,1` stuck in delay slot
- `goto success; … success: return 1;` — same
- Early-return `if (rb._data[3] == 1) return 1; …` — still delay-slot, inverted branch sense
- `#pragma control sched=1` — no effect on this placement
- `__asm__ volatile("" : "+r"(result));` — optimizer erases the barrier
- `__asm__ volatile("" ::: "memory");` after `int result = 1;` — reduced diffs 67→58 but didn't flip the hoist
- Explicit result=0 on fail path (changes `move v0,zero` wrong way)

**Candidate next steps:**
1. **Inspect a matched Read (if any exist) for the source form.** If one of the already-matched 1208 functions is a `cFile::Read` with this shape, its source is the Rosetta Stone. Grep `src/**/*.cpp` for matched Reads (query func_db.py for class=X method=Read status=matched) and copy.
2. **Hypothesis: `cReadBlock` was originally a macro, not a class.** If it expanded inline instead of a ctor call, the `li s0,1` would naturally precede the inlined arg setup. Test: replace RAII with explicit `cReadBlock rb; rb.OpenRead(file, 1, true); … rb.Close();` pattern.
3. **Hypothesis: `result` is actually the cReadBlock's valid-bit copy** — i.e., the original source reads `result = rb.IsValid()` or similar, so `result` is live at `_data[3]` read time. Test: `int result; cReadBlock rb(file,1,true); result = (rb._data[3] == 1); if (!result) { …; return 0; } return 1;` → SNC may assign `result` earlier if it's read-dependent.
4. **Hypothesis: `-Xsched` variant.** Only `-Xsched=2` (default) and `-Xsched=1` have been tried per-file. Try `-Xsched=0` and combinations with `-Xmopt` / `-Xxopt` on ONE Read function systematically.
5. **Permuter with function-only flags.** Permuter uses base CFLAGS — running it on this pattern for 30+ minutes (vs the 5-min standard) may find a source form that flips the hoist.

**Success criterion:** one source form that makes SNC -O2 schedule `li s0,1` in the prologue of a cReadBlock-Read function. Unlocks an estimated 50-100 Read matches class-wide.

---

## 3. (lower priority) `move`-in-prologue scheduling divergence caught by verifier

**Pattern:** 28 `verify_failed` events in the 2026-04-21 run all share signature: compiled version has `move rN, rM` at offset 4 of the function; expected has `sw ra, K(sp)`. The agent self-reports `attempts=1 matched` but the orchestrator verifier catches the mismatch and forces a retry (all 28 eventually matched).

**Question to answer:** Is this a real scheduling divergence agents are working around iteratively, or is the agent's compile/compare loop producing false positives that only the orchestrator-side verification catches? If the latter, `compare_func.py` has a blind spot.

**Candidate next steps:**
1. Pick one — e.g. `ePhysicsControllerConfig::GetName` — and reproduce: compile, run `compare_func.py`, see what it says vs what the orchestrator verify says. Find the gap.
2. If there's a tool-side blind spot (e.g. relocation masking over-aggressive on prologue), fix `compare_func.py`.
3. If it's real divergence agents resolve in-session, just log that attempts=1 in session_result is inaccurate and the real iteration count was higher. Update the agent prompt to report true attempt counts.

---

## Tracking

When a TODO is converted to a full research doc:
1. Add the file to `docs/research/snc-*.md`
2. Update the table in `CLAUDE.md` ("SNC compiler research" section) with the doc + "Read when..." guidance
3. Strike through or remove the entry here
