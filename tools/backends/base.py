"""Backend abstraction for headless coding-agent CLIs driving match sessions."""

from __future__ import annotations

import json
import os
import queue
import subprocess
import threading
import time
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Callable, List, Literal, Optional, Tuple


# Each pattern MUST contain an explicit refusal verb ("refuse", "decline",
# "cannot help with", "won't"/"will not") — matching on content-adjacent
# words like "malware" alone would fire on agents reasoning *about* a
# misfire rather than actually refusing.
_DEFAULT_REFUSAL_PATTERNS: Tuple[str, ...] = (
    "refuse to improve",
    "refuse to augment",
    "refuse to modify",
    "refuse to edit",
    "refuse to help",
    "decline to improve",
    "decline to augment",
    "cannot help with modifying",
    "cannot help with editing",
    "won't be augmenting",
    "will not be augmenting",
    "will not augment",
)


_RESULT_KEYWORDS = ("match", "mismatch", "fail", "error", "permuter",
                    "diff", "verify", "byte", "segfault", "exit")


EventKind = Literal["text", "thinking", "tool_use", "tool_result", "raw",
                    "status", "usage"]


def format_tool_use(tool_name: str, tool_input) -> str:
    """One-line display string for a tool_use block.

    Backend parsers should normalize their tool names to the Claude set
    (Bash/Read/Write/Edit/Grep/Glob/Agent) before calling this so the TUI
    renders uniformly. Unknown names fall through to a generic peek.
    """
    if not isinstance(tool_input, dict):
        return f"{tool_name}"
    if tool_name == "Bash":
        cmd = str(tool_input.get("command", ""))
        return f"$ {cmd[:200]}"
    if tool_name in ("Read", "Write", "Edit", "NotebookEdit"):
        return f"{tool_name.lower()} {tool_input.get('file_path', '')}"
    if tool_name == "Grep":
        pat = str(tool_input.get("pattern", ""))[:80]
        path = tool_input.get("path", "")
        tail = f" in {path}" if path else ""
        return f"grep '{pat}'{tail}"
    if tool_name == "Glob":
        return f"glob {tool_input.get('pattern', '')}"
    if tool_name == "Agent":
        desc = tool_input.get("description", "")
        return f"agent: {desc}"[:200]
    peek = str(tool_input)[:80]
    return f"{tool_name} {peek}"


def format_tool_result(content, is_error: bool) -> str:
    """One-line signal-extraction from a tool_result block (stdout of a tool)."""
    if isinstance(content, list):
        parts = []
        for c in content:
            if isinstance(c, dict):
                parts.append(str(c.get("text", "")))
            else:
                parts.append(str(c))
        text = "\n".join(parts)
    else:
        text = str(content or "")

    lines = [ln.strip() for ln in text.splitlines() if ln.strip()]
    if not lines:
        return "(empty)"

    key_lines = [ln for ln in lines
                 if any(k in ln.lower() for k in _RESULT_KEYWORDS)]
    if key_lines:
        head = " | ".join(key_lines[:2])
        prefix = "! " if is_error else ""
        return f"{prefix}{head}"[:240]

    prefix = "! " if is_error else ""
    return f"{prefix}{lines[0]}"[:240]


@dataclass
class AgentEvent:
    kind: EventKind
    text: str = ""
    tool: Optional[str] = None
    tool_use_id: Optional[str] = None
    is_error: bool = False
    # kind == "usage" only. cost_usd=None means the backend doesn't
    # report cost natively; the base layer computes from RATE_CARD.
    input_tokens: int = 0
    output_tokens: int = 0
    cached_tokens: int = 0
    cost_usd: Optional[float] = None

    def to_log_fields(self) -> dict:
        d = {"kind": self.kind, "text": self.text[:500]}
        if self.tool is not None:
            d["tool"] = self.tool
        if self.tool_use_id is not None:
            d["tool_use_id"] = self.tool_use_id
        if self.kind == "tool_result":
            d["is_error"] = self.is_error
        return d


