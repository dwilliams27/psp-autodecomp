"""Shared prompt rendering helpers used by every variant.

Each variant owns its prose (the HARD RULES / WORKFLOW / diagnostic text —
the actual A/B knob) and calls these helpers for the mechanical per-function
rendering so we don't duplicate the loop across N variants.
"""

from common import build_addr_map
from orchestrator import (
    disassemble_function,
    get_class_header,
    get_m2c_output,
    get_matched_neighbors,
    get_sched_hint,
)


def render_context_blocks(batch, functions, class_name,
                          already_matched_suffix=None):
    """Emit CLASS HEADER and ALREADY MATCHED blocks. Returns list of parts."""
    parts = []
    header_content = get_class_header(class_name)
    if header_content:
        parts.append(
            f"== CLASS HEADER: include/{class_name}.h ==\n"
            f"```\n{header_content}\n```\n\n"
        )
    if class_name:
        neighbors = get_matched_neighbors(functions, batch[0])
        if neighbors:
            parts.append(f"== ALREADY MATCHED in {class_name} ==\n")
            for n in neighbors:
                parts.append(f"  {n['name']} ({n['size']}B) — matched\n")
            if already_matched_suffix:
                parts.append(already_matched_suffix)
            parts.append("\n")
    return parts


def render_function_block(func, func_num, addr_map, source_file, warnings,
                          prior_notes_guidance=None,
                          m2c_unavailable_note="// m2c unavailable — see orchestrator log"):
    """Emit one == FUNCTION N: ... == block.

    Returns (list_of_parts, ok). ok=False means disasm failed and caller
    should skip this function.
    """
    parts = []
    addr = int(func["address"], 16)
    try:
        disasm = disassemble_function(addr, func["size"])
    except RuntimeError as e:
        warnings.append({"type": "disasm_failed", "address": func["address"],
                         "name": func["name"], "error": str(e)})
        return parts, False

    try:
        m2c = get_m2c_output(func)
    except RuntimeError as e:
        warnings.append({"type": "m2c_failed", "address": func["address"],
                         "name": func["name"], "error": str(e)})
        m2c = m2c_unavailable_note

    callees = func.get("callees", [])
    callee_names = []
    for c_addr in callees:
        target = addr_map.get(c_addr)
        callee_names.append(target["name"] if target else f"unknown@{c_addr}")

    parts.append(f"== FUNCTION {func_num}: {func['name']} ==\n")
    parts.append(f"Address: {func['address']}, Size: {func['size']} bytes, "
                 f"Obj: {func['obj_file']}\n")
    parts.append(f"Leaf: {func.get('is_leaf', 'unknown')}")
    if callee_names:
        parts.append(f", Calls: {', '.join(callee_names)}")
    parts.append("\n")

    sched_hint = get_sched_hint(func)
    if sched_hint:
        parts.append(f"\n{sched_hint}\n")

    prior_notes = func.get("failure_notes", [])
    if prior_notes:
        parts.append(f"\nPRIOR ATTEMPTS ({len(prior_notes)} failed):\n")
        for note in prior_notes:
            parts.append(f"  Session {note['session']}: {note['notes']}\n")
        guidance = prior_notes_guidance or (
            "Use these notes to avoid repeating the same approaches. "
            "Try something different.\n"
        )
        parts.append(guidance)

    parts.append(f"\nDisassembly:\n{disasm}\n\n")
    parts.append(f"m2c output:\n{m2c}\n\n")
    parts.append(f"Write to: {source_file}\n\n")
    return parts, True
