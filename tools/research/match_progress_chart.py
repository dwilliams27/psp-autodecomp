#!/usr/bin/env python3
"""Chart matched-function and matched-byte totals over time.

Data source:
- current config/functions.json for the set of currently matched functions and
  their sizes;
- earliest verified matched timestamp from logs/match_*.jsonl function_result
  events, falling back to config/functions.json matched_at where present.

Functions that are currently matched but have no timestamp in either source are
placed in the initial baseline bucket and called out in the chart note.
"""

from __future__ import annotations

import argparse
import csv
import glob
import json
import os
import tempfile
from collections import defaultdict
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[2]
FUNCTION_DB = REPO_ROOT / "config/functions.json"
MATCH_LOG_GLOB = str(REPO_ROOT / "logs/match_*.jsonl")


@dataclass(frozen=True)
class Point:
    timestamp: datetime
    matched_functions: int
    matched_bytes: int
    added_functions: int
    added_bytes: int
    source: str


def parse_timestamp(value: str) -> datetime:
    if value.endswith("Z"):
        value = value[:-1] + "+00:00"
    timestamp = datetime.fromisoformat(value)
    if timestamp.tzinfo is not None:
        timestamp = timestamp.astimezone().replace(tzinfo=None)
    return timestamp


def load_current_matched() -> dict[str, dict]:
    functions = json.loads(FUNCTION_DB.read_text())
    return {
        f["address"]: f
        for f in functions
        if f.get("match_status") == "matched" and f.get("address")
    }


def earliest_log_matches(current_matched: dict[str, dict]) -> dict[str, datetime]:
    earliest: dict[str, datetime] = {}
    for path in glob.glob(MATCH_LOG_GLOB):
        with open(path) as f:
            for line in f:
                try:
                    event = json.loads(line)
                except json.JSONDecodeError:
                    continue

                if (
                    event.get("event") != "function_result"
                    or event.get("status") != "matched"
                    or event.get("address") not in current_matched
                    or not event.get("timestamp")
                ):
                    continue

                address = event["address"]
                timestamp = parse_timestamp(event["timestamp"])
                if address not in earliest or timestamp < earliest[address]:
                    earliest[address] = timestamp
    return earliest


def timestamped_matches(current_matched: dict[str, dict]) -> tuple[dict[str, datetime], int, int]:
    timestamps = earliest_log_matches(current_matched)
    log_covered = len(timestamps)

    for address, function in current_matched.items():
        matched_at = function.get("matched_at")
        if not matched_at:
            continue
        timestamp = parse_timestamp(matched_at)
        if address not in timestamps or timestamp < timestamps[address]:
            timestamps[address] = timestamp

    return timestamps, log_covered, len(timestamps)


def build_points() -> tuple[list[Point], dict[str, int]]:
    current_matched = load_current_matched()
    timestamps, log_covered, timestamp_covered = timestamped_matches(current_matched)

    if not timestamps:
        raise SystemExit("No matched timestamps found in logs or config/functions.json")

    baseline_time = min(timestamps.values())
    buckets: dict[datetime, list[dict]] = defaultdict(list)
    missing_timestamps = 0
    log_or_matched_at = "logs/match_*.jsonl + matched_at"

    for address, function in current_matched.items():
        timestamp = timestamps.get(address)
        if timestamp is None:
            timestamp = baseline_time
            missing_timestamps += 1
        buckets[timestamp].append(function)

    points: list[Point] = []
    matched_functions = 0
    matched_bytes = 0

    for timestamp in sorted(buckets):
        added_functions = len(buckets[timestamp])
        added_bytes = sum(int(f.get("size") or 0) for f in buckets[timestamp])
        matched_functions += added_functions
        matched_bytes += added_bytes
        source = "baseline" if timestamp == baseline_time and missing_timestamps else log_or_matched_at
        points.append(
            Point(
                timestamp=timestamp,
                matched_functions=matched_functions,
                matched_bytes=matched_bytes,
                added_functions=added_functions,
                added_bytes=added_bytes,
                source=source,
            )
        )

    total_functions = len(json.loads(FUNCTION_DB.read_text()))
    total_bytes = sum(int(f.get("size") or 0) for f in json.loads(FUNCTION_DB.read_text()))
    metadata = {
        "current_matched": len(current_matched),
        "current_matched_bytes": sum(int(f.get("size") or 0) for f in current_matched.values()),
        "total_functions": total_functions,
        "total_bytes": total_bytes,
        "log_covered": log_covered,
        "timestamp_covered": timestamp_covered,
        "missing_timestamps": missing_timestamps,
    }
    return points, metadata


