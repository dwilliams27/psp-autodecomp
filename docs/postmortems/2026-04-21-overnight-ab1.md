# Post-Mortem: Overnight Run 2026-04-21 — First A/B Test (base vs tier12)

## Summary

| Metric | Value | vs previous run |
|--------|-------|-----------------|
| Start | 2026-04-21 00:40 | — |
| End | 2026-04-21 08:20 | — |
| Duration | 7h 40m (9h budget; finished early — ran out of queued priorities) | -1h 22m |
| Sessions | 36 | +5 |
| Functions attempted | 180 | +25 |
| Matched | 140 (**77.8%**) | +4.3 pp |
| Failed | 40 | -1 |
| Bytes matched | 9,652 | +1,608 |
| Errors (all types) | 12 | +5 |
| Commits | 32 | +4 |

**Net DB movement**: `matched` 912 → 1056 (+144), `failed` 148 → 193 (+45), `untried` 8185 → 7993.

---

## The headline finding: A/B is a null result

Aggregate match rate went up (73.5% → 77.8%) but **the two variants are statistically indistinguishable.**

| Variant | Sessions | Attempted | Matched | Failed | Rate | Bytes matched |
|---------|---------:|----------:|--------:|-------:|-----:|--------------:|
| base    | 17 | 85 | 66 | 19 | **77.6%** | 4,436 |
| tier12  | 19 | 95 | 74 | 21 | **77.9%** | 5,216 |

Chi-squared on the 2×2 matched/failed contingency: **χ² = 0.002, p >> 0.10.** The 0.3pp difference is pure noise.

The 4.3pp improvement over the previous run's 73.5% came from *both* variants equally — not from tier12. Likely drivers: the function pool shifted toward easier targets, and/or the growing pool of already-matched siblings (+114 from last run) made pattern-copy-from-sibling work land more often across the board.

---

## Did the tier12 prompt changes actually land in agent behavior?

Mixed — some yes, some no:

| Intervention (Tier 1+2) | Signal | Outcome |
|---|---|---|
| Research-doc reads explicit in prompt | **6× more mentions in tier12 notes** (6 vs 1) | ✅ landed |
| Pattern-copy-first new step 0 | **2.4× more sibling-reference mentions** (17 vs 7) | ✅ landed |
| Permuter with runtime floors | **60% more permuter mentions** (8 vs 5) | ✅ landed |
| `#pragma control sched=0` surfaced | Same usage (4 vs 4) | ⟷ no change |
| System-reminder abort disclaimer | base 0, tier12 1 | ✗ abort STILL happened in tier12 |
| `attempts: 0` prohibition | **base 0, tier12 10** | ✗ BACKFIRED — tier12 had 10 `attempts: 0` entries |

