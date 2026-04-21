#!/usr/bin/env python3
"""Launcher UI demo #3 — Overstory.

Big pyfiglet header with gradient shading, rich panels underneath.
More dramatic / splash-screen feel. Jungle palette: forest green + amber.
"""

import json
import os
from datetime import datetime, timedelta

import pyfiglet
from rich.align import Align
from rich.box import HEAVY
from rich.console import Console, Group
from rich.panel import Panel
from rich.table import Table
from rich.text import Text


# 256-color codes that play nice with rich
FOREST = "color(22)"
MOSS   = "color(70)"
LEAF   = "color(107)"
SUN    = "color(178)"
SUNLIT = "color(214)"
BODY   = "color(252)"
DIM    = "color(242)"


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


def gradient_header(text, font="slant"):
    """Render figlet text with a vertical green-gradient."""
    ascii_lines = pyfiglet.figlet_format(text, font=font, width=100).rstrip("\n").split("\n")
    # Gradient from deep forest -> moss -> leaf top-to-bottom
    ramp = ["color(22)", "color(28)", "color(34)", "color(70)",
            "color(107)", "color(108)"]
    out = Text()
    for i, line in enumerate(ascii_lines):
        shade = ramp[min(i * len(ramp) // max(len(ascii_lines), 1), len(ramp) - 1)]
        out.append(line + "\n", style=shade)
    return out


def render(hours=9, variants=("base", "tier12")):
    console = Console()
    stats = load_stats()
    deadline = (datetime.now() + timedelta(hours=hours)).strftime("%a %H:%M")

    console.print()
    console.print(Align.center(gradient_header("autodecomp", font="slant")))

    # Status panel
    status = Table.grid(padding=(0, 3))
    status.add_column(style=LEAF, justify="right")
    status.add_column(style=f"bold {SUNLIT}", justify="left")
    status.add_column(width=3)
    status.add_column(style=LEAF, justify="right")
    status.add_column(style=f"bold {SUNLIT}", justify="left")
    status.add_row("matched", f"{stats['matched']:,}", "",
                   "failed",  f"{stats['failed']:,}")
    status.add_row("untried", f"{stats['untried']:,}", "",
                   "progress",    f"{stats['progress']:.1f}%")

    # Config panel
    config = Table.grid(padding=(0, 3))
    config.add_column(style=LEAF, justify="right")
    config.add_column(style=BODY)
    mode = Text("A/B", style=f"bold {SUN}") if len(variants) > 1 else Text("")
    vtxt = Text()
    if mode:
        vtxt.append(mode)
        vtxt.append(" · ", style=DIM)
    vtxt.append(", ".join(variants), style=f"bold {MOSS}")
    config.add_row("duration", Text(f"{hours}h", style=f"bold {BODY}"))
    config.add_row("deadline", Text(deadline, style=BODY))
    config.add_row("variants", vtxt)

    console.print(Align.center(Panel(
        Align.center(Group(status, Text(""), config)),
        border_style=MOSS,
        box=HEAVY,
        padding=(1, 6),
        width=66,
    )))

    cmd = f"./tools/run_overnight.sh --hours {hours} --variants {','.join(variants)}"
    console.print()
    console.print(Align.center(Text(f"→  {cmd}", style=f"{SUN} bold")))
    console.print()


if __name__ == "__main__":
    render()