class AgentRefused(RuntimeError):
    """Agent produced refusal text matching a safety-classifier misfire.
    Distinct from generic failure so the outer loop can short-backoff +
    separate ceiling instead of the standard exponential.

    `session_usage` carries whatever tokens were captured before the
    refusal so the attempts.jsonl row still gets cost attribution —
    refusals shouldn't look free.
    """
    def __init__(self, reason: str, session_usage: Optional[dict] = None):
        super().__init__(f"agent refused mid-session ({reason})")
        self.reason = reason
        self.session_usage = session_usage or {}


class Backend(ABC):
    """Interface every headless CLI adapter implements."""

    name: str = "base"

    # Per-Mtok USD pricing keyed by model name. compute_cost() falls
    # back to this when the backend's stream-JSON doesn't carry cost.
    RATE_CARD: dict = {}

    def __init__(self, model: str, system_append: str):
        self.model = model
        self.system_append = system_append

    @abstractmethod
    def spawn_cmd(self, prompt: str, session_id: str) -> List[str]:
        """Build the argv for subprocess.Popen."""

    def env(self) -> dict:
        """Extra env vars to merge over os.environ for the child."""
        return {}

    @abstractmethod
    def parse_line(self, raw: str) -> List[AgentEvent]:
        """Parse one stdout line into zero or more AgentEvents.

        Returning an empty list means "nothing useful here — don't log".
        For unparseable non-empty lines, return a single kind='raw' event
        so the operator sees it in the log.
        """

    def refusal_patterns(self) -> Tuple[str, ...]:
        return _DEFAULT_REFUSAL_PATTERNS

    def is_refusal(self, event: AgentEvent) -> Optional[str]:
        if event.kind not in ("text", "thinking"):
            return None
        lt = event.text.lower()
        if any(p in lt for p in self.refusal_patterns()):
            return f"refusal_in_{event.kind}"
        return None

    def compute_cost(self, input_tokens: int, output_tokens: int,
                     cached_tokens: int) -> Optional[float]:
        """USD cost from RATE_CARD. None when the card has no entry for
        this model — `.get` is deliberately not used on the inner keys:
        a partially-populated card is a bug, not a free model.
        """
        card = self.RATE_CARD.get(self.model)
        if card is None:
            return None
        billed_in = max(0, input_tokens - cached_tokens)
        return (
            billed_in * card["input_per_mtok"]
            + cached_tokens * card["cache_read_per_mtok"]
            + output_tokens * card["output_per_mtok"]
        ) / 1_000_000


