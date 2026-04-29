#!/usr/bin/env python3
"""Regression tests for provider rate-limit retry hint parsing.

Run from the repo root: `python3 tools/test_rate_limit_parser.py`.
"""

from __future__ import annotations

import os
import sys
from datetime import datetime, timedelta
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from backends import base  # noqa: E402


class DummyBackend(base.Backend):
    def spawn_cmd(self, prompt: str, session_id: str):
        return []

    def parse_line(self, raw: str):
        return []


class FixedDateTime(datetime):
    @classmethod
    def now(cls):
        return cls(2026, 4, 29, 2, 15, 33)


def _with_fixed_now(fn):
    fixed_epoch = FixedDateTime.now().timestamp()
    with mock.patch.object(base, "datetime", FixedDateTime):
        with mock.patch.object(base.time, "time", lambda: fixed_epoch):
            return fn(fixed_epoch)


def test_stale_absolute_time_gets_short_pause():
    def check(now_epoch):
        retry = base._parse_retry_at_epoch(
            "You've hit your usage limit. Try again at 2:15 AM.")
        assert retry == now_epoch + 5 * 60, retry - now_epoch

    _with_fixed_now(check)
    print("stale absolute reset time: OK")


def test_future_absolute_time_stays_today():
    def check(now_epoch):
        retry = base._parse_retry_at_epoch(
            "You've hit your usage limit. Try again at 2:45 AM.")
        expected = FixedDateTime.now().replace(
            hour=2, minute=45, second=0, microsecond=0).timestamp()
        assert retry == expected, retry - now_epoch

    _with_fixed_now(check)
    print("future absolute reset time: OK")


def test_old_absolute_time_rolls_to_tomorrow():
    def check(now_epoch):
        retry = base._parse_retry_at_epoch(
            "You've hit your usage limit. Try again at 1:15 AM.")
        expected = (
            FixedDateTime.now().replace(
                hour=1, minute=15, second=0, microsecond=0)
            + timedelta(days=1)
        ).timestamp()
        assert retry == expected, retry - now_epoch

    _with_fixed_now(check)
    print("old absolute reset time rolls over: OK")


def test_line_number_containing_429_is_not_rate_limit():
    backend = DummyBackend("dummy", "")
    assert backend.rate_limit_info('4297:      "0x0030cafc",') is None
    print("line number containing 429 ignored: OK")


def test_http_429_is_rate_limit():
    backend = DummyBackend("dummy", "")
    assert backend.rate_limit_info("request failed with HTTP status 429")
    assert backend.rate_limit_info("429 Too Many Requests")
    print("HTTP 429 detected: OK")


def main():
    test_stale_absolute_time_gets_short_pause()
    test_future_absolute_time_stays_today()
    test_old_absolute_time_rolls_to_tomorrow()
    test_line_number_containing_429_is_not_rate_limit()
    test_http_429_is_rate_limit()


if __name__ == "__main__":
    main()
