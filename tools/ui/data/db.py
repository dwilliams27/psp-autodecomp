"""Seed the status bar with the absolute matched count before progress_tick
events arrive. Delegates to tools/common.load_db for the actual read.
"""

import sys
from pathlib import Path

_REPO_ROOT = Path(__file__).resolve().parents[3]
_TOOLS = _REPO_ROOT / "tools"
if str(_TOOLS) not in sys.path:
    sys.path.insert(0, str(_TOOLS))

from common import load_db  # noqa: E402


def read_matched_total():
    """Return the number of matched functions in the DB.

    Raises FileNotFoundError if the DB doesn't exist — that's a real
    misconfiguration, not a fallback-worthy case.
    """
    funcs = load_db()
    return sum(1 for f in funcs if f.get("match_status") == "matched")
