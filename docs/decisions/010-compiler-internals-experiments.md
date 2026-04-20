# 010: SNC Compiler Internals — Seven Experiments

**Date:** 2026-04-19
**Status:** Verified (seven parallel experiments)

## Context

Decision 009 concluded that pspsnc 1.2.7503.0 appears to be the only publicly available build of the PSP SNC compiler, and we cannot find an alternative version. Before committing to a binary patch of our compiler (or any other heavy intervention), we ran seven parallel reverse-engineering experiments to fully map the relevant compiler internals. This doc captures those findings — several of them significantly change the picture of how hard the divergence problem actually is.

## Experiments run

| # | Goal | Method |
|---|------|--------|
| A | Map del_slot.c in pspcor.exe | radare2 disassembly + string xrefs |
| B | String-to-function index for pspcor.exe | Capstone linear disasm + xref analysis |
| C | Find undocumented -X flags and pragmas | Empirical probing with `=99` out-of-range trick |
| D | Environment variables that affect codegen | Static string analysis of all binaries |
| E | Locate mul.s emitter + FPU register allocator | Static analysis + test compiles |
| F | Statistical branch-likely heuristic from EBOOT | objdump + Python classifier on 75,365 branches |
| G | Feasibility of dynamic tracing via lldb/wibo | Empirical + wibo source inspection |

## A. del_slot.c is mapped surgically

**pspcor.exe layout** (ImageBase `0x400000`):
- `.text`   VA `0x401000 - 0x4ae596`
- `.rdata`  VA `0x4af000 - 0x4c66f2`
- `.data`   VA `0x4c7000 - 0x505a34`

**del_slot cluster** spans `0x43af20..0x43c0d8` (~5KB, 12 functions):

| VA | Size | Role |
|----|------|------|
| `0x43af20` | 156 | Init — predecessor-count table |
| `0x43afbc` | 387 | **Eligibility checker** — the "FAILED because…" function |
| `0x43b13f` | 146 | Wrapper: calls `find_first_CGINS` then eligibility check |
| `0x43b380` | 730 | Phase 1 in-block slot filler |
| `0x43b695` | 754 | Phase 2 BB-scan slot filler (no likely promotion) |
| `0x43b987` | 211 | `try_fall_thru_fill` — cross-block fill, sets promotion flag |
| `0x43ba5a` | 775 | `fmds_driver` — main FMDS loop |
| `0x43bd61` | 887 | Public `del_slot` entry; dispatches phases by bitmask |

**Key mechanism**: bne→bnel promotion is NOT an opcode rewrite. It is a single flag bit: `instruction->flags_1b |= 0x20`. Set in exactly three places:

- `0x43bc13` — `fmds_driver` main path
- `0x43ba4b` — end of `try_fall_thru_fill`
- `0x43ad29` / `0x43ad55` — shared fill helpers

Read downstream by the MIPS encoder (label-suppression + filled-delay-slot triggers `-likely` emission).

**Eligibility checker** at `0x43afbc` tests `MOP_info[opcode]` for flag bits and emits these failure messages via `[0x4f8044] > 1` verbosity gate: store/call, load, float op, div op, target reg conflict, and find_first_CGINS-returned-NULL.

**Opcode eligibility range**: `[0x32e..0x34c]` (31-opcode window covering the MOP_b*/b*l family). Range-checked at `0x43b9bb` and `0x43b18e`.

**Patchable 1-byte flips** if we ever want to kill branch-likely globally:
- `0x43bc13`: `80 4E 1B 20` → `80 4E 1B 00` (main driver promotion)
- `0x43ba4b`: `80 4F 1B 20` → `80 4F 1B 00` (fall-thru promotion)
- `0x43be78`: `04` → `00` (phase 4 enable bit)

## B. pspcor.exe is fully string-indexed

Artifact: `/tmp/pspcor_string_index.json` (~212 KB). 1,972 functions identified; 386 carry string references.

**Key data tables in pspcor.exe:**

| Address | Contents |
|---------|----------|
| `0x4e1fe0` | MOP descriptor table — 319 entries × 0x40 bytes (covers scalar/FP/VFPU) |
| `0x4eb238` | Secondary opcode-flags table (indexed via `[0x4ec944]`) |
| `0x4ecf04` | IR opcode name table (319 entries) |
| `0x4cc3b8` | `-X`/`-C` option descriptor table — 151 entries × 36 bytes |
| `0x4bcdb4` | MOP→mnemonic string map (e.g., `MOP_bnel` → `bnel`) |
| `0x4edf00` | Central compiler global-state struct (1,323 xrefs) |

