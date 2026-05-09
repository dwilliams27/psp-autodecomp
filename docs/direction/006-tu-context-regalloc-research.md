# 006: TU-Context Register Allocation Research

**Date:** 2026-05-07
**Status:** Research complete; Phase 4 decision pending
**Branch:** `research/tu-context-regalloc`
**Stop point:** finish research and testing, then pause before production implementation.

## Goal

Determine whether SNC's context-sensitive register allocation can be made
predictable enough to recover currently failed source-level matches, especially
the shape `Collide` wrappers and sibling `GetText` / `Evaluate` near-misses.

The immediate objective is not to ship a new matcher path. The objective is to
answer these questions with controlled experiments:

1. Can we reproduce the register-allocation/context effect outside the
   overnight agent workflow?
2. Which surrounding translation-unit inputs change the allocator decision:
   preceding function bodies, symbol order, class declarations, pragmas, or
   compiler flags?
3. Is there a small, reusable "context seed" that flips known failures into the
   original allocation without damaging matched siblings?
4. If a seed exists, what implementation path should we discuss next:
   generated context-seeded TUs, original-order monolithic shards, permuter
   integration, or compiler patching?

Do not implement item 4 in this research pass. Stop after the matrix and
recommendation are written.

## Motivation

ADR-012 established that SNC can produce different register allocation for
identical source depending on earlier functions in the same translation unit.
The 2026-05-06 GPT-5.5 run reinforced the pattern: many failures had correct
semantics and close control flow, but diverged through stable register swaps,
branch-shape cascades, or delay-slot/scheduling decisions that local source
perturbations did not fix.

Current guidance says to classify those as `failed` with `category=REG_ALLOC`
after one permuter pass. That is correct operational behavior, but it leaves a
large pool of bytes blocked unless we learn how to reproduce or neutralize the
compiler context.

## Candidate Families

Primary families:

- Shape `Collide` wrappers:
  - `eMultiSphereShape::Collide(eMultiSphereShape*)`
  - `eMultiSphereShape::Collide(eCylinderShape*)`
  - `eConvexHullShape::{MultiSphere,Capsule,ConvexHull} Collide`
  - Known symptom: structurally correct 140B wrappers, loop/call register swaps,
    repeated 300s permuter no-improvement runs.
- Generic shape `Collide(const eShape*,...)` 172B dispatchers from ADR-012.
- `cFactory::MarkForClean` / `CleanGroups` / `DeleteMarkedForClean`:
  repeated `s1`/`s2` assignment drift.
- `gcViewport::UpdateAll` / `UpdateUI` / current `gcViewport::Get`:
  sibling allocator instability.
- Representative `GetText` failures from the 2026-05-06 run:
  `gcDoLog::GetText`, `gcValVariable::GetText`, `gcValExternalVariable::GetText`,
  and `gcValEnumerationEntry::GetText`.

Secondary families:

- `Evaluate` / `Set(float)` handle-table lookups with a correct broad structure
  but argument/register cascades.
- Reset functions with short size or branch-shape drift.

## Research Plan

### Phase 0: Evidence Inventory

Produce a compact dataset of known context-sensitive failures:

- address, function name, size, current closest source file/snapshot
- baseline diff count under the current repo source
- symptom category: pure register rename, branch-shape cascade, size shortfall,
  delay-slot blocker, or mixed
- prior attempts: sched flags, same-TU move, split-TU move, permuter result

Output:

- `logs/research/tu_context_regalloc/failure_inventory.json`
- summary table appended to this doc

### Phase 1: Harness

Build a small research harness that can compile generated translation units and
score one target symbol against EBOOT bytes without editing `src/` or the DB.

Minimum requirements:

- generate temporary `.cpp` files under `logs/research/tu_context_regalloc/tmp/`
  or `/tmp`
- accept a target address and candidate TU source
- compile with SNC + selected flags (`-Xsched=1/2`, optional extra flags)
- extract the exact symbol, not first same-sized symbol
- compare against EBOOT bytes with relocation masking
- emit JSON rows with: experiment id, target, seed description, flags, symbol
  found, size, diff count, first-diff summary, compile status

Output:

- `tools/research/tu_context_harness.py` or a clearly scoped script under
  `tools/research/`
- `logs/research/tu_context_regalloc/results.jsonl`

### Phase 2: Reproduction Matrix

For each primary family, test whether changing only surrounding TU context
changes the target's bytes.