The "attempts: 0 prohibition" is particularly striking. The tier12 prompt explicitly says "Every function in your batch gets at least one compile/compare cycle. Never report 'attempts: 0' for a batch sibling." Tier12 had **10 such entries** (all in a single session `ff166a25` where the agent decided 5 functions in the batch didn't belong in the chosen source file and bailed without attempting). base had zero.

Behaviors landed where the prompt added **information** (doc references, tool names). Behaviors did NOT land where the prompt added **prohibitions** (don't abort, don't skip siblings). Claude appears to deprioritize negative directives when the prompt is large.

---

## Size distribution of matches (by variant)

| Bucket | base | tier12 |
|--------|-----:|-------:|
| ≤32B | 13 | 14 |
| 33-64B | 28 | 31 |
| 65-128B | 22 | 24 |
| 129-256B | 3 | 5 |
| 257-512B | 0 | 0 |
| >512B | 0 | 0 |

Basically identical. Regression from last run: last run had 1 match in 257-512B, this run has 0. The "first ≥256B" match is a one-time event; we don't sustain it.

---

## Failure categorization (40 failed)

| Category | base | tier12 | Total | Share |
|----------|-----:|-------:|------:|------:|
| BRANCH_LIKELY | 6 | 7 | 13 | 33% |
| VFPU | 4 | 4 | 8 | 20% |
| REGISTER_ALLOC | 3 | 3 | 6 | 15% |
| SCHEDULING | 3 | 1 | 4 | 10% |
| OTHER | 0 | 3 | 3 | 8% |
| SCHED_FLAG (Makefile) | 2 | 0 | 2 | 5% |
| SYSTEM_REMINDER_ABORT | 0 | 2 | 2 | 5% |
| CLOSE_MISMATCH | 1 | 0 | 1 | 2% |
| M2C_FAILURE | 0 | 1 | 1 | 2% |

**BRANCH_LIKELY is now the top category at 33%** — up from 17% last run. It's not going away. The deferred compiler patch (decision 011) becomes more attractive as the failure tail gets dominated by this.

VFPU (20%) is the second-largest and unchanged — these are genuine "can't be expressed in C" cases.

---

## Errors (non-function-result)

| Event | Count | Detail |
|-------|------:|--------|
| `verify_failed` | 7 | 4 in one `eTriangleShape` cluster (tier12), 2 on `eDynamicFluid` (base), 1 other |
| `session_error` (timeout) | 4 | Split 2-2 between variants |
| `m2c_failed` | 1 | `eConfigBase::GetAudioBytesPerSecond` (base) |

Verify-failed up from 1 last run to 7 — agents are over-claiming matches more this run, particularly tier12's `eTriangleShape` cluster (4 of 7). Worth noting: tier12's more aggressive pattern-copy-first behavior may lead to false positives when the pattern visually matches but the bytes don't.

## Quality / norms audit

| Check | Result |
|-------|--------|
| File-scope `__asm__` (banned) | 0 files (verified via `awk '/^__asm__/'`) |
| Files with >40% asm content | 0 files |
| `tools/` modifications | 0 |
| `Makefile` modifications | 0 |
| `config/functions.json` | Match status only (expected) |
| Net diff | +3,673 / -222 lines across 59 files, 32 commits |

Clean. Source quality gate held.

---

## What to do about the null result

The tier12 prompt overhaul is a **real null result**. It's more expensive (prompt is ~3× larger = more tokens per session) and delivers 0.3pp improvement (pure noise). We shouldn't keep paying for it.

Options (ranked):

1. **Keep a slimmed-down tier12 as the new default.** Retain only the changes that showed behavioral signal: research-doc references, pattern-copy-first, permuter guidance. Drop the parts that didn't move the needle (system-reminder disclaimer, attempts:0 prohibition). Result: cheaper than tier12, more informed than base, no "large prompt drowning out directives" problem.

2. **Keep running base vs tier12 as-is and collect more data.** 170 function-pairs is a small sample for a bundled change; effects may show up later when the function pool shifts. Cost: ongoing token tax on half the sessions, no guarantee of significance.

3. **Revert to base as default, iterate on tier1+2 changes individually.** Run new A/B tests one intervention at a time (research-doc mandate only, then add pattern-copy, etc.) so we can isolate which specific changes actually help. Cost: 4-5 overnight runs to fully sweep. Highest signal.

4. **Stop A/B'ing prompts; attack failure categories directly.** BRANCH_LIKELY is now 33% of failures. The deferred compiler patch (decision 011) would address it. Higher expected impact than any prompt change at this point.

---

## Recommendations

1. **Default this run: Option 1 (slim tier12).** Build a `tier12b` variant that keeps the research-library section, pattern-copy-first step, and permuter runtime floors; drops the system-reminder disclaimer and attempts:0 prohibition. Run next overnight as `base,tier12b` to measure the slimmed-down version.

2. **Revisit decision 011 (bnel compiler patch) priority.** BRANCH_LIKELY climbed from 16% to 33% of failures in one run. If next run confirms the trend, the ~55-byte surgical patch becomes the best ROI path forward.

3. **Investigate the `ff166a25` tier12 session** that produced 10 attempts:0 entries. Read its transcript to understand why it decided batch siblings didn't belong in the chosen file — may be legitimate orchestrator-side mis-routing, or prompt-induced over-caution.

4. **Verify_failed cluster on eTriangleShape** (4/7 of this run's verify fails). Likely indicates the agent pattern-copied another class's implementation that compiles to similar but not identical bytes. Worth retrying that batch on base to see if it's variant-specific.

5. **Still no ≥256B matches** and last run's 280B was a one-off. Consider a targeted run on 257-512B functions specifically — bigger functions need different strategies and we're not stress-testing the tooling on them.

## Verdict

Productive run (+144 matched). But the prompt A/B is a null result — 77.8% vs 77.6%, indistinguishable. The tier12 changes landed partially in agent behavior but didn't translate to match-rate improvement. Recommend slimming tier12 and/or pivoting to BRANCH_LIKELY as the next lever. No norm violations. Safe to merge.
