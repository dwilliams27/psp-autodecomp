#!/usr/bin/env python3
"""Smoke tests for TUI run-scoped state handling."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))

from tools.ui.state import RunState


def main():
    state = RunState()

    # Simulate a previous run that displayed 5 slots, then a new run_start
    # declaring 4 workers. The TUI should not retain the stale fifth slot.
    state.ensure_slots(5)
    state.assign_session("old0", identity="codex/old")
    state.orch_log.append("old run line")
    state.outcomes.append({"session_id": "old0"})

    state.reset_for_run()
    state.worker_count = 4
    state.ensure_backend("codex/gpt-5.4/high")
    state.ensure_variant("base")
    state.ensure_slots(state.worker_count)

    assert state.worker_count == 4
    assert len(state.slots) == 4
    assert state.session_to_slot == {}
    assert not state.outcomes
    assert list(state.this_run_matched_by_backend) == ["codex/gpt-5.4/high"]

    print("TUI state smoke: PASS")


if __name__ == "__main__":
    main()
