#!/usr/bin/env python3
"""Smoke tests for battle-packet rendering."""

from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)


def _func(addr, name, status, size=188, notes=None, callees=None):
    return {
        "address": addr,
        "name": name,
        "class_name": name.split("::", 1)[0] if "::" in name else "",
        "method_name": name.split("::")[-1].split("(", 1)[0] if "::" in name else name,
        "obj_file": "test.obj",
        "section": ".text",
        "is_static": False,
        "is_leaf": not bool(callees),
        "size": size,
        "match_status": status,
        "callees": callees or [],
        "callers": [],
        "failure_notes": [{"session": "s", "notes": notes, "src_file": "src/f.cpp"}] if notes else [],
    }


def test_packet_contains_classifier_and_graph_without_toolchain():
    import battle_packets as bp

    callee = _func("0x00000020", "gcFoo::Helper(void)", "matched", size=64)
    target = _func(
        "0x00000010",
        "gcFoo::AssignCopy(const cBase *)",
        "failed",
        size=784,
        notes="Closest attempt is 4-byte short. Remaining issue is branch-direction divergence: bnel jump-in vs beql skip-over.",
        callees=[callee["address"]],
    )
    packet = bp.render_packet(
        target,
        [target, callee],
        target_metadata={"score": 200, "reason": "test queue"},
        include_disasm=False,
        include_m2c=False,
        include_exemplars=False,
        include_header=False,
    )
    assert "Failure Classification" in packet
    assert "`branch_direction_codegen`" in packet
    assert "test queue" in packet
    assert "gcFoo::Helper" in packet
    assert "Agent Checklist" in packet


def test_packet_filename_is_stable_and_safe():
    import battle_packets as bp

    func = _func("0x00000010", "gcFoo::operator=(const gcFoo &)", "untried")
    name = bp.packet_filename(func)
    assert name.startswith("0x00000010__")
    assert "/" not in name
    assert name.endswith(".md")


def test_packet_filename_rejects_bad_address():
    import battle_packets as bp

    func = _func("../evil", "gcFoo::Set(int)", "untried")
    try:
        bp.packet_filename(func)
    except ValueError as exc:
        assert "expected hex address" in str(exc)
    else:
        raise AssertionError("expected bad address to raise")


def test_failed_packet_without_notes_says_so():
    import battle_packets as bp

    target = _func("0x00000010", "gcFoo::Set(int)", "failed", notes=None)
    packet = bp.render_packet(
        target,
        [target],
        include_disasm=False,
        include_m2c=False,
        include_exemplars=False,
        include_header=False,
    )
    assert "Failure Classification" in packet
    assert "No failure notes recorded." in packet


def main():
    test_packet_contains_classifier_and_graph_without_toolchain()
    test_packet_filename_is_stable_and_safe()
    test_packet_filename_rejects_bad_address()
    test_failed_packet_without_notes_says_so()
    print("battle packet smoke: PASS")


if __name__ == "__main__":
    main()
