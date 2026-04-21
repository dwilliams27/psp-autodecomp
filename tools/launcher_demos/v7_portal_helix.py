#!/usr/bin/env python3
"""Helical assembly-code stream flowing between two portals.

Each character is a persistent "slot" on the double helix. Slots travel along
the helix path over time (u increases, wraps at u=1), so individual tokens
visibly emerge from the small left portal, spiral through two intertwined
strands with a funneling radius, and enter the larger right portal.

The helix's own rotation phase also advances with time (independent of the
slot travel), so the visual is: flow along the axis + rotation around it.
Depth shading via the jungle palette.

Runs until Ctrl-C.
"""

import math
import random
import time

from rich.align import Align
from rich.box import HEAVY
from rich.console import Console
from rich.live import Live
from rich.panel import Panel
from rich.text import Text


# ─── palette ────────────────────────────────────────────────────────────────
# Back→front depth ramp.
DEPTH_RAMP = [
    "color(22)", "color(28)", "color(34)", "color(70)",
    "color(107)", "color(108)", "color(178)", "color(214)", "color(220)",
]
PORTAL_BORDER = "color(70)"
PORTAL_CORE_SMALL = "bold color(178)"
PORTAL_CORE_LARGE = "bold color(220)"


# ─── assembly stream generator ──────────────────────────────────────────────
_MNEM = [
    "lw", "sw", "lh", "sh", "lb", "sb", "lhu", "lbu",
    "addu", "addiu", "subu", "and", "andi", "or", "ori", "xor", "xori",
    "bnez", "beqz", "bne", "beq", "bnezl", "beqzl", "bnel", "beql",
    "jal", "jalr", "jr", "b", "nop", "sll", "srl", "sra",
    "slt", "slti", "sltu", "mult", "multu", "div", "mfhi", "mflo",
    "mtc1", "mfc1", "lwc1", "swc1",
    "mul.s", "add.s", "sub.s", "div.s", "neg.s", "mov.s", "cvt.w.s",
    "c.lt.s", "bc1t", "bc1f", "move", "li", "lui",
]
_REGS = ["$zero", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
         "$t0", "$t1", "$t2", "$t3", "$t4",
         "$s0", "$s1", "$s2", "$sp", "$ra"]
_FREGS = ["$f0", "$f2", "$f4", "$f6", "$f8", "$f12", "$f14", "$f16"]
_OFFSETS = [0, 4, 8, 12, 16, 24, -4, 32, 48]


def _r():  return random.choice(_REGS)
def _f():  return random.choice(_FREGS)
def _o():  return random.choice(_OFFSETS)


def _one_instr():
    op = random.choice(_MNEM)
    if op == "nop":
        return "nop"
    if op in ("bnez", "beqz", "bnezl", "beqzl"):
        return f"{op} {_r()},.L{random.randint(1, 99)}"
    if op in ("bne", "beq", "bnel", "beql"):
        return f"{op} {_r()},{_r()},.L{random.randint(1, 99)}"
    if op in ("bc1t", "bc1f"):
        return f"{op} .L{random.randint(1, 99)}"
    if op in ("lw", "sw", "lh", "sh", "lb", "sb", "lhu", "lbu"):
        return f"{op} {_r()},{_o()}({_r()})"
    if op in ("lwc1", "swc1"):
        return f"{op} {_f()},{_o()}({_r()})"
    if op == "jr":   return f"{op} $ra"
    if op in ("jal", "b"):
        return f"{op} sub_{random.randint(0x1000, 0xffff):X}"
    if op == "jalr": return f"{op} {_r()}"
    if op in ("addu", "subu", "and", "or", "xor", "slt", "sltu"):
        return f"{op} {_r()},{_r()},{_r()}"
    if op in ("addiu", "andi", "ori", "xori", "slti"):
        return f"{op} {_r()},{_r()},{random.choice([0, 1, -1, 0xff, 8])}"
    if op in ("sll", "srl", "sra"):
        return f"{op} {_r()},{_r()},{random.randint(1, 31)}"
    if op in ("mult", "multu", "div"):
        return f"{op} {_r()},{_r()}"
    if op in ("mfhi", "mflo"):
        return f"{op} {_r()}"
    if op in ("mtc1", "mfc1"):
        return f"{op} {_r()},{_f()}"
    if op in ("mul.s", "add.s", "sub.s", "div.s"):
        return f"{op} {_f()},{_f()},{_f()}"
    if op in ("neg.s", "mov.s", "cvt.w.s"):
        return f"{op} {_f()},{_f()}"
    if op == "c.lt.s":
        return f"{op} {_f()},{_f()}"
    if op == "move": return f"{op} {_r()},{_r()}"
    if op == "li":   return f"{op} {_r()},{random.randint(-16, 256)}"
    if op == "lui":  return f"{op} {_r()},0x{random.randint(0, 0xffff):04X}"
    return op


