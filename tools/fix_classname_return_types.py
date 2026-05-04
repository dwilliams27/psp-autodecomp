#!/usr/bin/env python3
"""Fix DB entries where return type leaked into class_name.

209 entries have class_name like "cBase * gcDesiredObjectT<...>" or
"void cArray<...>" instead of just the class name. This causes the
quality gate (reject_extern_c_class_methods) to reject valid matches
because it searches for "cBase * gcDesiredObjectT<...>::Method("
which is not valid C++ syntax.

Run after stopping the orchestrator (it holds DB in memory).
"""

import json
import re
import sys

DB_PATH = "config/functions.json"

# Return type patterns that leak into class_name.
# Order matters — longer prefixes first to avoid partial matches.
RETURN_PREFIXES = re.compile(
    r'^(?:'
    r'const\s+cType\s*\*\s*'
    r'|const\s+cBase\s*\*\s*'
    r'|const\s+char\s*\*\s*'
    r'|cBase\s*\*\s*'
    r'|cType\s*\*\s*'
    r'|unsigned\s+int\s+'
    r'|unsigned\s+short\s+'
    r'|unsigned\s+char\s+'
    r'|void\s+'
    r'|bool\s+'
    r'|int\s+'
    r'|float\s+'
    r'|short\s+'
    r'|char\s+'
    r')'
)


def fix_entry(f):
    cn = f.get("class_name") or ""
    if not cn:
        return False
    m = RETURN_PREFIXES.match(cn)
    if not m:
        return False
    new_cn = cn[m.end():]
    if not new_cn:
        return False

    old_name = f.get("name", "")
    f["class_name"] = new_cn
    if old_name.startswith(cn + "::"):
        f["name"] = new_cn + "::" + old_name[len(cn) + 2:]
    return True


def main():
    with open(DB_PATH) as fh:
        db = json.load(fh)

    fixed = 0
    for f in db:
        if fix_entry(f):
            fixed += 1
            if "--verbose" in sys.argv:
                print(f"  {f['address']} -> [{f['class_name']}]")

    if fixed == 0:
        print("No entries to fix.")
        return

    print(f"Fixed {fixed} entries.")
    if "--dry-run" in sys.argv:
        print("Dry run — not saving.")
        return

    with open(DB_PATH, "w") as fh:
        json.dump(db, fh, indent=2)
        fh.write("\n")
    print("Saved.")


if __name__ == "__main__":
    main()
