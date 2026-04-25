"""Mutable state the running-dashboard screen derives from the event stream."""

from collections import deque
from datetime import datetime


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

        self.current_session_sid = None
        self.current_session_variant = None
        self.current_batch_names = []
        # address (lowercase hex) → short function name, populated by
        # session_start so compare_func.py 0xADDR tool_use events can
        # resolve to a name for the "on" status field.
        self.current_addr_to_name = {}
        self.current_working = "(waiting)"

        # Caps generous enough to fill tall terminals (120+ rows) without
        # unbounded memory growth. Panels clip to the visible height at
        # render time, but a bigger deque means scroll-back history is
        # available when the terminal is resized larger.
        self.orch_log = deque(maxlen=120)
        self.agent_log = deque(maxlen=120)
        # Top-of-screen narrative panel: text + thinking events. The
        # agent's "out-loud" reasoning, separated from the compact
        # tool_use/tool_result stream so verbose narrative doesn't crowd
        # the per-call log.
        self.agent_narrative_log = deque(maxlen=200)
        self.outcomes = deque(maxlen=40)

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