**Key functions:**
- `0x41589a` — CG phase driver (CG_expand → CG_optimize → CG_sched → CG_LRA → CG_Emit)
- `0x471809` — CG_Peephole
- `0x42a083` — cgemit.c (relocation emitter)
- `0x42fadb` — controls.c (option dispatcher)
- `0x48e32c` — peephole commute routine (cmov-only — not mul.s!)
- `0x478534` — gra debug/LRA

## C. Flag hunt — two major discoveries

**`-Xsched=0` globally kills branch-likely emission** (verified empirically):

```cpp
bool f(int *p) { if (p) return *p; return 0; }
```

- Baseline `-O2` (sched=2): `beqzl a0,+4; lwc1` (branch-likely with filled slot)
- `-Xsched=0 -O2`: `beqz a0,+5; nop; lwc1` (regular branch, empty slot)

Caveat: `-Xsched=0` also disables **all** scheduler-placed delay slots, not just branch-likely. Global application would regress the 22 existing bnel-matched functions.

**BUT**: `#pragma control sched=0` scopes the effect to a single translation unit or routine. Confirmed via strings in pspcfe.exe:

> `"Attempt to change ROUTINE scope control %s inside routine"`
> `"control_%s can not be specified via a pragma. Ignored"`
> `"Invalid change in controls ALIAS, CALLMOD or REG in routine %s"`

So `sched` is changeable via pragma (ALIAS/CALLMOD/REG are not).

**New -X flags discovered** (~30 undocumented knobs):

| Flag | Range | Notes |
|------|-------|-------|
| `-Xcih` | 0..1 | Constant invariant hoisting (requires mopt≥3) |
| `-Xivrep` | 0..1 | Induction variable replacement |
| `-Xdomain` | 0..1 | Domain analysis for global OPT |
| `-Xinllev` | 0..5 | Inline level (distinct from sinllev) |
| `-Xfprreserve=N` | reg# | **Reserve FP register from allocator** |
| `-Xgprreserve=N` | reg# | Reserve GP register from allocator |
| `-Xbindreg=var:reg` | — | Bind global to specific register |
| `-Xfltedge`, `-Xfltconst`, `-Xfltdbl`, `-Xflttp`, `-Xfltfold` | varies | FP optimization tuning |
| `-Xrestrict`, `-Xparamrestrict` | 0..2 | Alias analysis tuning |
| `-Xpack` | 0..4 | Default struct packing |

Full list in the experiment artifact. Most interesting for the existing divergences: **`-Xfprreserve`** — lets us force specific FP registers out of the allocator's pool, which may resolve the f19-vs-f4 divergence per-function.

**Tracing flags** (pspcor internal):
- `-tr<N>` — dump basic blocks after named pass
- `-ts<N>` — dump symbol table / types
- `-tt10,<mask>` — OPT pass trace (bits: 1=exec, 2=no global OPT, 4=move-unsafe, 256=memory)
- `-tt26,<mask>` — CFG/elim trace
- `-keeptemp` — preserve intermediate `.B`/`.I`/`.D`/`.s` files

## D. No environment variables affect codegen

Confirmed: no `SN_DEBUG`/`SNC_DEBUG`/`SNC_TRACE` env vars exist in any binary. Only `SN_COMMON_PATH` (licensing), `CLASSPATH` (Java frontend), `SN_LANG` (error localization), and `TMP`/`TEMP` (temp file location) are read.

Dead end for env-var-based tracing. All debug levers are CLI flags.

## E. mul.s and FPU allocator — not simple patches

**mul.s operand order reflects source AST order, with no canonicalization.**

Test:
- `k * x` (const first in source) → `mul.s f0, f0, f12` (const first in emit)
- `x * k` (var first in source) → `mul.s f0, f12, f0` (var first in emit)

The peephole "commute 2-addr" routine at `0x48e32c` exists but only handles cmov-family ops (fmovn/fmovz/fmovf). The commutability flag (bit `0x10` of byte `+0x25` in the secondary opcode table) is NOT set for mul.s. Setting it would require both a table flip AND teaching the peephole's remap switch about fmuls — not a 1-byte patch.

**Workaround**: write `var * const` in decomp source.

**FPU register allocation is algorithmic, not table-driven.**

Test results show our SNC's allocator uses sequential priority `f0` (return), then `f12, f13, f14, ..., f19` above the highest live arg reg. No static priority array exists in `.rdata`. Priority coloring happens in `gra.c` (string VA `0x4b73f8`, "attempt priority coloring" diag at `0x4b83c0`).

Could not reproduce the exact "1.0f → f19" case from the CLAUDE.md claim — our compiler never lands scalar floats in `f4..f11` in O2 output of test programs. The original EBOOT's different behavior strongly suggests a different GRA heuristic in a different SNC build, which we cannot reproduce without that build.

