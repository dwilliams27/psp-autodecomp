#!/usr/bin/env python3
"""v11 — v5 dashboard with single-coil fixed-ends helix embedded in the
run-status panel. Left block: time info + this-run deltas + current work.
Right block: variant scoreboard with mini-bars. Middle: live helix.

Run: python3 tools/launcher_demos/v11_dashboard_with_helix.py
"""

import math
import random
import time
from collections import deque
from datetime import datetime, timedelta

import pyfiglet
from rich.align import Align
from rich.box import HEAVY, ROUNDED
from rich.console import Console, Group
from rich.layout import Layout
from rich.live import Live
from rich.panel import Panel
from rich.table import Table
from rich.text import Text


# ─── palette ────────────────────────────────────────────────────────────────
FOREST = "color(22)"
MOSS   = "color(70)"
LEAF   = "color(107)"
SUN    = "color(178)"
SUNLIT = "color(214)"
BODY   = "color(252)"
DIM    = "color(242)"
OK     = "color(107)"
WARN   = "color(178)"
BAD    = "color(167)"

# Helix depth ramp (back → front)
DEPTH_RAMP = [
    "color(22)", "color(28)", "color(34)", "color(70)",
    "color(107)", "color(108)", "color(178)", "color(214)", "color(220)",
]


# ─── simulated run parameters (same as v5) ──────────────────────────────────
HOURS    = 9
VARIANTS = ("base", "tier12")
DEADLINE = datetime.now() + timedelta(hours=HOURS)


# ─── scripted event timeline (same as v5) ───────────────────────────────────
TIMELINE = [
    (0.0,  "orch",    "session 4b5d3549 [tier12] started · batch=5"),
    (0.0,  "session", {"sid": "4b5d3549", "variant": "tier12",
                       "batch": "eFluidVolume::{ctor, ~dtor, GetType, Read, Write}",
                       "working": "eFluidVolume::ctor"}),
    (0.2,  "agent",   "$ python3 tools/func_db.py query --class eFluidVolume --status matched"),
    (0.5,  "agent",   "$ grep -l 'GetType' src/ | head -5"),
    (0.8,  "agent",   "read docs/research/snc-constructor-codegen.md"),
    (1.1,  "agent",   "$ make build/src/eFluidVolume.cpp.o"),
    (1.4,  "agent",   "$ python3 tools/compare_func.py src/eFluidVolume.cpp"),
    (1.6,  "agent",   "  → MATCH  eFluidVolume::ctor  (64B)"),
    (1.7,  "outcome", {"status": "matched", "name": "eFluidVolume::ctor",
                       "size": 64, "variant": "tier12"}),
    (1.8,  "orch",    "  matched: eFluidVolume::ctor"),
    (1.9,  "working", "eFluidVolume::GetType"),
    (2.0,  "agent",   "$ python3 tools/compare_func.py src/eFluidVolume.cpp"),
    (2.2,  "agent",   "  → MATCH  eFluidVolume::GetType  (160B)"),
    (2.3,  "outcome", {"status": "matched", "name": "eFluidVolume::GetType",
                       "size": 160, "variant": "tier12"}),
    (2.4,  "orch",    "  matched: eFluidVolume::GetType"),
    (2.5,  "working", "eFluidVolume::~dtor"),
    (2.7,  "agent",   "$ python3 tools/compare_func.py src/eFluidVolume.cpp"),
    (3.0,  "agent",   "  → MISMATCH  12/196 bytes  (eFluidVolume::~dtor)"),
    (3.2,  "agent",   "read docs/research/snc-delay-slots.md"),
    (3.6,  "agent",   "$ python3 tools/permuter.py src/eFluidVolume.cpp 0x0012b8a4 --time 300"),
    (4.2,  "agent",   "  permuter: 834 candidates · best still 12/196"),
    (4.5,  "agent",   "  → FAILED  eFluidVolume::~dtor"),
    (4.6,  "outcome", {"status": "failed", "name": "eFluidVolume::~dtor",
                       "size": 196, "variant": "tier12"}),
    (4.7,  "orch",    "  failed:  eFluidVolume::~dtor"),
    (5.0,  "orch",    "session 4b5d3549 done · 2 matched, 1 failed · 4m27s"),
    (5.1,  "orch",    "session e5ff9032 [base] started · batch=5"),
    (5.1,  "session", {"sid": "e5ff9032", "variant": "base",
                       "batch": "gcStateInfo::{New, Read, Write, HandleUpdate, Reset}",
                       "working": "gcStateInfo::New"}),
    (5.3,  "agent",   "$ python3 tools/decompile_func.py 0x00128c94"),
    (5.7,  "agent",   "$ make build/src/gcStateInfo.cpp.o"),
    (6.0,  "agent",   "$ python3 tools/compare_func.py src/gcStateInfo.cpp"),
    (6.3,  "agent",   "  → MATCH  gcStateInfo::New  (152B)"),
    (6.4,  "outcome", {"status": "matched", "name": "gcStateInfo::New",
                       "size": 152, "variant": "base"}),
    (6.5,  "orch",    "  matched: gcStateInfo::New"),
    (6.6,  "working", "gcStateInfo::HandleUpdate"),
    (6.9,  "agent",   "$ python3 tools/compare_func.py src/gcStateInfo.cpp"),
    (7.1,  "agent",   "  → MISMATCH  56/240 bytes  (cTimeValue layout mismatch?)"),
    (7.4,  "agent",   "grep -r 'cTimeValue' include/"),
    (7.8,  "agent",   "  → MISMATCH  56/240 bytes (no change)"),
    (8.1,  "agent",   "  → FAILED  gcStateInfo::HandleUpdate"),
    (8.2,  "outcome", {"status": "failed", "name": "gcStateInfo::HandleUpdate",
                       "size": 240, "variant": "base"}),
    (8.3,  "orch",    "  failed:  gcStateInfo::HandleUpdate"),
    (8.4,  "working", "gcStateInfo::Write"),
    (8.6,  "agent",   "$ python3 tools/compare_func.py src/gcStateInfo.cpp"),
    (8.9,  "agent",   "  → VERIFY FAILED  gcStateInfo::Write"),
    (9.0,  "outcome", {"status": "verify", "name": "gcStateInfo::Write",
                       "size": 128, "variant": "base"}),
    (9.1,  "orch",    "  verify_failed: gcStateInfo::Write · reverted"),
    (9.4,  "orch",    "(tick) progress: 921→923 matched  (+2 this minute)"),
]


