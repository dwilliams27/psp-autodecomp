#!/bin/bash
# Run the overnight autonomous matching system.
# Enables PF sandbox, runs orchestrator as autodecomp user, cleans up on exit.
#
# Usage:
#   ./tools/run_overnight.sh --hours 8                    # 8-hour run, default priorities
#   ./tools/run_overnight.sh --hours 2 --size-max 8       # 2 hours, trivial functions only
#   ./tools/run_overnight.sh --hours 8 --class eWorld     # target specific class
#   ./tools/run_overnight.sh --dry-run --limit 3          # test with 3 functions, no sandbox

set -euo pipefail

REPO_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SANDBOX_USER="autodecomp"

# Check if --dry-run flag is present (skip sandbox for testing)
DRY_RUN=false
for arg in "$@"; do
    if [[ "$arg" == "--dry-run" ]]; then
        DRY_RUN=true
    fi
done

if [[ "$DRY_RUN" == "true" ]]; then
    echo "=== DRY RUN (no sandbox) ==="
    cd "$REPO_DIR"
    python3 tools/orchestrator.py "$@"
    exit $?
fi

# Verify sandbox is set up
if ! dscl . -read /Users/$SANDBOX_USER &>/dev/null; then
    echo "Error: sandbox user '$SANDBOX_USER' not found."
    echo "Run: sudo ./tools/sandbox_setup.sh"
    exit 1
fi

if [[ ! -f /etc/pf.anchors/autodecomp ]]; then
    echo "Error: PF rules not found."
    echo "Run: sudo ./tools/sandbox_setup.sh"
    exit 1
fi

echo "=== Overnight Matching Run ==="
echo "Repo: $REPO_DIR"
echo "User: $SANDBOX_USER"
echo "Args: $*"
echo ""

# Enable PF firewall — fail if this doesn't work (sandbox is the whole point)
echo "Enabling PF sandbox..."
if ! sudo pfctl -e -f /etc/pf.conf 2>&1; then
    echo "Error: failed to enable PF firewall. Cannot run without sandbox."
    exit 1
fi

# Cleanup: disable PF on exit (Ctrl-C, crash, normal exit)
cleanup() {
    echo ""
    echo "Flushing PF sandbox rules..."
    if ! sudo pfctl -a autodecomp -F all 2>&1; then
        echo "WARNING: Failed to flush PF autodecomp anchor. Run 'sudo pfctl -a autodecomp -F all' manually."
    fi
    echo "Done."
}
trap cleanup EXIT

# Run orchestrator as sandboxed user
echo "Starting orchestrator as '$SANDBOX_USER'..."
echo ""
sudo -u "$SANDBOX_USER" bash -c 'cd "$1" && shift && python3 tools/orchestrator.py "$@"' -- "$REPO_DIR" "$@"
