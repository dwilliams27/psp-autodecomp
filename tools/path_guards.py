#!/usr/bin/env python3
"""Path validation helpers for repo-local tooling outputs."""

from __future__ import annotations

from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]


def repo_output_path(value: str, *, allowed_roots: tuple[str, ...], label: str) -> Path:
    raw = Path(value)
    if raw.is_absolute():
        raise ValueError(f"{label}: absolute paths are not allowed: {value}")
    resolved = (REPO_ROOT / raw).resolve()
    try:
        rel = resolved.relative_to(REPO_ROOT)
    except ValueError as exc:
        raise ValueError(f"{label}: path escapes repo: {value}") from exc

    allowed = False
    for root in allowed_roots:
        root_path = Path(root)
        try:
            rel.relative_to(root_path)
            allowed = True
            break
        except ValueError:
            continue
    if not allowed:
        roots = ", ".join(allowed_roots)
        raise ValueError(f"{label}: path must live under one of: {roots}; got {value}")
    return resolved


def repo_relative_path(value: str, *, allowed_roots: tuple[str, ...], label: str) -> str:
    path = repo_output_path(value, allowed_roots=allowed_roots, label=label)
    return path.relative_to(REPO_ROOT).as_posix()