Seed types:

- isolated target body
- current repo file order
- same class file with target moved before/after matched siblings
- matched sibling prefix functions from classes that already byte-match
- no-op synthetic prefix functions with increasing register pressure
- prefix functions copied from failure snapshots or matching exemplar classes
- sched flag variants and local `#pragma control sched=N`

Questions to answer:

- Does any context change reduce the diff?
- Does any context reach exact match?
- Which register choices move first?
- Is the effect deterministic across repeated compiles?
- Is the effect target-local, or does the context seed also perturb sibling
  matched functions in the same TU?

Output:

- matrix rows in `results.jsonl`
- per-family summary in this doc

### Phase 3: Seed Search

If Phase 2 shows movement, search for minimal reusable context seeds.

Search space:

- 1-5 small prefix functions with controlled local variable pressure
- matched sibling prefixes from the same object file
- synthetic functions that force particular `s0/s1/s2` live ranges
- function-order permutations within a safe generated TU
- optional `-Xgprreserve=N` sweeps if SNC accepts them for this compiler

Success tiers:

- **Tier A:** exact match for at least one failed production target.
- **Tier B:** repeatable reduction by >=50% on at least two targets in one
  family.
- **Tier C:** no match, but strong evidence that only original monolithic
  context or compiler internals can move the allocator.

Output:

- best seed source snippets under `logs/research/tu_context_regalloc/seeds/`
- updated `results.jsonl`
- recommendation section in this doc

### Phase 4: Productionization Options (Do Not Start Yet)

Pause before implementing any of these:

- generated context-seeded split TUs for specific families
- orchestrator support for context-seeded matching attempts
- permuter integration with context seeds
- pspcor.exe allocator/scheduler patch research
- marking a family as permanently compiler-blocked with stronger evidence

When Phases 0-3 are complete, summarize the data and discuss which option is
worth doing.

## Workstreams

- **Logs/failure analysis:** identify the best targets and extract prior notes.
- **Harness design:** find the safest way to compile temp TUs and score exact
  symbols using existing byte-match helpers.
- **Context experiments:** run the matrix and keep source generation isolated
  from production `src/`.
- **Compiler internals reconnaissance:** inspect existing pspcor/bnel/scheduler
  docs for nearby allocator hooks and useful flag experiments, without patching.
- **Permuter hardening side track:** separate worktree; fix targeting/telemetry
  only if it does not collide with the research harness.

## Progress Log

- 2026-05-07: Proposal created. Starting Phase 0 and harness design.
- 2026-05-07: Completed Phases 0-3 research. Built the exact-symbol harness,
  ran family matrices for shape `Collide`, cFactory/gcViewport saved-register
  drift, and value/GetText handle lookup drift. Stopped before Phase 4.

## Phase 0-3 Results

Research artifacts live under `logs/research/tu_context_regalloc/`:

- `failure_inventory.json` and `failure_inventory_summary.md`
- `context_source_notes.md`
- `results.jsonl` for local harness smoke/baseline rows
- `shape_matrix.jsonl` and `shape_matrix_summary.md`
- `saved_reg_matrix.jsonl` and `saved_reg_matrix_summary.md`
- `value_text_matrix.jsonl` and `value_text_matrix_summary.md`
- generated seed TUs under `seeds/`

The harness was validated against both a known exact match and known failures.
It compiles a copied temporary TU, resolves the DB target to an exact mangled
symbol, masks relocations, scores against EBOOT bytes, and never edits `src/` or
`config/functions.json`.

### Local Controls

- `0x000696f8` from `src/eMultiSphereShape_Collide_eMultiSphereShape.cpp`
  reproduced the known `17/140` baseline.
- Full `src/eCylinderShape.cpp` reproduced matched
  `eCylinderShape::Collide(const eShape*)` exactly at `0/172`.
- Full `src/eMultiSphereShape.cpp` kept simple matched siblings exact:
  `0x00069784` at `0/36` and `0x00069834` at `0/36`.
- Full `src/eMultiSphereShape.cpp` did not rescue its failed dispatcher/wrapper:
  `0x00069534` stayed bad and size-divergent, and `0x000695e0` stayed
  size-divergent.

### Shape Collide Matrix

Rows: 43. Failed-target exact matches: 0.