# ─── state ──────────────────────────────────────────────────────────────────
class RunState:
    def __init__(self):
        self.start_time = datetime.now()
        self.sim_elapsed = 0.0
        self.sessions_done = 7
        self.sessions_target = 30
        self.matched_total = 921
        self.failed_total = 148
        self.untried_total = 8173
        self.this_run_matched = {"base": 3, "tier12": 5}
        self.this_run_failed  = {"base": 1, "tier12": 2}
        self.verify_fails = 0
        self.current_session_sid = None
        self.current_session_variant = None
        self.current_batch = "(waiting)"
        self.current_working = "(waiting)"
        self.orch_log = deque(maxlen=12)
        self.agent_log = deque(maxlen=14)
        self.outcomes = deque(maxlen=8)


STATE = RunState()


# ─── assembly stream (helix content) ────────────────────────────────────────
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


def _one_instr():
    op = random.choice(_MNEM)
    if op == "nop":                         return "nop"
    if op in ("bnez", "beqz", "bnezl", "beqzl"):
        return f"{op} {random.choice(_REGS)},.L{random.randint(1, 99)}"
    if op in ("bne", "beq", "bnel", "beql"):
        return f"{op} {random.choice(_REGS)},{random.choice(_REGS)},.L{random.randint(1, 99)}"
    if op in ("lw", "sw", "lh", "sh", "lb", "sb", "lhu", "lbu"):
        return f"{op} {random.choice(_REGS)},{random.choice([0,4,8,16,-4])}({random.choice(_REGS)})"
    if op in ("lwc1", "swc1"):
        return f"{op} {random.choice(_FREGS)},{random.choice([0,4,8])}({random.choice(_REGS)})"
    if op == "jr":                           return "jr $ra"
    if op in ("jal", "b"):                   return f"{op} sub_{random.randint(0x1000, 0xffff):X}"
    if op in ("addu", "subu", "and", "or", "xor", "slt", "sltu"):
        return f"{op} {random.choice(_REGS)},{random.choice(_REGS)},{random.choice(_REGS)}"
    if op in ("addiu", "andi", "ori", "xori"):
        return f"{op} {random.choice(_REGS)},{random.choice(_REGS)},{random.choice([0,1,-1,0xff,8])}"
    if op in ("sll", "srl", "sra"):
        return f"{op} {random.choice(_REGS)},{random.choice(_REGS)},{random.randint(1,31)}"
    if op in ("mul.s", "add.s", "sub.s", "div.s"):
        return f"{op} {random.choice(_FREGS)},{random.choice(_FREGS)},{random.choice(_FREGS)}"
    if op in ("mov.s", "cvt.w.s", "c.lt.s"): return f"{op} {random.choice(_FREGS)},{random.choice(_FREGS)}"
    if op in ("bc1t", "bc1f"):               return f"{op} .L{random.randint(1,99)}"
    if op == "move":                          return f"{op} {random.choice(_REGS)},{random.choice(_REGS)}"
    if op == "li":                            return f"{op} {random.choice(_REGS)},{random.randint(-16,256)}"
    if op == "lui":                           return f"{op} {random.choice(_REGS)},0x{random.randint(0,0xffff):04X}"
    if op in ("mtc1", "mfc1"):               return f"{op} {random.choice(_REGS)},{random.choice(_FREGS)}"
    if op in ("mult", "mfhi", "mflo"):       return f"{op} {random.choice(_REGS)}"
    return op


