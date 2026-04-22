"""Event type names emitted by the orchestrator into logs/match_*.jsonl.

This module is documentation — the schema lives in orchestrator.py, here we
just enumerate what the screens know how to consume so lint/grep is easy.

Producer → consumer list (name, key fields):

  run_start         — hours, deadline, variants, batch_size, mode, start_time
  run_done          — branch, duration_s, attempted, matched, failed, errors
  session_start     — session_id, variant, class_name, functions[]
  session_done      — session_id, variant, matched, failed, duration_s
  function_result   — session_id, variant, address, name, size, status
  verify_failed     — session_id, variant, address, name, size, diff_count
  verify_error      — session_id, variant, address, name, error
  session_error     — session_id, variant, error, functions, duration_s
  system_error      — session_id, variant, error, functions, duration_s
  prep_error        — session_id, variant, error
  git_error         — session_id, variant, error
  rejected_assembly_only — session_id, variant, file, reason
  missing_failure_notes  — session_id, variant, address
  unreported_function    — session_id, variant, address, name
  progress_tick     — matched_total, failed_total, untried_total, elapsed_s
  orch_note         — text  (every log() call in the orchestrator)
  agent_event       — session_id, variant, kind (text|thinking|tool_use|
                      tool_result|raw), text, tool?, tool_use_id?, is_error?
"""

# Known event types, for validation / future switch-tables.
RUN_EVENTS = frozenset({"run_start", "run_done"})

SESSION_EVENTS = frozenset({
    "session_start", "session_done", "session_error", "system_error",
    "prep_error", "git_error",
})

FUNCTION_EVENTS = frozenset({
    "function_result", "verify_failed", "verify_error",
    "rejected_assembly_only", "missing_failure_notes", "unreported_function",
})

STREAM_EVENTS = frozenset({"orch_note", "agent_event", "progress_tick"})

ALL_EVENTS = RUN_EVENTS | SESSION_EVENTS | FUNCTION_EVENTS | STREAM_EVENTS
