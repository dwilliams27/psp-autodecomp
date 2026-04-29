#!/usr/bin/env python3
"""SNC-aware C source mutation engine.

Produces random single-transformation mutations of decompiled C source.
Each mutation targets an experimentally verified SNC codegen sensitivity
(see docs/decisions/005-snc-permuter.md).

Not a full AST — uses line-level and regex-based transformations tuned
for the patterns found in decompiled PSP code.
"""

import random
import re


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _brace_depth_map(lines):
    """Return per-line brace depth (depth BEFORE the line executes)."""
    depths = []
    depth = 0
    for line in lines:
        depths.append(depth)
        depth += line.count("{") - line.count("}")
    return depths


def _is_simple_statement(line):
    """True if line is a simple statement (not control flow, not blank/brace)."""
    s = line.strip()
    if not s or not s.endswith(";"):
        return False
    # Exclude control flow, labels, preprocessor
    for kw in ("if", "else", "for", "while", "do", "switch", "case",
               "return", "goto", "break", "continue", "#"):
        if s.startswith(kw):
            return False
    return True


def _is_declaration(line):
    """Heuristic: line declares a local variable."""
    s = line.strip()
    if not s.endswith(";"):
        return False
    # Match common type patterns at start of statement
    # e.g. "int x = ...", "float *p = ...", "unsigned char c;", "eTextureMap *obj = ..."
    return bool(re.match(
        r"^(const\s+)?(unsigned\s+|signed\s+)?"
        r"(void|char|short|int|long|float|double|bool|auto"
        r"|[A-Z_]\w*)"  # user types start with uppercase or _
        r"[\s*&]+\w",
        s
    ))


_BARRIER = '__asm__ volatile("" ::: "memory");'


def _find_barrier_lines(lines):
    """Return indices of asm memory barrier lines."""
    return [i for i, l in enumerate(lines) if _BARRIER in l]


# Regex for binary expressions with commutative operators
# Matches: <expr> <op> <expr> where op is +, *, &, |, ^
# Uses a non-greedy approach to find the operator
_COMMUTATIVE_OPS = re.compile(
    r"(\([^()]*\)|[\w>.\[\]]+(?:\s*\([^()]*\))?)"  # left operand
    r"\s*([+*&|^])\s*"                                # operator
    r"(\([^()]*\)|[\w>.\[\]]+(?:\s*\([^()]*\))?)"    # right operand
)

# Compound boolean: X && Y or X || Y
_COMPOUND_BOOL = re.compile(
    r"(\([^()]*\)|[^&|()]+?)"   # left
    r"\s*(&&|\|\|)\s*"           # operator
    r"(\([^()]*\)|[^&|()]+)"    # right
)

# Type keywords that can be toggled signed/unsigned
_SIGN_TOGGLES = {
    "int": "unsigned int",
    "unsigned int": "int",
    "char": "unsigned char",
    "unsigned char": "char",
    "signed char": "unsigned char",
    "short": "unsigned short",
    "unsigned short": "short",
}

# Cast pattern: (type)expr or (type *)expr
_CAST_RE = re.compile(r"\((\w[\w\s]*\*?)\)\s*(\w)")


# ---------------------------------------------------------------------------
# Mutations — each returns (mutated_lines, name) or None if not applicable
# ---------------------------------------------------------------------------

def swap_adjacent_statements(lines):
    """Swap two adjacent simple statements at the same brace depth."""
    depths = _brace_depth_map(lines)
    candidates = []
    for i in range(len(lines) - 1):
        if (depths[i] == depths[i + 1]
                and depths[i] >= 1
                and _is_simple_statement(lines[i])
                and _is_simple_statement(lines[i + 1])):
            candidates.append(i)
    if not candidates:
        return None
    i = random.choice(candidates)
    out = list(lines)
    out[i], out[i + 1] = out[i + 1], out[i]
    return out, "swap_stmt"


