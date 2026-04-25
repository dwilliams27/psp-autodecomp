#!/bin/bash
# Run the overnight autonomous matching system.
# Enables PF sandbox, runs orchestrator as autodecomp user, cleans up on exit.
#
# Usage:
#   ./tools/run_overnight.sh --hours 8                        # 8-hour run, default priorities
#   ./tools/run_overnight.sh --hours 2 --size-max 8           # 2 hours, trivial functions only
#   ./tools/run_overnight.sh --hours 8 --class eWorld         # target specific class
#   ./tools/run_overnight.sh --dry-run --limit 3              # test with 3 functions, no sandbox
#   ./tools/run_overnight.sh --hours 8 --backend codex        # drive sessions with OpenAI Codex
#   ./tools/run_overnight.sh --hours 8 --backend claude,codex --shootout
#                                                             # Mode B: every backend attempts every function
#   ./tools/run_overnight.sh --hours 8 --backend claude,codex --paired-reserve 50
#                                                             # Mode C: 50 functions reserved for shootout
#
# Auth: --backend claude uses the autodecomp user's Keychain (unlocked below).
#       --backend codex uses the autodecomp user's ~/.codex/auth.json
#       (run `sudo -u autodecomp codex login` once) or a parent OPENAI_API_KEY,
#       which is preserved through sudo -i.

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

# Enable PF firewall — load rules and enable
echo "Enabling PF sandbox..."
# Load rules (may warn about flushing, that's fine)
sudo pfctl -f /etc/pf.conf 2>&1 | grep -v "^$"
# Enable PF (-e returns non-zero if already enabled, which is fine)
sudo pfctl -e 2>&1 | grep -v "^$" || true
# Verify PF is actually running
if ! sudo pfctl -s info 2>/dev/null | grep -q "Status: Enabled"; then
    echo "Error: PF firewall is not running. Cannot run without sandbox."
    exit 1
fi
echo "PF sandbox active."

# Unlock the autodecomp keychain (created with empty password) so Claude can auth
echo "Unlocking keychain..."
sudo -i -u "$SANDBOX_USER" security unlock-keychain -p "" /Users/$SANDBOX_USER/Library/Keychains/login.keychain-db 2>&1 || true
# Prevent auto-lock during the overnight run
sudo -i -u "$SANDBOX_USER" security set-keychain-settings /Users/$SANDBOX_USER/Library/Keychains/login.keychain-db 2>&1 || true

# Cleanup: disable PF on exit (Ctrl-C, crash, normal exit)
cleanup() {
    echo ""
    echo "Flushing PF sandbox rules..."
    # sudo credentials may have expired during long run — use -n (non-interactive)
    # If it fails, tell user to run manually
    if ! sudo -n pfctl -a autodecomp -F all 2>/dev/null; then
        echo "WARNING: Could not flush PF rules (sudo expired). Run manually:"
        echo "  sudo pfctl -a autodecomp -F all"
    else
        echo "PF sandbox rules flushed."
    fi
}
trap cleanup EXIT

# Run orchestrator as sandboxed user
echo "Starting orchestrator as '$SANDBOX_USER'..."
echo ""
# -i required: sets up login environment so macOS Keychain is accessible for Claude auth.
# --preserve-env=OPENAI_API_KEY lets the codex backend use a parent-exported key
# (no-op if unset; codex falls through to its own auth.json).
# Embed repo path directly in the command string since -i sanitizes env.
sudo --preserve-env=OPENAI_API_KEY -i -u "$SANDBOX_USER" bash -c "cd '$REPO_DIR' && python3 tools/orchestrator.py $*"
