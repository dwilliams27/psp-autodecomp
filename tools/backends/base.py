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


EventKind = Literal["text", "thinking", "tool_use", "tool_result", "raw", "status"]


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
    """
    def __init__(self, reason: str):
        super().__init__(f"agent refused mid-session ({reason})")
        self.reason = reason


class Backend(ABC):
    """Interface every headless CLI adapter implements."""

    name: str = "base"

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


def run_session(
    backend: Backend,
    prompt: str,
    session_id: str,
    log_fn: Callable[[dict], None],
    variant: str,
    timeout: int,
) -> Tuple[bool, Optional[str]]:
    """Spawn the backend's CLI and stream its output to log_fn.

    Returns (success, error_msg). Raises AgentRefused on safety-classifier
    misfire so the outer loop can treat it separately.
    """
    cmd = backend.spawn_cmd(prompt, session_id)

    child_env = os.environ.copy()
    for k, v in backend.env().items():
        child_env[k] = v

    try:
        proc = subprocess.Popen(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            stdin=subprocess.DEVNULL,
            text=True, bufsize=1, env=child_env,
        )
    except OSError as e:
        return False, f"failed to spawn {backend.name}: {e}"

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

    while eof_count < 2:
        elapsed = time.time() - start
        remaining = timeout - elapsed
        if remaining <= 0:
            proc.kill()
            return False, f"session timed out after {timeout}s"
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
                    raise AgentRefused(reason)
        elif tag == "err":
            stderr_tail.append(line)
            if len(stderr_tail) > 20:
                stderr_tail = stderr_tail[-20:]

    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        proc.kill()

    if proc.returncode != 0:
        # Surface the full tail (capped above at 20 lines) so silent-exit
        # bugs are diagnosable from the run log alone. "(no stderr)" is
        # itself a useful signal — narrows the failure to "child did not
        # write to its stderr" rather than "we lost the message".
        err = ("".join(stderr_tail).strip()
               if stderr_tail else "(no stderr)")
        return False, f"{backend.name} exited {proc.returncode}: {err}"

    return True, None
