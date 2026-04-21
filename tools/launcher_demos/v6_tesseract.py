#!/usr/bin/env python3
"""Focused tesseract demo — the chosen winner shown solo.

Big canvas, slow rotation, jungle-palette vertical gradient. Runs until Ctrl-C.
"""

import math
import time

from rich.align import Align
from rich.box import HEAVY
from rich.console import Console
from rich.live import Live
from rich.panel import Panel
from rich.text import Text

import spinners


MOSS = "color(70)"
LEAF = "color(107)"
SUN  = "color(178)"


# Larger canvas than the preview — dedicates more dots per edge so detail pops
BOX_W = 40
BOX_H = 15
# Slow rotation — ~14s for Y axis, ~18s for X
OMEGA_Y = 2 * math.pi / 14
OMEGA_X = 2 * math.pi / 18


def render_tesseract(t):
    rows = spinners.render("tesseract",
                           t * OMEGA_X, t * OMEGA_Y,
                           width=BOX_W, height=BOX_H, t=t)
    body = Text()
    # Vertical gradient: edges feel lit from "canopy" above + "sun" below
    ramp = ["color(22)", "color(28)", "color(34)", "color(70)",
            "color(107)", "color(108)", "color(107)", "color(70)",
            "color(34)",  "color(28)",  "color(22)"]
    for i, row in enumerate(rows):
        shade = ramp[min(i * len(ramp) // BOX_H, len(ramp) - 1)]
        body.append(row + "\n", style=shade)
    return body


def main():
    console = Console()
    with Live(console=console, refresh_per_second=30, screen=False) as live:
        start = time.time()
        try:
            while True:
                t = time.time() - start
                panel = Panel(
                    Align.center(render_tesseract(t)),
                    title=Text(" tesseract ", style=f"bold {SUN}"),
                    border_style=MOSS,
                    box=HEAVY,
                    padding=(0, 2),
                    width=BOX_W + 6,
                )
                live.update(Align.center(panel))
                time.sleep(1 / 30)
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    main()
