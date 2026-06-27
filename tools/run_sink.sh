#!/bin/bash
# Sink-mode overnight launcher — the workload half of the psp-autodecomp "sink
# sub-tenant" (see the assistant repo's docs/projects/psp-decomp-tenant.md).
#
# This is what the mini's nix-store `psp-run` invokes, AS the `autodecomp` uid,
# AFTER it has already (a) checked the household-halt flag, (b) enabled the PF
# egress firewall as root, and (c) sudo'd down to autodecomp. So unlike
# run_overnight.sh this script does NOT manage the sandbox user, PF, sudo, or the
# keychain — it just runs the matching orchestrator, with two sink-tenancy
# invariants enforced loudly:
#
#   1. SUBSCRIPTION COMPUTE ONLY (sink decision 0002). The OPENAI_API_KEY metered
#      path that run_overnight.sh tolerates is REFUSED here — model auth must be
#      app-managed subscription OAuth (~/.claude, ~/.codex). psp-run scrubs the
#      var before we run; we re-assert it as defense in depth.
#   2. File-based auth, no keychain. The mini's autodecomp account authenticates
#      Claude/Codex from ~/.claude and ~/.codex files, like _sink — so there is
#      no `security unlock-keychain` dance.
#
# It is safe to run by hand for a supervised test:
#   sudo -i -u autodecomp env -u OPENAI_API_KEY \
#     bash -lc 'cd ~/psp-autodecomp && tools/run_sink.sh --hours 1'
# (but PF is then NOT enabled — only psp-run enables the egress jail.)
#
# Usage:
#   tools/run_sink.sh --hours 8                 # 8h overnight, Claude (subscription)
#   tools/run_sink.sh --hours 8 --size-max 64   # extra orchestrator args pass through
#   tools/run_sink.sh --hours 8 --allow-drift   # loud override for known DB byte drift
#   tools/run_sink.sh --hours 8 --push          # push the overnight branch to origin at the end
#   tools/run_sink.sh --hours 8 --stock-compiler
#
# Orchestrator git behavior (unchanged): it requires a clean tree on `main`,
# creates an `overnight/<ts>` branch, and commits matches there locally. It does
# NOT push — so this script optionally pushes that branch at the end (--push) for
# off-box backup + GitHub review; default is local-only, David reviews on the box.

set -euo pipefail

REPO_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$REPO_DIR"

# ── Self-logging (we run AS autodecomp, so we own ~ and can write the log) ──
# psp-run, which invokes us, runs as rob and can't write into autodecomp's home,
# so the persistent log lives here on the autodecomp side. Everything below is
# tee'd to a timestamped file plus a stable psp-current.log symlink; keep ~40.
LOGDIR="$HOME/psp-sink-logs"
mkdir -p "$LOGDIR"
LOGFILE="$LOGDIR/$(date +%Y%m%d-%H%M%S)-psp.log"
ln -sf "$LOGFILE" "$LOGDIR/psp-current.log"
# `|| true`: grep exits 1 when it filters out every line (e.g. an otherwise-empty
# LOGDIR), and under `pipefail` that would abort the whole run before it starts.
{ ls -t "$LOGDIR" 2>/dev/null | grep -v -- '-current.log$' | tail -n +40 || true; } \
    | while read -r f; do rm -f "$LOGDIR/$f"; done
exec > >(tee "$LOGFILE") 2>&1

# ── Status breadcrumb (so the run is introspectable; Phase-2 page can read it) ──
STATUS="$LOGDIR/run-status.json"
write_status() {
    printf '{"state":"%s","hours":"%s","started_epoch":%s,"updated_epoch":%s,"pid":%s,"log":"%s"}\n' \
        "$1" "${HOURS:-?}" "${STARTED:-0}" "$(date +%s)" "$$" "$LOGFILE" \
        > "$STATUS.tmp" 2>/dev/null && mv "$STATUS.tmp" "$STATUS" 2>/dev/null || true
}
STARTED=$(date +%s)
trap 'write_status stopped' EXIT
write_status starting

# ── Invariant 1: subscription compute only ──────────────────────────────────
if [[ -n "${OPENAI_API_KEY:-}" ]]; then
    echo "run_sink: OPENAI_API_KEY is set — REFUSING." >&2
    echo "  Sink tenancy is subscription-compute only (sink decision 0002): no metered" >&2
    echo "  API spend. Authenticate Codex with 'codex login' (subscription) and unset the" >&2
    echo "  key. Claude uses ~/.claude subscription OAuth. See psp-decomp-tenant.md." >&2
    exit 2
fi

