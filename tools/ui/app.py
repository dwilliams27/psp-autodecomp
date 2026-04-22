#!/usr/bin/env python3
"""Live-view TUI for the overnight orchestrator.

Launches a Rich Live loop that tails the run's jsonl and renders the
active screen. The default screen is `running` — the overnight dashboard.

Usage:
    # Tail the current overnight run (symlink updated by orchestrator on start).
    python3 tools/ui/app.py

    # Replay a historical run.
    python3 tools/ui/app.py --log logs/match_20260421_102005.jsonl --replay

    # Resume tailing from the end (skip backfill, live only).
    python3 tools/ui/app.py --log logs/match_latest.jsonl --no-backfill
"""

import argparse
import os
import sys
import time
from pathlib import Path

# Support `python3 tools/ui/app.py` from the repo root (no install).
_REPO_ROOT = Path(__file__).resolve().parents[2]
if str(_REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(_REPO_ROOT))

from rich.console import Console
from rich.live import Live
from rich.text import Text

from tools.ui.data.log_tailer import LogTailer
from tools.ui.data.db import read_matched_total
from tools.ui.state import RunState
from tools.ui.screens.base import Screen
from tools.ui.screens.running import RunningScreen


DEFAULT_LOG = "logs/match_latest.jsonl"

# Give the orchestrator up to this long to create the log file. Longer than
# typical orchestrator startup (~2-3s) but short enough that a typo or a
# stopped orchestrator surfaces quickly.
LOG_WAIT_DEADLINE_S = 30


class App:
    """Owns the Live loop, log tailer, state, and screen registry."""

    def __init__(self, log_path, replay=False, backfill=True, refresh_hz=30):
        self.log_path = log_path
        self.replay = replay
        self.backfill = backfill
        self.refresh_hz = refresh_hz
        self.state = RunState()
        self._screens: dict[str, Screen] = {}
        self._active_name: str | None = None
        self._console = Console()
        self._start_wall = time.time()
        self._tailer: LogTailer | None = None

    # -- screen registry --
    def register(self, screen: Screen):
        self._screens[screen.name] = screen

    def switch_to(self, name: str):
        if name not in self._screens:
            raise KeyError(f"no screen registered: {name}")
        if self._active_name:
            self._screens[self._active_name].on_unmount(self)
        self._active_name = name
        self._screens[name].on_mount(self)

    @property
    def active_screen(self) -> Screen:
        return self._screens[self._active_name]

    # -- time helpers --
    def wall_clock_s(self):
        return time.time() - self._start_wall

    # -- main loop --
    def run(self):
        if not self._screens:
            raise RuntimeError("no screens registered")
        if not self._active_name:
            self.switch_to(next(iter(self._screens)))

        # Seed the absolute matched count so the first progress_tick delta
        # isn't computed from zero.
        self.state.matched_total = read_matched_total()

        self._tailer = LogTailer(self.log_path, from_start=self.backfill)

        # Wait for the log file to appear if it doesn't exist yet — but bail
        # with a clear error if it never shows up, rather than hanging forever
        # on a typo'd --log path.
        deadline = time.time() + LOG_WAIT_DEADLINE_S
        waited = False
        while not os.path.exists(self._resolved_path()):
            if time.time() > deadline:
                raise FileNotFoundError(
                    f"log path {self.log_path} (resolves to "
                    f"{self._resolved_path()}) did not appear within "
                    f"{LOG_WAIT_DEADLINE_S}s — is the orchestrator running?"
                )
            if not waited:
                self._console.print(
                    f"[dim]waiting for log at {self.log_path}...[/dim] "
                    f"(run the orchestrator to create it)",
                    end="\r",
                )
                waited = True
            time.sleep(0.5)
        if waited:
            self._console.print(" " * 80, end="\r")

        # Drain any backfilled history so the state is caught up before the
        # Live loop starts painting — avoids the panels jump-filling on frame 1.
        initial = self._tailer.poll()
        screen = self.active_screen
        for ev in initial:
            screen.on_event(self, ev)

        frame_s = 1.0 / self.refresh_hz
        try:
            with Live(self._render(), console=self._console,
                      refresh_per_second=self.refresh_hz,
                      screen=False, transient=False) as live:
                while True:
                    for ev in self._tailer.poll():
                        screen.on_event(self, ev)
                    screen.on_tick(self, self.wall_clock_s())
                    live.update(self._render())
                    if self.replay and self.state.run_done:
                        break
                    time.sleep(frame_s)
        except KeyboardInterrupt:
            self._console.print("\n[dim]exit[/dim]")

    def _render(self):
        return self.active_screen.render(self, self._console)

    def _resolved_path(self):
        """Resolve symlinks — os.path.exists on a dangling symlink returns False."""
        return os.path.realpath(self.log_path)


def main():
    parser = argparse.ArgumentParser(
        description="Live-view TUI for the overnight orchestrator")
    parser.add_argument(
        "--log", default=DEFAULT_LOG,
        help=f"Path to match_*.jsonl (default: {DEFAULT_LOG})")
    parser.add_argument(
        "--replay", action="store_true",
        help="Replay-mode: exit after the run_done event instead of tailing forever")
    parser.add_argument(
        "--no-backfill", action="store_true",
        help="Skip existing events, only show ones appended after launch")
    parser.add_argument(
        "--refresh", type=int, default=30,
        help="UI refresh rate in Hz (default 30)")
    args = parser.parse_args()

    app = App(
        log_path=args.log,
        replay=args.replay,
        backfill=not args.no_backfill,
        refresh_hz=args.refresh,
    )
    app.register(RunningScreen())
    app.switch_to("running")
    app.run()


if __name__ == "__main__":
    main()
