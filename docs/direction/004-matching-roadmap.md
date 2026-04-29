# Matching Roadmap

Status snapshot: 2026-04-29, while `overnight/20260429-145324` was active. Numbers in this document are directional; refresh them after each merged run before using them for hard projections.

Implementation update: commit `cfad8f7` landed the first near-term match-lift slice: known unmatchable/import picker filtering, multi-exemplar prompt injection, static method-template guidance, and chained/N-way permuter mutations. The current production focus is to measure that lift on targeted `Write`, `GetType`, `New`, `AssignCopy`, constructor, and small/medium `VisitReferences` runs before choosing the next research-heavy slice.

This document extends `docs/direction/001-decompilation-phases.md` with the current production strategy for pushing the function matcher through the remaining work. The goal is to keep the project focused on the next highest-leverage work without pretending the whole remaining path is knowable upfront.

## Current Position

The project has moved out of the bootstrap phase. The matching pipeline works, the multi-agent orchestrator works, cost/token logging exists, and recent match-lift work has raised the hit rate on selected small and medium function categories.

Current database snapshot:

| Status | Functions | Bytes |
|--------|-----------|-------|
| Matched | 3,055 | 300,824 |
| Failed | 677 | 93,812 |
| Untried | 5,509 | 3,188,740 |
| In progress | 1 | 188 |
| Total | 9,242 | 3,583,564 |

That is about 33% matched by function count, but only about 8.4% matched by bytes. The byte percentage is a warning, not a direct time estimate. Small functions have high fixed overhead: setup, prompting, source inspection, compilation, verification, commit handling, and DB updates all cost time even when the function is only 80 bytes. Recent data already shows better byte throughput on selected 257-512B functions than on tiny wrappers.

The remaining pool is much larger than the matched pool:

| Untried size range | Functions | Bytes |
|--------------------|-----------|-------|
| 0-128B | 613 | 39,676 |
| 129-256B | 1,034 | 200,904 |
| 257-512B | 2,165 | 777,632 |
| 513-1024B | 1,101 | 775,420 |
| 1025-2048B | 434 | 622,120 |
| 2049B+ | 162 | 772,988 |

The center of gravity has shifted to 257-1024B functions, with the largest 596 untried functions holding about 1.4MB of code. The project should now optimize for bytes matched per agent-hour and category unlocks, not just raw function count.

## Interpretation

We should not assume "8.4% matched by bytes" means "8.4% through by time." That would be too pessimistic for three reasons:

1. Many medium functions are repetitive serialization, visitor, factory, copy, or type-registration code. Once a family is understood, each match buys several hundred bytes for similar session overhead.
2. Match lift can transfer across a whole category. A few well-chosen exemplars may turn many 300-1000B functions from research tasks into routine matches.
3. The early corpus included many tiny functions where fixed overhead dominated useful byte progress.

But we should also not extrapolate the recent small/medium hit rate across the whole remaining byte pool. The remaining work is not uniform. It contains:

- Easy churnable functions that just need agent throughput.
- Research-unlocked clusters that are slow until one pattern is solved, then fast.
- Tooling/compiler-blocked functions that should not be repeatedly retried until the blocker is fixed.
- Hard end-game functions where every match may need manual research, source restructuring, and diff-guided iteration.

The practical planning model is: the next large chunk can be faster by bytes than the first chunk, but the last chunk will be much slower.

## Operating Principles

1. Protect the matched corpus. A byte-exact match is valuable training data and future context. Never trade verification quality for speed.
2. Stop wasting model cycles on known-blocked categories. Track them explicitly and revisit after tooling/compiler work.
3. Prefer category unlocks over isolated hero matches. One hard exemplar is valuable when it becomes reusable context for dozens of related functions.
4. Measure by bytes, functions, hit rate, time, tokens, and cost. No single metric is enough.
5. Use cheaper horizontal models where they are productive, and reserve the strongest models for hard retries, research buckets, and uncertain categories.
6. Treat this roadmap as adaptive. If a category unexpectedly opens up, shift toward it. If a model or prompt starts failing, demote it quickly.

## Remaining Work by Category

The current rough classifier shows the following untried categories:

