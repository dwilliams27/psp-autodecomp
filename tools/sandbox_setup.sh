#!/bin/bash
# One-time setup for the autodecomp sandbox.
# Creates a restricted macOS user and PF firewall rules.
#
# Usage: sudo ./tools/sandbox_setup.sh
#
# After running, authenticate Claude Code for the new user:
#   sudo -u autodecomp claude --version

set -euo pipefail

SANDBOX_USER="autodecomp"
SANDBOX_UID="599"
REPO_DIR="$(cd "$(dirname "$0")/.." && pwd)"
PF_ANCHOR_FILE="/etc/pf.anchors/autodecomp"

if [[ $EUID -ne 0 ]]; then
    echo "Error: must run as root (sudo ./tools/sandbox_setup.sh)"
    exit 1
fi

echo "=== Autodecomp Sandbox Setup ==="
echo "Repo: $REPO_DIR"
echo ""

# ──────────────────────────────────────────
# Step 1: Create macOS user
# ──────────────────────────────────────────

if dscl . -read /Users/$SANDBOX_USER &>/dev/null; then
    echo "[skip] User '$SANDBOX_USER' already exists"
else
    echo "[create] Creating user '$SANDBOX_USER'..."
    dscl . -create /Users/$SANDBOX_USER
    dscl . -create /Users/$SANDBOX_USER UserShell /bin/bash
    dscl . -create /Users/$SANDBOX_USER UniqueID $SANDBOX_UID
    dscl . -create /Users/$SANDBOX_USER PrimaryGroupID 20
    dscl . -create /Users/$SANDBOX_USER NFSHomeDirectory /Users/$SANDBOX_USER
    # Hide from login screen
    dscl . -create /Users/$SANDBOX_USER IsHidden 1
    mkdir -p /Users/$SANDBOX_USER
    chown $SANDBOX_USER:staff /Users/$SANDBOX_USER
    echo "[done] User created (UID $SANDBOX_UID, hidden from login)"
fi

# ──────────────────────────────────────────
# Step 2: Grant repo-only file access via ACLs
# ──────────────────────────────────────────

echo "[acl] Granting '$SANDBOX_USER' access to $REPO_DIR..."
chmod -R +a "$SANDBOX_USER allow read,write,execute,delete,add_file,add_subdirectory,list,search,readattr,writeattr,readextattr,writeextattr,readsecurity" "$REPO_DIR"
echo "[done] ACLs applied"

# Also ensure the user's home dir exists for Claude Code config
mkdir -p /Users/$SANDBOX_USER/.claude
chown -R $SANDBOX_USER:staff /Users/$SANDBOX_USER

# ──────────────────────────────────────────
# Step 3: PF firewall rules
# ──────────────────────────────────────────

echo "[pf] Writing firewall rules to $PF_ANCHOR_FILE..."
cat > "$PF_ANCHOR_FILE" << 'PFRULES'
# Autodecomp sandbox: restrict network for the autodecomp user.
# Allow only DNS (for hostname resolution) and HTTPS (for Anthropic API).
# All other outbound traffic is blocked.

# Allow loopback (local tool communication)
pass quick on lo0 from any to any

# Allow DNS (needed to resolve api.anthropic.com)
pass out quick proto udp from any to any port 53 user autodecomp

# Allow HTTPS (Anthropic API uses Cloudflare, IPs rotate)
pass out quick proto tcp from any to any port 443 user autodecomp

# Block everything else from autodecomp
block out log quick user autodecomp
PFRULES

# Check if anchor already in pf.conf
if grep -q 'anchor "autodecomp"' /etc/pf.conf; then
    echo "[skip] PF anchor already in /etc/pf.conf"
else
    echo "[pf] Adding anchor to /etc/pf.conf..."
    # Insert before the last line (usually the default pass/block rule)
    cp /etc/pf.conf /etc/pf.conf.backup.$(date +%Y%m%d)
    # Append anchor lines
    echo '' >> /etc/pf.conf
    echo 'anchor "autodecomp"' >> /etc/pf.conf
    echo 'load anchor "autodecomp" from "/etc/pf.anchors/autodecomp"' >> /etc/pf.conf
    echo "[done] PF anchor added (backup at /etc/pf.conf.backup.*)"
fi

# Load rules (don't enable yet — run_overnight.sh does that)
if ! pfctl -f /etc/pf.conf 2>&1; then
    echo "[ERROR] Failed to load PF rules. Fix /etc/pf.conf and retry."
    exit 1
fi
echo "[done] PF rules loaded"

# ──────────────────────────────────────────
# Step 4: Verify
# ──────────────────────────────────────────

echo ""
echo "=== Verification ==="
PASS=0
FAIL=0

check() {
    local desc="$1"
    shift
    local output
    if output=$("$@" 2>&1); then
        echo "  ✓ $desc"
        PASS=$((PASS + 1))
    else
        echo "  ✗ $desc"
        echo "    $output"
        FAIL=$((FAIL + 1))
    fi
}

check_fail() {
    local desc="$1"
    shift
    local output
    if output=$("$@" 2>&1); then
        echo "  ✗ $desc (should have failed but succeeded)"
        echo "    $output"
        FAIL=$((FAIL + 1))
    else
        echo "  ✓ $desc (correctly denied)"
        PASS=$((PASS + 1))
    fi
}

check "User '$SANDBOX_USER' exists" \
    dscl . -read /Users/$SANDBOX_USER

check "Can read repo directory" \
    sudo -u $SANDBOX_USER ls "$REPO_DIR/Makefile"

check "Can write to repo directory" \
    sudo -u $SANDBOX_USER bash -c "touch '$REPO_DIR/.sandbox_test' && rm -f '$REPO_DIR/.sandbox_test'"

REPO_OWNER="${SUDO_USER:-$(stat -f '%Su' "$REPO_DIR/..")}"
OWNER_HOME="$(dscl . -read /Users/$REPO_OWNER NFSHomeDirectory 2>/dev/null | awk '{print $2}')"
if [[ -z "$OWNER_HOME" || ! -d "$OWNER_HOME" ]]; then
    echo "  ✗ Cannot determine repo owner's home directory (owner=$REPO_OWNER, home=$OWNER_HOME)"
    FAIL=$((FAIL + 1))
else
    check_fail "Cannot read owner's Documents ($OWNER_HOME/Documents)" \
        sudo -u $SANDBOX_USER ls "$OWNER_HOME/Documents"

    check_fail "Cannot read owner's .ssh ($OWNER_HOME/.ssh)" \
        sudo -u $SANDBOX_USER ls "$OWNER_HOME/.ssh"
fi

check "PF anchor file exists" \
    test -f "$PF_ANCHOR_FILE"

check "PF autodecomp anchor loaded" \
    bash -c "pfctl -s anchors 2>/dev/null | grep -q autodecomp"

echo ""
echo "Results: $PASS passed, $FAIL failed"

if [[ $FAIL -gt 0 ]]; then
    echo ""
    echo "WARNING: Some checks failed. Review above and fix before running overnight."
    exit 1
fi

echo ""
echo "Setup complete. Next steps:"
echo "  1. Authenticate Claude Code: sudo -u $SANDBOX_USER claude --version"
echo "     (follow the auth prompts if needed)"
echo "  2. Test: sudo -u $SANDBOX_USER python3 $REPO_DIR/tools/func_db.py stats"
echo "  3. Run: ./tools/run_overnight.sh --hours 8"
