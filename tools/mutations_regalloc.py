#!/usr/bin/env python3
"""Register-allocation-affecting C source mutations.

Companion to ``tools/mutations.py``. The transforms here are specifically the
ones that move SNC's register allocator: introducing/removing temporaries,
splitting a value's live range through a duplicate local, sinking/hoisting a
computation across a statement, and renaming a local. These are exactly the
edits that flip a REG_ALLOC near-miss (right instructions, wrong registers)
without changing program structure.

Conventions match ``tools/mutations.py``: every mutation is a callable taking
``lines`` (a list of strings produced by ``str.splitlines(keepends=True)``) and
returning ``(mutated_lines, name)`` or ``None`` when not applicable. The
integrator registers these by extending ``mutations.MUTATIONS`` with the
``REGALLOC_MUTATIONS`` list below.

The transforms are intentionally syntactic and heuristic; compile-and-score in
the permuter is the correctness filter. They must never silently corrupt source
in a way that looks applied but is not — when a mutation cannot apply cleanly it
returns ``None`` (fail-loud is preserved by the caller's compile gate).
"""

import random
import re

# Reuse the shared helpers from the sibling mutation engine. Importing keeps the
# brace-depth / statement-classification logic in one place. This module is
# imported as ``tools.mutations_regalloc`` or run directly from ``tools/``; try
# both so the __main__ smoke test works without package context.
try:  # pragma: no cover - exercised by importer / __main__ respectively
    from mutations import (
        _brace_depth_map,
        _is_simple_statement,
        _is_declaration,
    )
except ImportError:  # pragma: no cover
    from tools.mutations import (
        _brace_depth_map,
        _is_simple_statement,
        _is_declaration,
    )


# ---------------------------------------------------------------------------
# Local helpers
# ---------------------------------------------------------------------------

# A C identifier.
_IDENT = re.compile(r"[A-Za-z_]\w*")

# C keywords / common type words that are NOT user locals — never treat these as
# rename/duplicate targets.
_RESERVED = frozenset({
    "if", "else", "for", "while", "do", "switch", "case", "default",
    "return", "goto", "break", "continue", "sizeof", "struct", "union",
    "enum", "typedef", "static", "const", "volatile", "register", "extern",
    "void", "char", "short", "int", "long", "float", "double", "bool",
    "signed", "unsigned", "auto", "this", "true", "false", "null", "NULL",
    "new", "delete", "operator",
})

# Declaration with an initializer:  <type> name = <rhs>;
# Group 1: indent, 2: type (incl. pointer stars / qualifiers), 3: name,
# 4: rhs.
_DECL_INIT_RE = re.compile(
    r"^(\s*)"
    r"((?:const\s+)?(?:unsigned\s+|signed\s+)?"
    r"(?:void|char|short|int|long|float|double|bool|auto|[A-Z_]\w*)"
    r"(?:\s*\*+|\s*&)?\s*)"           # type, optional pointer/ref
    r"([A-Za-z_]\w*)"                  # variable name
    r"\s*=\s*"
    r"(.+?);\s*$"
)

# Plain assignment:  lhs = rhs;  (lhs is a single identifier, not a member/index)
_SIMPLE_ASSIGN_RE = re.compile(
    r"^(\s*)([A-Za-z_]\w*)\s*=\s*(.+?);\s*$"
)

# A subexpression worth hoisting into a temp: a member-chain / indexed access,
# optionally a single binary op between two such terms. Conservative on purpose.
#
# IMPORTANT: ``*`` and ``&`` are syntactically overloaded in C/C++ — they denote
# pointer/reference declarators, casts (``(char *)``), and address-of (``&m``) in
# addition to the binary multiply/bitwise-and we actually want to hoist. Matching
# them naively produced uncompilable garbage like ``unsigned int t = char *rec;``
# or ``... = gcDesiredEntity &operator;`` on real source. To disambiguate we:
#   * require ``*`` / ``&`` to be a *binary* operator with whitespace on BOTH
#     sides (``t0 & 1``, ``rec + 0x50``); declarators/casts/address-of are written
#     glued (``*rec``, ``(char *)``, ``&m``), so they no longer match; and
#   * require a value term (identifier-rooted) on the right, NOT a ``)``/``;``,
#     which excludes the trailing ``*`` of a cast like ``(char *)``.
# The ``+ | ^`` operators never collide with declaration syntax, so they keep the
# tighter ``\s*`` spacing for reach.
_TERM = r"[A-Za-z_]\w*(?:\s*(?:->|\.)\s*\w+|\s*\[[^\[\]]*\])*"
# Right-hand operand: a value term, optionally a small constant suffix / bit-not.
_RTERM = r"~?\s*(?:0[xX][0-9a-fA-F]+|\d+[uUlL]*|" + _TERM + r")"
_HOISTABLE_RE = re.compile(
    # group(1) left term, group(2) operator, group(3) right operand.
    r"(" + _TERM + r")"
    r"(?:\s*([+|^])\s*|\s+([*&])\s+)"   # +|^ may be glued; * & must be spaced
    r"(" + _RTERM + r")"
)

