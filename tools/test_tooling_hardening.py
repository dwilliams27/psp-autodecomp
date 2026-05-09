#!/usr/bin/env python3
"""Focused smoke tests for matching-tool hardening.

Run from repo root:
    python3 tools/test_tooling_hardening.py
"""

from __future__ import annotations

import os
import shutil
import sys
import tempfile
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)


def _make_func(addr, class_name, name=None, size=8):
    return {
        "address": addr,
        "name": name or (f"{class_name}::DoIt(void)" if class_name else "free_func(void)"),
        "class_name": class_name,
        "method_name": "DoIt(void)" if class_name else "",
        "obj_file": "test.obj",
        "size": size,
        "is_leaf": True,
        "match_status": "untried",
    }


def test_orchestrator_placement():
    import orchestrator

    foo = _make_func("0x00001000", "eFoo")
    foo["failure_notes"] = [
        {"src_file": "/tmp/old/src/eFoo_Attempt.cpp", "notes": "close"}
    ]
    free = _make_func("0x00001100", "")

    exact, prefixes = orchestrator.compute_allowed_paths(
        [foo, free], class_to_header={"eFoo": "include/eShared.h"})

    assert "src/eFoo.cpp" in exact
    assert "src/eFoo_Attempt.cpp" in exact
    assert "src/free_functions.c" in exact
    assert "include/eFoo.h" in exact
    assert "include/eShared.h" in exact
    assert "src/eFoo_" in prefixes

    placements = orchestrator.determine_source_files(
        [foo, free], class_to_header={"eFoo": "include/eShared.h"})
    assert placements[foo["address"]] == "src/eFoo.cpp"
    assert placements[free["address"]] == "src/free_functions.c"


def test_prompt_placement_block():
    from prompt_variants import _common

    func = _make_func("0x00001000", "eBar")
    placement = {
        "class_name": "eBar",
        "source_file": "src/eBar.cpp",
        "default_source_file": "src/eBar.cpp",
        "source_files": ["src/eBar.cpp", "src/eBar_Attempt.cpp"],
        "header_files": ["include/eBar.h", "include/eShared.h"],
        "sibling_prefix": "src/eBar_",
    }

    with mock.patch.object(_common, "disassemble_function", return_value="jr ra"), \
            mock.patch.object(_common, "get_m2c_output", return_value="int x;"), \
            mock.patch.object(_common, "get_sched_hint", return_value=None), \
            mock.patch.object(_common, "get_method_template_guidance", return_value=None), \
            mock.patch.object(_common, "get_method_exemplars", return_value=[]):
        parts, ok = _common.render_function_block(
            func, 1, {}, "src/eBar.cpp", [], all_functions=[],
            placement=placement)

    text = "".join(parts)
    assert ok
    assert "Canonical write target: src/eBar.cpp" in text
    assert "Related allowed source(s): src/eBar_Attempt.cpp" in text
    assert "Write to: src/eBar.cpp" in text


def test_compare_func_read_only_by_default():
    import compare_func

    funcs = [_make_func("0x00001000", "eFoo")]
    matched_result = [("sym_eFoo", True, funcs[0], "MATCH")]
    saved = {"called": False}

    def fake_save_db(_funcs):
        saved["called"] = True

    old_argv = sys.argv
    try:
        sys.argv = ["compare_func.py", "src/eFoo.cpp"]
        with mock.patch.object(compare_func, "load_db", return_value=funcs), \
                mock.patch.object(compare_func, "load_eboot", return_value=b""), \
                mock.patch.object(compare_func, "compare_file", return_value=matched_result), \
                mock.patch.object(compare_func, "save_db", side_effect=fake_save_db):
            compare_func.main()
    finally:
        sys.argv = old_argv

    assert funcs[0]["match_status"] == "untried"
    assert "src_file" not in funcs[0]
    assert not saved["called"]


