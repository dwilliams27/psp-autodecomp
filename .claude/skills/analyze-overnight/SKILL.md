# Analyze Overnight Run

Analyze the most recent overnight matching run and produce a detailed post-mortem report.

## Steps

### 1. Find the latest log

```bash
ls -t logs/match_*.jsonl | head -1
```

### 2. Parse the match log

Read the JSONL log file. Each line is a JSON object with fields: `address`, `name`, `size`, `status` (matched/failed), `attempts`, `file`, `notes`.

Produce counts: total attempted, matched, failed, match rate.

### 3. Read session result files

Check `logs/session_results/*.json` for detailed per-function notes. These contain the agent's explanation of what they tried and what blocked them.

Categorize each failure by root cause:
- **BRANCH_LIKELY**: beqzl/bnezl/bnel divergence between our SNC and the original
- **VFPU_DELAY_SLOT**: sv.q/mtv can't be scheduled into delay slots from inline asm
- **REGISTER_ALLOC**: correct instructions but wrong register assignment
- **SCHEDULING**: correct instructions but wrong order
- **CLOSE_MISMATCH**: within ~8-16 bytes, ran out of ideas
- **ASM_REJECTED**: source quality gate rejected pure assembly
- **TIMEOUT**: session ran out of time
- **OTHER**: doesn't fit above categories

### 4. Read Claude Code session logs

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

### 5. Produce the report

Save to `docs/postmortems/YYYY-MM-DD-overnight.md` with:

```markdown
# Post-Mortem: Overnight Run (YYYY-MM-DD)

## Summary
| Metric | Value |
|--------|-------|
| Duration | ... |
| Sessions | ... |
| Functions attempted | ... |
| Matched | ... (rate%) |
| Failed | ... |
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

### 6. Commit the report

Stage and commit the post-mortem:
```bash
git add docs/postmortems/
git commit -m "Add overnight run post-mortem YYYY-MM-DD"
```
