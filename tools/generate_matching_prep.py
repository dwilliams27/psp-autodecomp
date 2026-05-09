#!/usr/bin/env python3
"""Generate current matching-prep reports and focused target files."""

from __future__ import annotations

import argparse
import json
import re
import sys
from collections import Counter, defaultdict
from datetime import date
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from common import load_db  # noqa: E402


TODAY = date.today().strftime("%Y%m%d")

NEAR_MISS_PATTERNS = (
    re.compile(r"\b(\d+)\s*/\s*\d+\s*-?\s*bytes?\b", re.I),
    re.compile(r"\b(\d+)\s+bytes?\s+(?:differ|diff|off|wrong)\b", re.I),
    re.compile(r"\b(\d+)\s*-?\s*byte\s+(?:diff|mismatch)\b", re.I),
    re.compile(r"\bcompare_func(?:\.py)?\s+(?:reports|reported)\s+(\d+)\s*/\s*\d+", re.I),
)

READ_CBLOCK_TERMS = (
    "creadblock",
    "read prologue",
    "constructor prologue",
    "constructor setup",
)

PROLOGUE_TERMS = (
    "prologue",
    "scheduler",
    "scheduling",
    "save/move",
    "save ordering",
    "constructor window",
    "source-unreachable",
)

REGALLOC_TERMS = (
    "register allocation",
    "regalloc",
    "saved-register",
    "saved register",
    "callee-save",
    "s0/s1",
    "s1/s2",
    "s2/s3",
    "allocator drift",
)

TAGGED_TERMS = (
    "tagged pointer",
    "tagged-pointer",
    "nullable pointer",
    "nullable-pointer",
    "pointer repair",
    "name-fixup",
    "readbase repair",
    "readbase fallback",
    "tagged-base",
    "child & 1",
    "two-stage",
)

TOOLING_TERMS = (
    "out_of_scope_path",
    "placement conflict",
    "symbol isolation",
    "wrong symbol",
    "permuter.py still fails",
)

HIGH_YIELD_BASE = {
    "Write": 95,
    "AssignCopy": 85,
    "Destructor": 68,
    "Get": 62,
    "Set": 58,
    "Reset": 56,
    "Update": 45,
    "Evaluate": 36,
    "GetName": 36,
    "PlatformRead": 64,
    "Apply": 72,
    "Draw": 70,
    "Cull": 68,
    "GetInstanceType": 68,
}


def category(func: dict) -> str:
    name = func.get("name", "")
    method = func.get("method_name") or ""
    if method == "GetType":
        return "GetType"
    if method == "AssignCopy":
        return "AssignCopy"
    if method == "VisitReferences":
        return "VisitReferences"
    if method == "PlatformRead":
        return "PlatformRead"
    if method == "Read":
        return "Read"
    if method == "Write":
        return "Write"
    if method == "New":
        return "New"
    if method == "GetName":
        return "GetName"
    if method == "GetText":
        return "GetText"
    if method == "Collide":
        return "Collide"
    if method == "Evaluate":
        return "Evaluate"
    if method == "Set":
        return "Set"
    if method == "Get":
        return "Get"
    if method == "Reset":
        return "Reset"
    if method == "Update":
        return "Update"
    if method.startswith("~") or "::~" in name:
        return "Destructor"
    if "operator=" in name:
        return "operator="
    return method or "Other"


def notes_for(func: dict) -> str:
    notes = func.get("failure_notes") or []
    return "\n\n".join(str(note.get("notes", "")) for note in notes)


def last_note(func: dict) -> dict:
    notes = func.get("failure_notes") or []
    return notes[-1] if notes else {}


def near_miss_bytes(notes: str) -> int | None:
    values: list[int] = []
    for pattern in NEAR_MISS_PATTERNS:
        values.extend(int(match.group(1)) for match in pattern.finditer(notes))
    return min(values) if values else None


def has_any(text: str, terms: tuple[str, ...]) -> bool:
    lower = text.lower()
    return any(term in lower for term in terms)


