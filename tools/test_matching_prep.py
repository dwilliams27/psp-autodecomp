#!/usr/bin/env python3
"""Focused smoke tests for matching-prep target generation.

Run from repo root:
    python3 tools/test_matching_prep.py
"""

from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)


def _func(addr, name, method, status, size=188, notes=""):
    return {
        "address": addr,
        "name": name,
        "class_name": name.split("::", 1)[0] if "::" in name else "",
        "method_name": method,
        "obj_file": "test.obj",
        "size": size,
        "match_status": status,
        "failure_notes": [{"notes": notes, "snapshot": "snap"}] if notes else [],
    }


def test_category_uses_exact_method_name():
    import generate_matching_prep as prep

    assert prep.category(_func("0x1", "gcTableColumnShort::SetSize(int)", "SetSize", "untried")) == "SetSize"
    assert prep.category(_func("0x2", "gcValFoo::GetText(char *) const", "GetText", "failed")) == "GetText"
    assert prep.category(_func("0x3", "eFoo::Write(cFile &) const", "Write", "untried")) == "Write"


def test_blocker_tags_stay_specific():
    import generate_matching_prep as prep

    read = _func(
        "0x10",
        "eFoo::Read(cFile &, cMemPool *)",
        "Read",
        "failed",
        notes="cReadBlock constructor prologue scheduler drift; 20/188 bytes",
    )
    assert "read_cblock_prologue" in prep.blocker_tags(read)

    tagged = _func(
        "0x20",
        "gcDoSetValue::Read(cFile &, cMemPool *)",
        "Read",
        "failed",
        notes="tagged pointer nullable pointer branch repair; 16/464 bytes",
    )
    assert "tagged_pointer_branch" in prep.blocker_tags(tagged)

    branch_likely = _func(
        "0x30",
        "eFoo::Update(void)",
        "Update",
        "failed",
        notes="branch-likely mismatch only; 12/200 bytes",
    )
    assert "tagged_pointer_branch" not in prep.blocker_tags(branch_likely)


def test_target_builders_filter_quarantined_rows():
    import generate_matching_prep as prep

    functions = [
        _func("0x100", "eFoo::Write(cFile &) const", "Write", "matched", size=200),
        _func("0x104", "eBar::Write(cFile &) const", "Write", "untried", size=200),
        _func(
            "0x108",
            "eRead::Read(cFile &, cMemPool *)",
            "Read",
            "failed",
            notes="cReadBlock read prologue scheduler drift; 6/188 bytes",
        ),
        _func("0x10c", "eRetry::Write(cFile &) const", "Write", "failed", notes="10/200 bytes"),
    ]

    high_yield = prep.build_high_yield_targets(functions, 10)
    assert [row["address"] for row in high_yield] == ["0x104"]
    assert "exact-size matched method template" in high_yield[0]["reason"]

    failed_retry = prep.build_failed_retry_targets(functions, 10)
    assert [row["address"] for row in failed_retry] == ["0x10c"]

    read_research = prep.build_read_research_targets(functions, 10)
    assert [row["address"] for row in read_research] == ["0x108"]


def main():
    test_category_uses_exact_method_name()
    test_blocker_tags_stay_specific()
    test_target_builders_filter_quarantined_rows()
    print("matching prep smoke: PASS")


if __name__ == "__main__":
    main()