def permute_statement_run(lines):
    """Randomly permute a short run of simple statements.

    This targets SNC's sensitivity to source-order scheduling for independent
    loads/stores. It is intentionally heuristic: compile-and-score is the
    correctness filter.
    """
    depths = _brace_depth_map(lines)
    runs = []
    start = None
    for i, line in enumerate(lines + [""]):
        ok = (
            i < len(lines)
            and depths[i] >= 1
            and _is_simple_statement(line)
        )
        if ok and start is None:
            start = i
        if (not ok or i == len(lines)) and start is not None:
            end = i
            if end - start >= 3:
                runs.append((start, end))
            start = None

    # Prefer small windows to keep compile success high.
    windows = []
    for start, end in runs:
        run_len = end - start
        max_len = min(7, run_len)
        for length in range(3, max_len + 1):
            for s in range(start, end - length + 1):
                windows.append((s, s + length))

    if not windows:
        return None

    start, end = random.choice(windows)
    block = list(lines[start:end])
    shuffled = list(block)
    for _ in range(10):
        random.shuffle(shuffled)
        if shuffled != block:
            break
    if shuffled == block:
        return None

    out = list(lines)
    out[start:end] = shuffled
    return out, "permute_stmt_run"


def reorder_operands(lines):
    """Swap operands around a commutative binary operator on a random line."""
    eligible = []
    for i, line in enumerate(lines):
        if _COMMUTATIVE_OPS.search(line) and _is_simple_statement(line):
            eligible.append(i)
    if not eligible:
        return None
    i = random.choice(eligible)
    line = lines[i]
    matches = list(_COMMUTATIVE_OPS.finditer(line))
    m = random.choice(matches)
    left, op, right = m.group(1).strip(), m.group(2), m.group(3).strip()
    if left == right:
        return None  # no-op swap
    new_line = line[:m.start()] + right + " " + op + " " + left + line[m.end():]
    out = list(lines)
    out[i] = new_line
    return out, "reorder_op"


def reassociate_expression(lines):
    """Change association of chained binary ops: a+b+c -> (a+b)+c or a+(b+c)."""
    # Find lines with two of the same commutative operator
    pattern = re.compile(
        r"(\b[\w>.\[\]]+(?:\s*\([^()]*\))?)"
        r"\s*([+*])\s*"
        r"(\b[\w>.\[\]]+(?:\s*\([^()]*\))?)"
        r"\s*\2\s*"  # same operator
        r"(\b[\w>.\[\]]+(?:\s*\([^()]*\))?)"
    )
    eligible = [(i, pattern.search(line)) for i, line in enumerate(lines)]
    eligible = [(i, m) for i, m in eligible if m]
    if not eligible:
        return None
    i, m = random.choice(eligible)
    a, op, b, c = m.group(1).strip(), m.group(2), m.group(3).strip(), m.group(4).strip()
    # Generate a random reassociation
    variants = [
        f"({a} {op} {b}) {op} {c}",
        f"{a} {op} ({b} {op} {c})",
        f"({a} {op} {c}) {op} {b}",
    ]
    original = m.group(0)
    # Filter out the original form
    variants = [v for v in variants if v != original]
    if not variants:
        return None
    new_expr = random.choice(variants)
    line = lines[i]
    new_line = line[:m.start()] + new_expr + line[m.end():]
    out = list(lines)
    out[i] = new_line
    return out, "reassoc"


def reorder_declarations(lines):
    """Swap two adjacent variable declarations."""
    depths = _brace_depth_map(lines)
    candidates = []
    for i in range(len(lines) - 1):
        if (depths[i] == depths[i + 1]
                and depths[i] >= 1
                and _is_declaration(lines[i])
                and _is_declaration(lines[i + 1])):
            candidates.append(i)
    if not candidates:
        return None
    i = random.choice(candidates)
    out = list(lines)
    out[i], out[i + 1] = out[i + 1], out[i]
    return out, "reorder_decl"


def insert_barrier(lines):
    """Insert an asm memory barrier between two statements."""
    depths = _brace_depth_map(lines)
    candidates = []
    for i in range(len(lines)):
        if depths[i] >= 1 and _is_simple_statement(lines[i]):
            candidates.append(i)
    if not candidates:
        return None
    i = random.choice(candidates)
    indent = re.match(r"(\s*)", lines[i]).group(1)
    barrier_line = indent + _BARRIER + "\n"
    out = list(lines)
    # Insert after the chosen statement
    out.insert(i + 1, barrier_line)
    return out, "ins_barrier"


