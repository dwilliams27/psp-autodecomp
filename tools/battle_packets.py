#!/usr/bin/env python3
"""Render rich per-target prep packets for decompilation agents."""

from __future__ import annotations

import re

from common import build_addr_map
from failure_classifier import (
    classify_failure,
    failure_notes_list,
    optional_class_name,
    required_bool,
    required_size,
    required_text,
    validate_address,
)
from path_guards import repo_relative_path
from generate_matching_prep import category


def packet_filename(func: dict) -> str:
    address = validate_address(required_text(func, "address"))
    name = re.sub(r"[^A-Za-z0-9_.-]+", "_", required_text(func, "name")).strip("_")
    if len(name) > 96:
        name = name[:96].rstrip("_")
    if not name:
        # FALLBACK-OK: a function name can sanitize to empty if it consists
        # solely of punctuation/operators; the validated address keeps the
        # packet filename unique.
        name = "function"
    return f"{address}__{name}.md"


def _class_label(func: dict) -> str:
    class_name = optional_class_name(func)
    return class_name if class_name else "(free)"


def _optional_sequence(mapping: dict, key: str, *, context: str) -> list:
    if key not in mapping:
        raise ValueError(f"{context}: missing required {key}")
    if mapping[key] is None:
        # FALLBACK-OK: placement metadata uses None optional lists for
        # "no extra headers/sources"; callers render nothing for that case.
        return []
    value = mapping[key]
    if not isinstance(value, list):
        raise ValueError(f"{context}: {key} must be a list")
    return value


def _optional_text(mapping: dict, key: str, *, context: str) -> str:
    if key not in mapping:
        raise ValueError(f"{context}: missing required {key}")
    if mapping[key] is None or mapping[key] == "":
        # FALLBACK-OK: placement metadata uses None/empty optional strings for
        # "no sibling prefix"; callers render nothing for that case.
        return ""
    if not isinstance(mapping[key], str):
        raise ValueError(f"{context}: {key} must be a string")
    return mapping[key]


