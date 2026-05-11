Review the current staged changes, or the recent uncommitted changes if
nothing is staged, using four specialized perspectives. Launch all four as
separate sub-agents so each has isolated context. Run all four in parallel,
then provide a unified summary.

IMPORTANT: You MUST spawn 4 separate review sub-agents. Do NOT do the review
only yourself; if you wrote the code, you are not objective enough to be the
only reviewer.

## Agent 1: Code Quality

Review for:
- Clear Python/tooling code that a future decomp maintainer can debug quickly
- Focused tests for changed behavior, including edge cases and generated-target
  behavior when relevant
- Explicit error handling where DB rows, target files, compiler outputs, or
  subprocess results are malformed
- Type annotations on new helpers and no unnecessary abstractions
- Generated docs/configs are reproducible from committed tools

## Agent 2: Security & Path Safety

Review for:
- No hardcoded secrets, tokens, local credentials, or private key material
- CLI inputs, DB fields, target JSON rows, and output paths are validated before
  they influence filesystem writes
- No command injection or unsafe `shell=True`; subprocess commands use explicit
  argv lists unless there is a documented project reason
- No path traversal when creating logs, snapshots, battle packets, or config
  files
- No new network dependency or package install path in normal tooling
- No destructive filesystem/git operation hidden inside helper code

## Agent 3: Decomp Architecture & Workflow

Review for:
- Changes fit the existing `tools/`, `config/`, `docs/`, `src/`, and `include/`
  ownership boundaries
- Matching integrity remains source -> compiler -> identical bytes; no
  post-compilation byte rewriting or result laundering
- Generated target lists agree with orchestrator picker rules, especially
  import/runtime/unmatchable filters
- Data flow is simple and inspectable: DB -> classify/prep -> target list or
  prompt/battle packet
- No circular imports or tight coupling that makes the orchestrator, prompt
  variants, or research tools harder to run independently
- New docs explain methodology enough that we can regenerate or update them

## Agent 4: No Silent Fallbacks & Data Integrity

This is the most critical review. Specifically look for:
- Any `.get(key, default)` where the default masks a real DB/schema problem
- Any `try/except` that catches and continues without the caller knowing
- Any `or []`, `or {}`, `or ""`, or `or None` that hides malformed input
- Any `if x is None: return` that skips required work without raising
- Any "graceful degradation" that is not tagged with `# FALLBACK-OK`
- Any default parameter value that could mask missing required caller data
- Any logging-only error handling where the error should propagate

For each finding, state the file, the line, what the silent fallback is, and
what should happen instead: raise, propagate, or justify with `# FALLBACK-OK`.

## Output Format

Provide a unified summary with:

1. **Blocking issues**: must fix before commit
2. **Warnings**: should fix, but not necessarily blocking
3. **Notes**: observations and suggestions

If there are zero blocking issues, state `LGTM — no blocking issues found.`
