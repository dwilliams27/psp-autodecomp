"""Shared color palette. Same forest/sun ramp as the v11 demo."""

import math

FOREST = "color(22)"
MOSS   = "color(70)"
LEAF   = "color(107)"
SUN    = "color(178)"
SUNLIT = "color(214)"
BODY   = "color(252)"
DIM    = "color(242)"
OK     = "color(107)"
WARN   = "color(178)"
BAD    = "color(167)"

# Depth ramp used by the helix (back → front).
DEPTH_RAMP = [
    "color(22)", "color(28)", "color(34)", "color(70)",
    "color(107)", "color(108)", "color(178)", "color(214)", "color(220)",
]

HEADER_RAMP = [
    "color(22)", "color(28)", "color(34)", "color(70)",
    "color(107)", "color(108)",
]

# -- Identity 2-axis coloring (backend × model) --
# Hue = backend family, shade = model rank within family.
# (border_style, label_style, dim_style)
_IDENTITY_STYLES = {
    ("claude", 0): (SUN,    f"bold {SUN}",    "color(136)"),
    ("claude", 1): (SUNLIT, f"bold {SUNLIT}",  SUN),
    ("codex",  0): (MOSS,   f"bold {MOSS}",   "color(28)"),
    ("codex",  1): (LEAF,   f"bold {LEAF}",    MOSS),
}

_MODEL_RANK = {
    "claude": ["claude-opus-4-6", "claude-opus-4-7"],
    "codex":  ["gpt-5.5"],
}

_FALLBACK_STYLE = (DIM, f"bold {DIM}", "color(238)")


def identity_style(backend, model=""):
    """Return (border_style, label_style, dim_style) for a backend/model."""
    ranks = _MODEL_RANK.get(backend, [])
    rank = ranks.index(model) if model in ranks else 0
    rank = max(0, min(rank, 1))
    return _IDENTITY_STYLES.get((backend, rank), _FALLBACK_STYLE)


def wilson_ci(successes, total, z=1.96):
    """Wilson score 95% confidence interval for a binomial proportion.

    Returns (lower, upper) as fractions in [0, 1].
    """
    if total == 0:
        return (0.0, 1.0)
    p = successes / total
    denom = 1 + z * z / total
    centre = (p + z * z / (2 * total)) / denom
    spread = z * math.sqrt((p * (1 - p) + z * z / (4 * total)) / total) / denom
    return (max(0.0, centre - spread), min(1.0, centre + spread))
