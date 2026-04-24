# psp-autodecomp

Autonomous decompilation of PSP games using coding agent swarms. See `foundation.md` for goals, milestones, and principles. See `docs/decisions/` for verified research and design rationale. See `docs/direction/` for phase roadmap and north-star guidance. See `docs/bugs.md` for known bugs discovered during development — add new bugs there when found rather than leaving TODOs in code.

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
extern/m2c/                               # git clone -b psp-vfpu-passthrough https://github.com/dwilliams27/m2c (pip install -e .)
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
- **-O2 confirmed** as the compiler flag. -O2/-O3/-O4 produce identical bytes. -O5 differs only on specific loop patterns. **Only 3 SNC flags affect codegen: `-Xsched`, `-Xmopt`, `-Xxopt`** (43 flag variations tested; 26 other flags produce identical bytes at all levels). See `docs/decisions/003-compiler-flags.md`.
- **Compiler flag layout**: Most code uses `-Xsched=2` (SNC default). `eAll_psp.obj` is a **unity build** (`eAll_psp.cpp`) that uses `#pragma` to switch sched mid-file. The sched=1 zone is at addresses ~0x06e000-0x0bab28 — confirmed classes: eTextureMap, eBumpOffsetMap, eDynamicMeshMorphTarget, eCollisionConstraint, eCompoundShape, eInputKeyboard. **~40 more classes** in that zone are unverified (trivial stubs only). Transition zone at ~0x040000-0x06e000 is uncharted. The Makefile defaults to `sched=2` with per-class pattern overrides for sched=1 exceptions. When matching a new engine class, if bytes don't match, try `-Xsched=1`.
- **Pre-fine-tuning targets**: 44 strategic functions in `config/finetune_targets.json` must be matched before generating synthetic training data. These fill coverage gaps: transition zone boundary, sched=1 verification, mAll_psp.obj (0/58 matched), large functions (0 matches above 512B). See `docs/decisions/004-pre-finetune-matching-targets.md`.
- Use `__asm__ volatile("" ::: "memory")` barriers when the scheduler still reorders within basic blocks (e.g., `li` moved before `sw` to fill pipeline slots).
- **Branch-likely (bnel) compiler divergence**: Our SNC and the original compiler disagree on ~rare cases of when to apply `bnel` (branch-likely) vs `bne` + `nop`. Both compilers use `bnel` extensively (2,802 EBOOT functions have it, 22 of our matches have it), but the heuristics diverge for certain code patterns. This produces 5-8 byte diffs that NO flag combination or source restructuring can fix. See `docs/decisions/006-bnel-compiler-divergence.md` for diagnostic criteria. Functions confirmed to hit this pattern should be marked `unmatchable_bnel` — but see the strict criteria in Norms below before using this classification.
- **Native VFPU types**: SNC has a fully functional VFPU type system that generates `lv.q`/`sv.q` and schedules them with MIPS code (including delay slots). **The base type must be `int`, not `float`** (the Sony SDK headers are wrong). Use: `typedef int v4sf_t __attribute__((mode(V4SF)));` (quad) and `typedef int v16sf_t __attribute__((mode(V16SF)));` (matrix). Requires `-Xvfpumatrix=N` flag. Builtins: `__builtin_ftovs(float)` / `__builtin_vstof(vs)` for scalars. See `docs/decisions/007-vfpu-native-types.md`. **Do NOT use `.word` for VFPU** — use native types for stores/loads so SNC can schedule, and VFPU mnemonics in inline asm only for operations without C equivalents (vmidt.q, vmov.q, etc.).
- **Permuter tool** (`tools/permuter.py`) does mechanical last-mile matching by randomly mutating C source and comparing compiled output. Use it when within ~10-20 bytes of matching: `python3 tools/permuter.py src/file.cpp 0xADDR --time 300 --save-best`. See `docs/decisions/005-snc-permuter.md`.
- **Function database** at `config/functions.json` has 9,966 functions with class, size, .obj file, call graph, and match status. Query with `python3 tools/func_db.py`.

## SNC compiler research

Deep research on SNC codegen is in `docs/research/`. **Read the relevant docs before matching** — they contain hard-won knowledge that will save you hours of trial and error.

| Doc | Read when... |
|-----|-------------|
| `snc-fpu-scheduling.md` | Function has FPU arithmetic (mul.s, add.s). Source expression ORDER matters. |
| `snc-struct-vfpu-codegen.md` | Function has lv.q/sv.q or the mtc1/mfc1/mtv/sv.q pattern |
| `snc-delay-slots.md` | Inline asm instructions aren't landing in delay slots |
| `snc-branch-likely.md` | Function has beqzl/bnezl/bnel — 7 patterns documented |
| `snc-vtable-layout.md` | Function has virtual dispatch (lw/addiu/lh/addu/lw/jalr pattern) |
| `snc-constructor-codegen.md` | Function is a constructor initializing float members |
| `snc-loop-switch.md` | Function has loops or switch statements |
| `snc-name-mangling.md` | Need to determine the mangled symbol name for a function |

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

The orchestrator (`tools/orchestrator.py`) drives a headless coding-agent CLI to match functions at scale. Two backends are supported: Claude Code (default) and OpenAI Codex (`--backend codex`). Backend code lives in `tools/backends/` — see `base.py` for the `Backend` ABC and shared `run_session` loop.

