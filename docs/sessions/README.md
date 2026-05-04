# Session Journals

Working-session narratives that capture intent, plan, and outcomes at the "campaign" level — between individual commit messages (too granular) and direction docs (too strategic).

## Format

One file per working session: `YYYY-MM-DD.md` (use `-a`/`-b` suffix for multiple sessions on the same day).

Each file has:
- **Starting state** — snapshot of matched/failed/untried counts, recent run results
- **Assessment** — what the data says right now, what's changed since last session
- **Plan** — phased goals for this session and rationale
- **Log** — timestamped updates as work progresses
- **Outcomes** — what actually happened, what shifted, what carries forward

## Purpose

- Correlate with `git log` to understand "why were these commits made"
- Track the narrative arc of the project's mid-level strategy
- Mine later for patterns: what worked, what didn't, where time was wasted
- Provide context for agents starting fresh — read the most recent session to understand current momentum

## Conventions

- Keep entries honest: record what actually happened, not what was planned
- Update the Log section as you go, don't backfill from memory
- Link to relevant commits, decisions, or research docs when referencing them
- Don't duplicate content from decisions/ or direction/ — reference them