random.seed(0)
STREAM = "  ·  ".join(_one_instr() for _ in range(400))


# ─── helix renderer (shrunken for in-panel embedding) ───────────────────────
HELIX_H = 7
# HELIX_W is now computed dynamically per-frame from the console width so the
# spiral stretches to fill whatever horizontal space is left over after the
# fixed left & right columns.
HELIX_TURNS = 2.2
HELIX_R_PEAK = 2.4
HELIX_Y_ASPECT = 0.55
HELIX_SLOTS = 150
HELIX_FLOW = 0.10
HELIX_PHASE = 0.9


def _depth_style(z, r):
    if r < 0.01:
        return DEPTH_RAMP[len(DEPTH_RAMP) // 2]
    norm = (z / r + 1) / 2
    idx = max(0, min(len(DEPTH_RAMP) - 1, int(norm * (len(DEPTH_RAMP) - 1))))
    return DEPTH_RAMP[idx]


def render_helix_lines(t, width):
    """Return HELIX_H Text objects, one per row, with per-char coloring.
    Width is computed at call time so the spiral fills whatever span is
    available between the left & right columns.
    """
    grid   = [[" "] * width for _ in range(HELIX_H)]
    styles = [[None] * width for _ in range(HELIX_H)]
    depth  = [[-math.inf] * width for _ in range(HELIX_H)]

    cy = HELIX_H // 2
    phase = t * HELIX_PHASE
    # Scale slot count with width so longer helices stay dense.
    slots = max(40, int(HELIX_SLOTS * width / 60))

    for j in range(slots):
        u = ((j / slots) + t * HELIX_FLOW) % 1.0
        r = HELIX_R_PEAK * math.sin(math.pi * u)
        theta = 2 * math.pi * HELIX_TURNS * u + phase

        xf = u * (width - 1)
        yf = cy - r * math.sin(theta) * HELIX_Y_ASPECT * 2
        z  = r * math.cos(theta)

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


# ─── other render helpers ───────────────────────────────────────────────────
def gradient_header(text, font="slant"):
    lines = pyfiglet.figlet_format(text, font=font, width=140).rstrip("\n").split("\n")
    ramp = ["color(22)", "color(28)", "color(34)", "color(70)",
            "color(107)", "color(108)"]
    out = Text()
    for i, line in enumerate(lines):
        shade = ramp[min(i * len(ramp) // max(len(lines), 1), len(ramp) - 1)]
        out.append(line + "\n", style=shade)
    return out


def fmt_hhmm(seconds):
    if seconds < 0: seconds = 0
    hh = int(seconds // 3600)
    mm = int((seconds % 3600) // 60)
    return f"{hh}h {mm:02d}m"


def progress_bar(frac, width=10):
    frac = max(0.0, min(1.0, frac))
    filled = int(round(frac * width))
    return "▰" * filled + "▱" * (width - filled)


def header_panel():
    return Align.center(gradient_header("autodecomp"))


# ─── status panel with embedded helix ───────────────────────────────────────
def status_panel(t, panel_inner_width):
    # Compress 1 wall-second of demo time into 100 seconds of "run" time so
    # the visible clock advances during the short timeline.
    elapsed_s = STATE.sim_elapsed * 100
    total_s = HOURS * 3600
    time_left_s = max(0, total_s - elapsed_s)
    frac = min(1.0, elapsed_s / total_s) if total_s else 0

    # LEFT column: 5 lines — time left, progress bar, this run, now, working on
    # Content is kept under LEFT_W=34 chars per line.
    matched_run = sum(STATE.this_run_matched.values())
    failed_run  = sum(STATE.this_run_failed.values())

    left_lines = []
    l = Text()
    l.append("time left  ", style=LEAF)
    l.append(fmt_hhmm(time_left_s), style=f"bold {BODY}")
    l.append(f" / {HOURS}h", style=DIM)
    left_lines.append(l)

    bar = progress_bar(frac)
    l = Text()
    l.append(bar, style=SUN)
    l.append(f"  {int(frac*100)}%", style=DIM)
    left_lines.append(l)

    l = Text()
    l.append("this run   ", style=LEAF)
    l.append(f"+{matched_run}m", style=f"bold {OK}")
    l.append(" · ", style=DIM)
    l.append(f"+{failed_run}f", style=f"bold {BAD}")
    left_lines.append(l)

    l = Text()
    l.append("now        ", style=LEAF)
    l.append(STATE.current_session_variant or "-",
             style=f"bold {SUN}" if STATE.current_session_variant else DIM)
    left_lines.append(l)

    l = Text()
    l.append("on         ", style=LEAF)
    w = STATE.current_working or "-"
    if len(w) > 22:
        w = w[:19] + "..."
    l.append(w, style=BODY if w != "-" else DIM)
    left_lines.append(l)

    # Pad to helix height for alignment (HELIX_H rows)
    while len(left_lines) < HELIX_H:
        left_lines.append(Text(""))

    # MIDDLE column: helix fills whatever span remains between left & right.
    # Wall chars use " │ " = 3 chars each (two walls = 6 chars).
    LEFT_W_fixed  = 34
    RIGHT_W_fixed = 34
    helix_w = max(20, panel_inner_width - LEFT_W_fixed - RIGHT_W_fixed - 6)
    middle_lines = render_helix_lines(t, helix_w)

    # RIGHT column: variant scoreboard with mini-bars. Bars are capped so the
    # column stays within RIGHT_W chars even as match counts grow.
    MAX_M_BARS = 8
    MAX_F_BARS = 4
    right_lines = []
    for v in VARIANTS:
        m = STATE.this_run_matched.get(v, 0)
        f = STATE.this_run_failed.get(v, 0)
        total = m + f
        rate = (100.0 * m / total) if total else 0.0
        l = Text()
        l.append(f"{v:<7}", style=f"bold {MOSS}")
        l.append("▰" * min(m, MAX_M_BARS), style=f"bold {OK}")
        if m > MAX_M_BARS:
            l.append("+", style=DIM)
        l.append("▰" * min(f, MAX_F_BARS), style=f"bold {BAD}")
        if f > MAX_F_BARS:
            l.append("+", style=DIM)
        l.append(f"  {m}m·{f}f", style=DIM)
        l.append(f"  {rate:.0f}%", style=f"bold {BODY}" if total else DIM)
        right_lines.append(l)
    # Pad to helix height
    while len(right_lines) < HELIX_H:
        right_lines.append(Text(""))

    # ASSEMBLE with vertical walls between columns. Right column floats to
    # the panel's right edge: we pad the right cell so it fills exactly
    # RIGHT_W_fixed chars, and the total line matches panel_inner_width.
    LEFT_W  = LEFT_W_fixed
    RIGHT_W = RIGHT_W_fixed
    body = Text()
    for i in range(HELIX_H):
        # LEFT
        l = left_lines[i]
        l_plain_len = len(l.plain)
        pad_left = max(0, LEFT_W - l_plain_len)
        body.append(l)
        body.append(" " * pad_left)
        # WALL
        body.append(" │ ", style=MOSS)
        # MIDDLE (helix fills dynamic width)
        m = middle_lines[i]
        body.append(m)
        m_plain_len = len(m.plain)
        pad_mid = max(0, helix_w - m_plain_len)
        body.append(" " * pad_mid)
        # WALL
        body.append(" │ ", style=MOSS)
        # RIGHT (float-right — ends at panel's right edge)
        r = right_lines[i]
        r_plain_len = len(r.plain)
        pad_right = max(0, RIGHT_W - r_plain_len)
        body.append(r)
        body.append(" " * pad_right)
        body.append("\n")

    return Panel(body, border_style=MOSS, box=HEAVY,
                 title=Text(" run status ", style=f"bold {LEAF}"),
                 title_align="left", padding=(0, 1))


# ─── unchanged from v5 ──────────────────────────────────────────────────────
def orch_panel():
    if not STATE.orch_log:
        content = Text("(idle)", style=DIM)
    else:
        t = Text()
        for i, line in enumerate(STATE.orch_log):
            is_latest = (i == len(STATE.orch_log) - 1)
            style = BODY if is_latest else DIM
            if "failed" in line.lower() or "verify" in line.lower():
                style = BAD if is_latest else f"dim {BAD}"
            elif "matched" in line.lower():
                style = OK if is_latest else f"dim {OK}"
            t.append(line + "\n", style=style)
        content = t
    return Panel(content, border_style=MOSS, box=ROUNDED,
                 title=Text(" orchestrator ", style=f"bold {LEAF}"),
                 title_align="left", padding=(0, 1))


def agent_panel():
    if not STATE.agent_log:
        content = Text("(waiting for session)", style=DIM)
    else:
        t = Text()
        for i, line in enumerate(STATE.agent_log):
            is_latest = (i == len(STATE.agent_log) - 1)
            if line.startswith("$"):
                style = SUNLIT if is_latest else f"{SUN}"
            elif line.strip().startswith("→") or line.lstrip().startswith("→"):
                if "MATCH" in line and "MISMATCH" not in line:
                    style = OK if is_latest else f"dim {OK}"
                elif "MISMATCH" in line or "FAILED" in line or "VERIFY" in line:
                    style = BAD if is_latest else f"dim {BAD}"
                else:
                    style = WARN if is_latest else DIM
            elif line.startswith("read") or line.startswith("grep"):
                style = LEAF if is_latest else DIM
            else:
                style = BODY if is_latest else DIM
            t.append(line + "\n", style=style)
        content = t
    subtitle = (Text(STATE.current_batch, style=DIM, overflow="ellipsis")
                if STATE.current_batch != "(waiting)" else None)
    return Panel(content, border_style=MOSS, box=ROUNDED,
                 title=Text(" agent activity ", style=f"bold {LEAF}"),
                 title_align="left",
                 subtitle=subtitle, subtitle_align="left",
                 padding=(0, 1))


def _size_cell(size_bytes):
    """One ▰ per 10 bytes, color-tiered by size. Actual byte count trails."""
    bars = max(1, size_bytes // 10)
    bars = min(bars, 30)  # cap visual width at 30 bars (=300B)
    if size_bytes < 50:
        bar_style = f"dim {LEAF}"
    elif size_bytes < 128:
        bar_style = LEAF
    elif size_bytes < 256:
        bar_style = SUN
    else:
        bar_style = f"bold {SUNLIT}"
    cell = Text()
    cell.append("▰" * bars, style=bar_style)
    cell.append(f"  {size_bytes}B", style=DIM)
    return cell


def outcomes_panel():
    if not STATE.outcomes:
        content = Text("(no outcomes yet)", style=DIM)
    else:
        tbl = Table.grid(padding=(0, 2), expand=True)
        tbl.add_column(width=2)
        tbl.add_column(style=BODY, width=10)
        tbl.add_column(style=BODY, ratio=1)
        tbl.add_column(justify="left", width=40)
        tbl.add_column(style=MOSS, justify="right", width=8)
        for o in STATE.outcomes:
            st = o["status"]
            if st == "matched":
                glyph, label, color = "✓", "matched", OK
            elif st == "failed":
                glyph, label, color = "✗", "failed", BAD
            elif st == "verify":
                glyph, label, color = "⚠", "verify", WARN
            else:
                glyph, label, color = "·", st, DIM
            tbl.add_row(
                Text(glyph, style=f"bold {color}"),
                Text(label, style=color),
                o["name"],
                _size_cell(o["size"]),
                o["variant"],
            )
        content = tbl
    return Panel(content, border_style=MOSS, box=ROUNDED,
                 title=Text(" recent outcomes ", style=f"bold {LEAF}"),
                 title_align="left", padding=(0, 1))


def build_layout(t, status_inner_width):
    root = Layout()
    root.split_column(
        Layout(name="header", size=8),
        Layout(name="status", size=9),
        Layout(name="mid", ratio=1),
        Layout(name="outcomes", size=12),
    )
    root["mid"].split_row(
        Layout(name="orchestrator", ratio=1),
        Layout(name="agent", ratio=1),
    )
    root["header"].update(header_panel())
    root["status"].update(status_panel(t, status_inner_width))
    root["orchestrator"].update(orch_panel())
    root["agent"].update(agent_panel())
    root["outcomes"].update(outcomes_panel())
    return root


# ─── event dispatch ─────────────────────────────────────────────────────────
def apply_event(ev_type, payload):
    if ev_type == "orch":
        STATE.orch_log.append(f"{datetime.now().strftime('%H:%M:%S')} {payload}")
    elif ev_type == "agent":
        STATE.agent_log.append(payload)
    elif ev_type == "session":
        STATE.current_session_sid = payload["sid"]
        STATE.current_session_variant = payload["variant"]
        STATE.current_batch = payload["batch"]
        STATE.current_working = payload.get("working", "(waiting)")
        STATE.agent_log.clear()
    elif ev_type == "working":
        STATE.current_working = payload
    elif ev_type == "outcome":
        STATE.outcomes.appendleft(payload)
        v = payload["variant"]
        if payload["status"] == "matched":
            STATE.this_run_matched[v] = STATE.this_run_matched.get(v, 0) + 1
            STATE.matched_total += 1
            STATE.untried_total -= 1
        elif payload["status"] == "failed":
            STATE.this_run_failed[v] = STATE.this_run_failed.get(v, 0) + 1
            STATE.failed_total += 1
            STATE.untried_total -= 1
        elif payload["status"] == "verify":
            STATE.verify_fails += 1


def _status_inner_width(console):
    # Panel has 2 border cols and 2 padding cols (padding=(0,1))
    return max(40, console.width - 4)


def main():
    console = Console()
    console.clear()
    layout = build_layout(0.0, _status_inner_width(console))

    run_start = time.time()
    last_idx = 0
    with Live(layout, console=console, refresh_per_second=30,
              screen=False, transient=False) as live:
        while True:
            now_sim = time.time() - run_start
            while last_idx < len(TIMELINE) and TIMELINE[last_idx][0] <= now_sim:
                _, ev_type, payload = TIMELINE[last_idx]
                apply_event(ev_type, payload)
                last_idx += 1
                STATE.sim_elapsed = now_sim

            # Recompute width every frame — resizes helix if terminal is resized.
            iw = _status_inner_width(console)
            layout["status"].update(status_panel(now_sim, iw))
            layout["orchestrator"].update(orch_panel())
            layout["agent"].update(agent_panel())
            layout["outcomes"].update(outcomes_panel())

            if last_idx >= len(TIMELINE) and now_sim > TIMELINE[-1][0] + 3.5:
                break
            time.sleep(1 / 30)


if __name__ == "__main__":
    main()
