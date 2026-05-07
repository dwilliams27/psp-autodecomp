# Failure Corpus and Read Family Analysis - 2026-05-07

This note updates the older ML2/cReadBlock findings against the current DB
after the 2026-05-06 and 2026-05-07 GPT-5.5 runs.

## Executive Summary

The next high-value path is not another broad matching run. Recent matching
has exhausted the clean boilerplate pools (`GetType`, `GetName`, `New`) and
is now dominated by repeated register-allocation and scheduling failures.

Highest-value next work:

1. Quarantine known compiler-blocked `Read(cFile &, cMemPool *)` failures from
   normal retry pools.
2. Build a near-miss retry list from failures that are close but do not cite
   cReadBlock/prologue/no-source-level blockers.
3. Treat the cReadBlock prologue issue as a compiler-patch project, not a
   source-level matching project.

## Recent Run Trend

| Log | Attempted | Matched | Failed | Matched Bytes | Bytes/Min |
| --- | ---: | ---: | ---: | ---: | ---: |
| `match_20260506_155600.jsonl` | 120 | 66 | 54 | 12,676 | 43.8 |
| `match_20260506_235212.jsonl` | 176 | 59 | 116 | 23,336 | 47.5 |
| `match_20260507_103437.jsonl` | 92 | 25 | 67 | 9,216 | 38.0 |

The latest targeted run was useful, but the drop to 38.0 bytes/min confirms
that broad target-list matching is now in the long tail.

Recent failure snapshots across those three runs:

| Pattern | Snapshots |
| --- | ---: |
| Register allocation / register-role drift | 209 |
| Branch / scheduling / delay-slot drift | 172 |
| Permuter invoked or discussed | 165 |
| Explicit size or byte-diff note | 91 |
| cReadBlock / prologue / RAII language | 49 |
| Permuter isolation failure | 25 |
| Explicit "no source-level lever" language | 16 |

These categories overlap. The important signal is that most failures are not
missing source knowledge; they are compiler-shape drift after the agent already
reconstructed the behavior.

## Current Read Inventory

`Read(cFile &, cMemPool *)`, excluding `PlatformRead`:

| Status | Count | Bytes |
| --- | ---: | ---: |
| Matched | 53 | 13,588 |
| Failed | 187 | 39,616 |
| Untried | 329 | 151,092 |
| Total | 569 | 194,608 |

Other read-like families:

| Signature | Matched | Failed | Untried | Notes |
| --- | ---: | ---: | ---: | --- |
| `Read(cReadBlock &)` | 22 | 5 | 57 | Smaller pool; not the same prologue issue. |
| `Read(cInStream &)` | 19 | 9 | 38 | Mixed failures; not the main ML2 blocker. |
| `PlatformRead(cFile &, cMemPool *)` | 19 | 4 | 5 | Mostly already harvested. |
| Other `Read(...)` | 3 | 2 | 6 | Too small to drive strategy. |

## cReadBlock Prologue Blocker

The old ML2 conclusion still holds: the dominant `Read(cFile &, cMemPool *)`
failure mode is compiler-level prologue scheduling around `cReadBlock`.

Failed `Read(cFile &, cMemPool *)` size buckets:

| Size | Failed | cReadBlock/Prologue Notes | Body-Only/Only-Prologue Notes | Compiler-Level Notes | Near `<=32B` Notes |
| ---: | ---: | ---: | ---: | ---: | ---: |
| 188 | 99 | 98 | 67 | 30 | 44 |
| 208 | 21 | 19 | 15 | 10 | 4 |
| 200 | 16 | 14 | 10 | 4 | 2 |
| 120 | 8 | 7 | 3 | 3 | 4 |
| 228 | 8 | 8 | 7 | 4 | 4 |
| 268 | 8 | 8 | 7 | 5 | 4 |
| 232 | 5 | 5 | 2 | 0 | 1 |
| 212 | 4 | 2 | 2 | 2 | 1 |

The 188B cluster is the clearest compiler-patch target:

- 21 matched 188B `Read(cFile &, cMemPool *)` entries exist, so size alone is
  not impossible.
- 99 failed 188B entries exist.
- 98/99 failed 188B entries have prologue/cReadBlock language in notes.
- 0 untried 188B entries remain, so another normal matching run cannot harvest
  this cluster without retrying failed rows.

