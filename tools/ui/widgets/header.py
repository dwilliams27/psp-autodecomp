"""The `autodecomp` gradient banner."""

import pyfiglet
from rich.align import Align
from rich.text import Text

from tools.ui.palette import HEADER_RAMP


def gradient_header(text, font="slant", width=140):
    lines = pyfiglet.figlet_format(text, font=font, width=width).rstrip("\n").split("\n")
    out = Text()
    ramp = HEADER_RAMP
    for i, line in enumerate(lines):
        shade = ramp[min(i * len(ramp) // max(len(lines), 1), len(ramp) - 1)]
        out.append(line + "\n", style=shade)
    return out


# pyfiglet.figlet_format is expensive (~1.3ms) and the header text is
# static — caching by (title, font) drops it to a single call per banner.
_HEADER_CACHE = {}


def header_panel(title="autodecomp", font="slant"):
    key = (title, font)
    if key not in _HEADER_CACHE:
        _HEADER_CACHE[key] = Align.center(gradient_header(title, font=font))
    return _HEADER_CACHE[key]
