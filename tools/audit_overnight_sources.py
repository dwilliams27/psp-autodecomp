#!/usr/bin/env python3
"""Audit overnight branches for failed-attempt source leaks."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


MATCH_COMMIT_RE = re.compile(r"^Match \d+ functions? \(session ([0-9a-f]+)\)$")
SOURCE_SUFFIXES = (".c", ".cpp")


@dataclass(frozen=True)
class Leak:
    path: str
    sessions: tuple[str, ...]


@dataclass(frozen=True)
class MixedFile:
    path: str
    matched_sessions: tuple[str, ...]
    failed_sessions: tuple[str, ...]


@dataclass(frozen=True)
class SchemaError:
    address: str
    message: str


def git(args: list[str]) -> str:
    proc = subprocess.run(
        ["git", *args],
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    if proc.returncode != 0:
        raise RuntimeError(
            f"git {' '.join(args)} failed with status {proc.returncode}:\n{proc.stderr}"
        )
    return proc.stdout


def ensure_ancestor(base: str, head: str) -> None:
    proc = subprocess.run(
        ["git", "merge-base", "--is-ancestor", base, head],
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    if proc.returncode == 0:
        return
    detail = proc.stderr.strip()
    if detail:
        detail = f"\n{detail}"
    raise RuntimeError(
        f"{base} is not an ancestor of {head}; overnight branch is not fast-forward "
        f"mergeable from the base ref.{detail}"
    )


def match_sessions(base: str, head: str) -> list[str]:
    sessions: list[str] = []
    lines = git(["log", "--reverse", "--format=%s", f"{base}..{head}"]).splitlines()
    for subject in lines:
        match = MATCH_COMMIT_RE.match(subject)
        if match:
            sessions.append(match.group(1))
    return sessions


def normalize_src_path(path: str) -> str | None:
    normalized = path.strip().lstrip("./")
    if normalized.startswith("src/") and normalized.endswith(SOURCE_SUFFIXES):
        return normalized
    return None


def repo_relative_path(path: Path) -> str:
    if not path.is_absolute():
        return path.as_posix()

    repo_root = Path(git(["rev-parse", "--show-toplevel"]).strip()).resolve()
    try:
        return path.resolve().relative_to(repo_root).as_posix()
    except ValueError as exc:
        raise RuntimeError(f"{path} is not under repo root {repo_root}") from exc


def load_db_for_mode(db_path: Path, head: str, mode: str) -> list[dict]:
    if mode == "worktree":
        with db_path.open() as f:
            return json.load(f)

    rel_db_path = repo_relative_path(db_path)
    spec = f":{rel_db_path}" if mode == "staged" else f"{head}:{rel_db_path}"
    return json.loads(git(["show", spec]))


def source_files_by_status(
    db: list[dict],
    sessions: set[str],
) -> tuple[dict[str, set[str]], dict[str, set[str]], list[SchemaError]]:
    matched_by_file: dict[str, set[str]] = {}
    failed_by_file: dict[str, set[str]] = {}
    schema_errors: list[SchemaError] = []
    for entry in db:
        address = str(entry.get("address") or "(unknown)")
        if entry.get("match_status") == "matched":
            session = entry.get("matched_by_session_id")
            src_file = normalize_src_path(str(entry.get("src_file") or ""))
            if session in sessions and src_file:
                matched_by_file.setdefault(src_file, set()).add(session)

        notes = entry.get("failure_notes")
        if notes is None:
            continue
        if not isinstance(notes, list):
            schema_errors.append(
                SchemaError(address, f"failure_notes must be a list, got {type(notes).__name__}")
            )
            continue
        for idx, note in enumerate(notes):
            if not isinstance(note, dict):
                schema_errors.append(
                    SchemaError(address, f"failure_notes[{idx}] must be an object")
                )
                continue
            note_session = note.get("session")
            note_src_file = normalize_src_path(str(note.get("src_file") or ""))
            if note_session in sessions and note_src_file:
                failed_by_file.setdefault(note_src_file, set()).add(note_session)

    return matched_by_file, failed_by_file, schema_errors


def net_changed_sources(base: str, head: str, mode: str) -> list[str]:
    if mode == "worktree":
        paths = git(["diff", "--name-only", "--diff-filter=ACMRT", base])
    elif mode == "staged":
        paths = git(["diff", "--cached", "--name-only", "--diff-filter=ACMRT", base])
    else:
        paths = git(["diff", "--name-only", "--diff-filter=ACMRT", base, head])
    result: list[str] = []
    for path in paths.splitlines():
        normalized = normalize_src_path(path)
        if normalized:
            result.append(normalized)
    return result


def audit(
    base: str,
    head: str,
    db_path: Path,
    *,
    mode: str,
) -> tuple[list[str], dict[str, set[str]], list[Leak], list[MixedFile], list[SchemaError]]:
    ensure_ancestor(base, head)
    sessions = match_sessions(base, head)
    session_set = set(sessions)
    db = load_db_for_mode(db_path, head, mode)
    matched_files, failed_files, schema_errors = source_files_by_status(db, session_set)
    changed_sources = net_changed_sources(base, head, mode)

    leaks = [
        Leak(path=path, sessions=tuple(sorted(matched_files.get(path, ()))))
        for path in changed_sources
        if path not in matched_files
    ]
    mixed_files = [
        MixedFile(
            path=path,
            matched_sessions=tuple(sorted(matched_files[path])),
            failed_sessions=tuple(sorted(failed_files[path])),
        )
        for path in changed_sources
        if path in matched_files and path in failed_files
    ]
    return sessions, matched_files, leaks, mixed_files, schema_errors


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Fail if an overnight branch's final src/ diff contains source files "
            "that are not tied to matched functions from the branch's match sessions."
        )
    )
    parser.add_argument("--base", default="main", help="base ref, default: main")
    parser.add_argument("--head", default="HEAD", help="head ref, default: HEAD")
    diff_mode = parser.add_mutually_exclusive_group()
    diff_mode.add_argument(
        "--worktree",
        action="store_true",
        help=(
            "Audit the working tree against --base while still deriving match sessions "
            "from --base..--head. Use this before staging cleanup."
        ),
    )
    diff_mode.add_argument(
        "--staged",
        action="store_true",
        help=(
            "Audit the index against --base while still deriving match sessions "
            "from --base..--head. Use this immediately before a cleanup commit."
        ),
    )
    parser.add_argument(
        "--db",
        default="config/functions.json",
        type=Path,
        help="function DB path, default: config/functions.json",
    )
    args = parser.parse_args()

    mode = "staged" if args.staged else "worktree" if args.worktree else "committed"
    try:
        sessions, matched_files, leaks, mixed_files, schema_errors = audit(
            args.base,
            args.head,
            args.db,
            mode=mode,
        )
    except Exception as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 1

    print(f"Match sessions in range: {len(sessions)}")
    print(f"Matched source files in DB for those sessions: {len(matched_files)}")

    if not sessions:
        print(f"ERROR: no match commits found in {args.base}..{args.head}", file=sys.stderr)
        return 1

    if schema_errors:
        print("DB SCHEMA ERRORS:")
        for err in schema_errors[:20]:
            print(f"  {err.address}: {err.message}")
        if len(schema_errors) > 20:
            print(f"  ... and {len(schema_errors) - 20} more")
        print()
        print("Malformed failure_notes make the failed-source audit incomplete.")
        return 1

    if leaks:
        print("FAILED-SOURCE LEAKS DETECTED:")
        for leak in leaks:
            print(f"  {leak.path}")
        print()
        print("These files are present in the branch's final src/ diff but are not")
        print("recorded as matched source files for any match session in this branch.")
        print("Restore files that existed on the base ref and delete files that did not,")
        print("then rerun this audit before merging.")
        return 1

    if mixed_files:
        print("MIXED MATCH/FAILED SOURCE FILES REQUIRE MANUAL REVIEW:")
        for mixed in mixed_files:
            print(f"  {mixed.path}")
            print(f"    matched sessions: {', '.join(mixed.matched_sessions)}")
            print(f"    failed sessions:  {', '.join(mixed.failed_sessions)}")
        print()
        print("These changed source files have a verified match from this branch and")
        print("also have failed-attempt notes from this branch. The audit cannot prove")
        print("the failed edit was removed while the matched edit was preserved.")
        print("Review these files manually before merging.")
        return 1

    print("OK: no unmatched source files in final branch diff.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