| Category | Untried funcs | Untried bytes | Avg size | Current read |
|----------|---------------|---------------|----------|--------------|
| Other / uncategorized | 2,792 | 1,552,972 | 556B | Mixed; needs finer taxonomy |
| Read | 555 | 226,796 | 409B | High count, but many blocked by known prologue scheduling divergence |
| Copy / assign | 422 | 271,176 | 643B | Likely good mid-game target after exemplars |
| Destructors | 362 | 139,340 | 385B | Mixed; many inheritance/delete idiom issues |
| VisitReferences | 315 | 499,480 | 1,586B | High byte value; likely template/research payoff |
| GetType | 296 | 94,180 | 318B | Good churn target |
| GetText / GetName | 192 | 82,644 | 430B | Mixed; string/layout dependent |
| Evaluate | 176 | 177,264 | 1,007B | High byte value; needs family research |
| Constructors | 151 | 43,372 | 287B | Useful for layout recovery |
| Write | 121 | 45,944 | 380B | Good churn target |
| Reset | 79 | 40,760 | 516B | Likely cluster-dependent |
| New | 43 | 12,992 | 302B | Good churn target |

The failed pile is also informative:

| Category | Failed funcs | Failed bytes | Likely implication |
|----------|--------------|--------------|--------------------|
| Read | 126 | 26,336 | Do not brute force until compiler/prologue issue is handled |
| Destructors | 64 | 10,528 | Need better destructor templates and inheritance context |
| Write | 53 | 8,240 | Some easy, some layout/order-sensitive |
| Copy / assign | 34 | 4,596 | Worth retrying with exemplars/permuter |
| New | 19 | 2,572 | Likely small idiom issues, good retry bucket |
| GetType | 11 | 1,988 | Good retry bucket with exemplars |

The failed pile is mostly not huge. That means many failures are not "too large for the model"; they are blocked by exact compiler behavior, missing source idioms, class layout gaps, or prompt/tooling weaknesses.

## Phase A: Current Churn Phase

Purpose: keep productive matching throughput high while avoiding known traps.

This phase should continue until the high-yield small/medium categories stop producing good match rates.

Primary targets:

- `Write`
- `GetType`
- `New`
- `AssignCopy`, `operator=`, and simple copy helpers
- constructors that reveal useful class layout
- small and medium destructors where inheritance context is simple
- small and medium `VisitReferences`
- simple `PlatformRead` and platform/config wrappers not affected by the known `Read` prologue issue

Recommended execution:

- Use `codex/gpt-5.4/high` as the main horizontal volume model while it remains cost-effective.
- Avoid `gpt-5.4-mini` for production matching unless later data contradicts the current poor signal.
- Use `gpt-5.5/high` selectively for hard retries, unfamiliar categories, and research-driven exemplar creation.
- Prefer disjoint matching for production throughput. Use shootouts only when deciding whether to promote or demote a model/prompt.
- Keep batch size small enough that failures do not tie up too much wall time, but large enough to amortize setup. Current batch size 2 is a reasonable default.

Landed support for this phase:

1. Known unmatchable CRT/SDK/import/mid-function-label targets are filtered out of normal picks.
2. Chained permuter mutations and short N-way simple-statement permutation are available for candidate search.
3. Prompts include ranked matched exemplars and static method-template guidance for the highest-yield method families.

Remaining operational work:

1. Make reports emphasize matched bytes per agent-hour and cost per matched byte, not just function hit rate.
2. Keep token/cost attribution working for every backend/model/effort tuple.
3. Keep the active run cleanup path strict: verify, revert out-of-scope edits, merge only clean matches.

Exit signals:

- High-yield 129-512B categories are mostly depleted.
- Match rate on disjoint production runs drops materially.
- A growing share of failures are due to repeated known patterns instead of one-off misses.

## Phase B: Exemplar and Template Lift

Purpose: turn medium functions into routine matches by giving agents better local patterns.

This is the most important near-term match-lift phase. The next byte-heavy pool is too large for generic prompting. Agents need category-specific examples from the same binary, same object, same class family, and same compiler behavior zone.

Baseline support is now landed; expand it where current-run data shows payoff:

1. Matched corpus exemplars in prompts.
   - Prefer same method name and same object file.
   - Prefer same class prefix/family when exact method exemplars are scarce.
   - Currently injects source exemplars; add compact disassembly shape only if failures show source alone is insufficient.
   - Avoid flooding prompts; 2-4 high-quality examples are usually better than a large dump.

