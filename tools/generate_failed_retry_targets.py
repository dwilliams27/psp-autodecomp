#!/usr/bin/env python3
"""Generate a ranked target list for retrying failed functions.

The normal orchestrator picker only selects `untried` rows. This tool
builds a curated target file from `match_status == "failed"` rows so an
operator can reset a focused subset back to `untried` and run a stronger
model over the best retry candidates.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from datetime import date
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from common import load_db  # noqa: E402


_NEAR_MISS_PATTERNS = (
    re.compile(r"\b(\d+)\s*/\s*\d+\s*-?\s*bytes?\b", re.I),
    re.compile(r"\b(\d+)\s+bytes?\s+(?:differ|diff|off|wrong)\b", re.I),
    re.compile(r"\b(\d+)\s*-?\s*byte\s+(?:diff|mismatch)\b", re.I),
    re.compile(r"\bdiff_count\s*=\s*(\d+)\b", re.I),
)

_CODEGEN_TERMS = (
    "register allocation",
    "saved-register",
    "saved register",
    "instruction scheduling",
    "scheduling",
    "delay slot",
    "delay-slot",
    "branch-likely",
    "loop-entry",
    "loop setup",
    "source-shape",
    "source shape",
    "declaration order",
)

_HARD_TERMS = (
    "prologue scheduling divergence",
    "cReadBlock RAII",
    "ML2",
    "fundamentally unmatchable",
    "known unmatchable",
)


def _notes_for(func: dict) -> str:
    notes = func.get("failure_notes") or []
    return "\n\n".join(str(n.get("notes", "")) for n in notes)


def _last_note_meta(func: dict) -> dict:
    notes = func.get("failure_notes") or []
    return notes[-1] if notes else {}


def _category(name: str) -> str:
    method = name.split("::")[-1]
    if "GetType(" in name:
        return "GetType"
    if "AssignCopy(" in name:
        return "AssignCopy"
    if "VisitReferences(" in name:
        return "VisitReferences"
    if "PlatformRead(" in name:
        return "PlatformRead"
    if "Read(" in name:
        return "Read"
    if "Write(" in name:
        return "Write"
    if "New(" in name:
        return "New"
    if method.startswith("~"):
        return "Destructor"
    if "(" in method:
        ctor_name = method.split("(", 1)[0]
        cls = (name.split("::")[-2] if "::" in name else "")
        if ctor_name == cls.split("::")[-1]:
            return "Constructor"
    if "operator" in name:
        return "Operator"
    return "Other"


def _near_miss_value(notes: str) -> int | None:
    values: list[int] = []
    for pat in _NEAR_MISS_PATTERNS:
        values.extend(int(m.group(1)) for m in pat.finditer(notes))
    return min(values) if values else None


def _is_known_hard(func: dict, notes: str) -> bool:
    name = func.get("name", "")
    lower = notes.lower()
    if "Read(cFile &, cMemPool *)" in name and "prologue" in lower:
        return True
    return any(term.lower() in lower for term in _HARD_TERMS)


def _score(func: dict) -> tuple[int, list[str], bool]:
    notes = _notes_for(func)
    lower = notes.lower()
    reasons: list[str] = []
    score = 0

    if notes:
        score += 25
        reasons.append("has failure_notes")
        if len(notes) >= 800:
            score += 15
            reasons.append("rich notes")
    else:
        score -= 25
        reasons.append("no failure_notes")

    miss = _near_miss_value(notes)
    if miss is not None:
        if miss <= 8:
            score += 90
        elif miss <= 16:
            score += 75
        elif miss <= 32:
            score += 55
        elif miss <= 64:
            score += 30
        else:
            score -= 10
        reasons.append(f"near miss {miss}B")

    if "correct size" in lower or re.search(r"\bcorrect\s+\d+\s*-?\s*byte", lower):
        score += 15
        reasons.append("correct function size")

    codegen_hits = [term for term in _CODEGEN_TERMS if term in lower]
    if codegen_hits:
        score += min(25, 8 * len(codegen_hits))
        reasons.append("codegen-specific diagnosis")

    meta = _last_note_meta(func)
    if meta.get("backend") == "codex" and str(meta.get("model", "")).startswith("gpt-5.4"):
        score += 10
        reasons.append("failed on 5.4; good 5.5 retry")

    cat = _category(func.get("name", ""))
    if cat in {"Write", "AssignCopy", "New", "Constructor"}:
        score += 8
        reasons.append(f"{cat} retry family")
    elif cat in {"Read", "PlatformRead", "VisitReferences"}:
        score -= 20
        reasons.append(f"{cat} hard family")

    size = int(func.get("size") or 0)
    if size <= 512:
        score += 8
        reasons.append("manageable size")
    elif size > 1024:
        score -= 15
        reasons.append("large function")

    hard = _is_known_hard(func, notes)
    if hard:
        score -= 100
        reasons.append("known-hard bucket")

    return score, reasons, hard


def build_targets(functions: list[dict], include_known_hard: bool,
                  include_no_notes: bool, min_score: int,
                  limit: int | None) -> tuple[list[dict], dict[str, int]]:
    rows = []
    stats = {
        "failed_total": 0,
        "no_notes_skipped": 0,
        "known_hard_skipped": 0,
        "below_score_skipped": 0,
        "eligible_before_limit": 0,
        "limit_truncated": 0,
    }
    for func in functions:
        if func.get("match_status") != "failed":
            continue
        stats["failed_total"] += 1
        notes = func.get("failure_notes") or []
        if not notes and not include_no_notes:
            stats["no_notes_skipped"] += 1
            continue
        score, reasons, hard = _score(func)
        if hard and not include_known_hard:
            stats["known_hard_skipped"] += 1
            continue
        if score < min_score:
            stats["below_score_skipped"] += 1
            continue
        rows.append((score, func, reasons))

    rows.sort(key=lambda item: (-item[0], int(item[1].get("size") or 0),
                               int(item[1]["address"], 16)))
    stats["eligible_before_limit"] = len(rows)
    if limit is not None:
        stats["limit_truncated"] = max(0, len(rows) - limit)
        rows = rows[:limit]

    targets = []
    for score, func, reasons in rows:
        targets.append({
            "address": func["address"],
            "size": func.get("size", 0),
            "name": func.get("name", ""),
            "obj_file": func.get("obj_file"),
            "class_name": func.get("class_name"),
            "category": _category(func.get("name", "")),
            "priority": max(1, 100 - score),
            "score": score,
            "reason": "; ".join(reasons),
        })
    return targets, stats


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--output", "-o",
                    default=f"config/targets_failed_retry_{date.today():%Y%m%d}.json",
                    help="Target JSON path to write.")
    ap.add_argument("--limit", type=int,
                    help="Maximum targets to emit after ranking.")
    ap.add_argument("--min-score", type=int, default=30,
                    help="Minimum retry score to include (default: 30).")
    ap.add_argument("--include-known-hard", action="store_true",
                    help="Include known compiler-hard buckets such as Read prologue failures.")
    ap.add_argument("--include-no-notes", action="store_true",
                    help="Include failed rows that lack failure_notes.")
    ap.add_argument("--dry-run", action="store_true",
                    help="Print summary and preview without writing.")
    args = ap.parse_args()

    targets, stats = build_targets(
        load_db(),
        include_known_hard=args.include_known_hard,
        include_no_notes=args.include_no_notes,
        min_score=args.min_score,
        limit=args.limit,
    )

    print(f"Failed rows in DB:       {stats['failed_total']}")
    print(f"Selected retry targets:  {len(targets)}")
    print(f"Skipped no notes:        {stats['no_notes_skipped']}")
    print(f"Skipped known-hard:      {stats['known_hard_skipped']}")
    print(f"Skipped below score:     {stats['below_score_skipped']}")
    if stats["limit_truncated"]:
        print(f"Truncated by --limit:    {stats['limit_truncated']}")
    print()
    for t in targets[:20]:
        print(f"{t['score']:>4}  {t['address']}  {t['size']:>4}B  "
              f"{t['category']:<15} {t['name']}")
    if len(targets) > 20:
        print(f"... and {len(targets) - 20} more")

    if args.dry_run:
        return 0

    out = Path(args.output)
    out.parent.mkdir(parents=True, exist_ok=True)
    with out.open("w") as f:
        json.dump(targets, f, indent=2)
        f.write("\n")
    print()
    print(f"Wrote {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
