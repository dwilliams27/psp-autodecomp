#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
Usage: scripts/sync_agent_skills.sh [--check]

Installs repo-local Claude skills into Codex's user skill directory as symlinks.

Source of truth:
  .claude/skills/<skill>/SKILL.md

Codex discovery target:
  ${CODEX_HOME:-$HOME/.codex}/skills/<skill>

Options:
  --check   Verify links are present and correct without changing anything.
USAGE
}

mode="install"
if [[ "${1:-}" == "--check" ]]; then
    mode="check"
elif [[ "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
    usage
    exit 0
elif [[ $# -gt 0 ]]; then
    usage >&2
    exit 2
fi

repo_root="$(git rev-parse --show-toplevel)"
source_root="$repo_root/.claude/skills"
codex_home="${CODEX_HOME:-$HOME/.codex}"
target_root="$codex_home/skills"

if [[ ! -d "$source_root" ]]; then
    echo "ERROR: missing source skill directory: $source_root" >&2
    exit 1
fi

status=0
if [[ "$mode" == "install" ]]; then
    mkdir -p "$target_root"
elif [[ ! -d "$target_root" ]]; then
    echo "ERROR: missing Codex skills directory: $target_root" >&2
    exit 1
fi

found=0
for source_dir in "$source_root"/*; do
    [[ -d "$source_dir" ]] || continue
    if [[ ! -f "$source_dir/SKILL.md" ]]; then
        echo "ERROR: skill directory is missing SKILL.md: $source_dir" >&2
        status=1
        continue
    fi

    found=1
    name="$(basename "$source_dir")"
    target_dir="$target_root/$name"

    if [[ -L "$target_dir" ]]; then
        current_target="$(readlink "$target_dir")"
        if [[ "$current_target" == "$source_dir" ]]; then
            echo "OK: $name -> $source_dir"
            continue
        fi
        echo "ERROR: $target_dir points to $current_target, expected $source_dir" >&2
        status=1
        continue
    fi

    if [[ -e "$target_dir" ]]; then
        echo "ERROR: $target_dir exists and is not the managed symlink" >&2
        echo "       Move it aside or delete it manually, then rerun this script." >&2
        status=1
        continue
    fi

    if [[ "$mode" == "check" ]]; then
        echo "MISSING: $target_dir -> $source_dir" >&2
        status=1
    else
        ln -s "$source_dir" "$target_dir"
        echo "LINKED: $name -> $source_dir"
    fi
done

for target_dir in "$target_root"/*; do
    [[ -L "$target_dir" ]] || continue
    current_target="$(readlink "$target_dir")"
    case "$current_target" in
        "$source_root"/*)
            if [[ ! -f "$current_target/SKILL.md" ]]; then
                echo "ERROR: stale Codex skill link: $target_dir -> $current_target" >&2
                status=1
            fi
            ;;
    esac
done

if [[ "$found" -eq 0 ]]; then
    echo "ERROR: no skills with SKILL.md found under $source_root" >&2
    exit 1
fi

if [[ "$status" -ne 0 ]]; then
    exit "$status"
fi

if [[ "$mode" == "install" ]]; then
    echo
    echo "Codex skill links are up to date. Start a new Codex session to refresh the active skill list."
fi
