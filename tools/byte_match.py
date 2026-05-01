"""Canonical byte-match verification. The ONE source of truth used by
compare_func, orchestrator, and verify_matches.

Given a function and a src file, answers: do the bytes of the symbol
inside the compiled .o match the EBOOT bytes at the function's address,
after relocation masking?

Returns a VerifyResult with a reason code for per-function outcomes.
Raises for tooling failures: compile errors (CompileFailed), nm/objcopy
failures, missing .o, short EBOOT read. Callers distinguish those from
verification failures when deciding whether to flip an entry to `failed`
vs `untried`.
"""

from __future__ import annotations

import functools
import os
import subprocess
import tempfile
from dataclasses import dataclass, field
from typing import Optional

from common import (
    EBOOT_PATH,
    NM,
    OBJCOPY,
    TEXT_FILE_OFFSET,
    get_all_code_relocations,
    get_text_relocations,
    mask_relocation_bytes,
)


class CompileFailed(RuntimeError):
    """A src_file did not compile. Distinct from generic tooling failures
    so callers can distinguish an agent-caused bad match (flip to failed)
    from a system problem (flip to untried).
    """


# Reason codes returned on failure. Kept short and stable so callers can
# branch on them without regex-matching human-readable text.
REASON_OK = "verified"
REASON_COMPILE_FAILED = "compile_failed"
REASON_NO_O_FILE = "no_o_file"
REASON_NO_SYMBOLS = "no_symbols_in_o"
REASON_NO_NAMED_SYMBOL = "no_symbol_with_matching_name"
REASON_BYTE_MISMATCH = "byte_mismatch"
REASON_AMBIGUOUS = "ambiguous_multiple_symbols_match"
REASON_SHORT_EBOOT_READ = "short_eboot_read"


# SNC operator codes (docs/research/snc-name-mangling.md §6.1).
_OP_CODES = {
    "operator+": "pl", "operator-": "mi", "operator*": "ml", "operator/": "dv",
    "operator%": "md", "operator=": "as", "operator==": "eq", "operator!=": "ne",
    "operator<": "lt", "operator>": "gt", "operator<=": "le", "operator>=": "ge",
    "operator+=": "apl", "operator-=": "ami", "operator*=": "amu",
    "operator/=": "adv", "operator%=": "amd", "operator&": "an", "operator|": "or",
    "operator^": "er", "operator~": "co", "operator!": "nt", "operator&&": "aa",
    "operator||": "oo", "operator<<": "ls", "operator>>": "rs",
    "operator<<=": "als", "operator>>=": "ars", "operator()": "cl",
    "operator[]": "vc", "operator->": "rf", "operator++": "pp",
    "operator--": "mm", "operator,": "cm",
    "operator new": "nw", "operator delete": "dl",
    "operator new[]": "nwa", "operator delete[]": "dla",
}


def sym_encodes_func(sym_name: str, func: dict) -> bool:
    """True iff sym_name mangles to the (class, method) pair in `func`.

    First checks for an exact match against the DB's authoritative
    `mangled_symbol` field (populated from the .sym file). Falls back
    to heuristic class/method substring matching for the rare entries
    without a mangled_symbol.

    Heuristic rules (SNC mangling from docs/research/snc-name-mangling.md):
      - Regular method: `<class><lenLetter><method>` with 1-3 char gap
      - Ctor: `<class>ct`
      - Dtor (patched post-splat): `<class>___dtor_<class>`
      - Member operator: `<class><op_code>`
      - Global new/delete: sym starts with `__0O<op_code>`
      - Free function: `<method>` as substring

    Unknown operators return False (we'd rather mark unverifiable than
    silently accept the wrong symbol).
    """
    # Exact match against authoritative mangled_symbol from .sym file.
    # Handles templates, nested classes, and other names the heuristic
    # can't substring-match.
    mangled = func.get("mangled_symbol")
    if mangled and sym_name == mangled:
        return True

    cls = func.get("class_name") or ""
    method = (func.get("method_name") or "").split("(", 1)[0]
    if not method:
        return False

    # Dtor: method_name begins with ~
    if method.startswith("~"):
        if not cls or method[1:] != cls:
            return False
        return (cls + "___dtor_" + cls) in sym_name or (cls + "dt") in sym_name

    # Ctor: method_name == class_name
    if cls and method == cls:
        return (cls + "ct") in sym_name

    # Operator: known table only
    if method.startswith("operator"):
        op_code = _OP_CODES.get(method)
        if not op_code:
            return False
        if cls:
            return (cls + op_code) in sym_name
        return sym_name.startswith(f"__0O{op_code}")

    # Regular method (instance / static): class then method with a short gap
    if cls:
        ci = sym_name.find(cls)
        if ci < 0:
            return False
        mi = sym_name.find(method, ci + len(cls))
        if mi < 0:
            return False
        gap = mi - (ci + len(cls))
        return 1 <= gap <= 3

    # Free function: just the method identifier as a substring
    return method in sym_name