**Workarounds**:
1. `-Xfprreserve=N` can forcibly take registers out of the allocator's pool — potentially steers allocation toward the original's pattern
2. Source restructuring — change variable declaration order, liveness, etc. to influence the allocator

## F. Branch-likely divergence is narrower than believed

Analyzed all 75,365 conditional branches in the EBOOT. Overall: 25.8% branch-likely, 74.2% regular.

**Strong rules inferred from the original compiler's heuristic:**

| Pattern | Likely % | Rule |
|---------|---------:|------|
| delay slot is `nop` | 0.0% | Never use likely when delay is nop (tautological) |
| load with `base == cmp_reg` (null-deref) | 99.5% | Always likely |
| load with `dst == cmp_reg` (reload-through-self) | 99.9% | Always likely |
| store with `base == cmp_reg` | 90.0% | Likely |
| store with `src == cmp_reg` | 0.8% | Never likely |
| alu writes cmp_reg | 3.2% | Never likely |
| alu reads cmp_reg | 1.7% | Never likely |
| `bne r1,r2` with load reusing r1/r2 | 95.6% | Likely |

**Matched-corpus cross-check** (all 462 matched .o vs expected):

- Only **1 branch-kind divergence** found across all existing matches
- Location: `eCollisionConstraint.cpp.o` offset `0x1ec` (ours: `beqz; nop`; expected: `beqzl; sw zero,0(s0)`)
- That one is a scheduler failure (couldn't fill delay slot) cascading into wrong branch kind — NOT a bnel-heuristic divergence
- **Zero cases where we emit `bnel` and the original emits `bne+nop`** in matched output

This means the CLAUDE.md framing of bnel divergence as widespread is wrong — our 22 matched bnel functions all agree with the original. The divergence only appears in unmatched functions, and may be driven by agent source choices that steer our compiler into branch-likely in patterns where the original used a different delay-slot-fillable sequence.

## G. Dynamic tracing — viable but needs out-of-sandbox verification

**wibo process model** (verified empirically):
- `wibo 1.1.0 (Darwin x86_64)` runs via Rosetta 2
- `pspsnc` invokes `pspcfe`, then `pspcor`, then `pspas` as **separate child wibo processes**
- Each child has its own ImageBase `0x00400000`
- `WIBO_DEBUG=1` produces 552 lines of API-level trace (fork/exec visible) but does NOT propagate to children unless set as an env var
- wibo does NOT expose pspcor-internal code execution — it only logs host Win32 API calls

**lldb attach feasibility** (analytical only — sandbox blocked empirical test):
- `/usr/bin/lldb` exists on the system
- Attach-by-PID is the recommended path: `ps -ef | grep 'wibo.*pspcor'` → `lldb -p <pid>`
- Absolute PE addresses should work as breakpoints (no ASLR, wibo uses preferred base)
- Rosetta may limit hardware watchpoints but should support software breakpoints

**dtruss** is SIP-gated on modern macOS — not usable without partial SIP disable.

**Recommended dynamic-tracing approach** (for future Stage 2 work if needed):
1. Feed pspcor a long-running input (~800KB trivial funcs keeps it alive for seconds)
2. From another shell: grab PID, attach lldb
3. Set absolute-VA breakpoints in pspcor's .text (`0x00401000..0x004ae596`)

pspcor.exe is only 960KB — Ghidra (installed, v12.0.4) can fully decompile it in minutes for deep static analysis if dynamic tracing proves problematic.

## Implications for matching strategy

**Binary patching of pspcor.exe for branch-likely is now of questionable value.** Experiment F's corpus-wide evidence shows the divergence barely exists in our matched output. The cases where it matters are unmatched functions where pragma scoping (`#pragma control sched=0`) can address the issue per-function without touching the compiler.

**Revised toolkit for divergence cases:**

1. **`#pragma control sched=0`** — per-function kill-switch for branch-likely and delay-slot filling. Use when a function has bnel-vs-bne+nop divergence.
2. **`-Xfprreserve=N`** — per-compile FP register reservation. Add to permuter flag search space for FP-allocation-divergence cases.
3. **Source craft for mul.s** — write `var * const` not `const * var`. Add to decomp guidance.
4. **Source craft for FP alloc** — restructure liveness/declaration order to influence allocator.

**Binary patch remains available** as a fallback if pragma scoping proves insufficient:
- 1-byte flips at `0x43bc13` and `0x43ba4b` disable branch-likely globally
- Would live in a **second copy** of pspcor.exe invoked selectively by the build system
- Not recommended as first move given experiment F's findings

## Validation plan (in progress)

Path 1 (no binary patch) is being validated by picking 5 of the 40 currently-failed functions and retrying with:
- `#pragma control sched=0` applied to the function body
- `-Xfprreserve` tried via permuter for FP-divergence cases
- Source restructuring for mul.s operand order

If 3+ of 5 match after pragma intervention, Path 1 is confirmed sufficient. If fewer, binary patch reconsidered.

## Artifacts

- `/tmp/pspcor_string_index.json` — full function-to-string index for pspcor.exe (212 KB)
- Seven experiment logs (sub-agent transcripts) under `.claude/projects/.../tasks/`

## References

- `docs/decisions/006-bnel-compiler-divergence.md` — original (now superseded by F's corpus analysis)
- `docs/decisions/009-snc-compiler-version-search.md` — why we cannot obtain the correct compiler version

---

## EDIT (2026-04-19): Corrections after deeper investigation

Several claims in the body of this doc were refined or corrected after follow-up verification on 2026-04-19. Summary:

### 1. Experiment F's scope was narrower than implied

F stated "zero cases where we emit `bnel` and the original emits `bne+nop` in the matched corpus." Accurate — but only covers **matched** `.o` files (462 of them). By definition, any function with a bnel divergence cannot be in the matched set. The divergence lives in the **40 failed** functions, which F did not analyze.

The conclusion "the bnel divergence is narrower than believed" was premature. Precise byte-level analysis of `gcStateVTableEntry::Set` (the canonical case from 006) on 2026-04-19 confirmed a real bnel-heuristic divergence: ours emits `bnel + move a3, a2`, original emits `bne + nop`. The 4-instruction diff (2 relocations masked + the bnel/nop pair) is exactly what decision 006 described.

### 2. `#pragma control sched=0` is too aggressive to be a matching tool

Validation on 2026-04-19: applying `#pragma control sched=0` to `gcStateVTableEntry::Set` grew the function from 96B to 116B (scheduler added nops everywhere). It does not produce a byte-exact match because the original was compiled WITH scheduling — we need scheduling on but branch-likely promotion off. The pragma is a single global knob that can't do that.

The pragma still exists and works functionally (verified), but it is not the "per-function scoping" solution this doc originally implied.

### 3. The `0x41d90c` stub / `0x41d910` "hidden function" hypothesis was disproven

A mid-investigation hypothesis was that the 3-byte stub at `0x41d90c` (always returns 1) had displaced a real eligibility predicate that still lives at `0x41d910`, and that redirecting the call at `0x43afd8` from the stub to `0x41d910` (a 1-byte patch) would restore the original compiler's behavior.

Verification on 2026-04-19 disproved this:
- `0x41d910` IS a complete 172-byte function — but it is **actively called** by 4 other delay-slot filler functions (`0x43ae1c`, `0x43b2b5`, `0x43b531`, `0x43b7e0`). Not unreferenced.
- Its arg2 expects a slot/vreg descriptor (`[+8]->[+8]->[+4]` layout). `del_slot_can_fill` pushes `ebx` (a block/frame pointer with `[+0x40]/[+0x44]` layout) as arg2. Type mismatch would dereference wrong memory.
- It validates opcodes in the `0x395..0x466` range; `del_slot_can_fill` only sees opcodes `0x32e..0x34c` (pre-filtered). Every valid input would fall through to reject.

Redirecting the call would effectively disable bnel promotion entirely for this gate, regressing the 22 existing bnel matches. **DO NOT PATCH this way.**

### 4. Current best patch path for bnel heuristic

- The right patch layer is `del_slot_can_fill` at `0x43afbc`, not the three promotion sites (which lack the register info needed to evaluate the rule).
- Two patch sizes available:
  - **~16 bytes (narrower)**: extend the existing "target-reg conflict" bitmask check at `0x43b095..0x43b0e7` to include `{branch.rs, branch.rt}`. Rejects any candidate whose dst aliases the compare register — strict superset of some F-rules, misses the "no overlap" case.
  - **~55-70 bytes (full)**: trampoline via `call rel32` at `0x43afec`, jumping to a new code region at end of `.text`. Implements the full "move-with-no-cmp-overlap → reject" rule.
- The narrower patch would NOT fix the canonical `gcStateVTableEntry::Set` case (the move has no register overlap). Only the full trampoline patch addresses it.
- Patching is warranted IF the bnel divergence accounts for enough of the 40 failed functions to justify the complexity. Preliminary survey of 3 other failed functions (2026-04-19) found mixed modes: one VFPU-only, one non-divergent (original also uses bnel), one potential bnel case. Bnel divergence is **one of several** failure modes in the failed-40, not a universal cause.

### 5. Action items going forward

- The binary-patch decision is paused pending a broader survey: how many of the 40 failed functions are actually bnel-heuristic divergences (vs VFPU, register allocation, etc.)? That count determines whether the ~55-byte patch is worth the engineering + regression risk.
- Decision 006's original framing (bnel as a real but narrow class) stands. The subsequent "narrower than believed" revision here was premature.
