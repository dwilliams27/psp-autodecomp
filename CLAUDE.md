# Autonoma

Autonomous decompilation of PSP games using coding agent swarms. See `foundation.md` for goals, milestones, and principles. See `docs/decisions/` for verified research and design rationale. See `docs/direction/` for phase roadmap and north-star guidance.

## Current target

**Days of Thunder** (USA, PSP Mini, NPUZ00185). ViciousEngine game compiled with SNC (SN Systems ProDG). 3.57 MB .text section, 9,047 named functions. Full debug symbols available.

## Repository layout

```
tools/           # our scripts (committed)
extern/          # third-party tools and extracted data (gitignored, obtained separately)
src/             # decompiled C/C++ source files
include/         # reconstructed struct headers
config/          # splat config, symbol addrs, function database
asm/             # splat-generated assembly (gitignored)
build/           # compilation output (gitignored)
expected/        # expected .o files for comparison (gitignored)
docs/            # decisions and direction docs
```

## External dependencies (must be obtained separately)

Everything in `extern/` is gitignored. An agent starting fresh needs:

```
Days of Thunder (USA) (minis) (PSN).iso   # game ISO, root of repo
extern/snc/                               # SNC compiler (pspsnc 1.2.7503.0) from decompme/compilers
extern/wibo                               # wibo binary from decompals/wibo (macOS x86_64, runs under Rosetta 2)
extern/include/                           # official Sony PSP SDK 6.60 headers (from psp_sdk_660.7z on archive.org)
extern/pspdecrypt/                        # git clone https://github.com/John-K/pspdecrypt (build with make)
extern/m2c/                               # git clone https://github.com/matt-kempster/m2c (pip install -e .)
extern/asm-differ/                        # git clone https://github.com/simonlindholm/asm-differ
extern/iso_extract/                       # extracted ISO contents (7z x the .iso)
extern/extracted_symbols/                 # .sym and .map files extracted from DATA.PAK
```

Additional system dependencies (Homebrew):
- `brew install mipsel-linux-gnu-binutils` — provides objdump, readelf, as, ld with Allegrex support
- OpenSSL for pspdecrypt build: `brew install openssl@3`

## Key facts agents should know

- The binary is a **PRX** (ELF type 0xFFA0, relocatable), not a static ELF. This is normal for PSP Minis.
- SNC compiler works on macOS ARM via: Rosetta 2 -> wibo -> pspsnc.exe. The linker (pspld.exe) is broken under wibo; use `mipsel-linux-gnu-ld` instead.
- SNC optimization levels are **-O0 through -O5** (no -Os). The game was compiled at -O2 or higher.
- SNC expects headers via `-I extern/include`. No `include_snc/` needed.
- The .sym files in DATA.PAK are **ELF binaries with debug info**, not text. The .map file is a text-format SN Systems linker map.
- These are NOT PPSSPP `.sym` format — ghidra-allegrex `PpssppImportSymFile` cannot import them directly.
- **asm/0.s was patched** after splat ran: `[]` → `_arr_`, `~` → `_dtor_` in symbol names, and all `.ent`/`.end` directives stripped. These are required for GAS compatibility. If splat is re-run, the same patches must be reapplied.
- **Full binary rebuild is blocked** by ~24K VFPU instructions in asm/0.s that standard `mipsel-linux-gnu-as` doesn't support (sv.q, lv.q, vdot, vsqrt, mfv, etc.). Needs a PSP-aware assembler or encoding as `.word` directives. Function-level .o comparison works fine without the full link.
- **asm-differ** works in `-o` mode for function-level comparison: `python3 extern/asm-differ/diff.py -o -f build/src/foo.cpp.o MANGLED_SYMBOL`. Expected .o files go in `expected/` mirroring the `build/` structure.
- **-O2 confirmed** as the compiler flag. -O2/-O3/-O4 produce identical bytes. -O5 differs only on specific loop patterns. **Most code uses `-Xsched=2`** (SNC default). Within `eAll_psp.obj`, some engine classes use `-Xsched=1` (eTextureMap, eBumpOffsetMap, eDynamicMeshMorphTarget, eCollisionConstraint, eInputKeyboard). The Makefile defaults to `sched=2` with per-class pattern overrides for the sched=1 exceptions. See `docs/decisions/003-compiler-flags.md` for the full analysis. Use `__asm__ volatile("" ::: "memory")` barriers when the scheduler still reorders within basic blocks.
- **Function database** at `config/functions.json` has 9,966 functions with class, size, .obj file, call graph, and match status. Query with `python3 tools/func_db.py`.

## Matching workflow

```bash
python3 tools/func_db.py query --class ClassName --size-max 64  # find targets
python3 tools/extract_func.py "ClassName::Method"                # create expected .o
python3 tools/decompile_func.py "ClassName::Method"              # initial C via m2c
# write/edit C in src/
python3 tools/compare_func.py src/file.cpp                       # compile + compare
python3 tools/call_graph.py show "ClassName::Method"             # check dependencies
```

## Overnight autonomous runs

The orchestrator (`tools/orchestrator.py`) runs Claude Code in headless mode to match functions at scale. It's designed for unattended overnight operation.

### Setup (one-time)
```bash
sudo ./tools/sandbox_setup.sh    # creates 'autodecomp' user + PF rules
sudo -u autodecomp claude        # authenticate Claude Code for the new user
```

### Running
```bash
./tools/run_overnight.sh --hours 8                  # full overnight run
./tools/run_overnight.sh --hours 2 --size-max 8     # trivial functions only
./tools/run_overnight.sh --dry-run --limit 3        # test without sandbox
```

### Checking progress
```bash
python3 tools/func_db.py stats                      # overall progress
python3 tools/func_db.py query --status matched      # matched functions
tail -f logs/match_*.jsonl                            # live log
```

Results are in `config/functions.json` (match_status field) and `logs/`.

## Norms

- All decompilation work is done by agents. Humans provide direction and unblock.
- Byte-exact matching is the only standard. A function is done when it compiles to identical bytes.
- Research before action. Investigate tooling and compiler behavior before writing code.
- Decisions go in `docs/decisions/` with numbered filenames (001, 002, ...).
- Direction docs go in `docs/direction/` with numbered filenames. These are aspirational/north-star — keep them up to date as phases complete (mark `[x]` in the doc). Unlike decisions (which are point-in-time records), direction docs are living documents.
- Do not commit binaries, ISOs, or SDK files. The `.gitignore` handles this.
- **STOP. Before committing ANY code changes, you MUST run `/pre-commit-review` first.** No exceptions. No "I'll do it after." No skipping for small changes. This is the single most important norm in this repo. Our custom `/pre-commit-review` (`.claude/skills/pre-commit-review/SKILL.md`) launches 4 review agents: code reuse, code quality, efficiency, and a **silent-fallback auditor**. Do NOT use the built-in `/simplify` — it only has 3 agents and misses silent fallbacks.
- **No silent fallbacks.** If something is broken, fail loudly and early. Never add graceful degradation, default values, or try/except swallowing without explicit human approval. Broken things must be visible so they get fixed.
- **No deferred shortcuts.** Handle edge cases fully when you encounter them, not "later." If a tool doesn't handle all 12,506 symbols correctly, fix it now — don't skip the hard ones with a TODO. Untracked "I'll handle this later" decisions accumulate invisibly and cause bizarre behavior in a system built by many agents over time.
