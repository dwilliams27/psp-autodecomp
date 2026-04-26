"""Mutable state the running-dashboard screen derives from the event stream."""

import time
from collections import deque
from dataclasses import dataclass, field
from datetime import datetime


@dataclass
class AgentSlot:
    """Per-concurrent-session state. Each worker slot in the orchestrator
    maps to one AgentSlot in the TUI. Slots are reused: when a session
    ends, the slot retains its logs until a new session binds (so the
    tile shows the last session's trail rather than going blank).
    """
    index: int
    session_id: str | None = None
    backend: str = ""
    model: str = ""
    identity: str = ""
    queue_kind: str = ""
    current_working: str = "(idle)"
    batch_names: list = field(default_factory=list)
    addr_to_name: dict = field(default_factory=dict)
    agent_log: deque = field(default_factory=lambda: deque(maxlen=80))
    narrative_log: deque = field(default_factory=lambda: deque(maxlen=120))
    idle_since: float | None = None


class RunState:
    def __init__(self):
        self.run_start_wall = None
        self.run_deadline = None
        self.hours_total = None
        self.variants = ("base",)
        self.mode = "general"
        self.batch_size = 5
        self.session_timeout_s = 1800
        self.run_done = False
        self.backend = ""
        self.model = ""

        # Absolute matched count, seeded from the DB at startup and updated
        # by progress_tick events. Used to compute the "+N since last tick"
        # delta log line.
        self.matched_total = 0

        self.this_run_matched = {}
        self.this_run_failed = {}
        self.this_run_verify_fail = 0
        self.backends = ()
        self.this_run_matched_by_backend = {}
        self.this_run_failed_by_backend = {}

        # Per-worker agent slots (populated on run_start or first session_start)
        self.slots: list[AgentSlot] = []
        self.session_to_slot: dict[str, int] = {}
        self.ab_mode = ""
        self.worker_count = 1

        # Caps generous enough to fill tall terminals (120+ rows) without
        # unbounded memory growth. Panels clip to the visible height at
        # render time, but a bigger deque means scroll-back history is
        # available when the terminal is resized larger.
        self.orch_log = deque(maxlen=120)
        self.outcomes = deque(maxlen=40)

    # -- slot management --

    def ensure_slots(self, n):
        """Pre-allocate n slots if not already present."""
        while len(self.slots) < n:
            self.slots.append(AgentSlot(index=len(self.slots)))

    def slot_for_session(self, session_id):
        """Look up the slot bound to a session_id, or None."""
        idx = self.session_to_slot.get(session_id)
        if idx is not None and idx < len(self.slots):
            return self.slots[idx]
        return None

    def assign_session(self, session_id, backend="", model="",
                       identity="", queue_kind=""):
        """Bind a new session to an idle slot (or create one). Returns the slot."""
        # Prefer an idle slot
        for s in self.slots:
            if s.session_id is None:
                self._bind(s, session_id, backend, model, identity, queue_kind)
                return s
        # All busy — create a new slot
        s = AgentSlot(index=len(self.slots))
        self.slots.append(s)
        self._bind(s, session_id, backend, model, identity, queue_kind)
        return s

    def release_session(self, session_id):
        """Unbind a session from its slot. Logs are retained for display."""
        idx = self.session_to_slot.pop(session_id, None)
        if idx is not None and idx < len(self.slots):
            slot = self.slots[idx]
            slot.session_id = None
            slot.current_working = "(idle)"
            slot.idle_since = time.time()

    def _bind(self, slot, session_id, backend, model, identity, queue_kind):
        slot.session_id = session_id
        slot.backend = backend
        slot.model = model
        slot.identity = identity
        slot.queue_kind = queue_kind
        slot.current_working = "(waiting)"
        slot.batch_names = []
        slot.addr_to_name = {}
        slot.idle_since = None
        slot.agent_log.clear()
        slot.narrative_log.clear()
        self.session_to_slot[session_id] = slot.index

    # -- convenience accessors --
    def elapsed_s(self, now=None):
        if not self.run_start_wall:
            return 0.0
        now = now or datetime.now()
        return (now - self.run_start_wall).total_seconds()

    def time_left_s(self, now=None):
        if not self.run_deadline:
            return None
        now = now or datetime.now()
        return max(0.0, (self.run_deadline - now).total_seconds())

    def progress_frac(self, now=None):
        if not self.hours_total:
            return 0.0
        total_s = self.hours_total * 3600.0
        if total_s <= 0:
            return 0.0
        return min(1.0, self.elapsed_s(now) / total_s)

    def ensure_variant(self, variant):
        """Register a variant seen in a function_result / session_start.

        Adds missing entries to the tally dicts and appends to self.variants
        so the scoreboard renders the column even if run_start didn't list it
        (replay of an old log, or a variant picked after run_start).
        """
        if not variant:
            return
        if variant not in self.this_run_matched:
            self.this_run_matched[variant] = 0
            self.this_run_failed[variant] = 0
        if variant not in self.variants:
            self.variants = self.variants + (variant,)

    def ensure_backend(self, backend):
        """Register a backend seen mid-run for A/B tally tracking.

        Mirrors `ensure_variant` — backends listed in the run_start event
        are registered up front; this catches any stragglers.
        """
        if not backend:
            return
        if backend not in self.this_run_matched_by_backend:
            self.this_run_matched_by_backend[backend] = 0
            self.this_run_failed_by_backend[backend] = 0
        if backend not in self.backends:
            self.backends = self.backends + (backend,)