def render_packet(
    func: dict,
    functions: list[dict],
    *,
    target_metadata: dict | None = None,
    include_disasm: bool = True,
    include_m2c: bool = True,
    include_exemplars: bool = True,
    include_header: bool = True,
    class_to_header: dict | None = None,
    max_exemplar_chars: int = 6000,
) -> str:
    """Return a Markdown battle packet for one function.

    Expensive compiler/disassembly helpers are imported lazily so tests can
    render structural packets without the external toolchain.
    """
    addr_map = build_addr_map(functions)
    address = validate_address(required_text(func, "address"))
    status = required_text(func, "match_status")
    size = required_size(func)
    name = required_text(func, "name")
    class_name = optional_class_name(func)
    lines = [
        f"# Battle Packet: `{name}`",
        "",
        "## Target",
        "",
        f"- Address: `{address}`",
        f"- Size: `{size}` bytes",
        f"- Status: `{status}`",
        f"- Object: `{required_text(func, 'obj_file')}`",
        f"- Class: `{_class_label(func)}`",
        f"- Method family: `{category(func)}`",
        f"- Leaf: `{required_bool(func, 'is_leaf')}`",
    ]

    if target_metadata:
        lines += ["", "## Queue Metadata", ""]
        for key in ("score", "priority", "reason", "next_run_reason",
                    "failure_action", "near_miss_bytes"):
            if key in target_metadata:
                lines.append(f"- {key}: `{target_metadata[key]}`")
        blocker_tags = _target_blocker_tags(target_metadata)
        if blocker_tags:
            lines.append(
                "- blocker_tags: "
                + ", ".join(f"`{tag}`" for tag in blocker_tags)
            )

    has_failure_notes = bool(failure_notes_list(func))
    if status == "failed" or has_failure_notes:
        cls = classify_failure(func)
        lines += [
            "",
            "## Failure Classification",
            "",
            f"- Primary: `{cls.primary}`",
            f"- Action: `{cls.action}`",
            f"- Confidence: `{cls.confidence}`",
            "- Tags: " + ", ".join(f"`{tag}`" for tag in cls.tags),
            f"- Near miss: `{cls.near_miss_bytes if cls.near_miss_bytes is not None else 'unknown'}`",
            f"- Summary: {cls.summary}",
        ]
        if cls.action == "quarantine":
            lines.append("- Guidance: do not put this in ordinary matching runs without new research leverage.")
        elif cls.action == "research":
            lines.append("- Guidance: use as a focused research target, not a broad retry target.")
        elif cls.action == "prep":
            lines.append("- Guidance: improve layout/context first, then retry.")

    _append_calls(lines, func, addr_map)

    if include_header or include_exemplars or include_disasm or include_m2c:
        from orchestrator import (  # noqa: WPS433 - intentionally lazy
            disassemble_function,
            get_class_header,
            get_m2c_output,
            get_matched_neighbors,
            get_method_exemplars,
            get_method_template_guidance,
            get_sched_hint,
            placement_for_function,
        )
    else:
        disassemble_function = None
        get_class_header = None
        get_m2c_output = None
        get_matched_neighbors = None
        get_method_exemplars = None
        get_method_template_guidance = None
        get_sched_hint = None
        placement_for_function = None

    if placement_for_function is not None:
        placement = placement_for_function(func, class_to_header=class_to_header)
        lines += ["", "## Placement", ""]
        lines.append(f"- Canonical source: `{placement['source_file']}`")
        header_files = _optional_sequence(placement, "header_files", context="placement")
        source_files = _optional_sequence(placement, "source_files", context="placement")
        sibling_prefix = _optional_text(placement, "sibling_prefix", context="placement")
        if header_files:
            lines.append(
                "- Header(s): "
                + ", ".join(f"`{h}`" for h in header_files)
            )
        if source_files:
            lines.append("- Allowed source(s): " + ", ".join(
                f"`{src}`" for src in source_files
            ))
        if sibling_prefix:
            lines.append(f"- Split-TU prefix: `{sibling_prefix}*.cpp`")

    sched_hint = get_sched_hint(func) if get_sched_hint else None
    template_guidance = get_method_template_guidance(func) if get_method_template_guidance else None
    if sched_hint or template_guidance:
        lines += ["", "## Compiler Guidance", ""]
        if sched_hint:
            lines.append(sched_hint)
            lines.append("")
        if template_guidance:
            lines.append(template_guidance)

    if include_header and get_class_header and class_name:
        header = get_class_header(class_name)
        if header:
            lines += [
                "",
                f"## Class Header: `include/{class_name}.h`",
                "",
                "```cpp",
                header.rstrip(),
                "```",
            ]
        else:
            # FALLBACK-OK: some DB classes do not have a canonical header yet;
            # make the missing context visible in the packet.
            lines += ["", f"## Class Header: `include/{class_name}.h`", "", "Not found."]

    if get_matched_neighbors:
        neighbors = get_matched_neighbors(functions, func)
        if neighbors:
            lines += ["", "## Matched Same-Class Neighbors", ""]
            for row in neighbors[:12]:
                # FALLBACK-OK: old matched DB rows can lack src_file metadata;
                # omit only that optional suffix while keeping the neighbor.
                src = str(row["src_file"]) if "src_file" in row and row["src_file"] else ""
                src_s = f" — `{src}`" if src else ""
                lines.append(f"- `{row['address']}` {row['size']}B `{row['name']}`{src_s}")

    if include_exemplars and get_method_exemplars:
        exemplars = get_method_exemplars(functions, func, limit=4)
        if exemplars:
            lines += ["", "## Matched Method Exemplars", ""]
            for idx, (exemplar, source) in enumerate(exemplars, 1):
                source = source.rstrip()
                if len(source) > max_exemplar_chars:
                    source = source[:max_exemplar_chars].rstrip() + "\n// ... truncated ..."
                # FALLBACK-OK: old matched exemplar rows can lack src_file
                # metadata; packet still shows the source body.
                src_file = exemplar["src_file"] if "src_file" in exemplar and exemplar["src_file"] else ""
                lines += [
                    f"### Exemplar {idx}: `{exemplar['name']}`",
                    "",
                    f"- Address: `{exemplar['address']}`",
                    f"- Size: `{exemplar['size']}` bytes",
                    f"- Source: `{src_file}`",
                    "",
                    "```cpp",
                    source,
                    "```",
                    "",
                ]

    _append_prior_notes(lines, func)

    if include_disasm and disassemble_function:
        disasm = disassemble_function(int(address, 16), size)
        lines += ["", "## Disassembly", "", "```asm", disasm.rstrip(), "```"]

    if include_m2c and get_m2c_output:
        m2c = get_m2c_output(func)
        lines += ["", "## m2c Starting Point", "", "```c", m2c.rstrip(), "```"]

    lines += [
        "",
        "## Agent Checklist",
        "",
        "- Read the classification before retrying old approaches.",
        "- Prefer same-class and same-method matched exemplars over generic m2c structure.",
        "- Keep failed notes specific: byte count, structural match status, and suspected blocker.",
        "- Verify with `python3 tools/compare_func.py <src> --no-update-db`.",
        "",
    ]
    return "\n".join(lines)


