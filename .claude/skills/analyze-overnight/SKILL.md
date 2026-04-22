# Analyze Overnight Run

Analyze the most recent overnight matching run and produce a detailed post-mortem report.

**Critical principle: repo integrity comes before narrative.** If the run left the DB in a bad state or introduced build breakage, surface that loudly BEFORE producing a post-mortem. Drift between the DB's "matched" claims and the actual repo state is how the 2026-04-21 silent-fallback incident accumulated unnoticed — every overnight audit from here on checks for it.

## Steps

### 1. Find the latest log

```bash
ls -t logs/match_*.jsonl | head -1
```

### 2. Repo-integrity audit (MUST RUN; STOPS THE PIPELINE IF ANY FAIL)

Before analyzing the run's content, verify the run didn't corrupt anything:

**2a. Full DB re-verification.**
```bash
python3 tools/verify_matches.py
```
Exit code must be 0. Any non-zero means the DB claims matches that no longer byte-verify — STOP, report what failed, and ask the user whether to run `--fix` before continuing. Do not write the post-mortem while the DB is lying.

**2b. Build-health sweep.** Every src file in `src/` must compile:
```bash
broken=()
for src in src/*.cpp src/*.c; do
    [ -f "$src" ] || continue
    if ! make "build/${src}.o" >/dev/null 2>&1; then
        broken+=("$src")
    fi
done
echo "Broken: ${#broken[@]}"
printf '  %s\n' "${broken[@]}"
```
Zero broken is the only acceptable result. If any file fails, stop and surface the list — a commit from overnight introduced the break. (See `enhancements.md` #8 for why this matters and the drift paths that cause it.)

**2c. Provenance coverage.** Every newly-matched entry MUST carry both `src_file` and `symbol_name` (proving it went through the hardened `byte_match.check_byte_match` pipeline). Run:
```bash
python3 -c "
import json
with open('config/functions.json') as f: db = json.load(f)
bad = [f for f in db if f.get('match_status') == 'matched'
       and (not f.get('src_file') or not f.get('symbol_name'))]
print(f'matched entries missing provenance: {len(bad)}')
for f in bad[:10]:
    print(f'  {f[\"address\"]} {f[\"name\"][:60]}')
"
```
Zero is required. Non-zero means something bypassed the orchestrator's verify flow — investigate before writing the post-mortem. Likely suspects: a human-run `compare_func.py` without `--update-db` discipline, or a direct DB edit.

**2d. Session-results ↔ DB consistency.** Every entry in this run's `logs/session_results/*.json` with `status: matched` should map to a DB entry with `match_status == "matched"` AND matching `src_file`. Mismatches indicate the orchestrator's verify rejected a claim (expected — log and count) OR committed something without recording provenance (not expected — investigate).

**2e. Commit-state sanity.** Working tree should be clean at end of run:
```bash
git status --porcelain
```
If non-empty, the run left uncommitted changes (partial session, crashed orchestrator, etc.). List them before writing the report.

**If any of 2a-2e fail**: put the failure list at the TOP of the post-mortem under a `## BROKEN` section, commit the post-mortem with that, and ask the user how to proceed. Don't silently carry on to the narrative analysis — the user's decision tree depends on knowing the repo is clean first.

### 3. Parse the match log

Read the JSONL log file. Each line is a JSON object with fields: `address`, `name`, `size`, `status` (matched/failed), `attempts`, `file`, `notes`.

Produce counts: total attempted, matched, failed, match rate. Cross-check against the DB delta: matched count reported in function_result events should equal (new `matched` entries in DB) + (claimed-then-verify-rejected entries). A discrepancy means either a race, a missed event, or another silent-path somewhere — worth calling out.

### 4. Read session result files

Check `logs/session_results/*.json` for detailed per-function notes. These contain the agent's explanation of what they tried and what blocked them.

Categorize each failure by root cause:
- **BRANCH_LIKELY**: beqzl/bnezl/bnel divergence between our SNC and the original
- **VFPU_DELAY_SLOT**: sv.q/mtv can't be scheduled into delay slots from inline asm
- **REGISTER_ALLOC**: correct instructions but wrong register assignment
- **SCHEDULING**: correct instructions but wrong order
- **CLOSE_MISMATCH**: within ~8-16 bytes, ran out of ideas
- **ASM_REJECTED**: source quality gate rejected pure assembly
- **COMPILE_FAILED**: agent wrote broken src; orchestrator flagged via `CompileFailed`
- **TIMEOUT**: session ran out of time
- **OTHER**: doesn't fit above categories

### 5. Read Claude Code session logs

The actual agent session logs are at:
```
/Users/autodecomp/.claude/projects/-Users-dwilliams-proj-psp-autodecomp/*.jsonl
```

If permission denied, tell the user to run:
```bash
sudo chmod -R a+r /Users/autodecomp/.claude/projects/-Users-dwilliams-proj-psp-autodecomp/
```

For each session, extract:
- Functions assigned (from the initial prompt — look for "FUNCTION" in user messages)
- Total tool calls (count tool_use blocks in assistant messages)
- Whether research docs were read (Read tool calls to `docs/research/`)
- Number of compare_func runs (Bash calls containing "compare_func")
- Number of Write calls (source file iterations)
- Key failure text from assistant messages (search for "mismatch", "byte", "diff", "stuck", "branch-likely", "beqzl", "bnezl", "delay slot")

### 6. Produce the report

Save to `docs/postmortems/YYYY-MM-DD-overnight.md` with:

```markdown
# Post-Mortem: Overnight Run (YYYY-MM-DD)

## Repo Integrity
| Check | Result |
|-------|--------|
| verify_matches (exit code 0) | ✓ / ✗ (N problems) |
| All src/*.cpp compile | ✓ / ✗ (N broken: …) |
| All matched entries have src_file + symbol_name | ✓ / ✗ (N missing) |
| Session-results ↔ DB consistency | ✓ / ✗ (details) |
| Working tree clean at run end | ✓ / ✗ |

If any row is ✗, the body should lead with "## BROKEN" section and the
operator decides whether to act before the analysis is useful.

## Summary
| Metric | Value |
|--------|-------|
| Duration | ... |
| Sessions | ... |
| Functions attempted | ... |
| Matched | ... (rate%) |
| Failed | ... |
| DB delta: matched | +N (from M to M+N) |
| DB delta: failed | +N |
| Avg tools/session | ... |
| Research docs read | X/Y sessions |

## Matches
For each matched function: name, size, what approach worked.

## Failure Analysis
### By Category
Table of failure categories with counts and percentages.

### Per-Function Details
For each failed function: name, size, category, byte diff (if known),
what the agent tried, what blocked it.

## Agent Behavior
- Did agents read research docs? Which ones?
- Average iterations (compare_func calls) per function
- Any agents that gave up too early?
- Any agents that submitted pure assembly? (rejected by quality gate)

## Recommendations
What should change for the next run? New research needed?
Compiler version issues? Tooling improvements?
```

### 7. Commit the report

Stage and commit the post-mortem:
```bash
git add docs/postmortems/
git commit -m "Add overnight run post-mortem YYYY-MM-DD"
```