@dataclass
class VerifyResult:
    """Outcome of a single byte-match check. Immutable record of everything
    downstream code (logging, DB updates, ambiguity reporting) needs."""

    ok: bool
    reason: str
    sym_name: Optional[str] = None
    o_file: Optional[str] = None
    ambiguous_syms: list[str] = field(default_factory=list)
    diff_count: int = 0
    byte_diffs: list[dict] = field(default_factory=list)  # orchestrator event schema


# ---------------------------------------------------------------------------
# Shared low-level helpers (used by compare_func too)
# ---------------------------------------------------------------------------


def compile_src(src_file: str, build_dir: str = "build/src",
                cwd: str = None) -> str:
    """Compile src_file via make; return the .o path.

    Verification must rebuild the target object even when Make thinks it
    is up to date. Otherwise a stale .o can make an old matched attempt
    pass after the committed source no longer emits matching bytes.

    `cwd` runs make inside a specific directory (Phase 3 shootout
    worktrees where the agent's src lives in an isolated checkout,
    not the orchestrator's main tree). None preserves pre-Phase-3
    behavior (make runs in the process's CWD).

    Raises CompileFailed when make exits non-zero (src/agent problem).
    Raises RuntimeError for other tooling issues (missing .o after
    successful make).
    """
    rel = src_file.lstrip("./")
    if rel.startswith("src/"):
        o_path = build_dir + "/" + rel[len("src/"):] + ".o"
    else:
        o_path = build_dir + "/" + os.path.basename(rel) + ".o"

    full_o = os.path.join(cwd, o_path) if cwd else o_path
    # `run_overnight.sh` executes agents as the `autodecomp` user, so
    # build/src can contain objects owned by another account. SNC opens
    # the output path for writing directly; if an old 0644 object exists,
    # verification fails with "Could not open output file" even though the
    # source compiles. Unlink first so the compiler creates a fresh object.
    try:
        os.unlink(full_o)
    except FileNotFoundError:
        pass
    except PermissionError:
        # Directory ACLs usually still allow deletion even when chmod would
        # fail. If unlink is genuinely blocked, leave the original make
        # error to report the exact failing path.
        pass

    result = subprocess.run(
        ["make", "-B", o_path], capture_output=True, text=True,
        cwd=cwd,
    )
    if result.returncode != 0:
        stderr = (result.stderr or result.stdout or "").strip()
        raise CompileFailed(
            f"compile failed for {src_file}: {stderr[:500]}"
        )
    if not os.path.exists(full_o):
        raise RuntimeError(
            f"make succeeded for {src_file} but {full_o} not found"
        )
    return full_o


def nm_symbols(path: str, *, defined_only: bool = False) -> list[tuple[int, str, str]]:
    """Return [(addr, type_code, sym_name)] from nm. Caller filters by type.

    Raises RuntimeError on nm failure or non-hex address (the only line
    shape we recognize is the standard `addr type name` triple).
    """
    cmd = [NM]
    if defined_only:
        cmd.append("--defined-only")
    cmd.append(path)
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"nm failed on {path}: {result.stderr.strip()}")
    syms: list[tuple[int, str, str]] = []
    for line in result.stdout.splitlines():
        parts = line.split(None, 2)
        if len(parts) != 3:
            continue
        addr_hex, type_code, sym = parts
        try:
            addr = int(addr_hex, 16)
        except ValueError:
            raise RuntimeError(
                f"nm produced a non-hex address on {path}: {line!r}"
            )
        syms.append((addr, type_code, sym))
    return syms


