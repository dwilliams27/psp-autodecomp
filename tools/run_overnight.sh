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

# Cleanup: disable PF on exit (Ctrl-C, crash, normal exit). Also
# return ownership of files the autodecomp user touched back to the
# operator — without this, config/functions.json (and any newly-
# created src/include files) end up `autodecomp:staff 600` and the
# operator's `tools/func_db.py query` fails with PermissionError
# until a manual chown.
cleanup() {
    echo ""
    echo "Flushing PF sandbox rules..."
    if ! sudo -n pfctl -a autodecomp -F all 2>/dev/null; then
        echo "WARNING: Could not flush PF rules (sudo expired). Run manually:"
        echo "  sudo pfctl -a autodecomp -F all"
    else
        echo "PF sandbox rules flushed."
    fi
    # Re-own anything the run touched. We scope to config/, src/,
    # include/, and logs/ — never broader, since extern/ is shared
    # SDK + tools the operator may have set up with specific perms.
    # `chown -R` (no -H/-L) does not follow symlinks during traversal,
    # so the in-logs match_latest.jsonl symlink and any orphaned
    # shootout-worktree symlinks (if teardown failed) only have their
    # own ownership changed, not the target's.
    OPERATOR_USER="$(stat -f '%Su' "$REPO_DIR" 2>/dev/null || echo '')"
    OPERATOR_GROUP="$(stat -f '%Sg' "$REPO_DIR" 2>/dev/null || echo '')"
    if [ -z "$OPERATOR_USER" ]; then
        echo "WARNING: could not stat $REPO_DIR to determine operator; "
        echo "         skipping chown. Run manually if needed."
    elif ! sudo -n true 2>/dev/null; then
        echo "WARNING: Could not chown back to $OPERATOR_USER (sudo expired)."
        echo "  Run manually: sudo chown -R $OPERATOR_USER:$OPERATOR_GROUP \\"
        echo "    $REPO_DIR/{config,src,include,logs}"
    else
        # Sudo is valid — let chown's own stderr surface so a real
        # filesystem error (e.g. an EXDEV on a weird symlink) doesn't
        # get misattributed to "sudo expired" by a swallowed pipe.
        if sudo chown -R "$OPERATOR_USER:$OPERATOR_GROUP" \
                "$REPO_DIR/config" "$REPO_DIR/src" "$REPO_DIR/include" \
                "$REPO_DIR/logs"; then
            echo "Ownership returned to $OPERATOR_USER."
        else
            echo "WARNING: chown failed — see error above. Repo paths may "
            echo "         still be owned by autodecomp."
        fi
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
