#!/usr/bin/env bash
# Launch the live-view TUI for the overnight orchestrator.
#
# Defaults to tailing logs/match_latest.jsonl (symlink the orchestrator
# maintains). Passes any extra args through to tools/ui/app.py so you can
# point at a specific log, enable --replay, --no-backfill, etc.
#
# Usage:
#   ./scripts/ui.sh                               # tail current run
#   ./scripts/ui.sh --log logs/match_X.jsonl --replay
#   ./scripts/ui.sh --no-backfill                 # skip historical events

set -euo pipefail

cd "$(dirname "$0")/.."

exec python3 tools/ui/app.py "$@"
