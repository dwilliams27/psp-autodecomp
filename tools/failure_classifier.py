#!/usr/bin/env python3
"""Classify failed matching attempts into actionable buckets.

The function DB stores rich free-form failure notes.  This module turns those
notes into stable tags that target generators and prompts can share.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from common import load_db  # noqa: E402
from path_guards import repo_output_path, repo_relative_path  # noqa: E402


NEAR_MISS_PATTERNS = (
    re.compile(r"\b(\d+)\s*/\s*\d+\s*-?\s*bytes?\b", re.I),
    re.compile(r"\b(\d+)\s*/\s*\d+\s+differing\s+bytes?\b", re.I),
    re.compile(r"\b(\d+)\s+bytes?\s+(?:differ|diff|off|wrong)\b", re.I),
    re.compile(r"\b(\d+)\s*-?\s*byte\s+(?:diff|mismatch)\b", re.I),
    re.compile(r"\b(\d+)\s*-?\s*byte\s+short\b", re.I),
    re.compile(r"\bdiff_count\s*=\s*(\d+)\b", re.I),
)
ADDRESS_PATTERN = re.compile(r"^0x[0-9a-fA-F]+$")

TAG_DETAILS = {
    "retryable_nearmiss": {
        "action": "retry",
        "severity": 10,
        "description": "Small byte diff with no stronger compiler-blocker signal.",
    },
    "regalloc_drift": {
        "action": "quarantine",
        "severity": 80,
        "description": "Register naming/live-range drift after structure is correct.",
    },
    "branch_direction_codegen": {
        "action": "research",
        "severity": 75,
        "description": "Branch-likely or jump-in/skip-over codegen divergence.",
    },
    "constant_cse_codegen": {
        "action": "research",
        "severity": 70,
        "description": "Compiler hoists/reuses constants differently from target.",
    },
    "read_prologue_compiler": {
        "action": "research",
        "severity": 90,
        "description": "cReadBlock Read prologue/scheduler compiler issue.",
    },
    "tagged_pointer_shape": {
        "action": "research",
        "severity": 45,
        "description": "Tagged/nullable owned-pointer branch-shape issue.",
    },
    "needs_layout": {
        "action": "prep",
        "severity": 35,
        "description": "Likely blocked by missing/uncertain class layout or field offsets.",
    },
    "large_semantic": {
        "action": "prep",
        "severity": 55,
        "description": "Large function where source semantics/scaffold are incomplete.",
    },
    "agent_low_confidence": {
        "action": "retry",
        "severity": 15,
        "description": "Failure note is too thin or says the agent ran out of ideas/time.",
    },
}

HARD_BLOCKER_TAGS = {
    "regalloc_drift",
    "branch_direction_codegen",
    "constant_cse_codegen",
    "read_prologue_compiler",
}


def validate_address(address: object, *, context: str = "address") -> str:
    """Return a canonical DB address or raise on malformed input."""
    if not isinstance(address, str) or not ADDRESS_PATTERN.fullmatch(address):
        raise ValueError(f"{context}: expected hex address like 0x00000010, got {address!r}")
    return f"0x{int(address, 16):08x}"


def _row_context(func: dict) -> str:
    if "address" not in func or func["address"] is None:
        return "<unknown>"
    return str(func["address"])


def required_size(func: dict) -> int:
    if "size" not in func:
        raise ValueError(f"{_row_context(func)}: missing required size")
    try:
        size = int(func["size"])
    except (TypeError, ValueError) as exc:
        raise ValueError(
            f"{_row_context(func)}: invalid size {func['size']!r}"
        ) from exc
    if size < 0:
        raise ValueError(f"{_row_context(func)}: negative size {size}")
    return size


def required_text(func: dict, key: str) -> str:
    if key not in func or func[key] is None:
        raise ValueError(f"{_row_context(func)}: missing required {key}")
    if not isinstance(func[key], str):
        raise ValueError(f"{_row_context(func)}: {key} must be a string")
    value = func[key].strip()
    if not value:
        raise ValueError(f"{_row_context(func)}: {key} must not be blank")
    return value


def required_bool(func: dict, key: str) -> bool:
    if key not in func or func[key] is None:
        raise ValueError(f"{_row_context(func)}: missing required {key}")
    if not isinstance(func[key], bool):
        raise ValueError(f"{_row_context(func)}: {key} must be a boolean")
    return func[key]


def classification_obj_file(func: dict) -> str:
    if "obj_file" in func and isinstance(func["obj_file"], str) and func["obj_file"].strip():
        return func["obj_file"].strip()
    name = required_text(func, "name")
    if name.startswith(("sceSas", "__sce", "sceKernel", "sceIo", "sceUtility", "scePower",
                        "sceAtrac", "sceMpeg", "sceUmd", "sceAudio", "sceDisplay",
                        "sceGe", "sceRtc")):
        # FALLBACK-OK: SDK/import wrapper rows in the DB are source-less and can
        # lack obj_file; keep them visible in classifier output with an empty object.
        return ""
    raise ValueError(f"{_row_context(func)}: missing required obj_file")


def optional_class_name(func: dict) -> str:
    if "class_name" not in func:
        raise ValueError(f"{_row_context(func)}: missing required class_name")
    if func["class_name"] is None or func["class_name"] == "":
        # FALLBACK-OK: free functions and import/runtime labels legitimately
        # have no owning class in the DB.
        return ""
    if not isinstance(func["class_name"], str):
        raise ValueError(f"{_row_context(func)}: class_name must be a string")
    return func["class_name"]

@dataclass(frozen=True)
class FailureClassification:
    primary: str
    tags: tuple[str, ...]
    action: str
    confidence: str
    near_miss_bytes: int | None
    summary: str

    def to_dict(self) -> dict:
        return {
            "primary": self.primary,
            "tags": list(self.tags),
            "action": self.action,
            "confidence": self.confidence,
            "near_miss_bytes": self.near_miss_bytes,
            "summary": self.summary,
        }


def notes_for(func: dict) -> str:
    notes = failure_notes_list(func)
    if not notes:
        # FALLBACK-OK: older failed DB rows predate structured notes.  They are
        # intentionally classified as low-confidence retry/prep candidates.
        return ""

    texts: list[str] = []
    for idx, note in enumerate(notes, 1):
        if not isinstance(note, dict):
            raise ValueError(
                f"{required_text(func, 'address')}: failure note {idx} must be an object"
            )
        if "notes" not in note:
            raise ValueError(
                f"{required_text(func, 'address')}: failure note {idx} missing notes"
            )
        text = str(note["notes"]).strip()
        if not text:
            raise ValueError(
                f"{required_text(func, 'address')}: failure note {idx} has empty notes"
            )
        texts.append(text)
    return "\n\n".join(texts)


def last_note(func: dict) -> dict | None:
    notes = failure_notes_list(func)
    if not notes:
        # FALLBACK-OK: historical failed rows can legitimately have no attempt
        # note; callers should render that fact rather than invent metadata.
        return None
    note = notes[-1]
    if not isinstance(note, dict):
        raise ValueError(f"{required_text(func, 'address')}: last failure note must be an object")
    if "notes" not in note or not str(note["notes"]).strip():
        raise ValueError(f"{required_text(func, 'address')}: last failure note has empty notes")
    return note


def failure_notes_list(func: dict) -> list[dict]:
    if "failure_notes" not in func or func["failure_notes"] is None:
        # FALLBACK-OK: historical rows can lack the field entirely.
        return []
    notes = func["failure_notes"]
    if not isinstance(notes, list):
        raise ValueError(f"{required_text(func, 'address')}: failure_notes must be a list")
    return notes


def near_miss_bytes(text: str) -> int | None:
    values: list[int] = []
    for pattern in NEAR_MISS_PATTERNS:
        values.extend(int(match.group(1)) for match in pattern.finditer(text))
    return min(values) if values else None


def _has_any(text: str, terms: tuple[str, ...]) -> bool:
    return any(term in text for term in terms)


def _negates_regalloc(text: str) -> bool:
    return bool(re.search(
        r"\b(?:not|no)\s+(?:a\s+)?(?:register[- ]allocation|regalloc|register allocator)\b",
        text,
    ))


def _infer_tags(name: str, method: str, size: int, text: str) -> set[str]:
    lower = text.lower()
    tags: set[str] = set()

    miss = near_miss_bytes(text)
    if miss is not None and miss <= 64:
        tags.add("retryable_nearmiss")

    if (
        "read(cfile &, cmempool *)" in name.lower()
        and "creadblock" in lower
        and _has_any(lower, ("prologue", "scheduler", "scheduling", "raii"))
    ):
        tags.add("read_prologue_compiler")
    if _has_any(lower, (
        "creadblock prologue",
        "read prologue",
        "constructor prologue",
        "li hoisting",
        "li s0, 1",
        "li s3, 1",
    )) and _has_any(lower, ("compiler", "scheduler", "scheduling", "prologue")) and (
        "creadblock" in lower
        or method.lower() == "read"
        or "::read(" in name.lower()
    ):
        tags.add("read_prologue_compiler")

    regalloc_terms = (
        "category=reg_alloc",
        "reg_alloc",
        "register allocation",
        "regalloc",
        "register-allocation",
        "callee-save",
        "callee-saved",
        "register naming swap",
        "register rename",
        "s0/s1",
        "s1/s0",
        "s2/s3",
        "s3/s2",
        "source-level fixes do not work",
        "no source leverage",
        "context-dependent register allocation",
        "tu-context register",
    )
    if _has_any(lower, regalloc_terms) and not _negates_regalloc(lower):
        tags.add("regalloc_drift")

    if _has_any(lower, (
        "branch-direction",
        "branch direction",
        "jump-in",
        "skip-over",
        "branch-likely",
        "branch likely",
        "bnel",
        "beql",
        "beqzl",
        "bnezl",
        "likely-beql",
        "inverse of docs/decisions/006",
    )):
        tags.add("branch_direction_codegen")

    if _has_any(lower, (
        "constant-cse",
        "constant cse",
        "hoists",
        "hoisted",
        "hoisting",
        "recomputes",
        "recompute",
        "re-loads",
        "reloads",
        "reuse the dead callee-save",
        "reuse s2",
        "0x37e6a8",
        "literal",
    )) and _has_any(lower, (
        "constant",
        "literal",
        "0x37e6a8",
        "vtable",
        "cse",
    )):
        tags.add("constant_cse_codegen")

    if _has_any(lower, (
        "tagged pointer",
        "tagged-pointer",
        "nullable pointer",
        "nullable-pointer",
        "tagflag",
        "tag flag",
        "owned-pointer",
        "owned pointer",
        "val & ~1",
        "|= 1",
        "parent path",
        "destruct/clone",
        "release-and-clone",
        "clone block",
    )):
        tags.add("tagged_pointer_shape")

    if _has_any(lower, (
        "layout",
        "field offset",
        "wrong offset",
        "missing field",
        "unknown field",
        "class layout",
        "header",
        "local redeclaration",
        "vtable offset",
    )):
        tags.add("needs_layout")

    if size >= 900 and _has_any(lower, (
        "semantic scaffold",
        "far too small",
        "size mismatch",
        "large",
        "incomplete",
    )):
        tags.add("large_semantic")

    if not lower.strip() or _has_any(lower, (
        "ran out of time",
        "could not finish",
        "not enough time",
        "no notes",
        "low confidence",
    )):
        tags.add("agent_low_confidence")

    # Thin notes without a concrete byte diagnosis are usually worth retrying,
    # but mark them separately so queues can prefer richer attempts.
    if len(lower.strip()) < 120 and miss is None:
        tags.add("agent_low_confidence")

    # Reads with cReadBlock/prologue language should not also look like ordinary
    # near-misses just because the byte diff is small.
    if "read_prologue_compiler" in tags:
        tags.discard("retryable_nearmiss")

    # Pure regalloc/codegen diagnoses are not normal retry near-misses unless
    # another tool explicitly asks for a research queue.
    if tags & HARD_BLOCKER_TAGS:
        tags.discard("retryable_nearmiss")

    return tags


def classify_failure(func: dict) -> FailureClassification:
    text = notes_for(func)
    name = required_text(func, "name")
    method = required_text(func, "method_name")
    size = required_size(func)
    tags = _infer_tags(name, method, size, text)
    miss = near_miss_bytes(text)

    if not tags:
        if miss is not None and miss <= 64:
            tags.add("retryable_nearmiss")
        else:
            tags.add("agent_low_confidence")

    primary = max(tags, key=lambda tag: TAG_DETAILS[tag]["severity"])
    action = _action_for_tags(tags)
    confidence = _confidence_for(tags, text, miss)
    summary = TAG_DETAILS[primary]["description"]
    return FailureClassification(
        primary=primary,
        tags=tuple(sorted(tags, key=lambda tag: (-TAG_DETAILS[tag]["severity"], tag))),
        action=action,
        confidence=confidence,
        near_miss_bytes=miss,
        summary=summary,
    )


def _action_for_tags(tags: set[str]) -> str:
    if "read_prologue_compiler" in tags:
        return "research"
    if tags & {"regalloc_drift"}:
        return "quarantine"
    if tags & {"branch_direction_codegen", "constant_cse_codegen", "tagged_pointer_shape"}:
        return "research"
    if tags & {"needs_layout", "large_semantic"}:
        return "prep"
    return "retry"


def _confidence_for(tags: set[str], text: str, miss: int | None) -> str:
    if "agent_low_confidence" in tags and len(text.strip()) < 120:
        return "low"
    if tags & HARD_BLOCKER_TAGS:
        return "high"
    if miss is not None and miss <= 16:
        return "medium"
    return "medium" if text.strip() else "low"


def classify_failures(functions: list[dict]) -> list[dict]:
    rows = []
    for func in functions:
        if required_text(func, "match_status") != "failed":
            continue
        cls = classify_failure(func)
        row = {
            "address": validate_address(required_text(func, "address")),
            "size": required_size(func),
            "name": required_text(func, "name"),
            "obj_file": classification_obj_file(func),
            "class_name": optional_class_name(func),
            "classification": cls.to_dict(),
        }
        note = last_note(func)
        if note is not None and "src_file" in note and note["src_file"]:
            row["failure_src_file"] = repo_relative_path(
                note["src_file"],
                allowed_roots=("src",),
                label=f"{row['address']} failure_src_file",
            )
        if note is not None and "snapshot" in note and note["snapshot"]:
            row["failure_snapshot"] = repo_relative_path(
                note["snapshot"],
                allowed_roots=("logs/failure_snapshots",),
                label=f"{row['address']} failure_snapshot",
            )
        rows.append(row)
    return rows


def classification_stats(functions: list[dict]) -> tuple[Counter, Counter, Counter]:
    by_primary: Counter = Counter()
    by_tag_bytes: Counter = Counter()
    by_action: Counter = Counter()
    for func in functions:
        if required_text(func, "match_status") != "failed":
            continue
        cls = classify_failure(func)
        size = required_size(func)
        by_primary[cls.primary] += 1
        by_action[cls.action] += 1
        for tag in cls.tags:
            by_tag_bytes[tag] += size
    return by_primary, by_tag_bytes, by_action


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", "-o", help="Write classified failed rows as JSON.")
    parser.add_argument("--limit", type=int, default=30,
                        help="Preview rows to print (default: 30).")
    parser.add_argument("--tag", help="Only preview rows containing this tag.")
    args = parser.parse_args()

    functions = load_db()
    rows = classify_failures(functions)
    if args.tag:
        rows = [row for row in rows if args.tag in row["classification"]["tags"]]

    primary, tag_bytes, action = classification_stats(functions)
    print("Failed classification by primary:")
    for tag, count in primary.most_common():
        print(f"  {tag:<28} {count:>4}")
    print("Action counts:")
    for act, count in action.most_common():
        print(f"  {act:<12} {count:>4}")
    print("Bytes by tag:")
    for tag, total in tag_bytes.most_common():
        print(f"  {tag:<28} {total:>8}")
    print()
    for row in rows[:args.limit]:
        cls = row["classification"]
        miss = cls["near_miss_bytes"]
        miss_s = "-" if miss is None else f"{miss}B"
        print(f"{cls['primary']:<28} {miss_s:>5}  "
              f"{row['address']} {row['size']:>4}B  {row['name']}")
    if len(rows) > args.limit:
        print(f"... and {len(rows) - args.limit} more")

    if args.output:
        out = repo_output_path(
            args.output,
            allowed_roots=("docs/research", "config"),
            label="--output",
        )
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(json.dumps(rows, indent=2) + "\n")
        print(f"\nWrote {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
