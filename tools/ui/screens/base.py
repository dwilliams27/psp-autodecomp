"""Abstract Screen. Each screen implements event handling + rendering.

The app owns the Live loop and the tailer; screens only care about their
view of the state. A screen may hold its own state, but the canonical
RunState lives on the App so cross-screen info (absolute match counts,
most-recent session) survives screen switches.
"""

from rich.console import RenderableType


class Screen:
    """Subclass and implement at least `render`."""

    name = "base"

    def on_mount(self, app):
        """Called once when the screen becomes active."""

    def on_unmount(self, app):
        """Called once when the screen is replaced by another."""

    def on_event(self, app, event):
        """Called for every parsed jsonl event. `event` is a dict."""

    def on_tick(self, app, t):
        """Called every frame. `t` is elapsed wall seconds since app start."""

    def render(self, app, console) -> RenderableType:
        raise NotImplementedError