def write_csv(path: Path, points: list[Point]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(
            [
                "timestamp",
                "matched_functions",
                "matched_bytes",
                "added_functions",
                "added_bytes",
                "source",
            ]
        )
        for point in points:
            writer.writerow(
                [
                    point.timestamp.isoformat(),
                    point.matched_functions,
                    point.matched_bytes,
                    point.added_functions,
                    point.added_bytes,
                    point.source,
                ]
            )


def render_chart(path: Path, points: list[Point], metadata: dict[str, int]) -> None:
    os.environ.setdefault("MPLCONFIGDIR", str(Path(tempfile.gettempdir()) / "psp-autodecomp-mpl"))
    import matplotlib.dates as mdates
    import matplotlib.pyplot as plt
    from matplotlib.ticker import FuncFormatter

    path.parent.mkdir(parents=True, exist_ok=True)
    times = [p.timestamp for p in points]
    matched_functions = [p.matched_functions for p in points]
    matched_bytes = [p.matched_bytes for p in points]

    fig, axes = plt.subplots(2, 1, figsize=(12, 8), sharex=True)
    fig.suptitle("PSP Autodecomp Match Progress", fontsize=16, fontweight="bold")

    axes[0].step(times, matched_functions, where="post", color="#2f6f4e", linewidth=2.2)
    axes[0].scatter(times[-1:], matched_functions[-1:], color="#173f2c", zorder=3)
    axes[0].set_ylabel("Matched functions")
    axes[0].grid(True, color="#d7d7d7", linewidth=0.8, alpha=0.8)
    axes[0].annotate(
        f"{metadata['current_matched']:,} / {metadata['total_functions']:,}",
        xy=(times[-1], matched_functions[-1]),
        xytext=(-8, 12),
        textcoords="offset points",
        ha="right",
        fontsize=10,
    )

    axes[1].step(times, matched_bytes, where="post", color="#355c9a", linewidth=2.2)
    axes[1].scatter(times[-1:], matched_bytes[-1:], color="#1f365d", zorder=3)
    axes[1].set_ylabel("Matched bytes")
    axes[1].yaxis.set_major_formatter(FuncFormatter(lambda x, _: f"{x / 1024:.0f} KiB"))
    axes[1].grid(True, color="#d7d7d7", linewidth=0.8, alpha=0.8)
    axes[1].annotate(
        f"{metadata['current_matched_bytes'] / 1024:.1f} KiB / {metadata['total_bytes'] / 1024:.1f} KiB",
        xy=(times[-1], matched_bytes[-1]),
        xytext=(-8, 12),
        textcoords="offset points",
        ha="right",
        fontsize=10,
    )

    locator = mdates.AutoDateLocator(minticks=5, maxticks=10)
    axes[1].xaxis.set_major_locator(locator)
    axes[1].xaxis.set_major_formatter(mdates.ConciseDateFormatter(locator))
    axes[1].set_xlabel("Verified match timestamp")

    note = (
        "Source: current config/functions.json sizes/statuses + earliest verified "
        "logs/match_*.jsonl function_result timestamps; matched_at fills gaps. "
        f"Timestamp coverage: {metadata['timestamp_covered']:,}/{metadata['current_matched']:,}; "
        f"baseline without timestamp: {metadata['missing_timestamps']:,}."
    )
    fig.text(0.01, 0.01, note, fontsize=9, color="#555555")
    fig.tight_layout(rect=(0, 0.045, 1, 0.96))
    fig.savefig(path, dpi=160)
    plt.close(fig)


def display_path(path: Path) -> str:
    try:
        return str(path.relative_to(REPO_ROOT))
    except ValueError:
        return str(path)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("build/research/match_progress.png"),
        help="PNG chart path, relative to repo root by default.",
    )
    parser.add_argument(
        "--csv",
        type=Path,
        default=Path("build/research/match_progress.csv"),
        help="CSV data path, relative to repo root by default.",
    )
    args = parser.parse_args()

    points, metadata = build_points()
    output = args.output if args.output.is_absolute() else REPO_ROOT / args.output
    csv_path = args.csv if args.csv.is_absolute() else REPO_ROOT / args.csv

    render_chart(output, points, metadata)
    write_csv(csv_path, points)

    print(f"Wrote {display_path(output)}")
    print(f"Wrote {display_path(csv_path)}")
    print(
        f"Final point: {metadata['current_matched']:,}/{metadata['total_functions']:,} functions, "
        f"{metadata['current_matched_bytes']:,}/{metadata['total_bytes']:,} bytes"
    )
    print(
        f"Timestamp coverage: {metadata['timestamp_covered']:,}/{metadata['current_matched']:,} "
        f"({metadata['log_covered']:,} from logs; {metadata['missing_timestamps']:,} baseline)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
