#!/usr/bin/env python3
"""Phase 3 promotion tool: cherry-pick winners off shootout branches
onto the main run-branch.

Reads `logs/attempts.jsonl` (Phase 2 schema) plus the shootout
branches the orchestrator created (`overnight/<ts>/<backend>-<modeltag>`)
to find which shootout commit matched each function. For each
function in the shootout/reserved set:

  * Both identities matched → operator picks the winner. Default is
    interactive; `--prefer <backend>` or `--auto-prefer <backend>`
    promote the named backend's commit without prompting.
  * Exactly one identity matched → that one's commit is promoted.
  * Neither matched → no promotion. Both shootout branches retain
    their `failed` records for analytics; the function stays
    `untried` in main, ready for a future run.

A "promotion" is a `git cherry-pick` of the shootout commit onto the
current main run-branch. The cherry-pick brings the src changes AND
the shootout's `functions.json` updates (the `matched_by_*` fields
the orchestrator's commit thread wrote inside the worktree). The
coordinator's `db_lock`-guarded merge in the orchestrator already
prevents concurrent same-function writes; cherry-picking is a
post-run operation, single-threaded by design.

**Cherry-pick conflicts abort loudly.** Two shootout commits matching
the same address from different identities is the typical cause —
the second cherry-pick conflicts on `config/functions.json`. We do
NOT auto-resolve (the operator must decide which match wins). The
tool prints the conflict and instructs `git cherry-pick --abort`,
leaving the tree unchanged so the operator can re-run with `--prefer`.

Usage:
    python3 tools/ab_promote.py                              # interactive
    python3 tools/ab_promote.py --prefer claude              # break ties for claude
    python3 tools/ab_promote.py --auto-prefer claude         # no prompts
    python3 tools/ab_promote.py --dry-run                    # plan only
    python3 tools/ab_promote.py --branch overnight/20260425  # explicit base branch
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path
from typing import Dict, List, Optional, Tuple

_REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(_REPO_ROOT / "tools"))

from ab_schedule import identity_key, safe_identity_tag

_GIT = "git"


def git_run(*args, cwd: Optional[Path] = None,
            check: bool = False) -> Tuple[int, str, str]:
    """Subprocess wrapper. Returns (rc, stdout, stderr). When `check`
    is True we raise on non-zero — used for ops where failure means
    the tool can't proceed safely (e.g., reading current branch).
    """
    proc = subprocess.run(
        [_GIT] + list(args),
        cwd=str(cwd) if cwd else None,
        capture_output=True, text=True
    )
    if check and proc.returncode != 0:
        raise RuntimeError(
            f"git {' '.join(args)} failed: {proc.stderr.strip()}"
        )
    return proc.returncode, proc.stdout, proc.stderr


def current_branch() -> str:
    rc, out, err = git_run("branch", "--show-current", check=True)
    return out.strip()


def scan_branch_promotions(branch: str
                            ) -> Dict[str, Tuple[str, str]]:
    """Scan one shootout branch once: returns `{address: (sha, session_id)}`
    for every match commit whose DB diff promotes that address.

    Single `git log -p` scan rather than per-address `git log` + per-
    candidate `git show` (which was K×M subprocesses for K addresses
    × M commits). Per-hunk parsing partitions co-mingled changes so
    one commit promoting multiple addresses still attributes each
    address correctly.
    """
    rc, out, err = git_run(
        "log", "-p", "--no-color", "--unified=15",
        "--format=__commit__%x1f%H%x1f%s%x1e",
        "--diff-filter=M",
        branch, "--", "config/functions.json",
    )
    if rc != 0:
        raise RuntimeError(
            f"git log -p on {branch} failed: {err}"
        )
    out_addrs: Dict[str, Tuple[str, str]] = {}
    matched_pat = '"match_status": "matched"'
    addr_re = re.compile(r'"address":\s*"(0x[0-9a-fA-F]+)"')
    sha = session_id = None
    current_hunk: Optional[List[str]] = None

    def flush(hunk: Optional[List[str]]):
        if hunk is None or sha is None or session_id is None:
            return
        body = "\n".join(hunk)
        if not any(l.startswith("+") and matched_pat in l for l in hunk):
            return
        for addr in addr_re.findall(body):
            # First-seen wins (newest-first git log order). Older
            # commits attempting the same address don't override.
            out_addrs.setdefault(addr, (sha, session_id))

    for line in out.splitlines():
        if line.startswith("__commit__"):
            flush(current_hunk)
            current_hunk = None
            parts = line.split("\x1f")
            if len(parts) < 3:
                continue
            sha = parts[1]
            subject = parts[2].rstrip("\x1e")
            m = re.match(
                r"^Match\s+\d+\s+functions\s+\(session\s+([0-9a-f]+)\)",
                subject)
            session_id = m.group(1) if m else None
        elif line.startswith("@@"):
            flush(current_hunk)
            current_hunk = [line]
        elif current_hunk is not None:
            current_hunk.append(line)
    flush(current_hunk)
    return out_addrs


def _diff_promotes_address_to_matched(diff: str, address: str) -> bool:
    """Per-hunk analysis: a hunk that mentions our address (in any
    form, context or added) AND has an added `match_status: matched`
    line counts as promoting that address. Address itself is a
    context line in the JSON pretty-print (added/removed for
    deletions only); using "any line" + "added matched" is the
    accurate signal.

    Conservative on multi-address commits: we still partition by
    `@@` hunk header so a hunk for address A doesn't claim it
    promoted address B.
    """
    addr_pat = f'"address": "{address}"'
    matched_pat = '"match_status": "matched"'
    hunks: List[List[str]] = []
    current: Optional[List[str]] = None
    for line in diff.splitlines():
        if line.startswith("@@"):
            if current is not None:
                hunks.append(current)
            current = [line]
        elif current is not None:
            current.append(line)
    if current is not None:
        hunks.append(current)
    for hunk in hunks:
        body = "\n".join(hunk)
        if addr_pat not in body:
            continue
        if any(l.startswith("+") and matched_pat in l for l in hunk):
            return True
    return False


def discover_shootout_branches(base: Optional[str]) -> Dict[str, str]:
    """Return `{identity_key: branch_name}` for branches matching
    `<base>/<backend>-<modeltag>`. When `base` is None we infer it
    from the latest `overnight/<ts>` ref (most recent committer date
    on a branch matching the convention).
    """
    if base is None:
        rc, out, err = git_run(
            "for-each-ref", "--format=%(refname:short)\t%(committerdate:iso8601)",
            "refs/heads/overnight/",
        )
        if rc != 0:
            raise RuntimeError(
                f"git for-each-ref on refs/heads/overnight/ failed: {err}"
            )
        if not out.strip():
            return {}
        rows = []
        for line in out.splitlines():
            parts = line.split("\t")
            if len(parts) < 2:
                continue
            name, date = parts[0], parts[1]
            # Base branches match `overnight/<ts>` exactly — shootout
            # siblings have a `-` after the timestamp.
            tail = name[len("overnight/"):]
            if "-" in tail.split("/", 1)[0] and tail.count("-") > 1:
                # Heuristic: timestamp is `YYYYMMDD-HHMMSS` (one dash);
                # shootout branches add another dash + tag. Skip those.
                continue
            rows.append((name, date))
        if not rows:
            return {}
        rows.sort(key=lambda r: r[1], reverse=True)
        base = rows[0][0]
    # Shootout branches are siblings of `<base>` (e.g. `<base>-<tag>`)
    # because git refs can't nest `<base>/...` if `<base>` itself is
    # a ref. The for-each-ref glob matches both the base and its
    # siblings; we filter to the latter.
    rc, out, err = git_run(
        "for-each-ref", "--format=%(refname:short)",
        f"refs/heads/{base}-*",
    )
    if rc != 0:
        raise RuntimeError(
            f"git for-each-ref on refs/heads/{base}-* failed: {err}"
        )
    branches = {}
    for line in out.splitlines():
        line = line.strip()
        if not line or line == base:
            continue
        suffix = line[len(base) + 1:]
        # Suffix is "<backend>-<modeltag>". We can't perfectly
        # round-trip to identity_key without the actual backend list,
        # but we can split on the first '-'. The smoke test exercises
        # this path with simple two-part suffixes; the real format
        # `claude-claude-opus-4_7` is harder. To stay accurate we
        # store the suffix verbatim and let the caller match it
        # against expected identity tags.
        branches[suffix] = line
    return branches


def _identity_to_branch_suffix(ident: str) -> str:
    return safe_identity_tag(ident)


def cherry_pick_one(sha: str, branch: str, dry_run: bool = False
                     ) -> Tuple[bool, str]:
    """Cherry-pick a single commit onto the current branch. Returns
    (ok, error_message). On conflict: aborts the cherry-pick (so the
    tree is left clean) and reports the conflicting paths to the
    operator. Forwards the operator-resolution choice to ab_promote
    callers — we never auto-resolve.
    """
    if dry_run:
        return True, f"(dry-run) would cherry-pick {sha[:10]}"
    rc, out, err = git_run("cherry-pick", "--allow-empty", sha)
    if rc == 0:
        return True, ""
    # Conflict path. Capture conflicting files for the abort message,
    # then run cherry-pick --abort to restore the tree.
    rc2, conflicts, err2 = git_run("diff", "--name-only", "--diff-filter=U")
    abort_rc, _, abort_err = git_run("cherry-pick", "--abort")
    if abort_rc != 0:
        # The repo is now stuck in CHERRY_PICKING state. Future git
        # commands will refuse until the operator clears it manually.
        # Raising here is the loudest signal we can give: don't let
        # the next commit/promote attempt run on a broken tree.
        raise RuntimeError(
            f"cherry-pick {sha[:10]} from {branch} conflicted AND "
            f"`git cherry-pick --abort` failed ({abort_err}). Repo is "
            f"in CHERRY_PICKING state — resolve manually before "
            f"continuing."
        )
    msg = (f"cherry-pick of {sha[:10]} from {branch} conflicted on:\n"
           f"  {conflicts.strip() or '(see git output)'}\n"
           f"git output:\n  {err.strip() or out.strip()}")
    return False, msg


def plan(attempts: List[dict], identities: List[str],
         shootout_branches: Dict[str, str],
         prefer: Optional[str] = None) -> List[dict]:
    """Build the list of cherry-pick actions.

    `attempts` is the parsed attempts.jsonl record list (so callers
    can read once and pass in — see ab_report.load_attempts).

    Each action entry:
      `{"address", "name", "winner_identity", "winner_sha",
        "winner_branch", "winner_session", "reason"}`
    or for ties without --prefer:
      `{"address", "name", "candidates": [{ident, sha, session, branch}],
        "needs_choice": True}`
    """
    by_addr: Dict[str, Dict[str, dict]] = defaultdict(dict)
    func_names: Dict[str, str] = {}
    for rec in attempts:
        ident = identity_key(rec.get("backend") or "",
                             rec.get("model") or "")
        if ident not in identities:
            continue
        addr = rec.get("address")
        if not addr:
            continue
        func_names[addr] = rec.get("name") or addr
        verified = rec.get("verified_status")
        cell = by_addr[addr].get(ident, {})
        # Last-write-wins so re-attempts collapse to current state.
        cell.update({
            "verified": verified,
            "session_id": rec.get("session_id"),
        })
        by_addr[addr][ident] = cell

    # Pre-scan each shootout branch once → {addr: (sha, session_id)}.
    # Single `git log -p` per branch beats K×M `git show` subprocesses
    # by orders of magnitude on real runs.
    branch_promotions: Dict[str, Dict[str, Tuple[str, str]]] = {}
    for suffix, branch in shootout_branches.items():
        branch_promotions[branch] = scan_branch_promotions(branch)

    actions: List[dict] = []
    for addr in sorted(by_addr):
        cells = by_addr[addr]
        winners = [ident for ident, c in cells.items()
                    if c.get("verified") == "matched"]
        if not winners:
            continue  # neither matched — no promotion
        candidates = []
        for ident in winners:
            suffix = _identity_to_branch_suffix(ident)
            branch = shootout_branches.get(suffix)
            if not branch:
                actions.append({
                    "address": addr,
                    "name": func_names.get(addr, addr),
                    "error": f"no shootout branch found for {ident} "
                             f"(expected suffix {suffix!r})",
                })
                break
            promo = branch_promotions[branch].get(addr)
            if promo is None:
                actions.append({
                    "address": addr,
                    "name": func_names.get(addr, addr),
                    "error": f"no commit on {branch} found promoting {addr}",
                })
                break
            sha, session_id = promo
            candidates.append({
                "identity": ident, "sha": sha, "branch": branch,
                "session_id": session_id,
            })
        else:
            if len(candidates) == 1:
                c = candidates[0]
                actions.append({
                    "address": addr,
                    "name": func_names.get(addr, addr),
                    "winner_identity": c["identity"],
                    "winner_sha": c["sha"],
                    "winner_branch": c["branch"],
                    "winner_session": c["session_id"],
                    "reason": "single match",
                })
            else:
                # Tie — apply --prefer if it picks one of these.
                pref_match = next((c for c in candidates
                                    if prefer
                                    and c["identity"].split("/", 1)[0] == prefer),
                                  None)
                if pref_match:
                    actions.append({
                        "address": addr,
                        "name": func_names.get(addr, addr),
                        "winner_identity": pref_match["identity"],
                        "winner_sha": pref_match["sha"],
                        "winner_branch": pref_match["branch"],
                        "winner_session": pref_match["session_id"],
                        "reason": f"--prefer {prefer}",
                    })
                else:
                    actions.append({
                        "address": addr,
                        "name": func_names.get(addr, addr),
                        "candidates": candidates,
                        "needs_choice": True,
                    })
    return actions


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    ap.add_argument("--attempts", default="logs/attempts.jsonl")
    ap.add_argument("--branch", default=None,
                    help="Base run-branch (default: latest overnight/<ts>).")
    ap.add_argument("--identities", default=None,
                    help="Comma-separated identities like "
                         "'claude/claude-opus-4-7,codex/gpt-5.5'. Default: "
                         "infer from attempts.jsonl.")
    ap.add_argument("--prefer", default=None,
                    help="Backend to prefer when both identities matched.")
    ap.add_argument("--auto-prefer", default=None,
                    help="Same as --prefer, but skip the interactive "
                         "confirmation entirely.")
    ap.add_argument("--dry-run", action="store_true",
                    help="Print the plan; don't cherry-pick.")
    ap.add_argument("--yes", action="store_true",
                    help="Skip the per-action confirmation prompt.")
    args = ap.parse_args()

    if args.prefer and args.auto_prefer and args.prefer != args.auto_prefer:
        print("error: --prefer and --auto-prefer disagree", file=sys.stderr)
        sys.exit(2)
    prefer = args.auto_prefer or args.prefer
    auto = bool(args.auto_prefer) or args.yes

    attempts_path = Path(args.attempts)
    if not attempts_path.exists():
        print(f"error: {attempts_path} not found", file=sys.stderr)
        sys.exit(2)

    # Single pass over attempts.jsonl: load once, derive identities
    # from it, then pass the parsed list into plan(). Re-reading the
    # file would matter on multi-GB logs — even on small ones the
    # double-parse is wasted work.
    attempts: List[dict] = []
    with open(attempts_path) as f:
        for lineno, line in enumerate(f, 1):
            line = line.strip()
            if not line:
                continue
            try:
                attempts.append(json.loads(line))
            except json.JSONDecodeError as e:
                raise RuntimeError(
                    f"{attempts_path}:{lineno}: malformed attempts.jsonl: {e}"
                )

    if args.identities:
        identities = [s.strip() for s in args.identities.split(",")
                      if s.strip()]
    else:
        idents = {identity_key(r.get("backend") or "",
                                r.get("model") or "")
                  for r in attempts}
        identities = sorted(i for i in idents if i and "/" in i)

    if not identities:
        print("no identities found in attempts.jsonl — nothing to promote")
        return

    shootout_branches = discover_shootout_branches(args.branch)
    if not shootout_branches:
        print("error: no shootout branches found. Pass --branch <run-branch> "
              "or run after an overnight shootout.", file=sys.stderr)
        sys.exit(2)

    # Orphan/missing audit: every identity should have a branch and
    # every discovered branch should match an identity. Mismatches
    # mean the operator's --identities arg is wrong, the run was
    # interrupted, or branches were renamed manually.
    expected_suffixes = {_identity_to_branch_suffix(i) for i in identities}
    discovered_suffixes = set(shootout_branches.keys())
    missing = expected_suffixes - discovered_suffixes
    orphan = discovered_suffixes - expected_suffixes
    if missing:
        print(f"warning: identities with no shootout branch: "
              f"{sorted(missing)}", file=sys.stderr)
    if orphan:
        print(f"warning: shootout branches not matching any identity: "
              f"{sorted(orphan)}", file=sys.stderr)

    print(f"Identities: {', '.join(identities)}")
    print(f"Shootout branches:")
    for suffix, br in sorted(shootout_branches.items()):
        print(f"  {suffix:30s} → {br}")
    print()

    actions = plan(attempts, identities, shootout_branches,
                    prefer=prefer)
    if not actions:
        print("Nothing to promote.")
        return

    promoted = []
    skipped = []
    for action in actions:
        addr = action["address"]
        name = action.get("name", addr)
        if action.get("error"):
            skipped.append((action, action["error"]))
            print(f"  SKIP {addr} {name[:40]:40s}  {action['error']}")
            continue
        if action.get("needs_choice"):
            options = action["candidates"]
            print(f"  AMBIG {addr} {name[:40]:40s}  matched by:")
            for c in options:
                print(f"      {c['identity']}  ({c['sha'][:10]} on {c['branch']})")
            if auto:
                # Auto-mode without --prefer hitting one of the
                # candidates: skip the ambiguous case rather than
                # picking arbitrarily.
                skipped.append((action, "ambiguous, no --prefer match"))
                continue
            choice = input(f"    pick identity (or empty to skip): ").strip()
            chosen = next((c for c in options
                            if c["identity"] == choice
                            or c["identity"].split("/", 1)[0] == choice),
                           None)
            if chosen is None:
                skipped.append((action, "operator skipped"))
                continue
            action = {**action, "winner_identity": chosen["identity"],
                      "winner_sha": chosen["sha"],
                      "winner_branch": chosen["branch"],
                      "winner_session": chosen["session_id"],
                      "reason": "operator choice",
                      "needs_choice": False}
        ident = action["winner_identity"]
        sha = action["winner_sha"]
        branch = action["winner_branch"]
        print(f"  PROMOTE {addr} {name[:40]:40s}  ←  {ident:30s}  "
              f"({sha[:10]} on {branch}, {action.get('reason', '')})")
        if not args.dry_run:
            if not auto:
                confirm = input("    cherry-pick? [Y/n]: ").strip().lower()
                if confirm and confirm != "y":
                    skipped.append((action, "operator skipped"))
                    continue
            ok, err = cherry_pick_one(sha, branch, dry_run=False)
            if not ok:
                print(err, file=sys.stderr)
                print("\nABORTED. Tree restored. Re-run with --prefer to "
                      "resolve, or resolve the conflict manually.",
                      file=sys.stderr)
                sys.exit(3)
        promoted.append(action)

    print()
    print(f"Promoted: {len(promoted)} function(s)")
    print(f"Skipped : {len(skipped)} function(s)")
    if args.dry_run:
        print("(dry-run — no commits were cherry-picked)")


if __name__ == "__main__":
    main()