def test_compare_func_update_db_opt_in():
    import compare_func

    funcs = [_make_func("0x00001000", "eFoo")]
    matched_result = [("sym_eFoo", True, funcs[0], "MATCH")]
    saved = {"called": False}

    def fake_save_db(_funcs):
        saved["called"] = True

    old_argv = sys.argv
    try:
        sys.argv = ["compare_func.py", "src/eFoo.cpp", "--update-db"]
        with mock.patch.object(compare_func, "load_db", return_value=funcs), \
                mock.patch.object(compare_func, "load_eboot", return_value=b""), \
                mock.patch.object(compare_func, "compare_file", return_value=matched_result), \
                mock.patch.object(compare_func, "save_db", side_effect=fake_save_db):
            compare_func.main()
    finally:
        sys.argv = old_argv

    assert funcs[0]["match_status"] == "matched"
    assert funcs[0]["src_file"] == "src/eFoo.cpp"
    assert funcs[0]["symbol_name"] == "sym_eFoo"
    assert saved["called"]


def test_permuter_symbol_selection_and_gate():
    import permuter

    syms = {
        "sym_a": (b"a" * 8, 0, []),
        "sym_b": (b"b" * 8, 8, []),
        "sym_c": (b"c" * 12, 16, []),
    }

    name, entry = permuter.select_symbol_bytes(
        syms, 8, target_symbol="sym_b")
    assert name == "sym_b"
    assert entry[0] == b"b" * 8

    try:
        permuter.select_symbol_bytes(syms, 8)
    except permuter.SymbolSelectionError as e:
        assert "ambiguous target" in str(e)
        assert "sym_a" in str(e) and "sym_b" in str(e)
    else:
        raise AssertionError("ambiguous same-size symbols did not fail closed")

    assert "symbol size" in permuter.permuter_suitability_reason(100, 120, 10)
    assert "exceeds" in permuter.permuter_suitability_reason(1000, 1000, 300)
    assert permuter.permuter_suitability_reason(1000, 1000, 100) is None


def test_permuter_main_passes_db_symbol():
    import permuter

    tmp = tempfile.mkdtemp(prefix="permuter_cli_")
    old_argv = sys.argv
    old_eboot = permuter.EBOOT_PATH
    captured = {}
    try:
        src = os.path.join(tmp, "eFoo.cpp")
        eboot = os.path.join(tmp, "EBOOT.BIN")
        with open(src, "w") as f:
            f.write("int eFoo_DoIt() { return 1; }\n")
        with open(eboot, "wb") as f:
            f.write(b"\0" * 64)
        permuter.EBOOT_PATH = eboot

        func = _make_func("0x00000000", "eFoo")
        func["mangled_symbol"] = "__0fGeFooJDoItv"

        def fake_run_search(**kwargs):
            captured.update(kwargs)
            return 0, kwargs["source"], {"baseline": 0}

        sys.argv = ["permuter.py", src, func["address"], "--time", "1"]
        with mock.patch.object(permuter, "load_db", return_value=[func]), \
                mock.patch.object(permuter, "find_function", return_value=func), \
                mock.patch.object(permuter, "run_search", side_effect=fake_run_search):
            try:
                permuter.main()
            except SystemExit as e:
                assert e.code == 0

        assert captured["target_symbol"] == "__0fGeFooJDoItv"

        captured.clear()
        sys.argv = [
            "permuter.py", src, func["address"], "--time", "1",
            "--symbol", "explicit_sym",
        ]
        with mock.patch.object(permuter, "load_db", return_value=[func]), \
                mock.patch.object(permuter, "find_function", return_value=func), \
                mock.patch.object(permuter, "run_search", side_effect=fake_run_search):
            try:
                permuter.main()
            except SystemExit as e:
                assert e.code == 0

        assert captured["target_symbol"] == "explicit_sym"
    finally:
        permuter.EBOOT_PATH = old_eboot
        sys.argv = old_argv
        shutil.rmtree(tmp, ignore_errors=True)


def main():
    test_orchestrator_placement()
    test_prompt_placement_block()
    test_compare_func_read_only_by_default()
    test_compare_func_update_db_opt_in()
    test_permuter_symbol_selection_and_gate()
    test_permuter_main_passes_db_symbol()
    print("tooling hardening smoke: PASS")


if __name__ == "__main__":
    main()