# ── Parse wrapper-only flags; pass the rest through to the orchestrator ──────
ALLOW_DRIFT=false
STOCK_COMPILER=false
PUSH_BRANCH=false
HOURS=""
ORCH_ARGS=()
while [[ $# -gt 0 ]]; do
    case "$1" in
        --allow-drift)    ALLOW_DRIFT=true; shift ;;
        --stock-compiler) STOCK_COMPILER=true; shift ;;
        --push)           PUSH_BRANCH=true; shift ;;
        --hours)          HOURS="${2:-}"; ORCH_ARGS+=("--hours" "${2:-}"); shift 2 ;;
        *)                ORCH_ARGS+=("$1"); shift ;;
    esac
done

if [[ -z "$HOURS" ]]; then
    echo "run_sink: --hours N is required (e.g. --hours 8)" >&2
    exit 2
fi

echo "=== psp-autodecomp sink-mode run ==="
echo "Repo:    $REPO_DIR"
echo "User:    $(id -un) (uid $(id -u))"
echo "Hours:   $HOURS"
echo "Auth:    subscription OAuth (~/.claude, ~/.codex / keychain); OPENAI_API_KEY unset"
if [[ "$STOCK_COMPILER" == "true" ]]; then
    echo "Compiler: extern/snc (stock override)"
else
    echo "Compiler: extern/snc-read-prologue (default)"
fi
echo ""

# ── Get to a clean main so the orchestrator can branch off it ───────────────
# (Private repo: this fetch uses autodecomp's git credential helper, which reads
# the agenix PAT at /run/agenix/psp-autodecomp-github-token.)
echo "Syncing to latest main..."
if ! git checkout main 2>&1; then
    echo "run_sink: could not checkout main — resolve the tree state first." >&2
    exit 1
fi
if ! git pull --ff-only 2>&1; then
    echo "run_sink: 'git pull --ff-only' failed — main is not fast-forwardable, or the" >&2
    echo "  PAT/network is unavailable. Refusing to start on a stale/ambiguous tree." >&2
    exit 1
fi
echo ""

# ── Preflights (same guards run_overnight.sh runs, minus the sandbox plumbing) ──
if [[ "$STOCK_COMPILER" == "true" ]]; then
    export USE_STOCK_PSPCOR=1
else
    unset USE_STOCK_PSPCOR
    echo "Pre-flight: preparing the default Read-prologue compiler..."
    make prepare-read-prologue-compiler
    echo ""
fi

if [[ "$ALLOW_DRIFT" == "true" ]]; then
    echo "WARNING: --allow-drift set; skipping verify_matches pre-flight."
    echo "This run may inherit already-broken matched DB entries."
    echo ""
else
    echo "Pre-flight: verifying all matched DB entries byte-match..."
    python3 tools/verify_matches.py
    echo "Pre-flight: OK — all matched DB entries byte-verify."
    echo ""
fi

echo "Pre-flight: verifying the source tree compiles cleanly..."
python3 -c 'import sys; sys.path.insert(0, "tools"); from orchestrator import verify_tree_compiles; verify_tree_compiles()'
echo "Pre-flight: OK — all src files compile."
echo ""

# ── Auth: Claude Code may keep its OAuth in the login keychain. Unlock it
# best-effort so a headless (never-GUI-login) autodecomp session can read it.
# Harmless if auth is purely file-based (~/.claude) or no keychain exists.
# (Mirrors run_overnight.sh; subscription auth either way — never an API key.) ──
KC="$HOME/Library/Keychains/login.keychain-db"
if [[ -f "$KC" ]]; then
    security unlock-keychain -p "" "$KC" 2>/dev/null || true
    security set-keychain-settings "$KC" 2>/dev/null || true
fi

# ── Run the orchestrator (we just did the tree preflight; tell it to skip its own) ──
echo "Starting orchestrator (--hours $HOURS)..."
echo ""
write_status running
# `|| ORCH_RC=$?`: the orchestrator's nonzero exit is EXPECTED (a session that hit
# its deadline with failures still exits clean-ish) and must NOT abort us under
# `set -e` — otherwise the capture, the optional --push, and the final exit code
# below would all be skipped. The `|| ...` list suppresses `set -e` for this one.
ORCH_RC=0
SKIP_TREE_PREFLIGHT=1 python3 tools/orchestrator.py "${ORCH_ARGS[@]}" || ORCH_RC=$?

# ── Optional off-box backup of the overnight branch ─────────────────────────
if [[ "$PUSH_BRANCH" == "true" ]]; then
    BRANCH="$(git branch --show-current)"
    if [[ -n "$BRANCH" && "$BRANCH" != "main" ]]; then
        echo ""
        echo "Pushing overnight branch '$BRANCH' to origin (off-box backup + review)..."
        # Best-effort: a push failure must not fail the run — the work is already
        # committed locally on the branch for review on the box.
        git push -u origin "$BRANCH" 2>&1 || echo "run_sink: WARNING — branch push failed (work is safe locally on $BRANCH)."
    fi
fi

exit "$ORCH_RC"
