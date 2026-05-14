---
name: analyze-overnight
description: Verify, clean, analyze, and decide whether to merge psp-autodecomp overnight runs. Use after an autonomous run finishes, when checking a mixed/circuit-breaker run, when investigating failed-source leaks, or when producing a post-mortem.
---

# Analyze Overnight Run

Use this skill for the whole post-run operator flow: repo integrity first, cleanup second, merge decision third, narrative analysis last.

## Principles

- Repo integrity comes before narrative. Do not write a post-mortem while `config/functions.json` claims matches that no longer byte-verify.
- Merge only clean runs automatically. Pause on mixed outcomes, circuit breakers, non-fast-forward branches, source leaks, or verification failures.
- Failed attempts may leave source files behind. Audit the final branch diff before merging.
- Do not use broad destructive cleanup. Restore or delete explicit paths only.
- Run `/pre-commit-review` before every commit, including cleanup/report commits.

## Fast Post-Run Flow

### 1. Identify the run

```bash
git status --short --branch
ls -t logs/match_*.jsonl | head -1
git log --oneline --decorate --format='%h %s' main..HEAD | head -30
```

If the branch is not based on `main`, stop and report the non-ff condition.

### 2. Verify repository integrity

Run all checks before analyzing agent behavior:

```bash
python3 tools/verify_matches.py
python3 tools/audit_overnight_sources.py --base main --head HEAD
python3 -c 'import sys; sys.path.insert(0, "tools"); from orchestrator import verify_tree_compiles; verify_tree_compiles()'
```

`verify_matches.py` verifies source files in parallel by default. Use `--jobs 1` only when debugging sequential verifier behavior.

Then check provenance and worktree state:

```bash
python3 -c "
import json
with open('config/functions.json') as f:
    db = json.load(f)
bad = [f for f in db if f.get('match_status') == 'matched'
       and (not f.get('src_file') or not f.get('symbol_name'))]
print(f'matched entries missing provenance: {len(bad)}')
for f in bad[:10]:
    print(f'  {f[\"address\"]} {f[\"name\"][:60]}')
"
git status --porcelain
```

Any failure goes at the top of the report. Do not merge.

### 3. Clean failed-source leaks

If `tools/audit_overnight_sources.py` reports leaked `src/` paths:

- For files that existed on `main`, restore explicit paths with `git checkout main -- path/to/file.cpp`.
- For files that did not exist on `main`, delete the explicit paths.
- Before committing cleanup, rerun `verify_matches.py`, `audit_overnight_sources.py --staged`, and `verify_tree_compiles()`.
- After committing cleanup, rerun `audit_overnight_sources.py` without `--staged`.
- Commit the cleanup only after `/pre-commit-review`.

This cleanup removes failed-attempt source edits from the final branch while preserving DB failure notes and run logs.

### 4. Classify merge readiness

Parse the latest match log and session results. A clean auto-merge requires all of:

- `verify_matches.py` passes.
- Tree compile passes.
- `audit_overnight_sources.py` passes.
- Matched entries have `src_file` and `symbol_name`.
- Working tree is clean after any cleanup commit.
- `main` is an ancestor of `HEAD`.
- No circuit breaker, dead backend, or untriaged system-error cluster.

If all pass:

```bash
git checkout main
git merge --ff-only <overnight-branch>
git log --oneline -5
```

If the run is mixed or hit a circuit breaker, stop after verification/cleanup and report the merge pause. Merge only if the user explicitly accepts that triage state.

## Analysis

### Run metrics

Read the JSONL log. Produce:

- Duration.
- Attempted, matched, failed, system errors.
- Match rate.
- Dead/circuit-breaker backends.
- DB status delta from before/after if available.

### Session results

Read `logs/session_results/*.json` for detailed per-function notes. Categorize failures:

- `BRANCH_LIKELY`: beqzl/bnezl/bnel divergence between our SNC and the original.
- `VFPU_DELAY_SLOT`: VFPU op cannot be scheduled into delay slots from inline asm.
- `REGISTER_ALLOC`: correct instructions but wrong register assignment.
- `SCHEDULING`: correct instructions but wrong order.
- `CLOSE_MISMATCH`: within roughly 8-16 bytes, ran out of ideas.
- `ASM_REJECTED`: source quality gate rejected pure assembly.
- `COMPILE_FAILED`: agent wrote source that does not compile.
- `TIMEOUT`: session ran out of time.
- `TOOLING`: tool or environment failure.
- `OTHER`: does not fit above categories.

### Agent behavior

For Claude Code logs, inspect:

```text
/Users/autodecomp/.claude/projects/-Users-dwilliams-proj-psp-autodecomp/*.jsonl
```

If permission is denied, tell the user to run:

```bash
sudo chmod -R a+r /Users/autodecomp/.claude/projects/-Users-dwilliams-proj-psp-autodecomp/
```

Extract:

- Functions assigned.
- Total tool calls.
- Whether `docs/research/` files were read.
- `compare_func.py` count.
- Source edit count.
- Key failure text.

For Codex runs, use the run log, session result JSON, and any backend-specific transcript paths available in the run metadata.

### Permuter audit

Count permuter use and judge interface quality:

- Actual `tools/permuter.py` invocations.
- Gate skips due to size mismatch or large baseline diff.
- Exact matches, non-exact improvements, and no-improvement evidence.
- Exceptions or raw tracebacks.
- Agent misuse such as running the permuter before generated size matches target size.

Recommended interpretation:

- Keep the permuter as a last-mile exact-size tool.
- If generated symbol size differs, source shape is wrong; do not spend long permuter time.
- For non-exact improvements, use `--save-best --save-improved` together unless the CLI behavior changes.
- Tooling issues should become bugs or prompt changes, not agent blame.

## Post-Mortem

When requested, save to `docs/postmortems/YYYY-MM-DD-overnight.md`:

```markdown
# Post-Mortem: Overnight Run (YYYY-MM-DD)

## Repo Integrity
| Check | Result |
|-------|--------|
| verify_matches | PASS/FAIL |
| tree compile | PASS/FAIL |
| failed-source leak audit | PASS/FAIL |
| matched provenance | PASS/FAIL |
| working tree | PASS/FAIL |

## Summary
| Metric | Value |
|--------|-------|
| Duration | ... |
| Functions attempted | ... |
| Matched | ... |
| Failed | ... |
| System errors | ... |
| Circuit breaker | ... |

## Matches
...

## Failure Analysis
...

## Permuter
...

## Recommendations
...
```