def blocker_tags(func: dict) -> set[str]:
    name = func.get("name", "")
    notes = notes_for(func)
    lower = notes.lower()
    tags: set[str] = set()

    if (
        "Read(cFile &, cMemPool *)" in name
        and has_any(lower, READ_CBLOCK_TERMS)
        and has_any(lower, PROLOGUE_TERMS)
    ):
        tags.add("read_cblock_prologue")
    if has_any(lower, REGALLOC_TERMS):
        tags.add("regalloc")
    if has_any(lower, TAGGED_TERMS):
        tags.add("tagged_pointer_branch")
    if has_any(lower, TOOLING_TERMS):
        tags.add("tooling_or_placement")
    if int(func.get("size") or 0) >= 900 and (
        "semantic scaffold" in lower
        or "far too small" in lower
        or "size mismatch" in lower
        or "large" in lower
    ):
        tags.add("large_semantic")
    if "Collide(" in name and ("shape" in name or func.get("class_name", "").endswith("Shape")):
        tags.add("shape_collide_regalloc")
    return tags


def matched_indexes(functions: list[dict]) -> tuple[set[tuple[str, int]], Counter[tuple[str, str]]]:
    method_size: set[tuple[str, int]] = set()
    class_method: Counter[tuple[str, str]] = Counter()
    for func in functions:
        if func.get("match_status") != "matched":
            continue
        method = func.get("method_name") or ""
        if method:
            method_size.add((method, int(func.get("size") or 0)))
        cls = func.get("class_name") or ""
        if cls and method:
            class_method[(cls, method)] += 1
    return method_size, class_method


def target_row(func: dict, score: int, reason: str, extra: dict | None = None) -> dict:
    note = last_note(func)
    row = {
        "address": func["address"],
        "size": int(func.get("size") or 0),
        "name": func.get("name", ""),
        "obj_file": func.get("obj_file"),
        "class_name": func.get("class_name"),
        "category": category(func),
        "priority": max(1, 1000 - score),
        "score": score,
        "reason": reason,
    }
    if note.get("src_file"):
        row["failure_src_file"] = note.get("src_file")
    if note.get("snapshot"):
        row["failure_snapshot"] = note.get("snapshot")
    if extra:
        row.update(extra)
    return row


def build_high_yield_targets(functions: list[dict], limit: int) -> list[dict]:
    method_size, class_method = matched_indexes(functions)
    scored = []
    for func in functions:
        if func.get("match_status") != "untried":
            continue
        cat = category(func)
        if cat in {"Read", "VisitReferences", "Collide", "GetText"}:
            continue
        score = HIGH_YIELD_BASE.get(cat, 0)
        if score <= 0:
            continue

        reasons = [f"{cat} family"]
        size = int(func.get("size") or 0)
        method = func.get("method_name") or ""
        cls = func.get("class_name") or ""
        if (method, size) in method_size:
            score += 28
            reasons.append("exact-size matched method template")
        siblings = class_method[(cls, method)] if cls and method else 0
        if siblings:
            score += min(20, siblings * 4)
            reasons.append(f"{siblings} matched same-class method(s)")
        if size <= 128:
            score += 8
            reasons.append("small")
        elif size <= 512:
            score += 12
            reasons.append("manageable size")
        elif size > 900:
            score -= 25
            reasons.append("large")
        elif size > 700:
            score -= 12
            reasons.append("larger target")
        scored.append(target_row(func, score, "; ".join(reasons)))

    scored.sort(key=lambda row: (-row["score"], row["size"], int(row["address"], 16)))
    return scored[:limit]


def build_failed_retry_targets(functions: list[dict], limit: int) -> list[dict]:
    scored = []
    for func in functions:
        if func.get("match_status") != "failed":
            continue
        notes = notes_for(func)
        if not notes:
            continue
        tags = blocker_tags(func)
        if tags & {
            "read_cblock_prologue",
            "regalloc",
            "large_semantic",
            "shape_collide_regalloc",
        }:
            continue
        miss = near_miss_bytes(notes)
        if miss is None or miss > 64:
            continue
        score = 160 - min(miss, 64)
        reasons = [f"near-miss {miss}B"]
        cat = category(func)
        if cat in {"Write", "AssignCopy", "New", "Set", "Get", "Reset"}:
            score += 20
            reasons.append(f"{cat} retry family")
        if last_note(func).get("snapshot"):
            score += 20
            reasons.append("has failure snapshot")
        if "tagged_pointer_branch" in tags:
            score += 15
            reasons.append("tagged-pointer branch research candidate")
        size = int(func.get("size") or 0)
        if size <= 512:
            score += 10
            reasons.append("manageable size")
        scored.append(target_row(
            func,
            score,
            "; ".join(reasons),
            {"near_miss_bytes": miss, "blocker_tags": sorted(tags)},
        ))

    scored.sort(key=lambda row: (-row["score"], row["near_miss_bytes"], row["size"]))
    return scored[:limit]


