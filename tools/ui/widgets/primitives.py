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


def size_cell(size_bytes):
    """One ▰ per 10 bytes (capped), color-tiered by size, byte count trails."""
    bars = max(1, size_bytes // 10)
    bars = min(bars, 30)
    if size_bytes < 50:
        style = f"dim {LEAF}"
    elif size_bytes < 128:
        style = LEAF
    elif size_bytes < 256:
        style = SUN
    else:
        style = f"bold {SUNLIT}"
    cell = Text()
    cell.append("\u25B0" * bars, style=style)
    cell.append(f"  {size_bytes}B", style=DIM)
    return cell