# C/C++ type-ish / reserved words that must never be the LEFT operand of a hoist
# (their presence means we matched declarator syntax, not an expression).
_TYPEISH_LEFT = frozenset({
    "void", "char", "short", "int", "long", "float", "double", "bool",
    "signed", "unsigned", "const", "volatile", "static", "auto", "register",
    "struct", "union", "enum", "operator", "return", "sizeof",
})


def _fresh_name(lines, base):
    """Return an identifier not already present as a whole word in ``lines``."""
    text = "".join(lines)
    existing = set(_IDENT.findall(text))
    for _ in range(1000):
        cand = f"{base}{random.randint(0, 99999)}"
        if cand not in existing:
            return cand
    # Fail loud rather than risk a shadowing collision.
    return None


def _indent_of(line):
    return re.match(r"(\s*)", line).group(1)


def _word_re(name):
    return re.compile(r"(?<![A-Za-z0-9_])" + re.escape(name) + r"(?![A-Za-z0-9_])")


def _find_decls_with_init(lines, depths):
    """Yield (index, match) for every initialized local declaration in a body."""
    for i, line in enumerate(lines):
        if depths[i] < 1:
            continue
        m = _DECL_INIT_RE.match(line)
        if not m:
            continue
        rhs = m.group(4)
        # Skip declarations whose RHS is itself a comparison/logical expr — those
        # are bool temps where retyping is risky.
        if "==" in rhs or "!=" in rhs or "&&" in rhs or "||" in rhs:
            continue
        yield i, m


def _block_range(lines, depths, decl_index):
    """Return [start, end) line range of the brace block containing ``decl_index``.

    Used to scope live-range / rename edits so we don't touch identically named
    locals in sibling scopes.
    """
    base = depths[decl_index]
    # Walk forward until depth drops below the declaration's depth (block end).
    end = len(lines)
    for j in range(decl_index + 1, len(lines)):
        if depths[j] < base:
            end = j
            break
    return decl_index, end


# ---------------------------------------------------------------------------
# Mutations
# ---------------------------------------------------------------------------

def _is_real_hoist(line, m):
    """Reject ``_HOISTABLE_RE`` matches that are actually declarator / cast /
    address-of syntax rather than a hoistable value expression.

    Guards (all conservative — when in doubt, reject so we never emit
    uncompilable source):
      * left operand is a C type/reserved word (``char *rec`` -> ``char`` & ``*``);
      * the match sits inside a cast: the char immediately before the left term
        is ``(`` and a ``)`` immediately follows the right operand
        (``(char *)`` / ``(gcDVRec *)``);
      * the right operand is immediately followed by ``)`` or ``;`` for the
        ``* / &`` operators, which is the dangling ``*`` of a cast or a trailing
        ``&`` declarator;
      * the operator is ``&``/``*`` and the left term root is an uppercase-led
        identifier (a user TYPE name, e.g. ``gcDesiredEntity &operator``).
    """
    left = m.group(1)
    op = m.group(2) or m.group(3)   # + | ^   OR   * &
    right = m.group(4)
    left_root = _IDENT.match(left.strip())
    left_word = left_root.group(0) if left_root else ""

    if left_word in _TYPEISH_LEFT:
        return False

    # Character immediately preceding / following the full match.
    pre = line[m.start() - 1] if m.start() > 0 else ""
    post = line[m.end()] if m.end() < len(line) else ""

    if op in ("*", "&"):
        # A cast like ``(char *)`` would have ``(`` before and ``)`` after; a
        # declarator/address-of trails into ``)``/``;``/``,``.
        if pre == "(" or post in (")", ";", ","):
            return False
        # Uppercase-led left root is almost certainly a user type name used as a
        # reference/pointer declarator (``gcDesiredEntity &operator``).
        if left_word[:1].isupper():
            return False
    return True


