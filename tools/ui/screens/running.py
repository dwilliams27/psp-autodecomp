"""Running-overnight dashboard. Port of launcher_demos/v11 with real data.

Phase 4: multi-agent TUI — each concurrent worker gets its own tile.
"""

import re
from datetime import datetime

from rich.align import Align
from rich.box import HEAVY, ROUNDED
from rich.console import Group
from rich.layout import Layout
from rich.panel import Panel
from rich.table import Table
from rich.text import Text

from tools.ui.palette import (BAD, BODY, DIM, FOREST, LEAF, MOSS, OK, SUN,
                              SUNLIT, WARN, identity_style, wilson_ci)
from tools.ui.screens.base import Screen
from tools.ui.widgets import helix
from tools.ui.widgets.header import header_panel
from tools.ui.widgets.primitives import fmt_hhmm, progress_bar, size_cell


# Backend → color/style map for the outcomes panel + scoreboard. Claude
# wears SUN orange, codex wears FOREST dark green, anything else falls
# back to MOSS so unknown backends are still legible.
_BACKEND_STYLE = {
    "claude": SUN,
    "codex": FOREST,
}


def _backend_style(name):
    return _BACKEND_STYLE.get(name, MOSS)


# Patterns in orch_note lines that duplicate a structured event we already
# show more cleanly — these are dropped from the orchestrator panel so the
# same line doesn't appear twice.
_ORCH_NOTE_SUPPRESS = (
    re.compile(r"^Session [0-9a-f]{8}"),  # session_start / session_done / session_error
    re.compile(r"^Progress: "),             # progress_tick
    re.compile(r"^  matched:"),              # function_result
    re.compile(r"^  failed:"),               # function_result
    re.compile(r"^  VERIFY FAILED:"),        # verify_failed
    re.compile(r"^  Orchestrator verification"),  # summary line
)

# compare_func.py src/<file>.cpp 0x<addr> — used to infer which function
# the agent is currently working on from its Bash tool_use events.
_COMPARE_RE = re.compile(r"compare_func\.py\s+\S+\s+(0x[0-9a-fA-F]+)")

# Maximum tiles rendered in the grid. Overflow slots get a compact strip.
_MAX_TILES = 4


def _short_name(full_name):
    """Strip the argument list off `Class::Method(args...)` for compact display."""
    return full_name.split("(", 1)[0]


def _styled_narrative(log_deque):
    """Build a Rich Text from a narrative_log deque (text + thinking lines)."""
    t = Text()
    lines = list(log_deque)
    last = len(lines) - 1
    for i, line in enumerate(lines):
        is_latest = (i == last)
        if line.startswith("~ "):
            style = f"italic {LEAF}" if is_latest else f"italic {DIM}"
        else:
            style = BODY if is_latest else DIM
        suffix = "" if is_latest else "\n"
        t.append(line + suffix, style=style)
    return t


def _styled_toollog(log_deque):
    """Build a Rich Text from an agent_log deque (tool_use + tool_result lines)."""
    t = Text()
    lines = list(log_deque)
    last = len(lines) - 1
    for i, line in enumerate(lines):
        is_latest = (i == last)
        if line.startswith("$"):
            style = SUNLIT if is_latest else SUN
        elif line.lstrip().startswith("\u2192"):
            lower = line.lower()
            if "mismatch" in lower or "fail" in lower or "error" in lower:
                style = BAD if is_latest else f"dim {BAD}"
            elif "match" in lower:
                style = OK if is_latest else f"dim {OK}"
            else:
                style = WARN if is_latest else DIM
        elif line.startswith("~"):
            style = f"italic {LEAF}" if is_latest else f"italic {DIM}"
        elif line.startswith("read ") or line.startswith("grep "):
            style = LEAF if is_latest else DIM
        else:
            style = BODY if is_latest else DIM
        suffix = "" if is_latest else "\n"
        t.append(line + suffix, style=style)
    return t