def nm_text_symbols(o_path: str) -> list[tuple[int, str]]:
    """Return sorted (offset, sym_name) for T-type symbols in o_path.

    Note: does NOT include W (weak) symbols from .gnu.linkonce.t.* sections.
    Use symbols_with_bytes_and_relocs() for the full picture including
    template instantiations.
    """
    syms = [(addr, name) for addr, t, name in nm_symbols(o_path) if t == "T"]
    syms.sort()
    return syms


def extract_text_section(o_path: str) -> bytes:
    """Extract raw .text section bytes from an .o file via objcopy.

    Returns empty bytes if the .text section does not exist (e.g., W-only .o).
    Raises RuntimeError on other objcopy failures.
    """
    return extract_section(o_path, ".text")


def extract_section(o_path: str, section_name: str) -> bytes:
    """Extract raw bytes of a named section from an .o file via objcopy.

    Returns empty bytes if the section doesn't exist.
    Raises RuntimeError on objcopy failure (other than missing section).
    """
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as tmp:
        tmp_path = tmp.name
    try:
        result = subprocess.run(
            [OBJCOPY, "-O", "binary", "-j", section_name, o_path, tmp_path],
            capture_output=True, text=True,
        )
        if result.returncode != 0:
            # objcopy exits non-zero when the section doesn't exist; return
            # empty bytes rather than raising (the caller may be probing for
            # optional linkonce sections).
            if "can't find" in (result.stderr or "").lower() or \
               "no such" in (result.stderr or "").lower():
                return b""
            raise RuntimeError(
                f"objcopy failed on {o_path} section {section_name}: "
                f"{result.stderr.strip()}"
            )
        with open(tmp_path, "rb") as f:
            return f.read()
    finally:
        if os.path.exists(tmp_path):
            os.unlink(tmp_path)


def symbols_with_bytes(o_path: str) -> dict[str, tuple[bytes, int]]:
    """Return {sym_name: (bytes_from_sym_start_to_next, start_offset)}.

    Thin wrapper combining nm + objcopy. Callers usually want this shape.
    Only includes T (text) symbols; use symbols_with_bytes_and_relocs()
    for the full picture including W (weak / template) symbols.
    """
    text_bytes = extract_text_section(o_path)
    syms = nm_text_symbols(o_path)
    if not syms:
        return {}
    out: dict[str, tuple[bytes, int]] = {}
    for i, (off, name) in enumerate(syms):
        end = syms[i + 1][0] if i + 1 < len(syms) else len(text_bytes)
        out[name] = (text_bytes[off:end], off)
    return out


def symbols_with_bytes_and_relocs(
    o_path: str,
) -> dict[str, tuple[bytes, int, list[tuple[int, int]]]]:
    """Return {sym_name: (func_bytes, start_offset, relocations)}.

    Like symbols_with_bytes but also:
      - Includes W (weak) symbols from .gnu.linkonce.t.* sections (template
        instantiations that SNC places in their own linkonce sections).
      - Returns per-symbol relocations already adjusted to be function-relative.

    For T symbols:  bytes come from .text, relocs from .rel.text.
    For W symbols:  bytes come from .gnu.linkonce.t.<sym>, relocs from the
                    corresponding .rel.gnu.linkonce.t.<sym>.
    """
    all_nm = nm_symbols(o_path)

    # -- T symbols from .text --
    t_syms = sorted([(addr, name) for addr, t, name in all_nm if t == "T"])
    text_bytes = extract_text_section(o_path) if t_syms else b""
    all_relocs = get_all_code_relocations(o_path)
    text_relocs = all_relocs.get(".rel.text", [])

    out: dict[str, tuple[bytes, int, list[tuple[int, int]]]] = {}

    for i, (off, name) in enumerate(t_syms):
        end = t_syms[i + 1][0] if i + 1 < len(t_syms) else len(text_bytes)
        sym_bytes = text_bytes[off:end]
        sym_size = end - off
        sym_relocs = [
            (r_off - off, r_type)
            for r_off, r_type in text_relocs
            if off <= r_off < off + sym_size
        ]
        out[name] = (sym_bytes, off, sym_relocs)

    # -- W symbols from .gnu.linkonce.t.* sections --
    w_syms = [(addr, name) for addr, t, name in all_nm if t == "W"]
    for _addr, name in w_syms:
        section = f".gnu.linkonce.t.{name}"
        section_bytes = extract_section(o_path, section)
        if not section_bytes:
            import sys
            print(f"WARNING: W symbol {name!r} has no {section} in {o_path}; skipping",
                  file=sys.stderr)
            continue
        rel_section = f".rel.gnu.linkonce.t.{name}"
        sym_relocs = all_relocs.get(rel_section, [])
        # W symbols always start at offset 0 within their section;
        # relocations are already function-relative.
        out[name] = (section_bytes, 0, sym_relocs)

    return out