def build_read_research_targets(functions: list[dict], limit: int) -> list[dict]:
    rows = []
    for func in functions:
        if func.get("match_status") != "failed":
            continue
        tags = blocker_tags(func)
        if "read_cblock_prologue" not in tags:
            continue
        notes = notes_for(func)
        miss = near_miss_bytes(notes)
        size = int(func.get("size") or 0)
        score = 120
        if miss is not None:
            score += max(0, 80 - miss)
        if size <= 512:
            score += 20
        elif size > 900:
            score -= 35
        rows.append(target_row(
            func,
            score,
            "cReadBlock prologue/scheduler research; body reported close",
            {"near_miss_bytes": miss, "blocker_tags": sorted(tags)},
        ))
    rows.sort(key=lambda row: (row.get("near_miss_bytes") is None,
                              row.get("near_miss_bytes") or 9999,
                              row["size"]))
    return rows[:limit]


def build_tagged_research_targets(functions: list[dict], limit: int) -> list[dict]:
    rows = []
    seed_names = (
        "gcDoUISendMessage::Read",
        "gcValLobbyScoreboardInfo::Read",
        "gcDoSetValue::Read",
        "gcValUnaryOp::Read",
        "gcStaticInstance::Read",
    )
    for func in functions:
        if func.get("match_status") != "failed":
            continue
        name = func.get("name", "")
        tags = blocker_tags(func)
        notes = notes_for(func)
        if "tagged_pointer_branch" not in tags and not any(seed in name for seed in seed_names):
            continue
        miss = near_miss_bytes(notes)
        score = 120
        if any(seed in name for seed in seed_names):
            score += 80
        if miss is not None:
            score += max(0, 70 - miss)
        rows.append(target_row(
            func,
            score,
            "tagged/nullable-pointer branch-shape research candidate",
            {"near_miss_bytes": miss, "blocker_tags": sorted(tags)},
        ))
    rows.sort(key=lambda row: (-row["score"],
                              row.get("near_miss_bytes") is None,
                              row.get("near_miss_bytes") or 9999))
    return rows[:limit]


def family_stats(functions: list[dict]) -> dict[str, Counter]:
    stats: dict[str, Counter] = defaultdict(Counter)
    for func in functions:
        stats[category(func)][func.get("match_status", "")] += 1
    return stats


def blocker_stats(functions: list[dict]) -> tuple[Counter, Counter]:
    counts: Counter = Counter()
    bytes_by_tag: Counter = Counter()
    for func in functions:
        if func.get("match_status") != "failed":
            continue
        tags = blocker_tags(func)
        if not tags:
            counts["uncategorized_failed"] += 1
            bytes_by_tag["uncategorized_failed"] += int(func.get("size") or 0)
            continue
        for tag in tags:
            counts[tag] += 1
            bytes_by_tag[tag] += int(func.get("size") or 0)
    return counts, bytes_by_tag


def write_json(path: Path, rows: list[dict]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(rows, indent=2) + "\n")