class RunningScreen(Screen):
    name = "running"

    # ------------------------------------------------------------------
    # event handling
    # ------------------------------------------------------------------

    def on_event(self, app, event):
        ev = event.get("event")
        state = app.state

        if ev == "run_start":
            self._on_run_start(state, event)
        elif ev == "run_done":
            state.run_done = True
            state.orch_log.append(_ts()
                + f"RUN DONE · attempted={event.get('attempted', 0)} "
                + f"matched={event.get('matched', 0)} "
                + f"failed={event.get('failed', 0)}")
        elif ev == "session_start":
            self._on_session_start(state, event)
        elif ev == "session_done":
            self._on_session_done(state, event)
        elif ev == "function_result":
            self._on_function_result(state, event)
        elif ev == "verify_failed":
            self._on_verify_failed(state, event)
        elif ev == "session_error":
            sid = event.get("session_id", "?")
            state.orch_log.append(_ts()
                + f"session {sid} ERROR · "
                + f"{event.get('error','')[:100]}")
            state.release_session(sid)
        elif ev == "progress_tick":
            self._on_progress_tick(state, event)
        elif ev == "orch_note":
            self._on_orch_note(state, event)
        elif ev == "agent_event":
            self._on_agent_event(state, event)
        elif ev == "backend_dead":
            backend = event.get("backend", "?")
            reason = event.get("reason", "?")
            state.orch_log.append(
                _ts() + f"BACKEND DEAD: {backend} ({reason})")
        elif ev == "malformed":
            state.orch_log.append(
                _ts() + "MALFORMED LOG LINE: " + (event.get("raw") or "")[:80]
            )

    def _on_run_start(self, state, event):
        state.hours_total = event.get("hours")
        if event.get("start_time"):
            state.run_start_wall = datetime.fromisoformat(event["start_time"])
        if event.get("deadline"):
            state.run_deadline = datetime.fromisoformat(event["deadline"])
        state.variants = tuple(event.get("variants", ("base",)))
        state.mode = event.get("mode", "general")
        state.batch_size = event.get("batch_size", 5)
        state.session_timeout_s = event.get("session_timeout", 1800)
        state.backend = event.get("backend") or ""
        state.model = event.get("model") or ""
        state.ab_mode = event.get("ab_mode") or ""
        state.worker_count = event.get("workers", 1)
        # Pre-allocate slots from the worker count
        state.ensure_slots(state.worker_count)
        for b in event.get("backends") or []:
            state.ensure_backend(b.get("name", ""))
        if not state.backends and state.backend:
            state.ensure_backend(state.backend)
        if len(state.backends) > 1:
            state.backend = "/".join(state.backends)
            state.model = ""
        for v in state.variants:
            state.ensure_variant(v)
        state.orch_log.append(
            _ts()
            + f"run started · {state.hours_total}h · variants={','.join(state.variants)}"
            + f" · batch={state.batch_size}"
        )

    def _on_session_start(self, state, event):
        sid = event.get("session_id", "?")
        variant = event.get("variant", "?")
        backend = event.get("backend") or ""
        model = event.get("model") or ""
        identity = event.get("identity") or ""
        queue_kind = event.get("queue_kind") or ""
        funcs = event.get("functions", []) or []
        state.ensure_variant(variant)
        if backend:
            state.ensure_backend(backend)
        # Ensure at least 1 slot exists (old logs without run_start.workers)
        if not state.slots:
            state.ensure_slots(1)
        slot = state.assign_session(sid, backend, model, identity, queue_kind)
        slot.batch_names = [_short_name(f.get("name", "?")) for f in funcs]
        slot.addr_to_name = {
            (f.get("address") or "").lower():
                _short_name(f.get("name") or "?")
            for f in funcs if f.get("address")
        }
        slot.current_working = (
            slot.batch_names[0] if slot.batch_names else "(waiting)"
        )
        state.orch_log.append(
            _ts()
            + f"session {sid} [{backend or variant}] started · batch={len(funcs)}"
        )

    def _on_session_done(self, state, event):
        sid = event.get("session_id", "?")
        m = event.get("matched", 0)
        f = event.get("failed", 0)
        dur_s = event.get("duration_s", 0)
        mm = int(dur_s // 60)
        ss = int(dur_s % 60)
        state.orch_log.append(
            _ts() + f"session {sid} done · {m} matched, {f} failed · {mm}m{ss:02d}s"
        )
        for o in state.outcomes:
            if o.get("session_id") == sid and o.get("duration_s") is None:
                o["duration_s"] = dur_s
        state.release_session(sid)

    def _on_function_result(self, state, event):
        status = event.get("status")
        variant = event.get("variant", "?")
        backend = event.get("backend") or ""
        state.ensure_variant(variant)
        if backend:
            state.ensure_backend(backend)
        outcome = {
            "status": status,
            "name": event.get("name", "?"),
            "size": event.get("size", 0),
            "variant": variant,
            "backend": backend,
            "session_id": event.get("session_id"),
            "duration_s": None,
        }
        state.outcomes.appendleft(outcome)

        short = _short_name(event.get("name", "?"))
        if status == "matched":
            state.this_run_matched[variant] = state.this_run_matched.get(variant, 0) + 1
            if backend:
                state.this_run_matched_by_backend[backend] = (
                    state.this_run_matched_by_backend.get(backend, 0) + 1
                )
            state.orch_log.append(_ts() + f"  matched: {short}")
        elif status == "failed":
            state.this_run_failed[variant] = state.this_run_failed.get(variant, 0) + 1
            if backend:
                state.this_run_failed_by_backend[backend] = (
                    state.this_run_failed_by_backend.get(backend, 0) + 1
                )
            state.orch_log.append(_ts() + f"  failed:  {short}")

    def _on_verify_failed(self, state, event):
        variant = event.get("variant", "?")
        backend = event.get("backend") or ""
        state.ensure_variant(variant)
        if backend:
            state.ensure_backend(backend)
        state.this_run_verify_fail += 1
        state.outcomes.appendleft({
            "status": "verify",
            "name": event.get("name", "?"),
            "size": event.get("size", 0),
            "variant": variant,
            "backend": backend,
            "session_id": event.get("session_id"),
            "duration_s": None,
        })
        state.orch_log.append(
            _ts()
            + f"  verify_failed: {_short_name(event.get('name','?'))} · reverted"
        )

    def _on_progress_tick(self, state, event):
        prev = state.matched_total
        state.matched_total = event.get("matched_total", state.matched_total)
        delta = state.matched_total - prev
        if prev > 0 and delta > 0:
            state.orch_log.append(
                _ts() + f"(tick) {prev}\u2192{state.matched_total} matched  (+{delta})"
            )

    def _on_orch_note(self, state, event):
        text = (event.get("text") or "").strip()
        if not text:
            return
        if any(p.search(text) for p in _ORCH_NOTE_SUPPRESS):
            return
        state.orch_log.append(_ts() + text[:100])

    def _on_agent_event(self, state, event):
        sid = event.get("session_id")
        slot = state.slot_for_session(sid) if sid else None
        if not slot:
            return

        kind = event.get("kind", "?")
        text = (event.get("text") or "").strip()
        if not text:
            return
        if kind == "tool_use":
            m = _COMPARE_RE.search(text)
            if m:
                addr = m.group(1).lower()
                name = slot.addr_to_name.get(addr)
                slot.current_working = name or f"@{addr}"

        if kind in ("text", "thinking"):
            prefix = "~ " if kind == "thinking" else ""
            line = prefix + text
            if len(line) > 400:
                line = line[:397] + "..."
            slot.narrative_log.append(line)
            return

        if kind == "tool_use":
            line = text
        elif kind == "tool_result":
            body = text.lstrip()
            if body.startswith("\u2192"):
                body = body[1:].lstrip()
            line = f"  \u2192 {body}"
        elif kind == "raw":
            line = f"(raw) {text}"
        else:
            line = text
        if len(line) > 120:
            line = line[:117] + "..."
        slot.agent_log.append(line)

    # ------------------------------------------------------------------
    # rendering
    # ------------------------------------------------------------------

    def render(self, app, console):
        state = app.state
        n_slots = len(state.slots)

        layout = Layout()
        if n_slots <= 1:
            return self._render_single(app, console, layout)
        return self._render_multi(app, console, layout, n_slots)

    def _render_single(self, app, console, layout):
        """N=1 layout — identical to the pre-Phase-4 layout."""
        state = app.state
        slot = state.slots[0] if state.slots else None

        layout.split_column(
            Layout(name="header", size=7),
            Layout(name="status", size=9),
            Layout(name="mid", ratio=1),
            Layout(name="outcomes", size=12),
        )
        layout["mid"].split_column(
            Layout(name="narrative", ratio=1),
            Layout(name="bottom", ratio=1),
        )
        layout["mid"]["bottom"].split_row(
            Layout(name="orchestrator", ratio=1),
            Layout(name="agent", ratio=1),
        )
        status_inner = max(40, console.width - 4)
        layout["header"].update(header_panel("autodecomp"))
        layout["status"].update(self._status_panel(app, status_inner))
        layout["mid"]["narrative"].update(self._narrative_panel(slot))
        layout["mid"]["bottom"]["orchestrator"].update(self._orch_panel(app))
        layout["mid"]["bottom"]["agent"].update(self._agent_panel(slot))
        layout["outcomes"].update(self._outcomes_panel(app))
        return layout

    def _render_multi(self, app, console, layout, n_slots):
        """N>1 layout — tiled agent slots above shared orch + outcomes."""
        state = app.state
        tile_slots = state.slots[:_MAX_TILES]
        overflow_slots = state.slots[_MAX_TILES:]
        n_tiles = len(tile_slots)

        layout.split_column(
            Layout(name="header", size=7),
            Layout(name="status", size=9),
            Layout(name="tiles_zone", ratio=1),
            Layout(name="orchestrator", size=8),
            Layout(name="outcomes", size=12),
        )

        # Build tile rows
        if n_tiles <= 3:
            # Single row of tiles
            tile_row = Layout(name="tile_row")
            parts = []
            for i, slot in enumerate(tile_slots):
                l = Layout(name=f"tile{i}", ratio=1)
                l.update(self._render_tile(slot))
                parts.append(l)
            tile_row.split_row(*parts)

            if overflow_slots:
                layout["tiles_zone"].split_column(
                    tile_row,
                    Layout(name="overflow", size=len(overflow_slots) + 2),
                )
                layout["tiles_zone"]["overflow"].update(
                    self._overflow_strip(overflow_slots))
            else:
                layout["tiles_zone"].update(tile_row)
        else:
            # 2x2 grid
            row0 = Layout(name="tile_row0")
            r0_parts = []
            for i in range(min(2, n_tiles)):
                l = Layout(name=f"tile{i}", ratio=1)
                l.update(self._render_tile(tile_slots[i]))
                r0_parts.append(l)
            row0.split_row(*r0_parts)

            row1 = Layout(name="tile_row1")
            r1_parts = []
            for i in range(2, n_tiles):
                l = Layout(name=f"tile{i}", ratio=1)
                l.update(self._render_tile(tile_slots[i]))
                r1_parts.append(l)
            row1.split_row(*r1_parts)

            if overflow_slots:
                layout["tiles_zone"].split_column(
                    row0, row1,
                    Layout(name="overflow", size=len(overflow_slots) + 2),
                )
                layout["tiles_zone"]["overflow"].update(
                    self._overflow_strip(overflow_slots))
            else:
                layout["tiles_zone"].split_column(row0, row1)

        status_inner = max(40, console.width - 4)
        layout["header"].update(header_panel("autodecomp"))
        layout["status"].update(self._status_panel(app, status_inner))
        layout["orchestrator"].update(self._orch_panel(app))
        layout["outcomes"].update(self._outcomes_panel(app))
        return layout

    def _render_tile(self, slot):
        """Render a single agent slot as a bordered panel with narrative + tools."""
        border, label_sty, dim_sty = identity_style(slot.backend, slot.model)
        tag = slot.identity or slot.backend or f"slot-{slot.index}"

        narr_text = (_styled_narrative(slot.narrative_log)
                     if slot.narrative_log else Text("(waiting)", style=DIM))
        tool_text = (_styled_toollog(slot.agent_log)
                     if slot.agent_log else Text("(waiting)", style=DIM))

        # Compose the two halves
        inner = Layout()
        inner.split_column(
            Layout(name="narr", ratio=1),
            Layout(name="tools", ratio=1),
        )
        inner["narr"].update(Align(narr_text, vertical="bottom"))
        inner["tools"].update(Align(tool_text, vertical="bottom"))

        w = slot.current_working or "(idle)"
        if len(w) > 30:
            w = w[:27] + "..."
        subtitle = Text(w, style=DIM) if w != "(idle)" else None

        return Panel(
            inner,
            border_style=border,
            box=ROUNDED,
            title=Text(f" {tag} ", style=label_sty),
            title_align="left",
            subtitle=subtitle,
            subtitle_align="left",
            padding=(0, 1),
        )

    def _overflow_strip(self, slots):
        """Render overflow slots (index >= MAX_TILES) as compact one-liners."""
        t = Text()
        for i, slot in enumerate(slots):
            if i > 0:
                t.append("  \u2502  ", style=DIM)
            _, label_sty, _ = identity_style(slot.backend, slot.model)
            tag = slot.identity or slot.backend or f"A{slot.index}"
            t.append(f"[{tag}]", style=label_sty)
            w = slot.current_working or "(idle)"
            if len(w) > 20:
                w = w[:17] + "..."
            t.append(f" on: {w}", style=BODY if w != "(idle)" else DIM)
        return Panel(t, border_style=DIM, box=ROUNDED,
                     title=Text(f" +{len(slots)} more ", style=DIM),
                     title_align="left", padding=(0, 1))

    # -- shared panels (used by both single and multi layouts) --

    def _narrative_panel(self, slot):
        """Full-width narrative panel for N=1 layout."""
        if not slot or not slot.narrative_log:
            content = Text("(waiting for agent activity)", style=DIM)
        else:
            content = _styled_narrative(slot.narrative_log)
        return Panel(
            Align(content, vertical="bottom"),
            border_style=MOSS, box=ROUNDED,
            title=Text(" agent activity ", style=f"bold {LEAF}"),
            title_align="left", padding=(0, 1),
        )

    def _status_panel(self, app, panel_inner_width):
        state = app.state
        now = datetime.now()
        time_left = state.time_left_s(now)
        frac = state.progress_frac(now)

        matched_run = sum(state.this_run_matched.values())
        failed_run = sum(state.this_run_failed.values())
        n_slots = len(state.slots)

        left_lines = []
        l = Text()
        l.append("time left  ", style=LEAF)
        if time_left is None:
            l.append("--", style=DIM)
        else:
            l.append(fmt_hhmm(time_left), style=f"bold {BODY}")
            if state.hours_total:
                l.append(f" / {state.hours_total:g}h", style=DIM)
        left_lines.append(l)

        l = Text()
        l.append(progress_bar(frac), style=SUN)
        l.append(f"  {int(frac * 100)}%", style=DIM)
        left_lines.append(l)

        l = Text()
        l.append("this run   ", style=LEAF)
        l.append(f"+{matched_run}m", style=f"bold {OK}")
        l.append(" \u00b7 ", style=DIM)
        l.append(f"+{failed_run}f", style=f"bold {BAD}")
        if state.this_run_verify_fail:
            l.append(" \u00b7 ", style=DIM)
            l.append(f"{state.this_run_verify_fail}v", style=WARN)
        left_lines.append(l)

        if n_slots > 1:
            # Multi-agent: show mode + workers instead of now/on
            l = Text()
            l.append("mode       ", style=LEAF)
            mode_label = {
                "disjoint": "A\u00b7disjoint",
                "shootout": "B\u00b7shootout",
                "hybrid": "C\u00b7hybrid",
            }.get(state.ab_mode, state.ab_mode or "single")
            l.append(mode_label, style=BODY)
            left_lines.append(l)

            l = Text()
            l.append("workers    ", style=LEAF)
            active = sum(1 for s in state.slots if s.session_id is not None)
            l.append(f"{active}/{n_slots} active", style=BODY if active else DIM)
            left_lines.append(l)
        else:
            # N=1: show now/on as before
            slot0 = state.slots[0] if state.slots else None
            l = Text()
            l.append("now        ", style=LEAF)
            if slot0 and slot0.backend and slot0.session_id:
                l.append(slot0.backend,
                         style=f"bold {_backend_style(slot0.backend)}")
            elif slot0 and slot0.session_id:
                l.append("active", style=f"bold {LEAF}")
            else:
                l.append("(idle)", style=DIM)
            left_lines.append(l)

            l = Text()
            l.append("on         ", style=LEAF)
            w = (slot0.current_working if slot0 else "(waiting)") or "(waiting)"
            if len(w) > 22:
                w = w[:19] + "..."
            l.append(w, style=BODY if w not in ("(waiting)", "(idle)", "-") else DIM)
            left_lines.append(l)

        if state.backend:
            l = Text()
            l.append("backend    ", style=LEAF)
            tag = state.backend
            if state.model:
                tag = f"{state.backend}/{state.model}"
            if len(tag) > 22:
                tag = tag[:19] + "..."
            l.append(tag, style=BODY)
            left_lines.append(l)

        while len(left_lines) < helix.HELIX_H:
            left_lines.append(Text(""))

        LEFT_W_fixed = 34
        RIGHT_W_fixed = 40
        helix_w = max(20, panel_inner_width - LEFT_W_fixed - RIGHT_W_fixed - 6)
        middle_lines = helix.render(app.wall_clock_s(), helix_w)

        MAX_M_BARS = 8
        MAX_F_BARS = 4

        def _scoreboard_row(label, m, f, label_style):
            total = m + f
            rate = (100.0 * m / total) if total else 0.0
            l = Text()
            l.append(f"{label[:7]:<7}", style=f"bold {label_style}")
            l.append("\u25B0" * min(m, MAX_M_BARS), style=f"bold {OK}")
            if m > MAX_M_BARS:
                l.append("+", style=DIM)
            l.append("\u25B0" * min(f, MAX_F_BARS), style=f"bold {BAD}")
            if f > MAX_F_BARS:
                l.append("+", style=DIM)
            l.append(f"  {m}m\u00b7{f}f", style=DIM)
            l.append(f"  {rate:.0f}%", style=f"bold {BODY}" if total else DIM)
            if total >= 5:
                lo, hi = wilson_ci(m, total)
                l.append(f"  [{lo*100:.0f}-{hi*100:.0f}]", style=DIM)
            return l

        right_lines = []
        for b in state.backends:
            right_lines.append(_scoreboard_row(
                b,
                state.this_run_matched_by_backend.get(b, 0),
                state.this_run_failed_by_backend.get(b, 0),
                _backend_style(b),
            ))
        while len(right_lines) < helix.HELIX_H:
            right_lines.append(Text(""))

        body = Text()
        for i in range(helix.HELIX_H):
            lhs = left_lines[i]
            pad_left = max(0, LEFT_W_fixed - len(lhs.plain))
            body.append(lhs)
            body.append(" " * pad_left)
            body.append(" \u2502 ", style=MOSS)
            mid = middle_lines[i]
            body.append(mid)
            pad_mid = max(0, helix_w - len(mid.plain))
            body.append(" " * pad_mid)
            body.append(" \u2502 ", style=MOSS)
            r = right_lines[i]
            pad_right = max(0, RIGHT_W_fixed - len(r.plain))
            body.append(r)
            body.append(" " * pad_right)
            body.append("\n")

        return Panel(body, border_style=MOSS, box=HEAVY,
                     title=Text(" run status ", style=f"bold {LEAF}"),
                     title_align="left", padding=(0, 1))

    def _orch_panel(self, app):
        state = app.state
        if not state.orch_log:
            content = Text("(waiting for orchestrator)", style=DIM)
        else:
            t = Text()
            lines = list(state.orch_log)
            last = len(lines) - 1
            for i, line in enumerate(lines):
                is_latest = (i == last)
                lower = line.lower()
                if "failed" in lower or "verify" in lower or "error" in lower or "dead" in lower:
                    style = BAD if is_latest else f"dim {BAD}"
                elif "matched" in lower or "run done" in lower:
                    style = OK if is_latest else f"dim {OK}"
                elif "(tick)" in lower:
                    style = LEAF if is_latest else DIM
                else:
                    style = BODY if is_latest else DIM
                suffix = "" if is_latest else "\n"
                t.append(line + suffix, style=style)
            content = t
        return Panel(
            Align(content, vertical="bottom"),
            border_style=MOSS, box=ROUNDED,
            title=Text(" orchestrator ", style=f"bold {LEAF}"),
            title_align="left", padding=(0, 1),
        )

    def _agent_panel(self, slot):
        """Agent actions panel for N=1 layout."""
        if not slot or not slot.agent_log:
            content = Text("(waiting for session)", style=DIM)
        else:
            content = _styled_toollog(slot.agent_log)
        subtitle = None
        if slot and slot.batch_names:
            subtitle = Text(
                ", ".join(slot.batch_names)[:120],
                style=DIM, overflow="ellipsis",
            )
        return Panel(
            Align(content, vertical="bottom"),
            border_style=MOSS, box=ROUNDED,
            title=Text(" agent actions ", style=f"bold {LEAF}"),
            title_align="left",
            subtitle=subtitle, subtitle_align="left",
            padding=(0, 1),
        )

    def _outcomes_panel(self, app):
        state = app.state
        if not state.outcomes:
            content = Text("(no outcomes yet)", style=DIM)
        else:
            tbl = Table.grid(padding=(0, 2), expand=True)
            tbl.add_column(width=2)
            tbl.add_column(style=BODY, width=10)
            tbl.add_column(style=BODY, ratio=1)
            tbl.add_column(justify="left", width=40)
            tbl.add_column(justify="right", width=10)
            tbl.add_column(style=DIM, justify="right", width=4)
            for o in state.outcomes:
                st = o["status"]
                if st == "matched":
                    glyph, label, color = "\u2713", "matched", OK
                elif st == "failed":
                    glyph, label, color = "\u2717", "failed", BAD
                elif st == "verify":
                    glyph, label, color = "\u26A0", "verify", WARN
                else:
                    glyph, label, color = "\u00b7", st, DIM
                dur_s = o.get("duration_s")
                dur_cell = f"{round(dur_s / 60)}m" if dur_s is not None else ""
                tag = o.get("backend") or o.get("variant", "")
                tag_cell = Text(tag, style=f"bold {_backend_style(tag)}")
                tbl.add_row(
                    Text(glyph, style=f"bold {color}"),
                    Text(label, style=color),
                    o["name"],
                    size_cell(o["size"]),
                    tag_cell,
                    dur_cell,
                )
            content = tbl
        return Panel(content, border_style=MOSS, box=ROUNDED,
                     title=Text(" recent outcomes ", style=f"bold {LEAF}"),
                     title_align="left", padding=(0, 1))


def _ts():
    return datetime.now().strftime("%H:%M:%S ")