_EBOOT_CACHE: Optional[bytes] = None


def _eboot() -> bytes:
    """Lazy-load the whole EBOOT once. Avoids ~1000 open+seek cycles in
    bulk audits. The binary is ~4 MB; holding it is trivial."""
    global _EBOOT_CACHE
    if _EBOOT_CACHE is None:
        with open(EBOOT_PATH, "rb") as f:
            _EBOOT_CACHE = f.read()
    return _EBOOT_CACHE


def read_eboot_bytes(addr: int, size: int) -> bytes:
    """Read `size` bytes at virtual `addr` from the EBOOT .text section."""
    start = addr + TEXT_FILE_OFFSET
    data = _eboot()[start:start + size]
    if len(data) != size:
        raise RuntimeError(
            f"short EBOOT read at {addr:#x}: got {len(data)}/{size} bytes"
        )
    return data


# Per-.o caches keyed by (path, mtime) so re-verifying many functions in
# the same .o (bulk audit) doesn't re-run nm/objcopy/readelf for each.
@functools.lru_cache(maxsize=None)
def _cached_symbols_with_bytes_and_relocs(
    o_path: str, _mtime: float,
) -> dict[str, tuple[bytes, int, list[tuple[int, int]]]]:
    return symbols_with_bytes_and_relocs(o_path)


def _mtime(path: str) -> float:
    return os.path.getmtime(path)


def find_db_func_for_sym(sym_name: str, functions: list[dict]) -> Optional[dict]:
    """Resolve a compiled symbol to the ONE DB function whose mangled form
    encodes it. Inverse of `sym_encodes_func`.

    Scans the DB linearly — caller can skip the scan by pre-indexing if
    it matters. Returns None if no DB entry's (class, method) encodes
    this sym_name (e.g., free functions not yet in the DB, or mangled
    forms the encoder doesn't recognize).
    """
    for f in functions:
        if f.get("size", 0) <= 0:
            continue
        if sym_encodes_func(sym_name, f):
            return f
    return None


# ---------------------------------------------------------------------------
# Canonical check
# ---------------------------------------------------------------------------


