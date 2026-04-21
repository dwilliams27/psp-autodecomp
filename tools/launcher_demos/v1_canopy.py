#!/usr/bin/env python3
"""Launcher UI demo #1 — Canopy.

Pure ANSI, no external deps (except pyfiglet for the header). Compact/dense.
Forest-green dominant with amber accents on the load-bearing numbers.
"""

import json
import os
import sys
from datetime import datetime, timedelta
import pyfiglet

# 256-color jungle palette
FG_DEEP    = "\x1b[38;5;22m"   # deep forest
FG_MOSS    = "\x1b[38;5;70m"   # moss / primary green
FG_CANOPY  = "\x1b[38;5;107m"  # dim gray-green (borders, labels)
FG_SUN     = "\x1b[38;5;178m"  # amber / accent
FG_BRIGHT  = "\x1b[38;5;252m"  # near-white body text
FG_DIM     = "\x1b[38;5;240m"  # dim grey
RESET      = "\x1b[0m"
BOLD       = "\x1b[1m"


def load_stats():
    path = os.path.join(os.path.dirname(__file__), "..", "..",
                        "config", "functions.json")
    with open(path) as f:
        funcs = json.load(f)
    c = {"matched": 0, "failed": 0, "untried": 0, "in_progress": 0}
    for fn in funcs:
        s = fn.get("match_status", "untried")
        if s in c:
            c[s] += 1
    c["total"] = len(funcs)
    c["progress"] = 100.0 * c["matched"] / c["total"] if c["total"] else 0
    return c


def render(hours=9, variants=("base", "tier12")):
    stats = load_stats()
    deadline = (datetime.now() + timedelta(hours=hours)).strftime("%a %H:%M")

    # Header
    header = pyfiglet.figlet_format("autodecomp", font="small", width=80)
    for line in header.rstrip("\n").split("\n"):
        print(f"  {FG_MOSS}{line}{RESET}")

    # Divider
    print(f"  {FG_CANOPY}{'─' * 64}{RESET}")
    print()

    # Status
    print(f"    {FG_CANOPY}status{RESET}")
    print(f"      {FG_BRIGHT}matched{RESET}    "
          f"{FG_SUN}{BOLD}{stats['matched']:>6,}{RESET}  "
          f"{FG_DIM}│{RESET}  "
          f"{FG_BRIGHT}failed{RESET}     "
          f"{FG_SUN}{BOLD}{stats['failed']:>5,}{RESET}")
    print(f"      {FG_BRIGHT}untried{RESET}    "
          f"{FG_BRIGHT}{stats['untried']:>6,}{RESET}  "
          f"{FG_DIM}│{RESET}  "
          f"{FG_BRIGHT}progress{RESET}   "
          f"{FG_SUN}{BOLD}{stats['progress']:>4.1f}%{RESET}")
    print()

    # Config
    print(f"    {FG_CANOPY}config{RESET}")
    print(f"      {FG_BRIGHT}duration{RESET}   {FG_SUN}{hours}h{RESET}")
    print(f"      {FG_BRIGHT}deadline{RESET}   {FG_BRIGHT}{deadline}{RESET}")
    mode = f"{FG_MOSS}A/B{RESET} · " if len(variants) > 1 else ""
    vlist = ", ".join(f"{FG_SUN}{v}{RESET}" for v in variants)
    print(f"      {FG_BRIGHT}variants{RESET}   {mode}{vlist}")
    print()

    # Ready
    print(f"  {FG_CANOPY}{'─' * 64}{RESET}")
    cmd = f"./tools/run_overnight.sh --hours {hours} --variants {','.join(variants)}"
    print(f"    {FG_DIM}$ {FG_MOSS}{cmd}{RESET}")
    print()


if __name__ == "__main__":
    render()
