# psp-autodecomp

Autonomous decompilation of PSP games using coding agent swarms.

## Documentation map

| Location | What | Updates? |
|----------|------|----------|
| `docs/sessions/` | Session journals — intent, plan, outcomes per working session | Yes (append) |
| `docs/direction/` | Phase roadmap and north-star strategy | Yes (living) |
| `docs/decisions/` | Numbered ADRs — verified research and design rationale | No (point-in-time) |
| `docs/research/` | Deep SNC compiler codegen findings | No (findings) |
| `docs/bugs.md` | Known bugs — add here, not as TODOs in code | Yes |
| `foundation.md` | Goals, milestones, principles | Rarely |

**Start of session:** Read the most recent file in `docs/sessions/` for current strategic context, active campaigns, and known blockers. Update it as you work.

## Current target

**Days of Thunder** (USA, PSP Mini, NPUZ00185). ViciousEngine/SNC (SN Systems ProDG). 3.57 MB .text, ~9K named functions, full debug symbols.

## Repository layout

```
tools/           # our scripts (committed)
extern/          # third-party tools and extracted data (gitignored)
src/             # decompiled C/C++ source files
include/         # reconstructed struct headers
config/          # splat config, symbol addrs, function database
asm/             # splat-generated assembly (gitignored)
build/           # compilation output (gitignored)
expected/        # expected .o files for comparison (gitignored)
docs/            # sessions, decisions, direction, research
```

## External dependencies

Everything in `extern/` is gitignored. Required:

```
Days of Thunder (USA) (minis) (PSN).iso   # game ISO, root of repo
extern/snc/                               # SNC compiler (pspsnc 1.2.7503.0)
extern/wibo                               # wibo binary (macOS x86_64, Rosetta 2)
extern/include/                           # Sony PSP SDK 6.60 headers
extern/pspdecrypt/                        # github.com/John-K/pspdecrypt
extern/m2c/                               # github.com/dwilliams27/m2c (psp-vfpu-passthrough branch)
extern/asm-differ/                        # github.com/simonlindholm/asm-differ
extern/iso_extract/                       # extracted ISO contents
extern/extracted_symbols/                 # .sym and .map from DATA.PAK
```

System: `brew install mipsel-linux-gnu-binutils openssl@3`

## Key facts

**Compiler:**
- SNC on macOS ARM: Rosetta 2 → wibo → pspsnc.exe. Linker broken under wibo; use `mipsel-linux-gnu-ld`.
- **-O2 confirmed.** -O2/O3/O4 identical. Only 3 flags affect codegen: `-Xsched`, `-Xmopt`, `-Xxopt` (`docs/decisions/003-compiler-flags.md`).
- Most code uses `-Xsched=2`. Transition zone (`eAll_psp.obj`) mixed at class/method granularity — use `#pragma control sched=N` per method (`docs/research/snc-transition-zone-sched.md`).
- Use `__asm__ volatile("" ::: "memory")` barriers when scheduler reorders within basic blocks.
- Branch-likely heuristic differs from original SNC in rare cases (≤8 byte diffs, no source fix). See `docs/decisions/006-bnel-compiler-divergence.md`.

**Binary:**
- PRX (ELF type 0xFFA0, relocatable). .sym files are ELF with debug info, not text. .map is SN Systems linker format.
- Full rebuild blocked by ~24K unsupported VFPU instructions in asm/0.s. Function-level .o comparison works fine.
- If splat is re-run, asm/0.s needs repatching: `[]`→`_arr_`, `~`→`_dtor_`, strip `.ent`/`.end`.

**VFPU:** Native type system — base type must be `int`, not `float`. `typedef int v4sf_t __attribute__((mode(V4SF)));`. Requires `-Xvfpumatrix=N`. Use native types for loads/stores; inline asm only for ops without C equivalents. See `docs/decisions/007-vfpu-native-types.md`.