def introduce_temporary(lines):
    """Bind a hoistable subexpression to a fresh local, then reference it.

    ``a->b + c->d``  becomes  ``unsigned int t = a->b + c->d;`` on a new line
    above, with the original occurrence replaced by ``t``. Adding a named
    temporary changes the live ranges the allocator sees and frequently shifts
    which hardware register a value lands in.
    """
    depths = _brace_depth_map(lines)
    eligible = []
    for i, line in enumerate(lines):
        if depths[i] < 1 or not _is_simple_statement(line):
            continue
        for m in _HOISTABLE_RE.finditer(line):
            if _is_real_hoist(line, m):
                eligible.append((i, m))
    if not eligible:
        return None
    i, m = random.choice(eligible)
    name = _fresh_name(lines, "rt")
    if name is None:
        return None
    line = lines[i]
    expr = m.group(0)
    # Pointer-ish heuristic: if the subexpression dereferences/indexes, a
    # pointer-width unsigned int is the safe carrier on MIPS32.
    decl_type = "unsigned int"
    indent = _indent_of(line)
    new_line = line[:m.start()] + name + line[m.end():]
    decl_line = f"{indent}{decl_type} {name} = {expr};\n"
    out = list(lines)
    out[i] = new_line
    out.insert(i, decl_line)
    return out, "rg_introduce_temp"


def inline_temporary(lines):
    """Remove a single-use temporary by substituting its RHS at the use site.

    The inverse of ``introduce_temporary``. Collapsing a live range can free the
    register it was occupying and re-color downstream values.
    """
    depths = _brace_depth_map(lines)
    decls = list(_find_decls_with_init(lines, depths))
    random.shuffle(decls)
    for i, m in decls:
        indent, _type, name, rhs = m.groups()
        start, end = _block_range(lines, depths, i)
        word = _word_re(name)
        # Count uses after the declaration within its block.
        use_lines = [
            j for j in range(i + 1, end) if word.search(lines[j])
        ]
        if len(use_lines) != 1:
            continue
        j = use_lines[0]
        # Don't inline if the name is re-assigned anywhere (would change meaning).
        if any(re.search(r"(?<![=!<>])\b" + re.escape(name) + r"\b\s*[-+*/%&|^]?=(?!=)",
                         lines[k]) for k in range(i + 1, end)):
            continue
        # Substitute exactly one occurrence, parenthesized to preserve precedence.
        replaced = word.sub("(" + rhs + ")", lines[j], count=1)
        if replaced == lines[j]:
            continue
        out = list(lines)
        out[j] = replaced
        out.pop(i)
        return out, "rg_inline_temp"
    return None


def duplicate_local(lines):
    """Split a local's live range by reading it through a second equal local.

    Introduces ``<type> dup = orig;`` right after the original declaration and
    rewrites a later read of ``orig`` to use ``dup``. The two now-distinct live
    ranges give the allocator a different coloring problem, which is the single
    most effective lever for the locker register-spelling near-misses.
    """
    depths = _brace_depth_map(lines)
    decls = list(_find_decls_with_init(lines, depths))
    random.shuffle(decls)
    for i, m in decls:
        indent, vtype, name, _rhs = m.groups()
        start, end = _block_range(lines, depths, i)
        word = _word_re(name)
        # Reads we may redirect: occurrences after the decl that are NOT an
        # assignment target (no trailing '=' that isn't '==').
        read_sites = []
        for j in range(i + 1, end):
            if not word.search(lines[j]):
                continue
            # Skip lines where name is assigned (live range would diverge wrongly).
            if re.search(r"\b" + re.escape(name) + r"\b\s*[-+*/%&|^]?=(?!=)", lines[j]):
                continue
            read_sites.append(j)
        if not read_sites:
            continue
        dup = _fresh_name(lines, "dup")
        if dup is None:
            continue
        j = random.choice(read_sites)
        redirected = word.sub(dup, lines[j], count=1)
        if redirected == lines[j]:
            continue
        dup_decl = f"{indent}{vtype.rstrip()} {dup} = {name};\n"
        out = list(lines)
        out[j] = redirected
        out.insert(i + 1, dup_decl)
        return out, "rg_duplicate_local"
    return None


def sink_computation(lines):
    """Move a simple statement one position later (across an adjacent statement).

    Sinking a definition closer to its use shortens a live range; the allocator
    then has the register free across the skipped statement. Only moves across a
    statement that does not reference the moved statement's defined name.
    """
    depths = _brace_depth_map(lines)
    candidates = []
    for i in range(len(lines) - 1):
        if not (depths[i] == depths[i + 1] and depths[i] >= 1):
            continue
        if not (_is_simple_statement(lines[i])
                and _is_simple_statement(lines[i + 1])):
            continue
        # Determine the name defined by line i (decl or simple assign).
        dm = _DECL_INIT_RE.match(lines[i]) or _SIMPLE_ASSIGN_RE.match(lines[i])
        if not dm:
            continue
        defined = dm.group(3) if dm.re is _DECL_INIT_RE else dm.group(2)
        # Safe to sink only if the next statement does not USE the defined name
        # and does not redefine names that line i reads (conservative: require
        # next statement to neither read nor write ``defined``).
        if _word_re(defined).search(lines[i + 1]):
            continue
        candidates.append(i)
    if not candidates:
        return None
    i = random.choice(candidates)
    out = list(lines)
    out[i], out[i + 1] = out[i + 1], out[i]
    return out, "rg_sink_compute"


