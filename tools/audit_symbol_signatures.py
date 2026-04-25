#!/usr/bin/env python3
"""Audit currently-matched DB entries for signature drift.

For each matched function: parse the parameter list from the DB's
demangled `name` field (the original game's authoritative signature)
and from the function definition in the reconstructed src file. Compare.

Categorizes each matched entry as:
  ok            — signatures match (or trivial whitespace-only diff)
  param-count   — different number of parameters (concrete bug)
  param-types   — same count but at least one parameter type differs
  src-not-found — src_file isn't on disk or doesn't contain the function
  parse-error   — couldn't parse one or both signatures
  no-src        — entry is matched but has no src_file recorded

Output is a JSON report at logs/symbol_signature_audit.json by default,
plus a one-line summary on stdout. Add --print-mismatches to dump every
non-ok entry to stdout for human triage.
"""

import argparse
import functools
import json
import os
import re
import sys
from collections import Counter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import filter_functions, load_db


def split_params(param_str):
    """Split a C++-style parameter list, respecting nested <>, (), and []."""
    params = []
    depth = 0
    cur = []
    for ch in param_str:
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1
        if ch == "," and depth == 0:
            params.append("".join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    last = "".join(cur).strip()
    if last:
        params.append(last)
    return params


_CV_QUALIFIERS = {"const", "volatile"}
_PRIMITIVES = {"int", "char", "short", "long", "float", "double",
               "void", "bool", "wchar_t", "unsigned", "signed"}
_KEYWORDS_KEPT = _PRIMITIVES | _CV_QUALIFIERS


def normalize_type(t):
    """Canonicalize a C++ type string for cross-source comparison.

    Equivalences applied:
      bool ≡ _Bool                        (C interop)
      struct X * ≡ X *                    (struct prefix optional in C++)
      unsigned const X ≡ const unsigned X (CV-qualifier order)
      void (*name)(...) ≡ void (*)(...)   (function-pointer param names)
      "T name" ≡ "T"                      (top-level parameter names)
    """
    t = re.sub(r"\s+", " ", t).strip()
    t = t.replace("_Bool", "bool")
    t = re.sub(r"\bstruct\s+", "", t)

    # Strip parameter name inside function-pointer syntax: ( *name)
    t = re.sub(r"\(\s*\*\s*[A-Za-z_]\w*\s*\)", "( *)", t)

    # Strip top-level param-name (trailing identifier after a type token).
    m = re.match(r"^(.*?[\*&\s>])\s*([a-zA-Z_]\w*)\s*$", t)
    if m and m.group(2) not in _KEYWORDS_KEPT:
        t = m.group(1).strip()

    # CV-qualifier-order canonicalization at the *outer* type.
    parts = t.split()
    head = []
    while parts and parts[0] in _KEYWORDS_KEPT:
        head.append(parts.pop(0))
    quals = sorted(p for p in head if p in _CV_QUALIFIERS)
    rest = [p for p in head if p not in _CV_QUALIFIERS]
    t = " ".join(quals + rest + parts)

    t = re.sub(r"\s*\*\s*", " *", t)
    t = re.sub(r"\s*&\s*", " &", t)
    t = re.sub(r"\s+", " ", t).strip()
    return t


def parse_demangled_params(name):
    """Extract param list from a demangled C++ function name.

    Returns list of normalized type strings, or None if parse failed.
    """
    # Find the matching parens for the param list. Walk from the end
    # because the name itself may contain parens (operator(), function
    # pointer types in template args).
    depth = 0
    end_paren = -1
    start_paren = -1
    for i in range(len(name) - 1, -1, -1):
        ch = name[i]
        if ch == ")":
            if depth == 0:
                end_paren = i
            depth += 1
        elif ch == "(":
            depth -= 1
            if depth == 0:
                start_paren = i
                break
    if start_paren < 0 or end_paren < 0:
        return None
    inner = name[start_paren + 1:end_paren]
    if inner.strip() in ("", "void"):
        return []
    return [normalize_type(p) for p in split_params(inner)]


def strip_comments(src):
    """Remove // line and /* block */ comments. Preserves line numbers
    by replacing comment chars with spaces."""
    out = []
    i = 0
    n = len(src)
    while i < n:
        c = src[i]
        if c == "/" and i + 1 < n and src[i + 1] == "/":
            j = src.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c == "/" and i + 1 < n and src[i + 1] == "*":
            j = src.find("*/", i + 2)
            if j < 0:
                out.append(" " * (n - i))
                i = n
            else:
                for k in range(i, j + 2):
                    out.append("\n" if src[k] == "\n" else " ")
                i = j + 2
        elif c == '"' or c == "'":
            # Skip string-literal contents so an embedded // doesn't trigger.
            quote = c
            out.append(c)
            i += 1
            while i < n:
                out.append(src[i])
                if src[i] == "\\" and i + 1 < n:
                    out.append(src[i + 1])
                    i += 2
                    continue
                if src[i] == quote:
                    i += 1
                    break
                i += 1
        else:
            out.append(c)
            i += 1
    return "".join(out)


@functools.lru_cache(maxsize=None)
def _load_stripped(src_path):
    """Cached read+strip of a src file. Returns None if the file isn't
    on disk; otherwise the comment-stripped contents.
    """
    if not os.path.exists(src_path):
        return None
    with open(src_path, encoding="utf-8") as f:
        return strip_comments(f.read())


def find_all_function_defs(src_path, class_name, method_name):
    """Return a list of param-lists for every definition of class::method
    in src_path. One entry per overload. Returns None if file is missing.

    Recognizes the canonical C++ form plus the legacy renamed shapes the
    project accumulated during earlier tooling eras:
      Class::method                — canonical C++
      Class::~Class                — dtor
      Class___dtor_Class           — patched dtor (post-splat)
      Class__method                — safe-name extern-C (older tooling era)
      Class_method                 — single-underscore safe-name variant
      method                       — bare method, ONLY when class_name is empty

    The bare-method fallback is intentionally restricted to free
    functions: cross-class collisions would silently misclassify (e.g.
    `Init` exists in many classes — matching it from another class's
    src file would be a wrong answer reported as "ok").
    """
    src = _load_stripped(src_path)
    if src is None:
        return None

    bare_method = method_name.lstrip("~")
    alternatives = []
    if class_name:
        cls_safe = class_name.replace("::", "_")
        alternatives.append(re.escape(class_name) + r"\s*::\s*" + re.escape(method_name))
        if method_name.startswith("~"):
            alternatives.append(re.escape(class_name) + r"___dtor_" + re.escape(class_name))
            alternatives.append(re.escape(class_name) + r"__dtor_" + re.escape(class_name))
        alternatives.append(re.escape(cls_safe) + r"__" + re.escape(bare_method) + r"(?:_\w+)?")
        alternatives.append(re.escape(cls_safe) + r"_" + re.escape(bare_method) + r"(?:_\w+)?")
    else:
        alternatives.append(r"(?<![A-Za-z0-9_:])" + re.escape(bare_method))

    pat = re.compile("(?:" + "|".join(alternatives) + r")\s*\(", re.MULTILINE)
    defs = []
    for m in pat.finditer(src):
        start = m.end() - 1
        depth = 0
        end = -1
        for i in range(start, len(src)):
            ch = src[i]
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth == 0:
                    end = i
                    break
        if end < 0:
            continue

        i = end + 1
        while i < len(src):
            if src[i].isspace():
                i += 1
                continue
            matched = False
            for kw in ("const", "volatile", "override", "final"):
                klen = len(kw)
                if (src.startswith(kw, i)
                        and (i + klen >= len(src)
                             or not (src[i + klen].isalnum()
                                     or src[i + klen] == "_"))):
                    i += klen
                    matched = True
                    break
            if not matched:
                break
        if i >= len(src) or src[i] != "{":
            continue

        inner = src[start + 1:end]
        if inner.strip() in ("", "void"):
            defs.append([])
        else:
            defs.append([normalize_type(p) for p in split_params(inner)])
    return defs


def find_function_in_src(src_path, class_name, method_name, expected_params):
    """Pick the overload of class::method in src_path whose param list
    matches expected_params (by length, then by exact type list).

    Returns (matched_params_list, error_str). On success, error_str is None.
    """
    defs = find_all_function_defs(src_path, class_name, method_name)
    if defs is None:
        return None, f"src_file not on disk: {src_path}"
    if not defs:
        return None, f"no definition of {class_name}::{method_name} in {src_path}"

    # Exact match (types align) first. Falls back to count-match for
    # diagnostics — but only one count-match counts as the "the" overload.
    exact = [d for d in defs if d == expected_params]
    if exact:
        return exact[0], None
    same_count = [d for d in defs if len(d) == len(expected_params)]
    if len(same_count) == 1:
        return same_count[0], None
    if len(same_count) > 1:
        return same_count[0], (
            f"ambiguous: {len(same_count)} overloads with "
            f"{len(expected_params)} params; picked the first"
        )
    # No overload with the expected param count. Return the closest one
    # by count for diagnostics.
    closest = min(defs, key=lambda d: abs(len(d) - len(expected_params)))
    return closest, (
        f"no overload with {len(expected_params)} params; "
        f"closest has {len(closest)}"
    )


def audit_entry(func):
    """Return a dict with audit findings for one matched DB entry."""
    name = func.get("name", "")
    cls = func.get("class_name")
    method_full = func.get("method_name") or ""
    # Splitting on `(` corrupts operator() and operator()(int) — punt
    # those into parse-error rather than misclassifying.
    if method_full.startswith("operator") and "(" in method_full[len("operator"):]:
        return {"address": func["address"], "name": name,
                "category": "parse-error",
                "detail": "operator overload — auditor doesn't model these"}
    method = method_full.split("(", 1)[0]
    src = func.get("src_file")
    addr = func["address"]

    if not src:
        return {"address": addr, "name": name, "category": "no-src"}

    expected = parse_demangled_params(name)
    if expected is None:
        return {"address": addr, "name": name, "category": "parse-error",
                "detail": "demangled name didn't yield a param list"}

    actual, err = find_function_in_src(src, cls or "", method, expected)
    if actual is None:
        return {"address": addr, "name": name, "category": "src-not-found",
                "detail": err}

    # Ambiguity (multiple overloads with the same param count) gets its
    # own category — even when the picked overload happens to match
    # `expected`, the auditor's choice was non-deterministic and a human
    # should pick which overload is actually at this address.
    if err and "ambiguous" in err:
        return {"address": addr, "name": name, "category": "ambiguous-overload",
                "src_file": src, "expected": expected, "actual": actual,
                "detail": err}

    if actual == expected:
        return {"address": addr, "name": name, "category": "ok"}

    if len(expected) != len(actual):
        return {"address": addr, "name": name, "category": "param-count",
                "src_file": src, "expected": expected, "actual": actual,
                "detail": err}

    type_diffs = [(i, e, a) for i, (e, a) in enumerate(zip(expected, actual))
                  if e != a]
    return {"address": addr, "name": name, "category": "param-types",
            "src_file": src, "expected": expected, "actual": actual,
            "diffs": type_diffs, "detail": err}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", default="logs/symbol_signature_audit.json",
                        help="JSON report destination")
    parser.add_argument("--print-mismatches", action="store_true",
                        help="Print every non-ok finding to stdout")
    args = parser.parse_args()

    db = load_db()
    matched = filter_functions(db, status="matched")
    print(f"Auditing {len(matched)} matched entries...")

    findings = [audit_entry(f) for f in matched]

    by_cat = Counter(r["category"] for r in findings)
    print("\nResults:")
    for cat, n in by_cat.most_common():
        print(f"  {cat:>15}: {n}")

    os.makedirs(os.path.dirname(args.out) or ".", exist_ok=True)
    with open(args.out, "w") as f:
        json.dump(findings, f, indent=2)
    print(f"\nFull report: {args.out}")

    if args.print_mismatches:
        print("\n--- Non-ok findings ---")
        for r in findings:
            if r["category"] == "ok":
                continue
            print(f"\n  [{r['category']}] {r['address']}  {r['name']}")
            if "src_file" in r:
                print(f"    src: {r['src_file']}")
            if "expected" in r:
                print(f"    expected: {r['expected']}")
                print(f"    actual  : {r['actual']}")
            if "diffs" in r:
                for i, e, a in r["diffs"]:
                    print(f"      param[{i}]: {e!r} vs {a!r}")
            if "detail" in r:
                print(f"    {r['detail']}")


if __name__ == "__main__":
    main()