def remove_barrier(lines):
    """Remove an existing asm memory barrier."""
    barrier_indices = _find_barrier_lines(lines)
    if not barrier_indices:
        return None
    i = random.choice(barrier_indices)
    out = list(lines)
    out.pop(i)
    return out, "rm_barrier"


def toggle_signedness(lines):
    """Toggle signed/unsigned on a type keyword in a random declaration or cast."""
    # Process longest patterns first to avoid partial matches
    # (e.g. "unsigned char" must be checked before bare "char")
    sorted_toggles = sorted(_SIGN_TOGGLES.items(), key=lambda x: -len(x[0]))
    eligible = []
    for i, line in enumerate(lines):
        for old, new in sorted_toggles:
            # Use negative lookbehind/ahead to avoid partial compound matches
            # e.g. don't match bare "char" when it's part of "unsigned char"
            if old in ("char", "short", "int"):
                pattern = r'(?<!unsigned )(?<!signed )\b' + re.escape(old) + r'\b'
            else:
                pattern = r'\b' + re.escape(old) + r'\b'
            if re.search(pattern, line):
                eligible.append((i, old, new, pattern))
                break  # one toggle per line to avoid conflicts
    if not eligible:
        return None
    i, old, new, pattern = random.choice(eligible)
    out = list(lines)
    out[i] = re.sub(pattern, new, out[i], count=1)
    return out, "sign_toggle"


def swap_compound_condition(lines):
    """Swap sides of a && or || in a compound boolean."""
    eligible = []
    for i, line in enumerate(lines):
        s = line.strip()
        if s.startswith("if") or s.startswith("while") or s.startswith("} else if"):
            m = _COMPOUND_BOOL.search(line)
            if m:
                eligible.append((i, m))
    if not eligible:
        return None
    i, m = random.choice(eligible)
    left, op, right = m.group(1).strip(), m.group(2), m.group(3).strip()
    if left == right:
        return None
    new_expr = right + " " + op + " " + left
    line = lines[i]
    new_line = line[:m.start()] + new_expr + line[m.end():]
    out = list(lines)
    out[i] = new_line
    return out, "swap_cond"


def split_expression(lines):
    """Split a complex RHS into a temp variable assignment."""
    depths = _brace_depth_map(lines)
    # Find assignments with a binary expression on the RHS
    assign_re = re.compile(r"^(\s*)(.*?)\s*=\s*(.+[+\-*/&|^].+);$")
    eligible = []
    for i, line in enumerate(lines):
        if depths[i] >= 1:
            m = assign_re.match(line)
            if m and "==" not in m.group(3) and "!=" not in m.group(3):
                eligible.append((i, m))
    if not eligible:
        return None
    i, m = random.choice(eligible)
    indent, lhs, rhs = m.group(1), m.group(2), m.group(3)
    # Create a temp variable
    tmp_name = f"_tmp_{random.randint(0, 999)}"
    # Try to guess the type from the LHS cast pattern
    cast_m = re.match(r"\(\(([^)]+)\)\w+\)\[", lhs)
    var_type = "int"  # default fallback
    if "float" in rhs or "float" in lhs:
        var_type = "float"
    out = list(lines)
    out[i] = f"{indent}{var_type} {tmp_name} = {rhs};\n"
    out.insert(i + 1, f"{indent}{lhs} = {tmp_name};\n")
    return out, "split_expr"


def insert_cast(lines):
    """Insert a (float) or (int) cast on a subexpression."""
    # Find lines with arithmetic that don't already have dense casts
    arith_re = re.compile(r"(\b\w+(?:\[[^\]]*\])?(?:->\w+)?)\s*([+\-*/])\s*(\b\w+)")
    eligible = []
    for i, line in enumerate(lines):
        if _is_simple_statement(line):
            for m in arith_re.finditer(line):
                eligible.append((i, m))
    if not eligible:
        return None
    i, m = random.choice(eligible)
    # Cast the left operand
    cast_type = random.choice(["float", "int", "unsigned"])
    left = m.group(1)
    new_left = f"({cast_type}){left}"
    line = lines[i]
    new_line = line[:m.start(1)] + new_left + line[m.end(1):]
    out = list(lines)
    out[i] = new_line
    return out, "ins_cast"


