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

## Phase 2 — Quality gate [done]

Stop new matches from landing with the same anti-patterns.

- [x] New `reject_extern_c_class_methods(matched_funcs, addr_to_src)` in
      `tools/orchestrator.py`. For every matched function with a
      `class_name`, requires the reconstructed src to contain a
      canonical `Class::method(...)` definition. If absent, rejects.
- [x] Mirrors the asm rejection's loud-failure semantics: emits a
      `rejected_extern_c_class_method` event, marks the function `failed`
      with detailed `failure_notes`, decrements the run's matched count.
- [x] Tested with three controls:
      - `cWriteBlock::Write(int, const char *)` (canonical cWriteBlock.cpp): OK
      - `cFile::OnCreated(void)` (safe-name in free_functions.c): REJECTED
      - `cRestartApp(bool)` (free function, no class_name): OK (skipped)
- [x] Commit.

**Result**: Future sessions that produce extern-C / safe-name
reconstructions of class methods will be flagged and reverted, just like
pure-asm matches today. Existing matched safe-name entries are left
untouched here — Phase 4 retires them.

## Phase 3 — Typedef-int header audit [done]

Migrate weak `typedef int X;` declarations to proper class/struct decls
where the original binary has X as a real C++ type.

- [x] Enumerated `typedef int X` / `typedef unsigned int X` in `include/`
      and `src/`. Found four real-but-collapsed types:
      `nwSocketHandle`, `nwConnectionHandle`, `cTimeValue`, `eColor`.
      Confirmed each is a class/struct in the original via mangled-name
      search (`6OnwSocketHandle`, `6SnwConnectionHandle`, `6KcTimeValue`,
      `6GeColor`).
- [x] Cross-referenced matched DB entries by mangled-symbol substring.
      Found 28 candidate matches; bisected by `symbol_name !=
      mangled_symbol` evidence (whose .o demonstrably produced a
      primitive-encoded mangled name where the original has the class
      form). 11 confirmed-collapsed; 17 already produce the right
      mangled (their reconstructions must declare the type properly).
- [x] Reset the 11 confirmed entries to untried with detailed notes
      that point at the typedef-collapse mechanism and recommend
      replacing the typedef with a `struct X { int handle; };` so SNC
      mangling produces `6OX` / `6SX`.
- [x] Header migration itself deferred — happens organically when an
      agent reattempts these functions with the corrected header.
- [x] Commit.

**Result**: 11 entries flipped matched → untried. matched count
1269 → 1258.

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
