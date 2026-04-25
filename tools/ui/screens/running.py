"""Running-overnight dashboard. Port of launcher_demos/v11 with real data."""

import re
from datetime import datetime, timedelta

from rich.align import Align
from rich.box import HEAVY, ROUNDED
from rich.console import Group
from rich.layout import Layout
from rich.panel import Panel
from rich.table import Table
from rich.text import Text

from tools.ui.palette import (BAD, BODY, DIM, LEAF, MOSS, OK, SUN, SUNLIT, WARN)
from tools.ui.screens.base import Screen
from tools.ui.widgets import helix
from tools.ui.widgets.header import header_panel
from tools.ui.widgets.primitives import fmt_hhmm, progress_bar, size_cell


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


def _short_name(full_name):
    """Strip the argument list off `Class::Method(args...)` for compact display."""
    return full_name.split("(", 1)[0]


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
            state.orch_log.append(_ts()
                + f"session {event.get('session_id','?')} ERROR · "
                + f"{event.get('error','')[:100]}")
            state.current_session_sid = None
            state.current_session_variant = None
            state.current_working = "(waiting)"
        elif ev == "progress_tick":
            self._on_progress_tick(state, event)
        elif ev == "orch_note":
            self._on_orch_note(state, event)
        elif ev == "agent_event":
            self._on_agent_event(state, event)
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
        funcs = event.get("functions", []) or []
        state.ensure_variant(variant)
        state.current_session_sid = sid
        state.current_session_variant = variant
        state.current_batch_names = [_short_name(f.get("name", "?")) for f in funcs]
        state.current_addr_to_name = {
            (f.get("address") or "").lower():
                _short_name(f.get("name") or "?")
            for f in funcs if f.get("address")
        }
        state.current_working = (
            state.current_batch_names[0] if state.current_batch_names
            else "(waiting)"
        )
        state.agent_log.clear()
        state.agent_narrative_log.clear()
        state.orch_log.append(
            _ts()
            + f"session {sid} [{variant}] started · batch={len(funcs)}"
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
        # Backfill the per-function duration on outcomes from this session
        # — orchestrator emits function_result before session_done so the
        # outcomes are already in the deque at this point. With batch_size=1
        # this is per-function; for larger batches every function from the
        # batch shares the session duration.
        for o in state.outcomes:
            if o.get("session_id") == sid and o.get("duration_s") is None:
                o["duration_s"] = dur_s
        state.current_session_sid = None
        state.current_session_variant = None
        state.current_addr_to_name = {}
        state.current_working = "(waiting)"

    def _on_function_result(self, state, event):
        status = event.get("status")
        variant = event.get("variant", "?")
        state.ensure_variant(variant)
        outcome = {
            "status": status,
            "name": event.get("name", "?"),
            "size": event.get("size", 0),
            "variant": variant,
            "session_id": event.get("session_id"),
            # Stamped by _on_session_done since orchestrator emits
            # function_result before session_done in the same batch.
            "duration_s": None,
        }
        state.outcomes.appendleft(outcome)

        short = _short_name(event.get("name", "?"))
        if status == "matched":
            state.this_run_matched[variant] = state.this_run_matched.get(variant, 0) + 1
            state.orch_log.append(_ts() + f"  matched: {short}")
        elif status == "failed":
            state.this_run_failed[variant] = state.this_run_failed.get(variant, 0) + 1
            state.orch_log.append(_ts() + f"  failed:  {short}")

    def _on_verify_failed(self, state, event):
        variant = event.get("variant", "?")
        state.ensure_variant(variant)
        state.this_run_verify_fail += 1
        state.outcomes.appendleft({
            "status": "verify",
            "name": event.get("name", "?"),
            "size": event.get("size", 0),
            "variant": variant,
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
        # Truncate to ~90 chars to stay within the panel width
        state.orch_log.append(_ts() + text[:100])

    def _on_agent_event(self, state, event):
        kind = event.get("kind", "?")
        text = (event.get("text") or "").strip()
        if not text:
            return
        if kind == "tool_use":
            m = _COMPARE_RE.search(text)
            if m:
                addr = m.group(1).lower()
                name = state.current_addr_to_name.get(addr)
                state.current_working = name or f"@{addr}"

        # text + thinking go to the narrative panel — these are the
        # agent's verbose reasoning, kept whole so a multi-sentence
        # rationale stays readable.
        if kind in ("text", "thinking"):
            prefix = "~ " if kind == "thinking" else ""
            line = prefix + text
            if len(line) > 400:
                line = line[:397] + "..."
            state.agent_narrative_log.append(line)
            return

        # tool_use / tool_result / raw / unknown go to the compact tool
        # log — narrow lines, one per call/result.
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
        state.agent_log.append(line)

    # ------------------------------------------------------------------
    # rendering
    # ------------------------------------------------------------------

    def render(self, app, console):
        layout = Layout()
        layout.split_column(
            Layout(name="header", size=7),
            Layout(name="status", size=9),
            Layout(name="mid", ratio=1),
            Layout(name="outcomes", size=12),
        )
        # Top of mid: full-width agent activity (text + thinking) — the
        # agent's verbose reasoning. Bottom: orchestrator events + agent
        # actions (compact tool calls + results) side-by-side.
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
        layout["mid"]["narrative"].update(self._narrative_panel(app))
        layout["mid"]["bottom"]["orchestrator"].update(self._orch_panel(app))
        layout["mid"]["bottom"]["agent"].update(self._agent_panel(app))
        layout["outcomes"].update(self._outcomes_panel(app))
        return layout

    def _narrative_panel(self, app):
        state = app.state
        if not state.agent_narrative_log:
            content = Text("(waiting for agent activity)", style=DIM)
        else:
            t = Text()
            lines = list(state.agent_narrative_log)
            last = len(lines) - 1
            for i, line in enumerate(lines):
                is_latest = (i == last)
                if line.startswith("~ "):
                    style = (f"italic {LEAF}" if is_latest
                             else f"italic {DIM}")
                else:
                    style = BODY if is_latest else DIM
                suffix = "" if is_latest else "\n"
                t.append(line + suffix, style=style)
            content = t
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

        l = Text()
        l.append("now        ", style=LEAF)
        if state.current_session_variant:
            l.append(state.current_session_variant, style=f"bold {SUN}")
        else:
            l.append("(idle)", style=DIM)
        left_lines.append(l)

        l = Text()
        l.append("on         ", style=LEAF)
        w = state.current_working or "(waiting)"
        if len(w) > 22:
            w = w[:19] + "..."
        l.append(w, style=BODY if w not in ("(waiting)", "-") else DIM)
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
        right_lines = []
        for v in state.variants:
            m = state.this_run_matched.get(v, 0)
            f = state.this_run_failed.get(v, 0)
            total = m + f
            rate = (100.0 * m / total) if total else 0.0
            l = Text()
            l.append(f"{v[:7]:<7}", style=f"bold {MOSS}")
            l.append("\u25B0" * min(m, MAX_M_BARS), style=f"bold {OK}")
            if m > MAX_M_BARS:
                l.append("+", style=DIM)
            l.append("\u25B0" * min(f, MAX_F_BARS), style=f"bold {BAD}")
            if f > MAX_F_BARS:
                l.append("+", style=DIM)
            l.append(f"  {m}m\u00b7{f}f", style=DIM)
            l.append(f"  {rate:.0f}%", style=f"bold {BODY}" if total else DIM)
            right_lines.append(l)
        while len(right_lines) < helix.HELIX_H:
            right_lines.append(Text(""))

        body = Text()
        for i in range(helix.HELIX_H):
            # LEFT
            lhs = left_lines[i]
            pad_left = max(0, LEFT_W_fixed - len(lhs.plain))
            body.append(lhs)
            body.append(" " * pad_left)
            body.append(" \u2502 ", style=MOSS)
            # MIDDLE
            mid = middle_lines[i]
            body.append(mid)
            pad_mid = max(0, helix_w - len(mid.plain))
            body.append(" " * pad_mid)
            body.append(" \u2502 ", style=MOSS)
            # RIGHT
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
                if "failed" in lower or "verify" in lower or "error" in lower:
                    style = BAD if is_latest else f"dim {BAD}"
                elif "matched" in lower or "run done" in lower:
                    style = OK if is_latest else f"dim {OK}"
                elif "(tick)" in lower:
                    style = LEAF if is_latest else DIM
                else:
                    style = BODY if is_latest else DIM
                # No trailing newline on the final line — with Align(vertical=
                # "bottom") a trailing \n would leave a blank row under the text.
                suffix = "" if is_latest else "\n"
                t.append(line + suffix, style=style)
            content = t
        return Panel(
            # Bottom-align so new lines pile up from the bottom (tail -f),
            # not from the top. Without this a deque shorter than the panel
            # height leaves the lower half empty.
            Align(content, vertical="bottom"),
            border_style=MOSS, box=ROUNDED,
            title=Text(" orchestrator ", style=f"bold {LEAF}"),
            title_align="left", padding=(0, 1),
        )

    def _agent_panel(self, app):
        state = app.state
        if not state.agent_log:
            content = Text("(waiting for session)", style=DIM)
        else:
            t = Text()
            lines = list(state.agent_log)
            last = len(lines) - 1
            for i, line in enumerate(lines):
                is_latest = (i == last)
                if line.startswith("$"):
                    style = SUNLIT if is_latest else SUN
                elif line.lstrip().startswith("\u2192"):  # "  → ..."
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
            content = t
        subtitle = None
        if state.current_batch_names:
            subtitle = Text(
                ", ".join(state.current_batch_names)[:120],
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
            tbl.add_column(style=MOSS, justify="right", width=10)
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
                tbl.add_row(
                    Text(glyph, style=f"bold {color}"),
                    Text(label, style=color),
                    o["name"],
                    size_cell(o["size"]),
                    o["variant"],
                    dur_cell,
                )
            content = tbl
        return Panel(content, border_style=MOSS, box=ROUNDED,
                     title=Text(" recent outcomes ", style=f"bold {LEAF}"),
                     title_align="left", padding=(0, 1))


def _ts():
    return datetime.now().strftime("%H:%M:%S ")
