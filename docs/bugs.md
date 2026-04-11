# Known Bugs

Bugs discovered during development that haven't been fixed yet. Fix these before they bite.

## 1. Orchestrator can mark verified matches as "failed" on retry

**Found:** 2026-04-11 overnight run

When a function is matched and verified in session A, then the same source file is modified by session B (which targets a different function in the same file), session B can overwrite the source, breaking the previously matched function. If session B reports that function as "failed", the database is updated to "failed" even though a verified match existed in an earlier commit.

**Impact:** 3 functions (eAudioChannel::CalcPanning, cRedBlackTree::FindNode, gcCamera::gcEntityFollowState) were verified byte-exact against EBOOT but show as "failed" in functions.json.

**Root cause:** The orchestrator doesn't protect previously-matched functions when a later session writes to the same source file.

**Possible fixes:**
- Before marking a function as "failed", check if it was previously "matched" and re-verify against EBOOT before downgrading
- Use separate source files per function (already mostly done, but some agents put multiple functions in one file)
- Lock matched source files from further modification

## 2. ~~compare_func.py matches functions by size, not address~~ FIXED

**Fixed:** 2026-04-11. Rewrote compare_func.py to compare directly against EBOOT bytes instead of expected .o files. Uses single-pass candidate matching with relocation masking. No longer depends on expected .o files at all.

## 3. func_db.py rebuild drops failure_notes

**Found:** 2026-04-11 code review

`func_db.py build` rebuilds `functions.json` from the linker map and only preserves `match_status` from the old database. The new `failure_notes` field (added for retry context) is not preserved during a rebuild.

**Impact:** Running `func_db.py build` after accumulating failure notes would silently lose all retry context.

**Fix:** Add `failure_notes` to the list of preserved fields in `func_db.py build`, alongside `match_status`.