def run_session(
    backend: Backend,
    prompt: str,
    session_id: str,
    log_fn: Callable[[dict], None],
    variant: str,
    timeout: int,
    cwd: Optional[str] = None,
) -> Tuple[bool, Optional[str], dict]:
    """Spawn the backend's CLI and stream its output to log_fn.

    Returns (success, error_msg, session_usage). `session_usage` is the
    same dict shape that's logged as the `session_usage` event so the
    caller doesn't need to scrape it back from the log stream.
    Raises AgentRefused on safety-classifier misfire so the outer loop
    can treat it separately.

    `cwd` (Phase 3 shootout): when set the agent CLI runs inside that
    directory so its file edits land in the right git worktree, not
    the orchestrator's main tree. None preserves Phase 1/2 behavior.
    """
    cmd = backend.spawn_cmd(prompt, session_id)

    child_env = os.environ.copy()
    for k, v in backend.env().items():
        child_env[k] = v

    session_usage: dict = {}

    try:
        proc = subprocess.Popen(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            stdin=subprocess.DEVNULL,
            text=True, bufsize=1, env=child_env,
            cwd=cwd,
        )
    except OSError as e:
        return False, f"failed to spawn {backend.name}: {e}", session_usage

    q: queue.Queue = queue.Queue()

    def _reader(stream, tag):
        try:
            for line in stream:
                q.put((tag, line))
        finally:
            q.put((tag, None))

    t_out = threading.Thread(target=_reader, args=(proc.stdout, "out"),
                             daemon=True)
    t_err = threading.Thread(target=_reader, args=(proc.stderr, "err"),
                             daemon=True)
    t_out.start()
    t_err.start()

    start = time.time()
    eof_count = 0
    stderr_tail: List[str] = []

    usage_in = 0
    usage_out = 0
    usage_cached = 0
    usage_cost = 0.0
    usage_cost_native = False
    saw_usage = False

    def emit_session_usage():
        if saw_usage:
            cost = usage_cost if usage_cost_native else backend.compute_cost(
                usage_in, usage_out, usage_cached)
        else:
            cost = None
        # Loud signal for stale/missing rate cards: we have token data
        # but no cost — A/B reporting needs to know that null is
        # "rate card miss," not "free session."
        if saw_usage and cost is None:
            log_fn({
                "event": "rate_card_missing",
                "session_id": session_id,
                "variant": variant,
                "backend": backend.name,
                "model": backend.model,
            })
        session_usage.update({
            "event": "session_usage",
            "session_id": session_id,
            "variant": variant,
            "backend": backend.name,
            "model": backend.model,
            "input_tokens": usage_in,
            "output_tokens": usage_out,
            "cached_tokens": usage_cached,
            "cost_usd": cost,
            "had_usage_data": saw_usage,
        })
        log_fn(dict(session_usage))

    try:
        while eof_count < 2:
            elapsed = time.time() - start
            remaining = timeout - elapsed
            if remaining <= 0:
                proc.kill()
                return False, f"session timed out after {timeout}s", session_usage
            try:
                tag, line = q.get(timeout=min(remaining, 1.0))
            except queue.Empty:
                if proc.poll() is not None and q.empty():
                    break
                continue
            if line is None:
                eof_count += 1
                continue
            if tag == "out":
                events = backend.parse_line(line)
                for ev in events:
                    if ev.kind == "usage":
                        usage_in += ev.input_tokens
                        usage_out += ev.output_tokens
                        usage_cached += ev.cached_tokens
                        if ev.cost_usd is not None:
                            usage_cost += ev.cost_usd
                            usage_cost_native = True
                        saw_usage = True
                        continue
                    fields = ev.to_log_fields()
                    fields["event"] = "agent_event"
                    fields["session_id"] = session_id
                    fields["variant"] = variant
                    fields["backend"] = backend.name
                    log_fn(fields)
                    reason = backend.is_refusal(ev)
                    if reason:
                        proc.kill()
                        log_fn({
                            "event": "agent_refused",
                            "session_id": session_id,
                            "variant": variant,
                            "backend": backend.name,
                            "reason": reason,
                        })
                        raise AgentRefused(reason, session_usage=session_usage)
            elif tag == "err":
                stderr_tail.append(line)
                if len(stderr_tail) > 20:
                    stderr_tail = stderr_tail[-20:]

        try:
            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()

        if proc.returncode != 0:
            # Surface the full tail (capped at 20 lines above) so
            # silent-exit bugs are diagnosable from the run log.
            # "(no stderr)" itself is signal — child wrote nothing.
            err = ("".join(stderr_tail).strip()
                   if stderr_tail else "(no stderr)")
            return False, f"{backend.name} exited {proc.returncode}: {err}", session_usage

        if not saw_usage:
            # Loud warning, not a hard failure. The agent exited 0,
            # which means its work landed (session_results.json, src
            # edits) — failing the session here would throw away a
            # successful match because of an instrumentation gap.
            # attempts.jsonl will carry None for cost and
            # had_usage_data=False, so ab_report can surface
            # "X% of sessions have no cost" as a quality signal
            # without losing the match.
            log_fn({
                "event": "usage_missing",
                "session_id": session_id,
                "variant": variant,
                "backend": backend.name,
                "model": backend.model,
                "warning": (
                    f"{backend.name} exited 0 but emitted no usage "
                    f"events. Stream-JSON shape may have drifted "
                    f"(expected `result` for claude, "
                    f"`turn.completed`/`token_count` for codex). "
                    f"Match (if any) is preserved; cost attribution "
                    f"is None for this session."
                ),
            })

        return True, None, session_usage
    finally:
        emit_session_usage()
