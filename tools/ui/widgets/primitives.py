"""Small formatting helpers shared across screens."""

from rich.text import Text

from tools.ui.palette import DIM, LEAF, SUN, SUNLIT


def fmt_hhmm(seconds):
    if seconds is None or seconds < 0:
        seconds = 0
    hh = int(seconds // 3600)
    mm = int((seconds % 3600) // 60)
    return f"{hh}h {mm:02d}m"


def progress_bar(frac, width=10):
    frac = max(0.0, min(1.0, frac))
    filled = int(round(frac * width))
    return "\u25B0" * filled + "\u25B1" * (width - filled)  # ▰ ▱


def size_cell(size_bytes, width=58, max_bytes=1000):
    """Right-aligned size bar with a fixed byte counter.

    The byte count is pinned to the right edge, and the proportional bar
    grows leftward toward it. Values at or above max_bytes fill the bar.
    """
    counter = f"{size_bytes}B"
    counter_width = max(6, len(counter))
    bar_width = max(1, width - counter_width - 1)
    frac = max(0.0, min(1.0, size_bytes / max_bytes if max_bytes else 0.0))
    bars = int(round(frac * bar_width))
    if size_bytes > 0:
        bars = max(1, bars)
    bars = min(bars, bar_width)

    if size_bytes < 50:
        style = f"dim {LEAF}"
    elif size_bytes < 128:
        style = LEAF
    elif size_bytes < 256:
        style = SUN
    else:
        style = f"bold {SUNLIT}"

    cell = Text()
    cell.append(" " * (bar_width - bars))
    cell.append("\u25B0" * bars, style=style)
    cell.append(" ")
    cell.append(counter.rjust(counter_width), style=DIM)
    return cell
