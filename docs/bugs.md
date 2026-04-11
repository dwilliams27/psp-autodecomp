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