def hoist_computation(lines):
    """Move a simple statement one position earlier (across an adjacent statement).

    The dual of ``sink_computation``: lengthens a live range / changes the order
    in which values become live, which re-pressures the allocator. Only hoists
    across a statement whose defined name the moved statement does not read.
    """
    depths = _brace_depth_map(lines)
    candidates = []
    for i in range(1, len(lines)):
        if not (depths[i] == depths[i - 1] and depths[i] >= 1):
            continue
        if not (_is_simple_statement(lines[i])
                and _is_simple_statement(lines[i - 1])):
            continue
        prev = _DECL_INIT_RE.match(lines[i - 1]) or _SIMPLE_ASSIGN_RE.match(lines[i - 1])
        if not prev:
            continue
        prev_def = prev.group(3) if prev.re is _DECL_INIT_RE else prev.group(2)
        # The statement we hoist must not depend on the value defined just above.
        if _word_re(prev_def).search(lines[i]):
            continue
        candidates.append(i)
    if not candidates:
        return None
    i = random.choice(candidates)
    out = list(lines)
    out[i], out[i - 1] = out[i - 1], out[i]
    return out, "rg_hoist_compute"


def rename_local(lines):
    """Rename a local variable throughout its declaring block.

    Renaming has no semantic effect but can perturb SNC's allocation order in
    some builds (allocator tie-breaks have been observed to follow declaration
    order). Cheap to try; scored like any other mutation.
    """
    depths = _brace_depth_map(lines)
    decl_indices = []
    for i, line in enumerate(lines):
        if depths[i] < 1:
            continue
        m = _DECL_INIT_RE.match(line)
        if m and m.group(3) not in _RESERVED:
            decl_indices.append((i, m.group(3)))
        elif _is_declaration(line):
            # Uninitialized / plain declaration: grab the first identifier after
            # the type words.
            toks = _IDENT.findall(line)
            # Drop leading type/qualifier tokens.
            names = [t for t in toks if t not in _RESERVED]
            if names:
                decl_indices.append((i, names[0]))
    if not decl_indices:
        return None
    random.shuffle(decl_indices)
    for i, name in decl_indices:
        if name in _RESERVED or len(name) == 0:
            continue
        new = _fresh_name(lines, "v")
        if new is None:
            continue
        start, end = _block_range(lines, depths, i)
        word = _word_re(name)
        out = list(lines)
        changed = False
        for j in range(start, end):
            replaced = word.sub(new, out[j])
            if replaced != out[j]:
                out[j] = replaced
                changed = True
        if changed:
            return out, "rg_rename_local"
    return None


# ---------------------------------------------------------------------------
# Registry — the integrator extends mutations.MUTATIONS with these.
# ---------------------------------------------------------------------------

# Plain list of callables (mutations.py weights are applied at registration). The
# ordering reflects descending allocation impact for the locker problem.
REGALLOC_MUTATIONS = [
    duplicate_local,
    introduce_temporary,
    inline_temporary,
    sink_computation,
    hoist_computation,
    rename_local,
]


# ---------------------------------------------------------------------------
# Smoke test
# ---------------------------------------------------------------------------

_SAMPLE = """\
void gcDoEntitySpawn::operator=(gcDoEntitySpawn *other)
{
    unsigned int a = this->m_count;
    unsigned int b = other->m_count;
    unsigned int scratch = other->m_id;
    int idx = a + b;
    this->m_flags = idx;
    other->m_flags = a + b;
    foo(idx);
    bar(a);
    baz(scratch);
}
"""


def _smoke():
    lines = _SAMPLE.splitlines(keepends=True)
    failures = 0
    for fn in REGALLOC_MUTATIONS:
        # Try a handful of times since several mutations pick randomly.
        produced = None
        for _ in range(50):
            res = fn(list(lines))
            if res is not None:
                produced = res
                break
        if produced is None:
            print(f"[WARN] {fn.__name__}: no variant produced on sample")
            failures += 1
            continue
        mutated_lines, name = produced
        mutated = "".join(mutated_lines)
        assert isinstance(name, str) and name, f"{fn.__name__}: bad name"
        assert mutated != _SAMPLE, f"{fn.__name__}: produced identical source"
        print(f"[OK]   {fn.__name__} -> {name}")
        print("".join("    " + l for l in mutated_lines))
        print("-" * 60)
    if failures:
        raise SystemExit(
            f"{failures} regalloc mutation(s) produced nothing on the sample"
        )
    print("All regalloc mutations produced a distinct variant.")


if __name__ == "__main__":
    random.seed(1234)
    _smoke()
