"""Claude Code backend (claude CLI in headless stream-json mode)."""

from __future__ import annotations

import json
from typing import List

from common import CLAUDE, CLAUDE_MODEL

from .base import (
    AgentEvent,
    Backend,
    format_tool_result,
    format_tool_use,
)


class ClaudeBackend(Backend):
    name = "claude"

    def __init__(self, model: str = CLAUDE_MODEL, system_append: str = ""):
        super().__init__(model=model, system_append=system_append)

    def spawn_cmd(self, prompt: str, session_id: str) -> List[str]:
        return [
            CLAUDE, "-p", prompt,
            "--model", self.model,
            "--dangerously-skip-permissions",
            "--append-system-prompt", self.system_append,
            "--output-format", "stream-json",
            "--verbose",
        ]

    def parse_line(self, raw: str) -> List[AgentEvent]:
        raw = raw.strip()
        if not raw:
            return []
        try:
            msg = json.loads(raw)
        except json.JSONDecodeError:
            return [AgentEvent(kind="raw", text=raw)]

        events: List[AgentEvent] = []
        mtype = msg.get("type")
        if mtype == "assistant":
            inner = msg.get("message", {}) or {}
            for block in inner.get("content", []) or []:
                btype = block.get("type")
                if btype == "text":
                    text = (block.get("text") or "").strip()
                    if text:
                        events.append(AgentEvent(kind="text", text=text))
                elif btype == "thinking":
                    text = (block.get("thinking") or "").strip()
                    if text:
                        events.append(AgentEvent(kind="thinking", text=text))
                elif btype == "tool_use":
                    tool = block.get("name") or "?"
                    inp = block.get("input") or {}
                    events.append(AgentEvent(
                        kind="tool_use",
                        tool=tool,
                        tool_use_id=block.get("id"),
                        text=format_tool_use(tool, inp),
                    ))
        elif mtype == "user":
            inner = msg.get("message", {}) or {}
            content = inner.get("content", []) or []
            if isinstance(content, list):
                for block in content:
                    if block.get("type") == "tool_result":
                        is_error = bool(block.get("is_error", False))
                        text = format_tool_result(block.get("content"), is_error)
                        events.append(AgentEvent(
                            kind="tool_result",
                            tool_use_id=block.get("tool_use_id"),
                            is_error=is_error,
                            text=text,
                        ))
        return events
