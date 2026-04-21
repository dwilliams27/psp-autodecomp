#!/usr/bin/env python3
"""Launcher UI demo #5 — Full live-run dashboard (extends v3 Overstory).

Simulates ~10 seconds of a live overnight run so you can see the full layout:
  ┌─────────────────────────────────────────────┐
  │  autodecomp (gradient pyfiglet header)     │
  ├─────────────────────────────────────────────┤
  │  run status  (elapsed, deadline, variant    │
  │  match counts, session info)                │
  ├────────────────────┬────────────────────────┤
  │  orchestrator log  │  current agent tail    │
  │  (tail)            │  (tail of bash/read)   │
  ├────────────────────┴────────────────────────┤
  │  recent outcomes  (last 8, with variant)    │
  └─────────────────────────────────────────────┘

Uses a scripted event timeline. Real runs would drive these panels from the
match_*.jsonl log + the live Claude transcript. Render terminates after the
timeline completes so you can inspect the final frame.

Run: python3 tools/launcher_demos/v5_dashboard.py
"""

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


# ─── jungle palette (same as v3 Overstory) ──────────────────────────────────
FOREST    = "color(22)"
MOSS      = "color(70)"
LEAF      = "color(107)"
SUN       = "color(178)"
SUNLIT    = "color(214)"
BODY      = "color(252)"
DIM       = "color(242)"
DARKER    = "color(237)"
OK        = "color(107)"
WARN      = "color(178)"
BAD       = "color(167)"


# ─── simulated run parameters ────────────────────────────────────────────────
HOURS   = 9
VARIANTS = ("base", "tier12")
DEADLINE = datetime.now() + timedelta(hours=HOURS)


# ─── scripted event timeline (seconds_from_start, type, payload) ─────────────
# Compressed — a real run would span 9 hours; this timeline compresses to ~9s.
TIMELINE = [
    (0.0,  "orch",    "session 4b5d3549 [tier12] started · batch=5"),
    (0.0,  "session", {"sid": "4b5d3549", "variant": "tier12",
                       "batch": "eFluidVolume::{ctor, ~dtor, GetType, Read, Write}"}),
    (0.2,  "agent",   "$ python3 tools/func_db.py query --class eFluidVolume --status matched"),
    (0.5,  "agent",   "$ grep -l 'GetType' src/ | head -5"),
    (0.8,  "agent",   "read docs/research/snc-constructor-codegen.md"),
    (1.1,  "agent",   "$ make build/src/eFluidVolume.cpp.o"),
    (1.4,  "agent",   "$ python3 tools/compare_func.py src/eFluidVolume.cpp"),
    (1.6,  "agent",   "  → MATCH  eFluidVolume::ctor  (64B)"),
    (1.7,  "outcome", {"status": "matched", "name": "eFluidVolume::ctor",
                       "size": 64, "variant": "tier12"}),
    (1.8,  "orch",    "  matched: eFluidVolume::ctor"),
    (2.0,  "agent",   "$ python3 tools/compare_func.py src/eFluidVolume.cpp"),
    (2.2,  "agent",   "  → MATCH  eFluidVolume::GetType  (160B)"),
    (2.3,  "outcome", {"status": "matched", "name": "eFluidVolume::GetType",
                       "size": 160, "variant": "tier12"}),
    (2.4,  "orch",    "  matched: eFluidVolume::GetType"),
    (2.7,  "agent",   "$ python3 tools/compare_func.py src/eFluidVolume.cpp"),
    (3.0,  "agent",   "  → MISMATCH  12/196 bytes  (eFluidVolume::~dtor)"),
    (3.2,  "agent",   "read docs/research/snc-delay-slots.md"),
    (3.6,  "agent",   "$ python3 tools/permuter.py src/eFluidVolume.cpp 0x0012b8a4 --time 300"),
    (4.2,  "agent",   "  permuter: 834 candidates · best still 12/196"),
    (4.5,  "agent",   "  → FAILED  eFluidVolume::~dtor  (bnel divergence?)"),
    (4.6,  "outcome", {"status": "failed", "name": "eFluidVolume::~dtor",
                       "size": 196, "variant": "tier12"}),
    (4.7,  "orch",    "  failed:  eFluidVolume::~dtor"),
    (5.0,  "orch",    "session 4b5d3549 done · 2 matched, 1 failed · 4m27s"),
    (5.1,  "orch",    "session e5ff9032 [base] started · batch=5"),
    (5.1,  "session", {"sid": "e5ff9032", "variant": "base",
                       "batch": "gcStateInfo::{New, Read, Write, HandleUpdate, Reset}"}),
    (5.3,  "agent",   "$ python3 tools/decompile_func.py 0x00128c94"),
    (5.7,  "agent",   "$ make build/src/gcStateInfo.cpp.o"),
    (6.0,  "agent",   "$ python3 tools/compare_func.py src/gcStateInfo.cpp"),
    (6.3,  "agent",   "  → MATCH  gcStateInfo::New  (152B)"),
    (6.4,  "outcome", {"status": "matched", "name": "gcStateInfo::New",
                       "size": 152, "variant": "base"}),
    (6.5,  "orch",    "  matched: gcStateInfo::New"),
    (6.9,  "agent",   "$ python3 tools/compare_func.py src/gcStateInfo.cpp"),
    (7.1,  "agent",   "  → MISMATCH  56/240 bytes  (cTimeValue layout mismatch?)"),
    (7.4,  "agent",   "grep -r 'cTimeValue' include/"),
    (7.8,  "agent",   "  → MISMATCH  56/240 bytes (no change)"),
    (8.1,  "agent",   "  → FAILED  gcStateInfo::HandleUpdate"),
    (8.2,  "outcome", {"status": "failed", "name": "gcStateInfo::HandleUpdate",
                       "size": 240, "variant": "base"}),
    (8.3,  "orch",    "  failed:  gcStateInfo::HandleUpdate"),
    (8.6,  "agent",   "$ python3 tools/compare_func.py src/gcStateInfo.cpp"),
    (8.9,  "agent",   "  → VERIFY FAILED  gcStateInfo::Write (bytes don't match expected)"),
    (9.0,  "outcome", {"status": "verify", "name": "gcStateInfo::Write",
                       "size": 128, "variant": "base"}),
    (9.1,  "orch",    "  verify_failed: gcStateInfo::Write · reverted"),
    (9.4,  "orch",    "(tick) progress: 921→923 matched  (+2 this minute)"),
]


