"""Tail a jsonl log file and yield parsed events.

Works on a file that may not yet exist (waits for it), and handles the
common case where the file is truncated or replaced mid-run (e.g. a new
overnight starts and rewrites match_latest.jsonl). Single-threaded — the
UI calls `poll()` every frame to drain whatever's new.
"""

import json
import os


class LogTailer:
    """Line-oriented tail of a jsonl file. Not thread-safe; one owner."""

    def __init__(self, path, from_start=True):
        self.path = path
        self._fh = None
        self._inode = None
        self._from_start = from_start

    def _open(self):
        try:
            st = os.stat(self.path)
        except FileNotFoundError:
            return False
        self._fh = open(self.path, "r")
        self._inode = st.st_ino
        if not self._from_start:
            self._fh.seek(0, os.SEEK_END)
        return True

    def _reopen_if_rotated(self):
        """If the underlying file was replaced (new inode) or truncated
        (smaller size than our position), re-open and start from the top.

        This handles the common "a new overnight started, symlink now points
        at a different file" case.
        """
        try:
            st = os.stat(self.path)
        except FileNotFoundError:
            if self._fh is not None:
                self._fh.close()
                self._fh = None
                self._inode = None
            return
        if self._inode != st.st_ino:
            if self._fh is not None:
                self._fh.close()
            self._fh = open(self.path, "r")
            self._inode = st.st_ino
            return
        # Truncation: position past end-of-file.
        if self._fh is not None:
            pos = self._fh.tell()
            if pos > st.st_size:
                self._fh.seek(0)

    def poll(self):
        """Return list of parsed events newly appended since last call.

        A line that fails to parse becomes a synthetic
        ``{"event": "malformed", "raw": <line>}`` event — kept visible rather
        than silently dropped, so a real log-format bug surfaces in the TUI.
        """
        if self._fh is None:
            if not self._open():
                return []
        self._reopen_if_rotated()
        if self._fh is None:
            return []

        events = []
        while True:
            line = self._fh.readline()
            if not line:
                break
            line = line.strip()
            if not line:
                continue
            try:
                events.append(json.loads(line))
            except json.JSONDecodeError:
                events.append({"event": "malformed", "raw": line[:500]})
        return events

    def close(self):
        if self._fh is not None:
            self._fh.close()
            self._fh = None
