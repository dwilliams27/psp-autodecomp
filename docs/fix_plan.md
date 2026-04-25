# Symbol-signature fidelity fix plan

Tracks remediation of `docs/bugs.md` #3 — a class of currently-matched
entries whose reconstructed C++ source has byte-correct compiled output
but structurally-wrong signatures (typedef-int collapses, extern-C with
explicit `void *self`, refactored shapes that expose intermediate types).

Discovered 2026-04-24 while fixing the AMBIGUOUS-overload disambiguator
(commit `c70a9ba`). Bytes are not at risk — the verifier's
byte-comparison was always correct. The risk is **training-data
poisoning**: a model trained on this corpus would learn extern-C
patterns for what should be C++ methods and weak typedefs for what
should be strong types.

Audit tool: `tools/audit_symbol_signatures.py` (lands in Phase 1).

No new matching runs proceed until all four phases are complete.

---

## Phase 1 — Triage [done]

Land the audit tool and reset confirmed-bad entries so they re-enter the
untried pool.

- [x] Commit `tools/audit_symbol_signatures.py`.
- [x] Re-run audit; identify the 17 "true-divergence" param-count entries
      cleanly (filtering out the 31 self-arg false positives via
      heuristic: `len(actual) == len(expected) + 1` and `actual[0] in
      {void *, const void *, void *self, const void *self}`).
- [x] Reset 7 param-types entries to untried with `failure_notes`
      pointing at the typedef-collapse / refactor concern.
- [x] Reset 17 true-divergence entries to untried with the same.
- [x] Commit.

**Result**: 24 entries flipped matched → untried. Audit re-run shows
`param-types: 0`, `param-count: 31` (down from 48 — the remaining 31
are exactly the self-arg pattern, queued for Phase 4).

## Phase 2 — Quality gate

Stop new matches from landing with the same anti-patterns.

- [ ] In `tools/orchestrator.py` (`validate_source_quality` or sibling):
      reject a match whose DB entry has a `class_name` AND whose
      reconstructed src defines the function as `extern "C"` or as a
      free function with `void *self` (or `void *`) first parameter.
- [ ] Mirror the existing pure-asm rejection's "loud failure" semantics
      — log the event, mark the function `failed` with notes.
- [ ] Test on existing rejected files (positive control) and a clean
      class-method match (negative control).
- [ ] Commit.

## Phase 3 — Typedef-int header audit

Migrate weak `typedef int X;` declarations to proper class/struct decls
where the original binary has X as a real C++ type.

- [ ] Enumerate every `typedef (unsigned )?int X;` in `include/`.
- [ ] For each, query `mangled_symbol` data: does any function in the
      original binary mention X as a class type (`6X` style) in its
      mangled signature? If yes, X is a real type, not a numeric alias.
- [ ] For X identified as a real type: produce a list of affected
      matched DB entries (functions whose original signature uses X).
- [ ] Reset affected entries to untried with notes.
- [ ] Leave the header migration itself for a follow-up — Phase 3
      ends when the affected entries are queued for retry. Header
      changes happen organically when codex/claude re-attempts them
      with a corrected header.
- [ ] Commit.

## Phase 4 — Safe-name / self-arg retirement

The ~31 stable-but-structurally-wrong matches: methods written as
extern-C functions with explicit `void *self` first-arg, or as
`Class__method` / `Class_method` safe-names. Reset to untried so the
next overnight pass produces proper C++.

- [ ] Use `tools/audit_symbol_signatures.py` to enumerate the entries
      whose reconstruction matches the safe-name / self-arg pattern.
- [ ] Reset all to untried with `failure_notes` referencing Phase 2's
      quality gate so the next attempt won't reproduce the anti-pattern.
- [ ] Commit.

## Done criteria

All four phases marked complete. `python3 tools/audit_symbol_signatures.py`
shows zero entries in `param-types` and `param-count` (modulo audit-tool
edge cases the tool itself documents). Then resume matching runs.