def check_byte_match(func: dict, src_file: str,
                     cwd: str = None,
                     o_path: str = None) -> VerifyResult:
    """Verify `func` against the bytes produced by compiling `src_file`.

    `cwd` (Phase 3 shootout): compile inside a worktree where the
    agent's src lives. None = compile in process CWD (Phase 1/2).

    Algorithm (name-gated, no silent fallback):
      1. Compile src_file (raise on failure), unless caller passed a fresh
         o_path already built for this src_file.
      2. Read every text symbol's bytes from the resulting .o.
      3. Filter to symbols whose mangled form encodes func's (class, method)
         per sym_encodes_func. This is the key gate against the old
         first-byte-match-wins bug.
      4. If zero named candidates: NO_MATCHING_SYMBOL.
      5. Read EBOOT bytes at func.address with func.size.
      6. Apply relocation masking within each named candidate, compare.
      7. If any named candidate byte-matches: verified (return its sym).
         (Multiple named candidates with matching bytes would be true
         overload ambiguity — returned as AMBIGUOUS; extremely rare since
         overloads with identical bodies still differ by sym_name.)
      8. Otherwise: BYTE_MISMATCH with the closest-diff summary.

    Raises RuntimeError on tooling failures (compile error, nm/objcopy
    failure, missing .o, short EBOOT read). Never silently returns OK.
    """
    addr = int(func["address"], 16)
    size = int(func["size"])

    if o_path is None:
        o_path = compile_src(src_file, cwd=cwd)
    syms = _cached_symbols_with_bytes_and_relocs(o_path, _mtime(o_path))
    if not syms:
        return VerifyResult(ok=False, reason=REASON_NO_SYMBOLS, o_file=o_path)

    # Name gate: only consider symbols that could legitimately be `func`.
    named_candidates = {
        sn: (sb, so, sr)
        for sn, (sb, so, sr) in syms.items()
        if sym_encodes_func(sn, func)
    }
    if not named_candidates:
        return VerifyResult(
            ok=False,
            reason=REASON_NO_NAMED_SYMBOL,
            o_file=o_path,
        )

    expected = read_eboot_bytes(addr, size)

    matches: list[str] = []
    best: Optional[tuple[str, int, list[dict]]] = None

    for sym_name, (sym_bytes, sym_off, sym_relocs) in named_candidates.items():
        if len(sym_bytes) < size:
            continue

        compiled = sym_bytes[:size]

        # sym_relocs are already function-relative for W symbols (offset 0).
        # For T symbols they were pre-adjusted in symbols_with_bytes_and_relocs.
        # Filter to only those within this function's size.
        func_relocs = [
            (off, rtype) for off, rtype in sym_relocs
            if 0 <= off < size
        ]
        if func_relocs:
            compiled_m = mask_relocation_bytes(compiled, func_relocs)
            expected_m = mask_relocation_bytes(expected, func_relocs)
        else:
            compiled_m = compiled
            expected_m = expected

        if compiled_m == expected_m:
            matches.append(sym_name)
        else:
            diffs = build_byte_diffs(compiled_m, expected_m, addr)
            word_diff_count = sum(1 for d in diffs if d["offset"] >= 0)
            if best is None or word_diff_count < best[1]:
                best = (sym_name, word_diff_count, diffs)

    if len(matches) == 1:
        return VerifyResult(
            ok=True,
            reason=REASON_OK,
            sym_name=matches[0],
            o_file=o_path,
        )

    if len(matches) > 1:
        # Multiple byte-matching candidates passed the substring gate
        # (overload twins that compile to identical bytes). Disambiguate
        # by exact mangled-name match against the authoritative .sym
        # mapping when available. Only converts AMBIGUOUS → OK; never the
        # reverse.
        mangled = func.get("mangled_symbol")
        if mangled and mangled in matches:
            return VerifyResult(
                ok=True,
                reason=REASON_OK,
                sym_name=mangled,
                o_file=o_path,
            )
        return VerifyResult(
            ok=False,
            reason=REASON_AMBIGUOUS,
            o_file=o_path,
            ambiguous_syms=matches,
        )

    if best is not None:
        sym, diff_count, diffs = best
        return VerifyResult(
            ok=False,
            reason=REASON_BYTE_MISMATCH,
            sym_name=sym,
            o_file=o_path,
            diff_count=diff_count,
            byte_diffs=diffs,
        )

    return VerifyResult(ok=False, reason=REASON_BYTE_MISMATCH, o_file=o_path)


def build_byte_diffs(compiled: bytes, expected: bytes, base_addr: int,
                     max_words: int = 8) -> list[dict]:
    """Build a list of dicts describing each mismatched 4-byte word, matching
    the orchestrator's existing jsonl `byte_diffs` schema.

    Capped at `max_words` entries (plus a truncation marker) to keep logs
    bounded. Adds a size-mismatch sentinel if lengths differ.
    """
    diffs: list[dict] = []
    total = 0
    length = min(len(compiled), len(expected))
    for off in range(0, length - 3, 4):
        c = compiled[off:off + 4]
        e = expected[off:off + 4]
        if c != e:
            total += 1
            if len(diffs) < max_words:
                diffs.append({
                    "offset": off,
                    "addr": f"0x{base_addr + off:08x}",
                    "compiled": f"0x{int.from_bytes(c, 'little'):08x}",
                    "expected": f"0x{int.from_bytes(e, 'little'):08x}",
                })

    if len(compiled) != len(expected):
        diffs.append({
            "offset": -1,
            "addr": "size_mismatch",
            "compiled": f"{len(compiled)}B",
            "expected": f"{len(expected)}B",
        })

    if total > max_words:
        diffs.append({
            "offset": -2,
            "addr": "truncated",
            "compiled": f"{total} total mismatched words",
            "expected": f"only first {max_words} shown",
        })

    return diffs
