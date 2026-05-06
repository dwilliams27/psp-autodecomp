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
#   ./tools/run_overnight.sh --hours 8 --workers 2 --identities codex/gpt-5.5/low,codex/gpt-5.5/high
#                                                             # compare Codex effort levels
#   ./tools/run_overnight.sh --hours 8 --backend claude,codex --shootout
#                                                             # Mode B: every backend attempts every function
#   ./tools/run_overnight.sh --hours 8 --backend claude,codex --paired-reserve 50
#                                                             # Mode C: 50 functions reserved for shootout
#   ./tools/run_overnight.sh --allow-drift --hours 8          # loud override for known DB byte drift
#
# Auth: --backend claude uses the autodecomp user's Keychain (unlocked below).
#       --backend codex uses the autodecomp user's ~/.codex/auth.json
#       (run `sudo -u autodecomp codex login` once) or a parent OPENAI_API_KEY,
#       which is preserved through sudo -i.

set -euo pipefail

REPO_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SANDBOX_USER="autodecomp"

# Check wrapper-only flags before passing the rest to the orchestrator.
DRY_RUN=false
ALLOW_DRIFT=false
ORCH_ARGS=()
for arg in "$@"; do
    case "$arg" in
        --dry-run)
            DRY_RUN=true
            ORCH_ARGS+=("$arg")
            ;;
        --allow-drift)
            ALLOW_DRIFT=true
            ;;
        *)
            ORCH_ARGS+=("$arg")
            ;;
    esac
done

# Pre-flight DB drift gate. Keep this outside the sandbox setup so a
# broken matched database fails before PF/keychain work begins.
cd "$REPO_DIR"
mkdir -p "$REPO_DIR/build/src"
chmod a+rwx "$REPO_DIR/build/src" 2>/dev/null || true
if [[ "$ALLOW_DRIFT" == "true" ]]; then
    echo "WARNING: --allow-drift set; skipping verify_matches pre-flight."
    echo "This run may inherit already-broken matched DB entries."
    echo ""
else
    echo "Pre-flight: verifying all matched DB entries..."
    set +e
    python3 tools/verify_matches.py
    VERIFY_RC=$?
    set -e
    if [[ "$VERIFY_RC" -ne 0 ]]; then
        echo ""
        echo "Pre-flight verify_matches failed. Refusing to start overnight run."
        echo "Fix the drift or rerun with --allow-drift to override explicitly."
        exit "$VERIFY_RC"
    fi
    echo "Pre-flight: OK — all matched DB entries byte-verify."
    echo ""
fi

if [[ "$DRY_RUN" == "true" ]]; then
    echo "=== DRY RUN (no sandbox) ==="
    python3 tools/orchestrator.py "${ORCH_ARGS[@]}"
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
echo "Args: ${ORCH_ARGS[*]}"
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

# Cleanup: disable PF on exit (Ctrl-C, crash, normal exit).
cleanup() {
    echo ""
    echo "Flushing PF sandbox rules..."
    if ! sudo -n pfctl -a autodecomp -F all 2>/dev/null; then
        echo "WARNING: Could not flush PF rules (sudo expired). Run manually:"
        echo "  sudo pfctl -a autodecomp -F all"
    else
        echo "PF sandbox rules flushed."
    fi
}
trap cleanup EXIT

# Ensure build/src/ exists and is writable by both users.
# If an agent nukes build/src/ mid-run and recreates it as autodecomp,
# the repo owner can't compile afterward (POSIX perms override ACL on mkdir).
mkdir -p "$REPO_DIR/build/src"
chmod a+rwx "$REPO_DIR/build/src"

# Run orchestrator as sandboxed user
echo "Starting orchestrator as '$SANDBOX_USER'..."
echo ""
# -i required: sets up login environment so macOS Keychain is accessible for Claude auth.
# --preserve-env=OPENAI_API_KEY lets the codex backend use a parent-exported key
# (no-op if unset; codex falls through to its own auth.json).
# Embed shell-quoted absolute paths directly in the command string. On macOS,
# `sudo -i ... bash -c script arg...` does not reliably preserve positional
# args after the login-shell boundary, so do not depend on `$1` for REPO_DIR.
ORCH_CMD="umask 0002 && cd $(printf '%q' "$REPO_DIR") && python3 $(printf '%q' "$REPO_DIR/tools/orchestrator.py")"
for arg in "${ORCH_ARGS[@]}"; do
    ORCH_CMD+=" $(printf '%q' "$arg")"
done
sudo --preserve-env=OPENAI_API_KEY -i -u "$SANDBOX_USER" \
    bash -lc "$ORCH_CMD"