# One long stream, joined so the ribbon reads as continuous disassembly.
random.seed(0)
STREAM = "  ·  ".join(_one_instr() for _ in range(400))


# ─── geometry ───────────────────────────────────────────────────────────────
CANVAS_W = 82
CANVAS_H = 17
CENTER_Y = CANVAS_H // 2
LEFT_PORTAL_CX  = 4
RIGHT_PORTAL_CX = CANVAS_W - 6
HELIX_X0 = LEFT_PORTAL_CX + 2
HELIX_X1 = RIGHT_PORTAL_CX - 3
HELIX_LEN = HELIX_X1 - HELIX_X0

TURNS = 2.2
R_MIN = 0.4
R_MAX = 4.0
Y_ASPECT = 0.55

# How many character slots live on each strand. Too few = sparse ribbon.
N_SLOTS = 130
# Flow speed: fraction of the helix traversed per second.
FLOW_SPEED = 0.11
# Independent phase-rotation speed (the helix "spins" even as text flows).
PHASE_SPEED = 0.9


# ─── rendering ──────────────────────────────────────────────────────────────
def _depth_style(z):
    norm = (z / R_MAX + 1) / 2
    idx = max(0, min(len(DEPTH_RAMP) - 1, int(norm * (len(DEPTH_RAMP) - 1))))
    return DEPTH_RAMP[idx]


def _draw_portal(grid, styles, cx, cy, large):
    if large:
        pattern = [
            "╭───╮",
            "│   │",
            "│ ◉ │",
            "│   │",
            "╰───╯",
        ]
        core_style = PORTAL_CORE_LARGE
        hw, hh = 2, 2
    else:
        pattern = [
            "╭─╮",
            "│◉│",
            "╰─╯",
        ]
        core_style = PORTAL_CORE_SMALL
        hw, hh = 1, 1
    for dy, line in enumerate(pattern):
        for dx, ch in enumerate(line):
            x = cx - hw + dx
            y = cy - hh + dy
            if 0 <= y < len(grid) and 0 <= x < len(grid[0]):
                grid[y][x] = ch
                if ch == "◉":
                    styles[y][x] = core_style
                elif ch != " ":
                    styles[y][x] = PORTAL_BORDER


def _render_frame(t):
    grid   = [[" "] * CANVAS_W for _ in range(CANVAS_H)]
    styles = [[None] * CANVAS_W for _ in range(CANVAS_H)]
    depth  = [[-math.inf] * CANVAS_W for _ in range(CANVAS_H)]

    phase = t * PHASE_SPEED

    # Each slot j has a persistent character. Its u-position evolves over time
    # so the char visibly travels along the helix.
    for strand_ix in range(2):
        strand_phase_offset = 0.0 if strand_ix == 0 else math.pi
        # Offset the slot positions between strands so they don't
        # start at the same u (interleaved).
        slot_offset = 0 if strand_ix == 0 else N_SLOTS // 2

        for j in range(N_SLOTS):
            u = ((j / N_SLOTS) + t * FLOW_SPEED) % 1.0
            r = R_MIN + (R_MAX - R_MIN) * u
            theta = 2 * math.pi * TURNS * u + phase + strand_phase_offset

            xf = HELIX_X0 + u * HELIX_LEN
            yf = CENTER_Y - r * math.sin(theta) * Y_ASPECT * 2
            z  = r * math.cos(theta)

            xc = int(round(xf))
            yc = int(round(yf))
            if not (0 <= xc < CANVAS_W and 0 <= yc < CANVAS_H):
                continue

            # This char's identity is fixed per slot.
            ch_idx = (j + slot_offset) % len(STREAM)
            ch = STREAM[ch_idx]

            if z > depth[yc][xc]:
                depth[yc][xc] = z
                grid[yc][xc] = ch
                styles[yc][xc] = _depth_style(z)

    _draw_portal(grid, styles, LEFT_PORTAL_CX,  CENTER_Y, large=False)
    _draw_portal(grid, styles, RIGHT_PORTAL_CX, CENTER_Y, large=True)
    return grid, styles


def _to_text(grid, styles):
    body = Text()
    for y in range(len(grid)):
        for x in range(len(grid[0])):
            ch = grid[y][x]
            st = styles[y][x]
            if st:
                body.append(ch, style=st)
            else:
                body.append(ch)
        body.append("\n")
    return body


def main():
    console = Console()
    with Live(console=console, refresh_per_second=30, screen=False) as live:
        start = time.time()
        try:
            while True:
                t = time.time() - start
                grid, styles = _render_frame(t)
                panel = Panel(
                    _to_text(grid, styles),
                    title=Text(" decomp stream ", style="bold color(178)"),
                    border_style="color(70)",
                    box=HEAVY,
                    padding=(0, 1),
                    width=CANVAS_W + 4,
                )
                live.update(Align.center(panel))
                time.sleep(1 / 30)
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    main()
