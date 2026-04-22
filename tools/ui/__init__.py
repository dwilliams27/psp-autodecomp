"""Live-view TUI for the overnight orchestrator.

Entry point: `python3 tools/ui/app.py` (tails logs/match_latest.jsonl by default).

The app owns a Rich Live loop plus a log tailer; screens implement on_event/
on_tick/render. Today the only screen is `running` (live overnight dashboard).
New screens (config, historical, postmortem) plug in via `App.register`.
"""
