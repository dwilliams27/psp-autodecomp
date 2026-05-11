#!/usr/bin/env python3
"""Smoke tests for failure-note classification."""

from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)


def _func(addr, name, notes, size=188, method=None):
    if method is None and "::" in name:
        method = name.split("::")[-1].split("(", 1)[0]
    return {
        "address": addr,
        "name": name,
        "method_name": method or "",
        "class_name": name.split("::", 1)[0] if "::" in name else "",
        "obj_file": "test.obj",
        "size": size,
        "match_status": "failed",
        "failure_notes": [{"session": "s", "notes": notes}],
    }


def test_read_prologue_is_research_not_retry():
    import failure_classifier as fc

    func = _func(
        "0x10",
        "eFoo::Read(cFile &, cMemPool *)",
        "cReadBlock constructor prologue scheduler drift; 6/188 bytes",
        method="Read",
    )
    cls = fc.classify_failure(func)
    assert cls.primary == "read_prologue_compiler"
    assert cls.action == "research"
    assert "retryable_nearmiss" not in cls.tags


def test_regalloc_quarantines_small_diffs():
    import failure_classifier as fc

    func = _func(
        "0x20",
        "cObject::~cObject(void)",
        "category=REG_ALLOC. 8-byte diff; pure s0/s1 register naming swap "
        "after one permuter pass. No source leverage remains.",
        size=432,
        method="~cObject",
    )
    cls = fc.classify_failure(func)
    assert cls.primary == "regalloc_drift"
    assert cls.action == "quarantine"
    assert "retryable_nearmiss" not in cls.tags


def test_branch_direction_beats_nearmiss():
    import failure_classifier as fc

    func = _func(
        "0x30",
        "gcValFoo::AssignCopy(const cBase *)",
        "Closest attempt is 4-byte short. Remaining issue is branch-direction "
        "divergence: target uses bnel jump-in, ours uses beql skip-over.",
        size=784,
        method="AssignCopy",
    )
    cls = fc.classify_failure(func)
    assert cls.primary == "branch_direction_codegen"
    assert cls.action == "research"
    assert cls.near_miss_bytes == 4


def test_plain_nearmiss_stays_retryable():
    import failure_classifier as fc

    func = _func(
        "0x40",
        "gcCamera::gcXYZMotion::Set(const mVec3 &, cTimeValue)",
        "4-byte diff after trying expression ordering; no compiler-blocker "
        "signal found, likely source shape.",
        size=136,
        method="Set",
    )
    cls = fc.classify_failure(func)
    assert cls.primary == "retryable_nearmiss"
    assert cls.action == "retry"


def test_legacy_differing_bytes_nearmiss():
    import failure_classifier as fc

    func = _func(
        "0x45",
        "gcFoo::Set(float)",
        "compare_func reports 1/100 differing bytes after expression reshuffle.",
        size=100,
        method="Set",
    )
    cls = fc.classify_failure(func)
    assert cls.near_miss_bytes == 1
    assert cls.primary == "retryable_nearmiss"


def test_regalloc_negation_is_not_quarantined():
    import failure_classifier as fc

    func = _func(
        "0x50",
        "gcDoUIFade::GetText(char *) const",
        "4-byte diff remains, but this is not register allocation or "
        "scheduling; likely control-flow/tail-merge source shape.",
        size=568,
        method="GetText",
    )
    cls = fc.classify_failure(func)
    assert cls.primary == "retryable_nearmiss"
    assert "regalloc_drift" not in cls.tags


def test_missing_size_fails_loud():
    import failure_classifier as fc

    func = _func("0x60", "gcFoo::Set(int)", "4-byte diff", method="Set")
    del func["size"]
    try:
        fc.classify_failure(func)
    except ValueError as exc:
        assert "missing required size" in str(exc)
    else:
        raise AssertionError("expected missing size to raise")


def main():
    test_read_prologue_is_research_not_retry()
    test_regalloc_quarantines_small_diffs()
    test_branch_direction_beats_nearmiss()
    test_plain_nearmiss_stays_retryable()
    test_legacy_differing_bytes_nearmiss()
    test_regalloc_negation_is_not_quarantined()
    test_missing_size_fails_loud()
    print("failure classifier smoke: PASS")


if __name__ == "__main__":
    main()
