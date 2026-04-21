#!/usr/bin/env python3
"""Launcher UI demo #2 — Fern.

rich-based panels + tables with organic green borders. No big ASCII.
Subtle vibe: structure over ornamentation.
"""

import json
import os
from datetime import datetime, timedelta

from rich.align import Align
from rich.box import ROUNDED
from rich.console import Console, Group
from rich.panel import Panel
from rich.table import Table
from rich.text import Text


# Truecolor jungle palette
GREEN_DEEP   = "#1f5030"
GREEN_MOSS   = "#4c864a"
GREEN_LEAF   = "#7baa5f"
AMBER        = "#c1892a"
AMBER_BRIGHT = "#e0a64a"
DIM          = "#707060"
BODY         = "#d4d4c6"


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
    console = Console()
    stats = load_stats()
    deadline = (datetime.now() + timedelta(hours=hours)).strftime("%a %H:%M")

    # Status table
    status = Table.grid(padding=(0, 2))
    status.add_column(style=BODY, justify="right")
    status.add_column(style=f"bold {AMBER_BRIGHT}")
    status.add_column(style=DIM, justify="center")
    status.add_column(style=BODY, justify="right")
    status.add_column(style=f"bold {AMBER_BRIGHT}")
    status.add_row("matched", f"{stats['matched']:,}",
                   "│",
                   "failed", f"{stats['failed']:,}")
    status.add_row("untried", f"{stats['untried']:,}",
                   "│",
                   "progress", f"{stats['progress']:.1f}%")

    # Config table
    config = Table.grid(padding=(0, 2))
    config.add_column(style=BODY, justify="right")
    config.add_column(style=f"bold {GREEN_LEAF}")
    mode = "A/B · " if len(variants) > 1 else ""
    config.add_row("duration", f"{hours}h")
    config.add_row("deadline", Text(deadline, style=BODY))
    config.add_row("variants", Text(f"{mode}{', '.join(variants)}",
                                     style=f"bold {GREEN_LEAF}"))

    console.print()
    console.print(Align.center(Panel(
        Group(
            Align.center(status),
            Text(""),
            Align.center(config),
        ),
        title=Text("   overnight run   ",
                   style=f"bold {GREEN_LEAF} on #0a1a0a"),
        title_align="center",
        border_style=GREEN_MOSS,
        box=ROUNDED,
        padding=(1, 4),
        width=62,
    )))

    # Command preview
    cmd = f"./tools/run_overnight.sh --hours {hours} --variants {','.join(variants)}"
    console.print()
    console.print(Align.center(Text(f"$ {cmd}",
                                    style=f"{DIM} not bold")))
    console.print()


if __name__ == "__main__":
    render()