# ─── state the dashboard renders from ────────────────────────────────────────
class RunState:
    def __init__(self):
        self.start_time = datetime.now()
        self.sim_elapsed = 0.0
        self.sessions_done = 7      # pretend we're already 7 sessions in
        self.sessions_target = 30   # rough
        self.matched_total = 921    # baseline from actual DB
        self.failed_total = 148
        self.untried_total = 8173
        self.this_run_matched = {"base": 3, "tier12": 5}
        self.this_run_failed  = {"base": 1, "tier12": 2}
        self.verify_fails = 0
        self.current_session_sid = None
        self.current_session_variant = None
        self.current_batch = "(waiting)"
        self.orch_log = deque(maxlen=12)
        self.agent_log = deque(maxlen=14)
        self.outcomes = deque(maxlen=8)


STATE = RunState()


# ─── render helpers ─────────────────────────────────────────────────────────
def gradient_header(text, font="slant"):
    lines = pyfiglet.figlet_format(text, font=font, width=140).rstrip("\n").split("\n")
    ramp = ["color(22)", "color(28)", "color(34)", "color(70)",
            "color(107)", "color(108)"]
    out = Text()
    for i, line in enumerate(lines):
        shade = ramp[min(i * len(ramp) // max(len(lines), 1), len(ramp) - 1)]
        out.append(line + "\n", style=shade)
    return out


def fmt_elapsed(seconds):
    hh = int(seconds // 3600)
    mm = int((seconds % 3600) // 60)
    ss = int(seconds % 60)
    if hh:
        return f"{hh}h {mm:02d}m"
    return f"{mm}m {ss:02d}s"


def header_panel():
    return Align.center(gradient_header("autodecomp"))


def status_panel():
    # elapsed / deadline in "real" sim — pretend 1 sim-sec = 100 real seconds of a run
    sim_minutes = STATE.sim_elapsed * 100 / 60
    elapsed_str = fmt_elapsed(sim_minutes * 60)
    remaining = HOURS * 3600 - sim_minutes * 60
    deadline_str = DEADLINE.strftime("%H:%M")

    # left side: run summary
    summary = Table.grid(padding=(0, 2))
    summary.add_column(style=LEAF, justify="right")
    summary.add_column(style=BODY)
    summary.add_row("elapsed", Text(f"{elapsed_str}  ", style=BODY)
                             + Text(f"/ {HOURS}h", style=DIM))
    summary.add_row("deadline", deadline_str)
    summary.add_row("sessions", f"{STATE.sessions_done} / ~{STATE.sessions_target}")
    summary.add_row("session", Text(
        f"{STATE.current_session_sid or '-'} "
        f"[{STATE.current_session_variant or '-'}]",
        style=f"bold {SUN}"
    ))

    # right side: variant A/B mini-scoreboard
    scoreboard = Table.grid(padding=(0, 2))
    scoreboard.add_column(style=f"bold {MOSS}", justify="right")
    scoreboard.add_column(style=f"bold {OK}", justify="right")
    scoreboard.add_column(style=DIM)
    scoreboard.add_column(style=f"bold {BAD}", justify="right")
    scoreboard.add_column(style=DIM, justify="right")
    for v in VARIANTS:
        m = STATE.this_run_matched.get(v, 0)
        f = STATE.this_run_failed.get(v, 0)
        total = m + f
        rate = (100.0 * m / total) if total else 0.0
        scoreboard.add_row(v,
                           f"{m}", "matched",
                           f"{f}", f"failed · {rate:.0f}%")

    body = Table.grid(expand=True)
    body.add_column(ratio=1)
    body.add_column(ratio=1)
    body.add_row(summary, Align.right(scoreboard))

    return Panel(body, border_style=MOSS, box=HEAVY,
                 title=Text(" run status ", style=f"bold {LEAF}"),
                 title_align="left", padding=(0, 2))


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


def outcomes_panel():
    if not STATE.outcomes:
        content = Text("(no outcomes yet)", style=DIM)
    else:
        tbl = Table.grid(padding=(0, 2), expand=True)
        tbl.add_column(width=2)
        tbl.add_column(style=BODY, width=10)
        tbl.add_column(style=BODY, ratio=1)
        tbl.add_column(style=SUN, justify="right", width=7)
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
                f"{o['size']}B",
                o["variant"],
            )
        content = tbl
    return Panel(content, border_style=MOSS, box=ROUNDED,
                 title=Text(" recent outcomes ", style=f"bold {LEAF}"),
                 title_align="left", padding=(0, 1))


def build_layout():
    root = Layout()
    root.split_column(
        Layout(name="header", size=8),
        Layout(name="status", size=7),
        Layout(name="mid", ratio=1),
        Layout(name="outcomes", size=12),
    )
    root["mid"].split_row(
        Layout(name="orchestrator", ratio=1),
        Layout(name="agent", ratio=1),
    )
    root["header"].update(header_panel())
    root["status"].update(status_panel())
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
        STATE.agent_log.clear()
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


def main():
    console = Console()
    # Clear screen cleanly
    console.clear()
    layout = build_layout()

    run_start = time.time()
    last_idx = 0
    with Live(layout, console=console, refresh_per_second=20,
              screen=False, transient=False) as live:
        while True:
            now_sim = time.time() - run_start
            # Apply any timeline events whose sim time has arrived
            while last_idx < len(TIMELINE) and TIMELINE[last_idx][0] <= now_sim:
                _, ev_type, payload = TIMELINE[last_idx]
                apply_event(ev_type, payload)
                last_idx += 1
                STATE.sim_elapsed = now_sim

            # Refresh panels
            layout["header"].update(header_panel())
            layout["status"].update(status_panel())
            layout["orchestrator"].update(orch_panel())
            layout["agent"].update(agent_panel())
            layout["outcomes"].update(outcomes_panel())

            # Done when all events played + 2s hold
            if last_idx >= len(TIMELINE) and now_sim > TIMELINE[-1][0] + 2.5:
                break
            time.sleep(0.05)


if __name__ == "__main__":
    main()