### Setup (one-time)
```bash
sudo ./tools/sandbox_setup.sh                       # creates 'autodecomp' user + PF rules
sudo -i -u autodecomp claude                        # authenticate Claude (Keychain-backed)
sudo -i -u autodecomp codex login                   # authenticate Codex (only if using --backend codex)
```

Alternative for Codex: export `OPENAI_API_KEY` in the parent shell before invoking `run_overnight.sh` — it is preserved through `sudo -i` and overrides `~/.codex/auth.json`.

### Running
```bash
./tools/run_overnight.sh --hours 8                                    # full overnight run (Claude)
./tools/run_overnight.sh --hours 2 --size-max 8                       # trivial functions only
./tools/run_overnight.sh --dry-run --limit 3                          # test without sandbox
./tools/run_overnight.sh --hours 8 --targets config/finetune_targets.json  # targeted finetune run
./tools/run_overnight.sh --hours 8 --backend codex                    # drive sessions with Codex
```

The `--targets` flag switches to targeted mode: pulls exclusively from the specified file, uses batch_size=2 and 1.5hr session timeout (tuned for larger functions). See `docs/decisions/004-pre-finetune-matching-targets.md`.

The `--backend` flag selects which CLI drives sessions. Default models: Claude uses `claude-opus-4-7`, Codex uses `gpt-5.5` (both in `tools/common.py`). Every log event carries a `backend` field for attribution.

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
- **Maximum effort. No shortcuts.** Every function gets your best attempt at real C/C++ decompilation. Iterate with `compare_func.py` extensively. Try multiple source restructurings. Run the permuter for last-mile matching. Only mark a function `failed` after exhausting all approaches — not after one attempt. Lazy work (wrapping disassembly in `__asm__()`) is worse than no work because it inflates match counts with zero training value.
- Research before action. Investigate tooling and compiler behavior before writing code.
- Decisions go in `docs/decisions/` with numbered filenames (001, 002, ...).
- Direction docs go in `docs/direction/` with numbered filenames. These are aspirational/north-star — keep them up to date as phases complete (mark `[x]` in the doc). Unlike decisions (which are point-in-time records), direction docs are living documents.
- Do not commit binaries, ISOs, or SDK files. The `.gitignore` handles this.
- **STOP. Before committing ANY code changes, you MUST run `/pre-commit-review` first.** No exceptions. No "I'll do it after." No skipping for small changes. This is the single most important norm in this repo. Our custom `/pre-commit-review` (`.claude/skills/pre-commit-review/SKILL.md`) launches 4 review agents: code reuse, code quality, efficiency, and a **silent-fallback auditor**. Do NOT use the built-in `/simplify` — it only has 3 agents and misses silent fallbacks.
- **No silent fallbacks.** If something is broken, fail loudly and early. Never add graceful degradation, default values, or try/except swallowing without explicit human approval. Broken things must be visible so they get fixed.
- **No deferred shortcuts.** Handle edge cases fully when you encounter them, not "later." If a tool doesn't handle all 12,506 symbols correctly, fix it now — don't skip the hard ones with a TODO. Untracked "I'll handle this later" decisions accumulate invisibly and cause bizarre behavior in a system built by many agents over time.
- **NEVER submit pure assembly as a match. EVER.** This is the #1 way agents waste work and poison training data. A matched function MUST contain real decompiled C/C++ code — not a `.cpp` file wrapping `__asm__()` around copied disassembly. File-scope `__asm__("...\n")` blocks that reproduce the entire function in assembly are **banned**. They produce zero training data and defeat the entire purpose of this project. The ONLY acceptable use of inline asm is small `__asm__ volatile()` blocks inside a C/C++ function body for specific VFPU operations that have no C equivalent (`vcrsp.t`, `vpfxs`, `vmidt.q`, `vcmovt.s`, etc.). The surrounding code MUST be C/C++. If a function is >50% inline asm by instruction count, you're doing it wrong — restructure or report it as `failed`. If a function genuinely cannot be expressed in C/C++ (e.g., 100% VFPU computation), mark it `failed`, do NOT bypass with assembly. The orchestrator will automatically reject matches that are pure assembly.
- **Agents must not modify tooling.** Overnight/headless agents should never modify files in `tools/`, `config/` (except `functions.json` match status), or the `Makefile`. If a tool has a bug, report it as a session error — a human will fix it. The only files agents should create or modify are `src/*.cpp`, `src/*.c`, and `include/*.h`.
- **Classifying functions as unmatchable requires proof.** A function may ONLY be marked `unmatchable_bnel` (or any unmatchable status) if ALL of the following are demonstrated in the session log: (1) the byte diff is ≤8 bytes and the differing instructions are specifically `bnel`/`beql` vs `bne`/`beq` + `nop` — confirmed via disassembly; (2) the permuter ran for ≥5 minutes with zero improvement; (3) at least 3 manual source restructurings were tested targeting the specific branch; (4) all other bytes match perfectly after relocation masking. If ANY criterion is not met, the function stays `failed` for retry. Diffs >10 bytes are NOT bnel issues — they are source/flag problems the agent hasn't solved yet. This classification exists for a narrow compiler-version divergence, not as an escape hatch for hard functions. See `docs/decisions/006-bnel-compiler-divergence.md`.
