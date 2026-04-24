"""OpenAI Codex CLI backend (codex exec --json)."""

from __future__ import annotations

import json
import shlex
from typing import List

from common import CODEX, CODEX_MODEL

from .base import (
    AgentEvent,
    Backend,
    format_tool_result,
    format_tool_use,
)


def _unwrap_shell(cmd: str) -> str:
    """Strip exactly the `/bin/zsh -lc '<one command>'` wrapping Codex applies.

    Strict len==3 match: a wrapped command that still has trailing tokens
    (e.g. shell-operator chains) is left raw so nothing is hidden from
    the TUI.
    """
    try:
        tokens = shlex.split(cmd)
    except ValueError:
        return cmd
    if (len(tokens) == 3
            and (tokens[0].endswith("/zsh") or tokens[0].endswith("/bash")
                 or tokens[0].endswith("/sh") or tokens[0] == "sh")
            and tokens[1] in ("-lc", "-c")):
        return tokens[2]
    return cmd


class CodexBackend(Backend):
    name = "codex"

    def __init__(self, model: str = CODEX_MODEL, system_append: str = ""):
        super().__init__(model=model, system_append=system_append)

    def spawn_cmd(self, prompt: str, session_id: str) -> List[str]:
        # Codex has no --append-system-prompt equivalent; prepend the
        # safety addendum to the user prompt so it leads the context.
        full_prompt = (f"{self.system_append}\n\n---\n\n{prompt}"
                       if self.system_append else prompt)
        return [
            CODEX, "exec",
            "--json",
            "--skip-git-repo-check",
            "--dangerously-bypass-approvals-and-sandbox",
            "--ephemeral",
            "-m", self.model,
            full_prompt,
        ]

    def parse_line(self, raw: str) -> List[AgentEvent]:
        raw = raw.strip()
        if not raw:
            return []
        try:
            msg = json.loads(raw)
        except json.JSONDecodeError:
            return [AgentEvent(kind="raw", text=raw[:500])]

        mtype = msg.get("type")

        if mtype in ("thread.started", "turn.started", "turn.completed"):
            return []

        # Failures outside the item stream: still stream over JSONL, don't
        # raise on their own. Surface as error-flagged tool_result so
        # downstream log/TUI consumers see them as failures.
        if mtype in ("turn.failed", "error", "item.failed"):
            err_text = (msg.get("error") or msg.get("message") or raw)
            if isinstance(err_text, dict):
                err_text = err_text.get("message") or json.dumps(err_text)
            return [AgentEvent(
                kind="tool_result",
                is_error=True,
                text=f"codex {mtype}: {str(err_text)[:200]}",
            )]

        if mtype not in ("item.started", "item.completed"):
            return [AgentEvent(kind="raw", text=raw[:500])]

        item = msg.get("item") or {}
        itype = item.get("type")
        item_id = item.get("id")

        events: List[AgentEvent] = []
        if mtype == "item.started":
            if itype == "command_execution":
                display_cmd = _unwrap_shell(item.get("command") or "")
                events.append(AgentEvent(
                    kind="tool_use",
                    tool="Bash",
                    tool_use_id=item_id,
                    text=format_tool_use("Bash", {"command": display_cmd}),
                ))
            elif itype == "file_change":
                changes = item.get("changes") or []
                if not changes:
                    return events
                tool = ("Write" if all(c.get("kind") == "add" for c in changes)
                        else "Edit")
                paths = ", ".join(c.get("path", "?") for c in changes[:3])
                if len(changes) > 3:
                    paths += f" (+{len(changes) - 3} more)"
                events.append(AgentEvent(
                    kind="tool_use",
                    tool=tool,
                    tool_use_id=item_id,
                    text=f"{tool.lower()} {paths}",
                ))
            return events

        # mtype == "item.completed"
        if itype == "agent_message":
            text = (item.get("text") or "").strip()
            if text:
                events.append(AgentEvent(kind="text", text=text))
        elif itype == "command_execution":
            exit_code = item.get("exit_code")
            # Missing exit_code is treated as error — per project norm,
            # schema drift should fail loud, not silently look successful.
            is_error = exit_code != 0
            output = item.get("aggregated_output") or ""
            events.append(AgentEvent(
                kind="tool_result",
                tool_use_id=item_id,
                is_error=is_error,
                text=format_tool_result(output, is_error),
            ))
        elif itype == "file_change":
            # Pair the item.started tool_use with a completion tool_result
            # so every tool_use has a matching result in the event stream.
            changes = item.get("changes") or []
            paths = ", ".join(c.get("path", "?") for c in changes[:3])
            if len(changes) > 3:
                paths += f" (+{len(changes) - 3} more)"
            events.append(AgentEvent(
                kind="tool_result",
                tool_use_id=item_id,
                is_error=False,
                text=f"wrote {paths}" if paths else "(no changes)",
            ))
        return events
