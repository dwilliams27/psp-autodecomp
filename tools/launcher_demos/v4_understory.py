#!/usr/bin/env python3
"""Launcher UI demo #4 — Understory.

Minimalist. Lots of whitespace. Just a thin ASCII flourish and the key numbers
tinted. Feels less like a dashboard, more like a quiet confirmation screen.
No figlet — relies entirely on typography + restraint.
"""

import json
import os
from datetime import datetime, timedelta


# 256-color palette, same as others
FG_MOSS   = "\x1b[38;5;70m"
FG_LEAF   = "\x1b[38;5;107m"
FG_SUN    = "\x1b[38;5;178m"
FG_BRIGHT = "\x1b[38;5;252m"
FG_DIM    = "\x1b[38;5;242m"
RESET     = "\x1b[0m"
BOLD      = "\x1b[1m"


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
    mode = f" · {FG_SUN}A/B{RESET}" if len(variants) > 1 else ""

    # Thin decorative separator using a subtle flourish
    flourish = f"{FG_LEAF}⋅⋅⋅{RESET}  {FG_MOSS}❦{RESET}  {FG_LEAF}⋅⋅⋅{RESET}"

    def kv(k, v, vstyle=FG_BRIGHT):
        return f"    {FG_DIM}{k:>10}{RESET}   {vstyle}{v}{RESET}"

    print()
    print(f"  {FG_LEAF}overnight run{RESET}{mode}")
    print()
    print(f"  {flourish}")
    print()
    print(kv("matched", f"{stats['matched']:,}", f"{FG_SUN}{BOLD}"))
    print(kv("failed",  f"{stats['failed']:,}"))
    print(kv("untried", f"{stats['untried']:,}"))
    print(kv("progress", f"{stats['progress']:.1f}%", f"{FG_SUN}{BOLD}"))
    print()
    print(f"  {flourish}")
    print()
    print(kv("duration", f"{hours}h"))
    print(kv("deadline", deadline))
    print(kv("variants", ", ".join(variants), f"{FG_MOSS}"))
    print()
    print(f"  {flourish}")
    print()
    cmd = f"./tools/run_overnight.sh --hours {hours} --variants {','.join(variants)}"
    print(f"    {FG_DIM}${RESET} {FG_MOSS}{cmd}{RESET}")
    print()


if __name__ == "__main__":
    render()