2. Cross-class method templates.
   - `New`
   - `Write`
   - `GetType`
   - `AssignCopy`
   - constructors
   - destructors
   - `VisitReferences`
   - `Evaluate`
   - safe subsets of `Read`
   - Current implementation is static guidance in the orchestrator; generated corpus-derived templates are a follow-up if static notes are too shallow.

3. Category-specific prompt notes.
   - Known source ordering sensitivities.
   - Common SNC codegen idioms.
   - Known false paths, especially `extern "C"` wrappers for class methods and blocked `Read` variants.

4. Template-aware target picking.
   - Once a strong exemplar exists, pick nearby related functions while that context is fresh.
   - Prefer clusters where matched siblings already define class layout.

Expected payoff:

- Higher first-attempt hit rate.
- Better byte throughput in 257-1024B functions.
- Faster retry resolution because the model sees the correct local idiom.

Unknowns:

- How far templates transfer across `gcDo*`, `gcVal*`, engine `e*`, and core `c*` families.
- Whether large `VisitReferences` and `Evaluate` methods are repetitive enough for templates or require deeper per-class analysis.
- How much prompt bloat increases token cost versus match rate.

Decision point:

- If exemplars improve match rate meaningfully, make exemplar selection a first-class part of every production prompt.
- If exemplars mostly help only narrow clusters, use them in targeted campaigns rather than globally.

## Phase C: Mid-Game Targeted Campaigns

Purpose: move from broad random matching to planned attacks on high-byte, high-transfer clusters.

The mid-game is mainly the 257-1024B pool:

- 2,165 untried functions from 257-512B.
- 1,101 untried functions from 513-1024B.
- Together they hold about 1.55MB, roughly half of all untried bytes.

At this point the scheduler should increasingly ask: "Which cluster can we unlock next?" rather than "Which function is next?"

Likely campaigns:

1. Serialization campaign.
   - `Write`
   - safe `Read`
   - `PlatformRead`
   - config/template objects
   - cFile/cReadBlock/cWriteBlock idioms

2. Reference visitor campaign.
   - `VisitReferences`
   - handle/reference fields
   - arrays/lists of handles
   - object graph traversal patterns

3. Copy/layout campaign.
   - `AssignCopy`
   - `operator=`
   - constructors
   - destructors
   - field layout confirmation

4. Action/value template campaign.
   - `gcDo*`
   - `gcVal*`
   - `Evaluate`
   - `Read`
   - `Write`
   - `VisitReferences`

5. UI campaign.
   - `gcUI*`
   - widgets/dialogs/controls
   - text/name getters
   - sprite/material/control state methods

6. Entity/controller campaign.
   - `gcEntity`
   - `gcEntityController*`
   - `eSimulatedController`
   - `gcPlayer`
   - controller template methods

7. Engine subsystem campaigns.
   - audio/movie/platform wrappers
   - rendering/material/light methods
   - camera methods
   - networking wrappers

The campaign unit should be small enough to evaluate quickly: one category/family for one run or half-run, then review hit rate, byte throughput, failure reasons, and exemplar quality.

Recommended model allocation:

- `gpt-5.4/high`: broad production campaigns where patterns are known.
- `gpt-5.5/high`: exemplar creation, failures that look logically close, and first passes at unfamiliar clusters.
- Claude or other backends: use only if A/B data shows a specific category advantage.

Exit signals:

- Most 257-1024B repetitive functions are either matched or classified.
- Remaining medium failures cluster around compiler bugs, missing type knowledge, or complex control flow.
- The >1KB pool becomes the dominant source of remaining bytes.

## Phase D: Compiler and Tooling Unlocks

Purpose: fix systemic blockers that are currently turning matchable logic into failed attempts.

This phase should run in parallel with Phase B/C whenever possible, because it prevents wasted agent time.

Highest-priority compiler/tooling items:

1. `Read` prologue scheduler divergence.
   - Many `Read(cFile &, cMemPool *)` failures differ only in prologue scheduling.
   - Repeated model retries are low-value until this is fixed.
   - Implement the smaller bnel patch scaffolding first if it accelerates the pspcor patch workflow.

2. Chained mutation and N-way statement permutation.
   - Medium functions often fail because independent stores or calls are in the wrong order.
   - A practical text/block permutation system is likely cheaper than a full AST rewrite.

3. Diff-guided retries.
   - Convert byte diffs into prompt hints.
   - Record common mismatch signatures.
   - Retry only when the mismatch class is known to be model-fixable.