def write_report(path: Path, functions: list[dict], outputs: dict[str, list[dict]]) -> None:
    status_counts = Counter(func.get("match_status") for func in functions)
    stats = family_stats(functions)
    blocker_counts, blocker_bytes = blocker_stats(functions)

    lines = [
        "# Current Matching Prep",
        "",
        f"Generated: {date.today().isoformat()}",
        "",
        "## Match Progress",
        "",
        "The tracked progress chart for this snapshot is in "
        "[`docs/research/match-progress.md`](match-progress.md).",
        "",
        "## DB Summary",
        "",
        "| Status | Count |",
        "|---|---:|",
    ]
    for status, count in status_counts.most_common():
        lines.append(f"| `{status}` | {count} |")

    lines += [
        "",
        "## Top Families",
        "",
        "| Family | Matched | Failed | Untried | Observed rate |",
        "|---|---:|---:|---:|---:|",
    ]
    for cat, counts in sorted(stats.items(), key=lambda item: (-item[1]["untried"], item[0]))[:24]:
        matched = counts["matched"]
        failed = counts["failed"]
        denom = matched + failed
        rate = f"{matched / denom:.1%}" if denom else "n/a"
        lines.append(
            f"| `{cat}` | {matched} | {failed} | {counts['untried']} | {rate} |"
        )

    lines += [
        "",
        "## Failure Quarantine Signals",
        "",
        "| Signal | Functions | Bytes |",
        "|---|---:|---:|",
    ]
    for tag in [
        "read_cblock_prologue",
        "regalloc",
        "tagged_pointer_branch",
        "large_semantic",
        "shape_collide_regalloc",
        "tooling_or_placement",
        "uncategorized_failed",
    ]:
        lines.append(
            f"| `{tag}` | {blocker_counts[tag]} | {blocker_bytes[tag]} |"
        )

    lines += [
        "",
        "## Generated Target Files",
        "",
        "| File | Rows | Bytes | Purpose |",
        "|---|---:|---:|---|",
    ]
    purpose = {
        "high_yield": "Best untried non-Read families for future matching runs.",
        "failed_retry": "Small failed near-misses after compiler-blocked buckets are filtered out.",
        "read_research": "Remaining cReadBlock prologue/scheduler research targets.",
        "tagged_research": "Tagged/nullable-pointer branch-shape research targets.",
    }
    for key, rows in outputs.items():
        lines.append(
            f"| `{key}` | {len(rows)} | {sum(row['size'] for row in rows)} | {purpose[key]} |"
        )

    for key, rows in outputs.items():
        lines += ["", f"## Preview: {key}", "", "| Address | Size | Score | Function | Reason |", "|---|---:|---:|---|---|"]
        for row in rows[:20]:
            lines.append(
                f"| `{row['address']}` | {row['size']} | {row['score']} | "
                f"`{row['name']}` | {row['reason']} |"
            )
        if len(rows) > 20:
            lines.append(f"| ... | ... | ... | {len(rows) - 20} more | |")

    lines += [
        "",
        "## Readiness Notes",
        "",
        "- Do not rerun stale Read follow-up target files. The 188B family is drained.",
        "- Use the high-yield list for ordinary matching only after current research/doc updates land.",
        "- Use the read and tagged lists as research queues, not broad overnight queues.",
        "- Treat `read_cblock_prologue`, `regalloc`, and `shape_collide_regalloc` as quarantine signals unless a targeted compiler/source-shape project is active.",
        "",
    ]
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--report", default=f"docs/research/current-matching-prep-{TODAY}.md")
    parser.add_argument("--high-yield-out", default=f"config/targets_high_yield_untried_{TODAY}.json")
    parser.add_argument("--failed-retry-out", default=f"config/targets_failed_nearmiss_{TODAY}.json")
    parser.add_argument("--read-research-out", default=f"config/targets_read_cblock_research_{TODAY}.json")
    parser.add_argument("--tagged-research-out", default=f"config/targets_tagged_pointer_research_{TODAY}.json")
    parser.add_argument("--limit-high-yield", type=int, default=160)
    parser.add_argument("--limit-failed-retry", type=int, default=80)
    parser.add_argument("--limit-read-research", type=int, default=90)
    parser.add_argument("--limit-tagged-research", type=int, default=40)
    args = parser.parse_args()

    functions = load_db()
    outputs = {
        "high_yield": build_high_yield_targets(functions, args.limit_high_yield),
        "failed_retry": build_failed_retry_targets(functions, args.limit_failed_retry),
        "read_research": build_read_research_targets(functions, args.limit_read_research),
        "tagged_research": build_tagged_research_targets(functions, args.limit_tagged_research),
    }

    write_json(Path(args.high_yield_out), outputs["high_yield"])
    write_json(Path(args.failed_retry_out), outputs["failed_retry"])
    write_json(Path(args.read_research_out), outputs["read_research"])
    write_json(Path(args.tagged_research_out), outputs["tagged_research"])
    write_report(Path(args.report), functions, outputs)

    print(f"Wrote {args.report}")
    for key, rows in outputs.items():
        print(f"{key}: {len(rows)} rows, {sum(row['size'] for row in rows)} bytes")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
