# pspcor Read Prologue Patch Project

**Status:** Milestone 1 complete; Milestone 2 in progress  
**Owner:** dwilliams + Codex  
**Created:** 2026-05-07  
**Related:** `docs/enhancements-match-lift.md` ML2,
`docs/research/failure-corpus-read-20260507.md`,
`docs/decisions/010-compiler-internals-experiments.md`,
`docs/decisions/011-bnel-compiler-patch-design.md`

## Goal

Patch or otherwise control `extern/snc/pspcor.exe` so the compiler emits the
original game's `Read(cFile &, cMemPool *)` prologue scheduling shape around
`cReadBlock` construction.

The target failure cluster is dense enough to justify compiler work:

- `Read(cFile &, cMemPool *)`, excluding `PlatformRead`: 569 total rows.
- Current status: 53 matched, 187 failed, 329 untried.
- 188B failed cluster: 99 rows.
- 98/99 failed 188B rows cite cReadBlock/prologue scheduling.
- 21 matched 188B rows prove the function size and body shape are achievable.

## Concrete Divergence

The canonical source shape is:

```cpp
int result = 1;
cReadBlock rb(file, 1, true);
if (rb._data[3] == 1 && Parent::Read(file, pool)) {
    return result;
}
cFile_SetCurrentPos(rb._data[0], rb._data[1]);
return 0;
```

For many failed 188B rows, the behavior and function body match, but our SNC
schedules the `cReadBlock` setup differently. The original interleaves
callee-save stores and `li s3,1` before the constructor call; our compiler
often defers `sw s3` and/or `li s3,1` until immediately before or after the
call setup.

Representative failed shape from `cFactory::Read`:

```text
Original:
  sw    s1,24(sp)
  sw    s3,32(sp)
  move  s1,a1
  li    s3,1
  move  a0,sp
  li    a2,1
  sw    ra,36(sp)
  jal   cReadBlock::cReadBlock
  li    a3,1

Our SNC:
  sw    s1,24(sp)
  move  s1,a1
  move  a0,sp
  li    a2,1
  sw    s3,32(sp)
  sw    ra,36(sp)
  jal   cReadBlock::cReadBlock
  li    a3,1
  li    s3,1
```

The desired patch should make the compiler emit the original ordering without
regressing existing matched output.

## Current Findings

The first implementation slice added a reproducible harness:
`tools/research/read_prologue_harness.py`.

Harness artifacts are written under `build/research/read_prologue/`:

- `0005dccc/report.md`: matched exemplar
  `eDynamicFluid::Read(cFile &, cMemPool *)`.
- `0000ab98/report.md`: failed exemplar
  `cFactory::Read(cFile &, cMemPool *)`.

The matched exemplar proves the harness is aligned with verifier behavior. Its
raw diff only shows unresolved `jal` relocation placeholders; the
verification-masked diff is clean through the prologue window.

The failed exemplar reproduces the scheduling drift in a narrow window:

```text
0x18: expected sw s3,32(sp)       compiled move s1,a1
0x1c: expected move s1,a1         compiled move a0,sp
0x20: expected li s3,1            compiled li a2,1
0x24: expected move a0,sp         compiled sw s3,32(sp)
0x28: expected li a2,1            compiled sw ra,36(sp)
0x2c: expected sw ra,36(sp)       compiled jal cReadBlock::cReadBlock
0x30: expected jal cReadBlock     compiled li a3,1
0x34: expected li a3,1            compiled li s3,1
```

This points at a compiler scheduling/liveness decision around the `result = 1`
value and callee-save setup. The source-level inline asm used in existing
matched rows can produce the right shape, but `cFactory::Read` shows that
source shaping alone does not consistently pin `li s3,1` before the
constructor call.

## Non-Goals

- Do not post-process object files or EBOOT bytes. The project should preserve
  the source -> compiler -> object verification model.
- Do not replace the canonical `extern/snc/pspcor.exe` in place.
- Do not run broad matching against failed Read rows until a compiler change or
  validated compiler flag path exists.

## Success Criteria

Milestone 1:

- A reproducible harness extracts and compares original-vs-compiled prologues
  for a matched 188B Read and a failed 188B Read.
- The harness records disassembly artifacts under `build/research/`.

Milestone 2:

- The compiler pass or code path responsible for the bad scheduling decision is
  narrowed to a concrete area in `pspcor.exe` using trace flags and/or static
  analysis.

Milestone 3:

- A patched or controlled compiler variant makes one failed 188B Read row match
  without breaking the matched 188B exemplar.

Milestone 4:

- Validate against the full matched DB and a selected 188B failed cluster.

## Implementation Plan

1. Build a Read prologue harness.
   - Inputs: DB address, optional source file, optional compiler flags.
   - Outputs: original bytes/disasm, compiled bytes/disasm, side-by-side
     prologue window, and diff summary.
   - Initial pair: matched `eDynamicFluid::Read` at `0x0005dccc`, failed
     `cFactory::Read` at `0x0000ab98`.

2. Collect trace outputs.
   - Use `-keeptemp`, `-tr<N>`, `-tt10,<mask>`, and `-tt26,<mask>` on the
     minimal source pair.
   - Compare `sched=1` and `sched=2` output for the failed exemplar.
   - Identify the last trace point before the bad ordering appears.

3. Map candidate scheduler/LRA routines.
   - Start from `docs/decisions/010-compiler-internals-experiments.md`:
     `0x41589a` phase driver, `CG_sched`, `CG_LRA`, `gra` area around
     `0x478534`, and central global state at `0x4edf00`.
   - Use string/function indexes if available; regenerate them if stale.

4. Design patch strategy.
   - Prefer a narrow heuristic patch or option switch if the responsible
     decision is local.
   - If a trampoline is required, reuse the PE-section-extension strategy from
     ADR-011.
   - Store output as `extern/snc/pspcor.read-prologue.exe` or similar, never as
     an in-place overwrite.

5. Add build integration only after a successful one-function validation.
   - Add an explicit opt-in Makefile variable.
   - Keep full verification against unpatched compiler as default.

## Risks

- The divergence may be allocator-driven rather than scheduler-driven; changing
  scheduling alone may not force the desired `s3` liveness/order.
- The patch may improve the failed 188B cluster but regress existing matched
  sched=1 Read rows.
- `pspcor.exe` patching may require PE section extension, which must be tested
  carefully to avoid corrupting the compiler binary.

## Progress Log

- 2026-05-07: Project opened. Current DB counts and failed-cluster analysis
  pulled from `docs/research/failure-corpus-read-20260507.md`.
  Implementation begins with a reproducible prologue comparison harness.
- 2026-05-07: Added `tools/research/read_prologue_harness.py`. The harness
  extracts original EBOOT bytes, compiles the current source candidate, writes
  raw and verification-masked word diffs, and stores prologue/object disassembly
  artifacts under `build/research/read_prologue/`.
- 2026-05-07: Ran the harness for matched `0x0005dccc`
  `eDynamicFluid::Read` and failed `0x0000ab98` `cFactory::Read`. Milestone 1
  is complete: the matched exemplar masks cleanly after relocations, while
  `cFactory::Read` reproduces the exact pre-`cReadBlock` scheduling drift.
  Next step is Milestone 2 trace collection with `-keeptemp`, `-tr<N>`,
  `-tt10,<mask>`, and `-tt26,<mask>`.