| Target | Best Result | Finding |
| --- | ---: | --- |
| `0x000696f8` `eMultiSphereShape::Collide(eMultiSphereShape*)` | `17/140` | No movement under target-only, current split source, matched-prefix, target-first, sched 1/2, or pressure seeds. |
| `0x000697a8` `eMultiSphereShape::Collide(eCylinderShape*)` | `21/140` | No movement under the same seed classes. |
| `0x00071424` `eConvexHullShape::Collide(eMultiSphereShape*)` | `21/140` | No movement under full source, generated target-only, matched-prefix, or pressure seeds. |
| `0x000738a8` `eCompoundShape::Collide(eMultiSphereShape*)` | `11/136` | No positive movement; one full-snapshot sched=2 row regressed to `22/136`. |

Matched sibling guards in the generated prefix TU stayed exact:
`0x00069784`, `0x00069834`, `0x00071618`, and `0x00073ac8`.
That means the tested prefix context is not damaging those simple siblings, but
it also is not steering the failed wrappers toward the original allocator state.

### Saved-Register Matrix

Rows: 38. Failed-target exact matches: 0.

| Target | Best Result | Finding |
| --- | ---: | --- |
| `0x0000bc90` `cFactory::DeleteMarkedForClean` | `7/180` | Minimal TU, `s1/s2/s3` pressure prefix, and `-Xgprreserve=17/18/19` did not improve the `s2/s3` drift. |
| `0x000fdd44` `gcViewport::UpdateAll` | `12/152` | Minimal/prefix/pair contexts preserved the same `s1/s2` drift. |
| `0x000fdddc` `gcViewport::UpdateUI` | `12/156` | Same result as `UpdateAll`. |
| `0x000fe230` `gcViewport::Get` | `112/208` | Prefix context and `-Xgprreserve` did not move the mixed regalloc/branch-shape miss. |

`-Xgprreserve` is accepted by this compiler path, but the tested saved-register
settings either preserved the baseline or worsened codegen.

### Value/GetText Matrix

Rows: 59. Failed-target exact matches: 0.

| Target | Best Result | Finding |
| --- | ---: | --- |
| `0x003431e0` `gcValExternalVariable::Evaluate` | `8/128` | Combined class-local order, prefix pressure, sched variants, and `-Xgprreserve` did not improve the `a1/a2` lookup drift. |
| `0x00343260` `gcValExternalVariable::Set(float)` | `8/116` | Same `a1/a2` drift as Evaluate. |
| `0x0036ae00` `gcValVariable::GetText` | `10/172` | Current/snapshot/seed contexts kept the final text pointer `a1/a2` drift. |
| `0x003432d4` `gcValExternalVariable::GetText` | `10/172` | Same final text pointer drift. |
| `0x002e6fb0` `gcDoLog::GetText` | `35/320` | Prefix pressure did not move the `s0/s1` this/buf allocation. |
| `0x00342470` `gcValEnumerationEntry::GetText` | `16/216` | Context and sched variants did not improve the dispatch-register drift. |

Sched=1 consistently worsened this family. `-Xgprreserve` was either neutral or
worse, including size regressions for some `r4` rows.

## Recommendation Before Phase 4

This pass lands in **Tier C**: no tested generated context seed or compiler flag
recovered bytes, and no row reduced a failed target's diff. The evidence argues
against immediately productionizing a context-seeded matcher/permuter path from
these simple seeds.

The next Phase 4 discussion should choose between:

- **Original-context reconstruction:** build larger original-order TU shards
  from source/failure snapshots for one family, especially the shape
  `Collide` cluster where matched simple siblings remain exact.
- **Compiler-internals work:** inspect `pspcor.exe` allocator/scheduler state
  around `CG_sched` / `CG_LRA` and the `gra` area. This is higher effort but now
  better justified than adding simple prefix seeds to the production workflow.
- **Compiler-blocked classification:** keep these families out of overnight
  target lists unless a new original-context or compiler-patch path exists.

Do not implement Phase 4 production changes until this recommendation is
reviewed.

## Side Track: Permuter Hardening

Superseded by the 2026-05-09 tooling hardening pass on main. `permuter.py`
now targets DB mangled symbols by default, fails closed on ambiguous same-size
symbols, uses shared `byte_match` symbol-byte/reloc extraction, skips
non-last-mile baselines, and only overwrites source on exact matches unless
`--save-improved` is explicit. Structured JSON telemetry remains future work in
`docs/enhancements.md`.