Conclusion: do not spend ordinary matching sessions on failed 188B
`Read(cFile &, cMemPool *)` rows. They belong behind ML2, the pspcor.exe
scheduler patch project.

## Retryable Near-Miss Pool

Across the whole failed DB, many failures are close. After filtering out notes
that cite cReadBlock/prologue/no-source-level/compiler-level blockers:

| Near-Miss Threshold | Retryable Count | Retryable Bytes | Main Categories |
| --- | ---: | ---: | --- |
| `<=4B` | 7 | 1,760 | Other, Operator, AssignCopy, New, Setter |
| `<=8B` | 24 | 5,212 | Other, Setter, AssignCopy, Operator, Write, New |
| `<=16B` | 38 | 8,124 | Other, AssignCopy, Setter, Operator, GetText |
| `<=32B` | 69 | 16,200 | Other, AssignCopy, Setter, Operator, GetText |
| `<=64B` | 85 | 20,568 | Other, AssignCopy, Setter, Operator, GetText, Write |

This is the best next matching substrate. It is smaller than the remaining
untried pool, but it is better instrumented: each row already has failure notes
and often a failure snapshot.

Good first retry batch:

- `<=16B` retryable near-misses first.
- Include `<=32B` if the run is long enough.
- Exclude any note with `prologue`, `cReadBlock`, `RAII`, `compiler-level`,
  `no source-level`, or `requires pspcor`.
- Prefer rows with snapshots and no prior explicit "no source lever" language.
- The generated artifact is `config/targets_failed_nearmiss_retry_20260507.json`.
  It contains failed rows, so it is a review/reset list. The current
  orchestrator only picks `untried` target rows; before running this list we
  need either a safe reset step for the selected addresses or an orchestrator
  retry-failed mode.

Top examples from the retryable `<=16B` pool:

| Diff | Size | Category | Address | Function |
| ---: | ---: | --- | --- | --- |
| 2 | 60 | Other | `0x001a63e4` | `nwConnection::nwMsgsBlock::nwMsgsBlock(void)` |
| 2 | 108 | Other | `0x00344524` | `gcValFPSInfo::Evaluate(void) const` |
| 2 | 316 | Setter | `0x00369f98` | `gcValUIVisible::Set(float)` |
| 3 | 180 | Other | `0x0000c0d8` | `cFactory::OnNameChanged(void)` |
| 3 | 292 | AssignCopy | `0x00244064` | `gcStreamedCinematic::AssignCopy(const cBase *)` |
| 4 | 292 | New | `0x002a7edc` | `gcDoEntityBipedSetShape::New(cMemPool *, cBase *) static` |
| 5 | 660 | AssignCopy | `0x00208b48` | `eBipedController::AssignCopy(const cBase *)` |
| 6 | 44 | Setter | `0x0008e714` | `eVideoPlatform::SetShadowInfo(float, float, const mPlane &) static` |
| 6 | 312 | Write | `0x00054c94` | `eLightGrid::Write(cFile &) const` |
| 8 | 116 | Setter | `0x00343260` | `gcValExternalVariable::Set(float)` |
| 10 | 172 | GetText | `0x003432d4` | `gcValExternalVariable::GetText(char *) const` |
| 15 | 432 | GetText | `0x00355da8` | `gcValObjectHasRelation::GetText(char *) const` |
| 16 | 288 | GetText | `0x003407ec` | `gcValEntityVariable::GetText(char *) const` |

## Recommendation

Do not run another broad target list next. Use this order:

1. Generate a `targets_failed_retry_nearmiss_20260507.json` list from the
   retryable `<=16B` pool, then optionally append retryable `<=32B`.
2. Run that list from the primary checkout, not a side worktree.
3. Keep `Read(cFile &, cMemPool *)` prologue rows out of retry lists until
   the ML2 compiler-patch work starts.
4. Treat ML2 as the dedicated deep project if the goal is a step-function
   unlock in matched bytes.

The current `Read` corpus is still the largest visible unlock, but the blocker
is not missing agent effort. It is the pspcor scheduler/LRA behavior documented
in `docs/enhancements-match-lift.md`.