def _append_calls(lines: list[str], func: dict, addr_map: dict[str, dict]) -> None:
    callees = _graph_addresses(func, "callees")
    callers = _graph_addresses(func, "callers")
    if not callees and not callers:
        return
    lines += ["", "## Graph Context", ""]
    if callees:
        lines.append("Callees:")
        for addr in callees[:24]:
            target = addr_map.get(addr)
            if target:
                lines.append(
                    f"- `{addr}` `{required_text(target, 'match_status')}` "
                    f"{required_size(target)}B `{required_text(target, 'name')}`"
                )
            else:
                # FALLBACK-OK: call graph extraction can include imported SDK
                # or stale edge addresses that are not DB function rows.
                lines.append(f"- `{addr}` not found in DB (external/import or stale edge)")
    if callers:
        lines.append("")
        lines.append("Callers:")
        for addr in callers[:24]:
            target = addr_map.get(addr)
            if target:
                lines.append(
                    f"- `{addr}` `{required_text(target, 'match_status')}` "
                    f"{required_size(target)}B `{required_text(target, 'name')}`"
                )
            else:
                # FALLBACK-OK: call graph extraction can include imported SDK
                # or stale edge addresses that are not DB function rows.
                lines.append(f"- `{addr}` not found in DB (external/import or stale edge)")


def _append_prior_notes(lines: list[str], func: dict) -> None:
    notes = failure_notes_list(func)
    if not notes:
        if required_text(func, "match_status") == "failed":
            # FALLBACK-OK: older failures have no structured note; surface that
            # fact in the generated packet rather than silently omitting it.
            lines += ["", "## Prior Failure Notes", "", "No failure notes recorded."]
        return
    lines += ["", "## Prior Failure Notes", ""]
    for idx, note in enumerate(reversed(notes[-5:]), 1):
        if not isinstance(note, dict):
            raise ValueError(f"{required_text(func, 'address')}: failure note {idx} must be an object")
        if "notes" not in note or not str(note["notes"]).strip():
            raise ValueError(f"{required_text(func, 'address')}: failure note {idx} has empty notes")
        meta = []
        if "session" in note and note["session"]:
            meta.append(f"session `{note['session']}`")
        if "src_file" in note and note["src_file"]:
            meta.append("src `" + repo_relative_path(
                note["src_file"],
                allowed_roots=("src",),
                label=f"{required_text(func, 'address')} failure note src_file",
            ) + "`")
        if "snapshot" in note and note["snapshot"]:
            meta.append("snapshot `" + repo_relative_path(
                note["snapshot"],
                allowed_roots=("logs/failure_snapshots",),
                label=f"{required_text(func, 'address')} failure note snapshot",
            ) + "`")
        lines.append(f"### Note {idx}")
        if meta:
            lines.append("")
            lines.append("- " + "; ".join(meta))
        lines += ["", str(note["notes"]).strip(), ""]
def _target_blocker_tags(target_metadata: dict) -> list[str]:
    if "blocker_tags" not in target_metadata:
        # FALLBACK-OK: only research/failed queues include blocker tags.
        return []
    tags = target_metadata["blocker_tags"]
    if tags == []:
        return []
    if not isinstance(tags, list) or not all(isinstance(tag, str) for tag in tags):
        raise ValueError("target metadata blocker_tags must be a list of strings")
    return tags


def _graph_addresses(func: dict, key: str) -> list[str]:
    if key not in func or func[key] is None:
        raise ValueError(f"{required_text(func, 'address')}: missing {key} list")
    values = func[key]
    if not isinstance(values, list):
        raise ValueError(f"{required_text(func, 'address')}: {key} must be a list")
    return [validate_address(addr, context=f"{required_text(func, 'address')} {key}") for addr in values]
