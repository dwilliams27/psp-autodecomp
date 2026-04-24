"""Backend registry for the orchestrator."""

from __future__ import annotations

from common import CLAUDE_MODEL, CODEX_MODEL

from .base import (
    AgentEvent,
    AgentRefused,
    Backend,
    run_session,
)
from .claude import ClaudeBackend
from .codex import CodexBackend


_REGISTRY: dict[str, type[Backend]] = {
    "claude": ClaudeBackend,
    "codex": CodexBackend,
}

_DEFAULT_MODELS: dict[str, str] = {
    "claude": CLAUDE_MODEL,
    "codex": CODEX_MODEL,
}

AVAILABLE_BACKENDS = sorted(_REGISTRY)


def get_backend(name: str, system_append: str = "", model: str = "") -> Backend:
    """Instantiate the named backend. Raises ValueError for unknown names."""
    key = name.lower()
    ctor = _REGISTRY.get(key)
    if ctor is None:
        raise ValueError(
            f"Unknown backend '{name}'. Supported: {', '.join(AVAILABLE_BACKENDS)}"
        )
    return ctor(model=model or _DEFAULT_MODELS[key], system_append=system_append)


__all__ = [
    "AgentEvent",
    "AgentRefused",
    "Backend",
    "ClaudeBackend",
    "CodexBackend",
    "get_backend",
    "run_session",
    "AVAILABLE_BACKENDS",
]
