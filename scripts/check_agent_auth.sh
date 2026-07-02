#!/usr/bin/env bash
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SANDBOX_USER="autodecomp"

# Optional backend filter: probe one harness or both (default). Lets a
# codex-first bring-up pass without a Claude login yet, and vice versa.
BACKEND="${1:-all}"
case "$BACKEND" in
  claude|codex|all) ;;
  *) echo "usage: check_agent_auth.sh [claude|codex|all]" >&2; exit 2 ;;
esac

# This script may run EITHER as the operator (who can sudo into the sandbox
# user) OR directly as the sandbox user (e.g. via the mini's \`psp\` helper —
# autodecomp's home is 0750, unreadable to the operator, and autodecomp
# itself has no sudo). Pick the hop accordingly.
if [ "$(whoami)" = "$SANDBOX_USER" ]; then
  AS_SANDBOX=(env)
  AS_SANDBOX_CODEX=(env)
else
  AS_SANDBOX=(sudo -i -u "$SANDBOX_USER" env)
  AS_SANDBOX_CODEX=(sudo --preserve-env=OPENAI_API_KEY -i -u "$SANDBOX_USER" env)
fi

TMP_DIR="$(mktemp -d "/private/tmp/agent-auth.XXXXXX")"
trap 'rm -rf "$TMP_DIR"' EXIT
chmod 755 "$TMP_DIR"

CLAUDE_PROBE="$TMP_DIR/claude_probe.sh"
CODEX_PROBE="$TMP_DIR/codex_probe.sh"

cat >"$CLAUDE_PROBE" <<'EOF'
#!/usr/bin/env bash
set -euo pipefail

cd "$REPO_DIR"
echo "probe-user=$(whoami)"
echo "probe-home=$HOME"
echo "probe-repo=$REPO_DIR"
echo "claude-path=$(command -v /usr/local/bin/claude)"
/usr/local/bin/claude --version
# FALLBACK-OK: the following model call is the real auth check; keychain
# unlock can be a no-op when already unlocked or when API-key auth is used.
security unlock-keychain -p "" "/Users/$SANDBOX_USER/Library/Keychains/login.keychain-db" >/dev/null 2>&1 || true
/usr/local/bin/claude -p "Reply with exactly OK." \
  --model claude-opus-4-8 \
  --dangerously-skip-permissions \
  --output-format json \
  --verbose \
  --no-session-persistence
EOF

cat >"$CODEX_PROBE" <<'EOF'
#!/usr/bin/env bash
set -euo pipefail

cd "$REPO_DIR"
echo "probe-user=$(whoami)"
echo "probe-home=$HOME"
echo "probe-repo=$REPO_DIR"
echo "codex-path=$(command -v codex)"
codex --version
test -r "$HOME/.codex/auth.json" && echo "codex-auth-file=present" || echo "codex-auth-file=missing"
codex exec --json \
  --skip-git-repo-check \
  --dangerously-bypass-approvals-and-sandbox \
  --ephemeral \
  -m gpt-5.5 \
  -c model_reasoning_effort=high \
  "Reply with exactly OK."
EOF

chmod 755 "$CLAUDE_PROBE" "$CODEX_PROBE"

validate_probe_output() {
  local name="$1"
  local logfile="$2"
  python3 - "$name" "$logfile" <<'PY'
import json
import sys
from pathlib import Path

name = sys.argv[1]
text = Path(sys.argv[2]).read_text(errors="replace")


def as_text(value):
    if isinstance(value, str):
        return value
    if isinstance(value, dict):
        parts = []
        for key in ("text", "output_text", "result"):
            if isinstance(value.get(key), str):
                parts.append(value[key])
        if "content" in value:
            nested = as_text(value["content"])
            if nested:
                parts.append(nested)
        return "\n".join(parts)
    if isinstance(value, list):
        return "\n".join(part for item in value for part in [as_text(item)] if part)
    return ""


def is_exact_ok(value):
    return value.strip() == "OK"


def iter_json_objects(text):
    for line in text.splitlines():
        line = line.strip()
        if not line or line[0] not in "{[":
            continue
        try:
            payload = json.loads(line)
        except json.JSONDecodeError:
            # FALLBACK-OK: probe logs intentionally mix diagnostics with JSON.
            continue
        if isinstance(payload, list):
            for item in payload:
                if isinstance(item, dict):
                    yield item
        elif isinstance(payload, dict):
            yield payload


for obj in iter_json_objects(text):
    if name == "Claude":
        result = obj.get("result")
        if isinstance(result, str) and is_exact_ok(result):
            sys.exit(0)
        # FALLBACK-OK: older Claude stream-json versions expose assistant text
        # inside message.content rather than a top-level result field.
        if obj.get("type") == "assistant" and is_exact_ok(as_text(obj.get("message", {}))):
            sys.exit(0)

    if name == "Codex":
        item = obj.get("item") if isinstance(obj.get("item"), dict) else {}
        role = item.get("role") or obj.get("role")
        if role == "assistant" and is_exact_ok(as_text(item or obj)):
            sys.exit(0)
        if obj.get("type") in {"response.output_text.delta", "response.completed"}:
            if is_exact_ok(as_text(obj)):
                sys.exit(0)

sys.exit(1)
PY
}

run_probe() {
  local name="$1"
  local logfile="$2"
  shift 2

  echo "== $name auth/responsiveness =="
  if "$@" >"$logfile" 2>&1; then
    if validate_probe_output "$name" "$logfile"; then
      echo "PASS: $name returned OK"
    else
      echo "FAIL: $name exited successfully but did not return OK"
      echo "--- captured output ---"
      tail -n 80 "$logfile"
      return 1
    fi
  else
    local rc=$?
    echo "FAIL: $name command exited with status $rc"
    echo "--- captured output ---"
    tail -n 80 "$logfile"
    return "$rc"
  fi
  echo
}

if [ "$BACKEND" != "codex" ]; then
  run_probe "Claude" "$TMP_DIR/claude.log" \
    "${AS_SANDBOX[@]}" REPO_DIR="$REPO_DIR" SANDBOX_USER="$SANDBOX_USER" "$CLAUDE_PROBE"
fi

if [ "$BACKEND" != "claude" ]; then
  run_probe "Codex" "$TMP_DIR/codex.log" \
    "${AS_SANDBOX_CODEX[@]}" REPO_DIR="$REPO_DIR" SANDBOX_USER="$SANDBOX_USER" "$CODEX_PROBE"
fi

case "$BACKEND" in
  all) echo "Both agent auth probes passed." ;;
  *)   echo "$BACKEND auth probe passed." ;;
esac