4. Safer compile/verify isolation.
   - Avoid direct verify and full compile writing the same object path concurrently.
   - Use unique object output paths or serialization where needed.
   - This is less strategic than matching logic, but it reduces wasted review time.

5. Better function classification.
   - Split the large `other` bucket into useful families.
   - Identify likely SDK/CRT/mid-function labels earlier.
   - Tag functions as `compiler_blocked`, `needs_layout`, `needs_exemplar`, `hard_logic`, or `unmatchable`.

Unknowns:

- Whether the prologue scheduling divergence is patchable cleanly in the compiler.
- How many currently failed `Read` functions become trivial after the patch.
- Whether additional compiler divergences appear as larger functions become common.

Decision point:

- If a compiler patch unlocks a large failure cluster, immediately rerun that category before broad random matching.
- If compiler patching stalls, quarantine the blocked functions so they do not poison production runs.

## Phase E: Large Function Strategy

Purpose: make progress on >1KB functions without treating each one as a bespoke heroic effort.

The large pool is small by count but huge by bytes:

- 434 untried functions from 1025-2048B.
- 162 untried functions over 2049B.
- Together they hold about 1.4MB of untried code.

The strategy should be to classify large functions before attempting them blindly.

Large function classes:

1. Repetitive large functions.
   - Long `VisitReferences`.
   - Long `Evaluate` dispatch/value logic.
   - Serialization over many fields.
   - Registration/static-initializer-like chains.
   - These may be good byte ROI once templated.

2. Domain logic functions.
   - Game state updates.
   - entity behavior.
   - camera/control systems.
   - UI flow.
   - These require stronger source understanding and likely more 5.5 usage.

3. Engine/math/physics functions.
   - collision
   - geometry
   - shape queries
   - simulation
   - These may need dedicated subsystem research.

4. Compiler-generated or library-like functions.
   - static initializers
   - CRT pieces
   - SDK wrappers
   - These need classification before model attempts.

Large function workflow:

1. Generate a compact preflight dossier.
   - size
   - callees/callers
   - matched siblings
   - class fields
   - similar matched functions
   - decompiler output quality
   - likely category

2. Decide attempt mode.
   - routine template attempt
   - research attempt
   - hold for compiler/tooling
   - mark unmatchable/library candidate

3. For research attempts, produce durable notes.
   - The value is not only the one match.
   - Capture idioms and constraints for future large functions.

4. Use staged verification.
   - First get compile-correct source.
   - Then get structurally close.
   - Then use diff-guided retry/permutation.

Unknowns:

- Whether the largest functions are mostly repetitive engine/template code or genuinely bespoke logic.
- Whether current m2c output is useful enough above 1KB.
- Whether Ghidra or another analysis pass becomes worth the setup cost for large functions.

Decision point:

- If large repetitive functions match well with templates, promote them into normal campaigns.
- If large functions are mostly bespoke, keep them for focused research sessions and avoid burning horizontal agents.

## Phase F: Failure Revisit and End-Game

Purpose: turn the failed backlog from a pile into a triage system.

The last 5-10% will likely consume a disproportionate share of total effort. It should be handled with classification and evidence, not repeated blind retries.

Failure statuses to add or enforce:

- `failed_retryable`: likely model/source issue; retry with better context.
- `blocked_compiler`: known or suspected compiler codegen divergence.
- `blocked_layout`: missing or wrong class/struct definition.
- `blocked_tooling`: verifier, symbol, object, or scheduler problem.
- `needs_research`: subsystem or large-function understanding needed.
- `unmatchable`: not a true C function target, SDK/import/mid-function label, or incompatible runtime artifact.

End-game loop:

1. Group failures by mismatch signature, method family, object file, and class family.
2. Solve or quarantine the largest group first.
3. Add a durable note or tool rule for each solved group.
4. Retry only when new information changes expected outcome.
5. Keep a separate list of functions that are valuable demonstration targets even if they are not the easiest remaining matches.

Likely hard buckets:

- physics/collision/geometry functions
- large `Evaluate` and update loops
- large `VisitReferences` with complex object graphs
- CRT/library fragments
- static initializer chains
- functions affected by SNC version or scheduler divergence
- functions requiring exact class layout across inheritance chains

End-game success requires accepting that some work becomes manual research. The automation goal is to shrink that manual set as much as possible and make every manual discovery reusable.