def remove_cast(lines):
    """Remove a cast expression."""
    eligible = []
    for i, line in enumerate(lines):
        for m in _CAST_RE.finditer(line):
            # Don't remove casts that are part of pointer derefs like ((int *)this)
            # Only remove "simple" casts
            eligible.append((i, m))
    if not eligible:
        return None
    i, m = random.choice(eligible)
    line = lines[i]
    # Remove the (type) part, keep the expression
    new_line = line[:m.start()] + m.group(2) + line[m.end():]
    out = list(lines)
    out[i] = new_line
    return out, "rm_cast"


def multiply_decompose(lines):
    """Decompose a multiply: x*3 -> (x<<1)+x or x+x+x."""
    mul_re = re.compile(r"(\b\w+(?:->\w+)?)\s*\*\s*(\d+)")
    eligible = []
    for i, line in enumerate(lines):
        for m in mul_re.finditer(line):
            n = int(m.group(2))
            if 2 <= n <= 8:
                eligible.append((i, m, n))
    if not eligible:
        return None
    i, m, n = random.choice(eligible)
    var = m.group(1)
    # Generate decomposition options
    variants = []
    if n == 2:
        variants = [f"({var} + {var})", f"({var} << 1)"]
    elif n == 3:
        variants = [f"(({var} << 1) + {var})", f"({var} + {var} + {var})"]
    elif n == 4:
        variants = [f"({var} << 2)", f"(({var} << 1) + ({var} << 1))"]
    elif n == 5:
        variants = [f"(({var} << 2) + {var})"]
    elif n == 6:
        variants = [f"(({var} << 2) + ({var} << 1))"]
    elif n == 7:
        variants = [f"(({var} << 3) - {var})"]
    elif n == 8:
        variants = [f"({var} << 3)"]
    if not variants:
        return None
    replacement = random.choice(variants)
    line = lines[i]
    new_line = line[:m.start()] + replacement + line[m.end():]
    out = list(lines)
    out[i] = new_line
    return out, "mul_decomp"


# ---------------------------------------------------------------------------
# Registry and main entry point
# ---------------------------------------------------------------------------

# (function, weight) — higher weight = tried more often
MUTATIONS = [
    # High value (always try)
    (swap_adjacent_statements, 10),
    (permute_statement_run, 8),
    (reorder_operands, 10),
    (reassociate_expression, 8),
    (reorder_declarations, 8),
    (insert_barrier, 10),
    (remove_barrier, 10),
    # Medium value
    (toggle_signedness, 5),
    (split_expression, 5),
    (swap_compound_condition, 5),
    # Low value
    (insert_cast, 2),
    (remove_cast, 2),
    (multiply_decompose, 2),
]

_TOTAL_WEIGHT = sum(w for _, w in MUTATIONS)


def _mutate_once(source):
    """Apply a single random mutation to source text.

    Returns (mutated_source, mutation_name) or None if no mutation applied
    after several attempts.
    """
    lines = source.splitlines(keepends=True)
    # Weighted random selection with retries
    for _ in range(20):
        r = random.random() * _TOTAL_WEIGHT
        cumulative = 0
        for fn, weight in MUTATIONS:
            cumulative += weight
            if r <= cumulative:
                result = fn(lines)
                if result is not None:
                    mutated_lines, name = result
                    mutated = "".join(mutated_lines)
                    if mutated != source:  # ensure actually changed
                        return mutated, name
                break
    return None


def mutate(source, max_steps=3):
    """Apply one or more random mutations to source text.

    Most candidates stay single-mutation for compile success. A meaningful
    minority chains 2-3 mutations, which catches cases where SNC needs both
    statement order and local expression/register-pressure changes.
    """
    if max_steps <= 1:
        return _mutate_once(source)

    steps = random.choices([1, 2, 3], weights=[65, 25, 10], k=1)[0]
    steps = min(steps, max_steps)
    current = source
    names = []
    for _ in range(steps):
        result = _mutate_once(current)
        if result is None:
            break
        current, name = result
        names.append(name)

    if not names or current == source:
        return None
    return current, "+".join(names)
