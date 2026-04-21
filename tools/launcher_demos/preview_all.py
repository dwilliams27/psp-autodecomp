#!/usr/bin/env python3
"""Run all four launcher UI demos back-to-back for visual comparison.
Usage: python3 tools/launcher_demos/preview_all.py
"""

import subprocess
import os
import sys


DEMOS = [
    ("v1_canopy.py",    "v1 — Canopy     (pure ANSI, dense, pyfiglet header)"),
    ("v2_fern.py",      "v2 — Fern       (rich panels, subtle, no big header)"),
    ("v3_overstory.py", "v3 — Overstory  (big gradient pyfiglet + rich panels)"),
    ("v4_understory.py","v4 — Understory (minimalist, flourishes only, no figlet)"),
]


def sep(label):
    print()
    print(f"\x1b[38;5;242m{'━' * 78}\x1b[0m")
    print(f"  \x1b[38;5;178m\x1b[1m{label}\x1b[0m")
    print(f"\x1b[38;5;242m{'━' * 78}\x1b[0m")


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    for script, label in DEMOS:
        sep(label)
        subprocess.run([sys.executable, os.path.join(here, script)], check=False)
    print()
    print(f"\x1b[38;5;242m{'━' * 78}\x1b[0m")
    print("  \x1b[38;5;107mPick one: v1 canopy | v2 fern | v3 overstory | v4 understory\x1b[0m")
    print(f"\x1b[38;5;242m{'━' * 78}\x1b[0m")
    print()


if __name__ == "__main__":
    main()
