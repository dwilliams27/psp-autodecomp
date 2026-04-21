#!/usr/bin/env python3
"""Preview 4 rotating candidates side-by-side with braille rendering.

Shows each shape rotating continuously until Ctrl-C. Rotation is slow enough
to be visually smooth; braille subpixels give ~8× the resolution of block
chars so frame-to-frame transitions are subtle, not jarring.
"""

import time

from rich.console import Console
from rich.live import Live
from rich.panel import Panel
from rich.table import Table
from rich.text import Text

import spinners


MOSS = "color(70)"
LEAF = "color(107)"
SUN  = "color(178)"
DIM  = "color(242)"


BOX_W = 22
BOX_H = 9


def render_spinner(shape_name, t):
    # Slower than before — one full rotation in ~10s around Y, ~14s around X
    ax = t * (2 * 3.14159 / 14)
    ay = t * (2 * 3.14159 / 10)
    rows = spinners.render(shape_name, ax, ay,
                           width=BOX_W, height=BOX_H, t=t)
    body = Text()
    ramp = ["color(34)", "color(70)", "color(107)", "color(108)",
            "color(70)",  "color(34)"]
    for i, row in enumerate(rows):
        shade = ramp[min(i * len(ramp) // BOX_H, len(ramp) - 1)]
        body.append(row + "\n", style=shade)
    return body


def main():
    console = Console()
    shapes = ["tesseract", "sphere", "torus", "icosphere"]

    print()
    print(f"\x1b[38;5;107m  4 candidates — braille subpixel rendering. "
          f"Ctrl-C to exit.\x1b[0m")
    print()

    with Live(console=console, refresh_per_second=30, screen=False) as live:
        start = time.time()
        try:
            while True:
                t = time.time() - start
                grid = Table.grid(padding=(0, 2), expand=False)
                grid.add_column()
                grid.add_column()
                panels = []
                for s in shapes:
                    body = render_spinner(s, t)
                    panels.append(Panel(
                        body,
                        title=Text(f" {s} ", style=f"bold {SUN}"),
                        title_align="center",
                        border_style=MOSS,
                        padding=(0, 1),
                        width=BOX_W + 4,
                    ))
                grid.add_row(panels[0], panels[1])
                grid.add_row(panels[2], panels[3])
                live.update(grid)
                time.sleep(1 / 30)
        except KeyboardInterrupt:
            pass


if __name__ == "__main__":
    main()