**Tools:**
- Function database: `config/functions.json` (~10K functions). Query: `python3 tools/func_db.py`.
- Permuter (last-mile): `python3 tools/permuter.py src/file.cpp 0xADDR --time 300 --save-best` (`docs/decisions/005-snc-permuter.md`).
- asm-differ: `python3 extern/asm-differ/diff.py -o -f build/src/foo.cpp.o MANGLED_SYMBOL`.

## SNC compiler research

Read relevant docs in `docs/research/` BEFORE matching.

| Doc | Read when... |
|-----|-------------|
| `snc-fpu-scheduling.md` | FPU arithmetic — expression order matters |
| `snc-struct-vfpu-codegen.md` | lv.q/sv.q or mtc1/mfc1/mtv/sv.q patterns |
| `snc-delay-slots.md` | Inline asm not landing in delay slots |
| `snc-branch-likely.md` | beqzl/bnezl/bnel — 7 patterns documented |
| `snc-vtable-layout.md` | Virtual dispatch patterns |
| `snc-constructor-codegen.md` | Constructors initializing float members |
| `snc-loop-switch.md` | Loops or switch statements |
| `snc-name-mangling.md` | Determining mangled symbol names |

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

Orchestrator (`tools/orchestrator.py`) drives headless coding-agent CLIs. Backends: Claude Code (default), OpenAI Codex (`--backend codex`). Backend code in `tools/backends/`. Default models in `tools/common.py`.

```bash
# Setup (one-time)
sudo ./tools/sandbox_setup.sh
sudo -i -u autodecomp claude                        # authenticate Claude
sudo -i -u autodecomp codex login                   # authenticate Codex (or export OPENAI_API_KEY)

# Running
./tools/run_overnight.sh --hours 8                                    # full overnight (Claude)
./tools/run_overnight.sh --hours 8 --backend codex                    # Codex backend
./tools/run_overnight.sh --hours 8 --targets config/finetune_targets.json  # targeted run
./tools/run_overnight.sh --dry-run --limit 3                          # test without sandbox

# Progress
python3 tools/func_db.py stats
tail -f logs/match_*.jsonl
```

## Norms

**Quality:**
- Byte-exact matching is the only standard.
- Build pipeline: `source → compiler → identical bytes`. No post-compilation rewriters. Compiler divergences are fixed by patching `pspcor.exe`, not post-processing.
- Maximum effort. Iterate with `compare_func.py`, try multiple restructurings, run the permuter. Mark `failed` only after exhausting all approaches.
- Research before action — read `docs/research/` before matching.

**Assembly ban:**
- **NEVER submit pure assembly as a match.** A `.cpp` wrapping `__asm__()` around disassembly is banned. Inline asm ONLY for small VFPU ops without C equivalents, inside a C/C++ body. >50% inline asm by instruction count → restructure or mark `failed`. The orchestrator auto-rejects pure assembly.

**Agent guardrails:**
- Agents must not modify `tools/`, `config/` (except `functions.json` match status), or `Makefile`. Report tool bugs as session errors.
- Agents may only create/modify: `src/*.cpp`, `src/*.c`, `include/*.h`.

**Unmatchable classification:** Requires ALL of: (1) ≤8 byte diff in specifically bnel/beql instructions; (2) permuter ≥5min, zero improvement; (3) ≥3 manual restructurings tested; (4) all other bytes match. Otherwise stays `failed`. See `docs/decisions/006-bnel-compiler-divergence.md`.

**Process:**
- **Run `/pre-commit-review` before EVERY commit.** No exceptions. 4 review agents including silent-fallback auditor. Do NOT use `/simplify`.
- No silent fallbacks — broken things fail loud. No graceful degradation without human approval.
- No deferred shortcuts — handle edge cases now, not "later."
- Decisions → `docs/decisions/NNN-*.md`. Direction → `docs/direction/NNN-*.md`. Sessions → `docs/sessions/YYYY-MM-DD.md`.
- Clean overnight runs: ff-merge into main without asking. Show `git log --oneline -5`. Pause if anything needs triage (mixed outcomes, circuit-breaker, non-ff merge).
