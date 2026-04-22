"""Animated single-coil DNA-style helix. Chars come from a fixed MIPS-mnemonic
stream so the visual never repeats exactly the same shape twice.

Ported verbatim from tools/launcher_demos/v11_dashboard_with_helix.py — the
shape, stream generation, and depth-shading math are the demo's; kept here
as the shared widget used by the running screen.
"""

import math
import random

from rich.text import Text

from tools.ui.palette import DEPTH_RAMP


HELIX_H = 7
HELIX_TURNS = 2.2
HELIX_R_PEAK = 2.4
HELIX_Y_ASPECT = 0.55
HELIX_SLOTS = 150
HELIX_FLOW = 0.10
HELIX_PHASE = 0.9


_MNEM = [
    "lw", "sw", "lh", "sh", "lb", "sb", "lhu", "lbu",
    "addu", "addiu", "subu", "and", "andi", "or", "ori", "xor", "xori",
    "bnez", "beqz", "bne", "beq", "bnezl", "beqzl", "bnel", "beql",
    "jal", "jalr", "jr", "b", "nop", "sll", "srl", "sra",
    "slt", "sltu", "mult", "mfhi", "mflo", "mtc1", "mfc1", "lwc1", "swc1",
    "mul.s", "add.s", "sub.s", "div.s", "mov.s", "cvt.w.s", "c.lt.s",
    "bc1t", "bc1f", "move", "li", "lui",
]
_REGS = ["$zero", "$v0", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2",
         "$s0", "$s1", "$sp", "$ra"]
_FREGS = ["$f0", "$f2", "$f4", "$f12", "$f14"]


def _one_instr(rng):
    op = rng.choice(_MNEM)
    if op == "nop":
        return "nop"
    if op in ("bnez", "beqz", "bnezl", "beqzl"):
        return f"{op} {rng.choice(_REGS)},.L{rng.randint(1, 99)}"
    if op in ("bne", "beq", "bnel", "beql"):
        return f"{op} {rng.choice(_REGS)},{rng.choice(_REGS)},.L{rng.randint(1, 99)}"
    if op in ("lw", "sw", "lh", "sh", "lb", "sb", "lhu", "lbu"):
        return f"{op} {rng.choice(_REGS)},{rng.choice([0,4,8,16,-4])}({rng.choice(_REGS)})"
    if op in ("lwc1", "swc1"):
        return f"{op} {rng.choice(_FREGS)},{rng.choice([0,4,8])}({rng.choice(_REGS)})"
    if op == "jr":
        return "jr $ra"
    if op in ("jal", "b"):
        return f"{op} sub_{rng.randint(0x1000, 0xffff):X}"
    if op in ("addu", "subu", "and", "or", "xor", "slt", "sltu"):
        return f"{op} {rng.choice(_REGS)},{rng.choice(_REGS)},{rng.choice(_REGS)}"
    if op in ("addiu", "andi", "ori", "xori"):
        return f"{op} {rng.choice(_REGS)},{rng.choice(_REGS)},{rng.choice([0,1,-1,0xff,8])}"
    if op in ("sll", "srl", "sra"):
        return f"{op} {rng.choice(_REGS)},{rng.choice(_REGS)},{rng.randint(1,31)}"
    if op in ("mul.s", "add.s", "sub.s", "div.s"):
        return f"{op} {rng.choice(_FREGS)},{rng.choice(_FREGS)},{rng.choice(_FREGS)}"
    if op in ("mov.s", "cvt.w.s", "c.lt.s"):
        return f"{op} {rng.choice(_FREGS)},{rng.choice(_FREGS)}"
    if op in ("bc1t", "bc1f"):
        return f"{op} .L{rng.randint(1,99)}"
    if op == "move":
        return f"{op} {rng.choice(_REGS)},{rng.choice(_REGS)}"
    if op == "li":
        return f"{op} {rng.choice(_REGS)},{rng.randint(-16,256)}"
    if op == "lui":
        return f"{op} {rng.choice(_REGS)},0x{rng.randint(0,0xffff):04X}"
    if op in ("mtc1", "mfc1"):
        return f"{op} {rng.choice(_REGS)},{rng.choice(_FREGS)}"
    if op in ("mult", "mfhi", "mflo"):
        return f"{op} {rng.choice(_REGS)}"
    return op


# Private RNG so the deterministic stream seed doesn't mutate the process-
# wide random module — other tools (permuter, orchestrator batch picker)
# rely on random.random() being un-seeded.
_STREAM_RNG = random.Random(0)
STREAM = "  \u00b7  ".join(_one_instr(_STREAM_RNG) for _ in range(400))


def _depth_style(z, r):
    if r < 0.01:
        return DEPTH_RAMP[len(DEPTH_RAMP) // 2]
    norm = (z / r + 1) / 2
    idx = max(0, min(len(DEPTH_RAMP) - 1, int(norm * (len(DEPTH_RAMP) - 1))))
    return DEPTH_RAMP[idx]


def render(t, width):
    """Return HELIX_H Text rows, one per line of the helix body."""
    width = max(20, int(width))
    grid = [[" "] * width for _ in range(HELIX_H)]
    styles = [[None] * width for _ in range(HELIX_H)]
    depth = [[-math.inf] * width for _ in range(HELIX_H)]

    cy = HELIX_H // 2
    phase = t * HELIX_PHASE
    slots = max(40, int(HELIX_SLOTS * width / 60))

    for j in range(slots):
        u = ((j / slots) + t * HELIX_FLOW) % 1.0
        r = HELIX_R_PEAK * math.sin(math.pi * u)
        theta = 2 * math.pi * HELIX_TURNS * u + phase

        xf = u * (width - 1)
        yf = cy - r * math.sin(theta) * HELIX_Y_ASPECT * 2
        z = r * math.cos(theta)

        xc = int(round(xf))
        yc = int(round(yf))
        if not (0 <= xc < width and 0 <= yc < HELIX_H):
            continue

        ch = STREAM[j % len(STREAM)]
        if z > depth[yc][xc]:
            depth[yc][xc] = z
            grid[yc][xc] = ch
            styles[yc][xc] = _depth_style(z, max(r, 0.5))

    lines = []
    for y in range(HELIX_H):
        line = Text()
        for x in range(width):
            ch = grid[y][x]
            st = styles[y][x]
            if st:
                line.append(ch, style=st)
            else:
                line.append(ch)
        lines.append(line)
    return lines
