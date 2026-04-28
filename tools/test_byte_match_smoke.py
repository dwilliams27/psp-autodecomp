#!/usr/bin/env python3
"""Smoke tests for canonical byte-match helpers.

Run from the repo root:
    python3 tools/test_byte_match_smoke.py
"""

from __future__ import annotations

import os
import sys
import tempfile
from contextlib import redirect_stdout
from io import StringIO
from types import SimpleNamespace
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import byte_match
import verify_matches


def test_compile_src_forces_rebuild():
    with tempfile.TemporaryDirectory(prefix="byte_match_smoke_") as tmp:
        obj = os.path.join(tmp, "build", "src", "Foo.cpp.o")
        os.makedirs(os.path.dirname(obj))
        open(obj, "wb").close()

        calls = []

        def fake_run(cmd, capture_output, text, cwd=None):
            calls.append((cmd, capture_output, text, cwd))
            return SimpleNamespace(returncode=0, stdout="", stderr="")

        with mock.patch.object(byte_match.subprocess, "run", fake_run):
            out = byte_match.compile_src("src/Foo.cpp", cwd=tmp)

        assert out == obj, out
        assert calls == [
            (["make", "-B", "build/src/Foo.cpp.o"], True, True, tmp)
        ], calls


def test_verify_fix_compile_failures_requires_opt_in():
    funcs = [{
        "address": "0x00001000",
        "size": 4,
        "name": "C::A(void)",
        "match_status": "matched",
        "src_file": "src/C.cpp",
    }]

    def fail_compile(_src_file):
        raise byte_match.CompileFailed("synthetic compile failure")

    with mock.patch.object(verify_matches, "load_db", lambda: funcs), \
         mock.patch.object(verify_matches.os.path, "exists", lambda _p: True), \
         mock.patch.object(verify_matches, "compile_src", fail_compile), \
         mock.patch.object(verify_matches, "save_db") as save_db, \
         redirect_stdout(StringIO()):
        rc = verify_matches.verify_all(fix=True)

    assert rc == 1, rc
    assert funcs[0]["match_status"] == "matched", funcs[0]
    save_db.assert_not_called()

    funcs[0]["match_status"] = "matched"
    with mock.patch.object(verify_matches, "load_db", lambda: funcs), \
         mock.patch.object(verify_matches.os.path, "exists", lambda _p: True), \
         mock.patch.object(verify_matches, "compile_src", fail_compile), \
         mock.patch.object(verify_matches, "save_db") as save_db, \
         redirect_stdout(StringIO()):
        rc = verify_matches.verify_all(
            fix=True,
            fix_compile_failures=True,
        )

    assert rc == 1, rc
    assert funcs[0]["match_status"] == "failed", funcs[0]
    save_db.assert_called_once()


def main():
    test_compile_src_forces_rebuild()
    test_verify_fix_compile_failures_requires_opt_in()
    print("byte_match smoke: PASS")


if __name__ == "__main__":
    main()
