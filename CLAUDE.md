# Autonoma

Autonomous decompilation of PSP games using coding agent swarms. See `foundation.md` for goals, milestones, and principles. See `docs/decisions/` for verified research and design rationale.

## Current target

**Days of Thunder** (USA, PSP Mini, NPUZ00185). ViciousEngine game compiled with SNC (SN Systems ProDG). 3.57 MB .text section, 9,047 named functions. Full debug symbols available.

## Gitignored files (must be obtained separately)

These are not checked in. An agent starting fresh needs to know where they go:

```
Days of Thunder (USA) (minis) (PSN).iso   # game ISO, root of repo
tools/snc/                                # SNC compiler (pspsnc 1.2.7503.0) from decompme/compilers
tools/wibo                                # wibo binary from decompals/wibo (macOS x86_64, runs under Rosetta 2)
tools/include/                            # official Sony PSP SDK 6.60 headers (from psp_sdk_660.7z on archive.org)
tools/pspdecrypt/                         # git clone https://github.com/John-K/pspdecrypt (build with make)
tools/m2c/                                # git clone https://github.com/matt-kempster/m2c (pip install -e .)
tools/iso_extract/                        # extracted ISO contents (7z x the .iso)
tools/extracted_symbols/                  # .sym and .map files extracted from DATA.PAK
tools/extract_pak.py                      # ViciousEngine DATA.PAK extraction script
```

Additional system dependencies (Homebrew):
- `brew install mipsel-linux-gnu-binutils` — provides objdump, readelf, as, ld with Allegrex support
- OpenSSL for pspdecrypt build: `brew install openssl@3`

## Key facts agents should know

- The binary is a **PRX** (ELF type 0xFFA0, relocatable), not a static ELF. This is normal for PSP Minis.
- SNC compiler works on macOS ARM via: Rosetta 2 -> wibo -> pspsnc.exe. The linker (pspld.exe) is broken under wibo; use `mipsel-linux-gnu-ld` instead.
- SNC optimization levels are **-O0 through -O5** (no -Os). The game was compiled at -O2 or higher.
- SNC expects headers via `-I tools/include`. No `include_snc/` needed.
- The .sym files in DATA.PAK are **ELF binaries with debug info**, not text. The .map file is a text-format SN Systems linker map.
- These are NOT PPSSPP `.sym` format — ghidra-allegrex `PpssppImportSymFile` cannot import them directly.

## Norms

- All decompilation work is done by agents. Humans provide direction and unblock.
- Byte-exact matching is the only standard. A function is done when it compiles to identical bytes.
- Research before action. Investigate tooling and compiler behavior before writing code.
- Decisions go in `docs/decisions/` with numbered filenames (001, 002, ...).
- Do not commit binaries, ISOs, or SDK files. The `.gitignore` handles this.
- **Before committing any code changes, run `/simplify` first.** This is mandatory — no exceptions.
- **No silent fallbacks.** If something is broken, fail loudly and early. Never add graceful degradation, default values, or try/except swallowing without explicit human approval. Broken things must be visible so they get fixed.