## Model and Cost Strategy

Current working hypothesis:

- `gpt-5.4/high` is the best volume engine when category/context is good.
- `gpt-5.5/high` is best reserved for difficult, ambiguous, or research-heavy matches.
- `gpt-5.4-mini/high` is not currently production-worthy for this task.
- Low-effort modes are not useful unless future data shows a specific easy bucket where they preserve hit rate.

The reporting system should answer these questions after every meaningful run:

1. Matched functions per agent-hour.
2. Matched bytes per agent-hour.
3. Matched bytes per dollar.
4. Success rate by size bucket.
5. Success rate by method family.
6. Failure reason distribution.
7. Token usage per matched byte and per matched function.

Decision rules:

- Promote a cheaper model only if it wins or ties on cost per matched byte after accounting for failures and cleanup.
- Demote a model quickly if it creates high failure volume without speed advantage.
- Use true paired shootouts for model promotion decisions, but keep normal production runs disjoint for throughput.

## Scheduling Strategy

Short-term default:

- Run disjoint production matching with enough workers to use available rate limits without causing request storms.
- Prefer `gpt-5.4/high` for broad production.
- Keep batch size at 2 unless data shows a better tradeoff.
- Avoid known-blocked categories in production picks.

Medium-term:

- Add target modes for category campaigns.
- Add exemplar-aware target scheduling.
- Add failure quarantine so bad buckets do not keep reappearing.

Long-term:

- Maintain a rolling pool of:
  - easy churn targets
  - template campaign targets
  - research targets
  - compiler-blocked targets
  - end-game targets

The scheduler should be allowed to change tactics as evidence changes. The important part is that every run has a clear purpose: churn, campaign, shootout, retry, or research.

## Metrics to Review After Each Run

Minimum run review:

1. How many functions and bytes matched?
2. How many functions and bytes failed?
3. Which categories produced the most matched bytes?
4. Which categories produced failures?
5. Were failures mostly model misses, known compiler blockers, or bad target selection?
6. Did any out-of-scope edits or verifier/tooling issues occur?
7. Did token/cost capture work?
8. Should the next run continue the same target pool or switch campaigns?

Periodic deeper review:

1. Recompute untried distribution by size, category, object, and class.
2. Recompute failed distribution by likely blocker.
3. Update model cost/performance tables.
4. Promote successful exemplars into templates.
5. Move newly identified impossible targets out of normal production.

## Roadmap Order

The likely best order from here:

1. Continue current production churn while it is producing clean matches.
2. Measure the landed match-lift slice on targeted campaigns and compare hit rate, bytes/hour, and cost/byte against recent random runs.
3. Improve reporting around matched bytes per agent-hour and cost per matched byte.
4. Run targeted campaigns on `Write`, `GetType`, `New`, `AssignCopy`, constructors, and small/medium `VisitReferences`.
5. Add diff-guided retry support once failures show repeated small byte diffs that prompts/permuter do not fix.
6. Start compiler patch work for the `Read` prologue divergence, using smaller compiler-patch scaffolding first if useful.
7. Shift from broad matching to mid-game campaigns across UI, entity/controller, action/value, serialization, visitor, and engine subsystem clusters.
8. Classify >1KB functions before attempting them at scale.
9. Build a failure triage system and revisit failures by group.
10. Reserve the final phase for compiler divergences, hard subsystem research, static initializers, CRT/library questions, and large bespoke functions.

## Unknowns and Maneuvering Room

The plan should change if any of these turn out differently than expected:

- Medium functions may stay easy longer than expected. If bytes/hour remains high, keep pushing production campaigns before pausing for deeper tooling.
- Large `VisitReferences`/`Evaluate` functions may template well. If so, they become priority byte targets.
- The `Read` compiler issue may unlock a large retry pool. If fixed, immediately retarget `Read` functions.
- A new compiler divergence may appear in larger functions. If so, quarantine affected categories early.
- A cheaper model may prove strong on narrow template categories. If so, use it only there.
- Current category classification may be too coarse. If the `other` bucket hides several easy families, split it aggressively.
- Some large functions may be non-game library/runtime code. If so, mark or isolate them instead of chasing them as normal source.

The central tactic is to keep the matching frontier moving while repeatedly turning discoveries into reusable leverage: templates, exemplars, scheduler rules, failure tags, compiler patches, and reports.
