# 009: SNC Compiler Version Search

**Date:** 2026-04-19
**Status:** Open (no alternative version found)

## Problem

Our SNC (pspsnc 1.2.7503.0, build "Apr 29 2010") has three confirmed codegen divergences from the compiler that built Days of Thunder (shipped Jan 13 2011):

1. **Branch-likely divergence** — our SNC generates `bnezl`/`beqzl` (branch-likely) where the original uses `beqz`/`bnez` + `nop` for null-check-and-dereference patterns. See `006-bnel-compiler-divergence.md`.
2. **mul.s operand ordering** — our SNC puts mtc1-loaded constants first (`mul.s f16, f14, f12` = const*var), the original puts variables first (`mul.s f16, f12, f14` = var*const).
3. **FPU register allocation** — our SNC allocates 1.0f to f19 (last primary register), the original allocates to f4 (first secondary). Different allocation priority ordering.

All three are hardwired — no flag combination changes any of them (43 flag variations tested, see `003-compiler-flags.md`).

## Our Compiler's Identity

```
pspsnc 1.2.7503.0 (dev,main @83716 #346325)
PE FileVersion: 1, 2, 7503, 0
Copyright: 2003-2007 SN Systems Ltd (PE), 2003-2009 SN Systems Limited (--version)
Internal codegen stamp: "4.112 SN Systems 1.1.07 Apr 29 2010"
EDG C/C++ Front End: Copyright 1988-2003 Edison Design Group, Inc.
SN licensing library: snlapi.lib 1.4.5.0
PDB build path references: ProDG_PSP_SDK550
Perforce changelist: @83716, build: #346325
```

All core compiler executables (pspsnc.exe, pspcfe.exe, pspcor.exe, pspcq.exe, pspipa.exe) share identical version `1.2.7503.0` and changelist `@83716`. The assembler (pspas.exe) is version `1.1.1737.0` from a separate Perforce depot (`@52548`). The linker (pspld.exe) is version `2.7.63.0`.

## Game Binary Identity

```
Build date: Jan 13 2011 18:15:39 (from __DATE__/__TIME__ macros in EBOOT strings)
SDK version: 6.20 (USA), 5.05 (PAL) (from sceKernelSetCompiledSdkVersion)
Compiler version: 0x00030306 (from sceKernelSetCompilerVersion — CRT-provided, not SNC-specific)
Build path: c:\work\bcracing\build\psp\FinalDVD\gGame\
SN Systems libs: C:\Program Files\SN Systems\PSP\lib\
DWARF DW_AT_producer: "GNU C 3.3.6+allegrex-2.2.2-psp-1.20.1.internal -g" (GCC CRT only, no SNC DWARF)
```

The EBOOT is fully stripped — no `.debug_info`, `.debug_str`, `.comment`, or `.mdebug` sections for game code. The .sym files contain debug info only for the GCC-compiled CRT, not the SNC-compiled game code. **There is no way to determine the exact SNC build from the binary.**

## Sources Searched

### decomp.me / decompals (GitHub)

**Result: Only one PSP SNC version exists — the same 7503 we have.**

- `decompme/compilers` releases: single entry `pspsnc_1.2.7503.0.tar.gz`
- Added in PR #5 by mkst (Mark Street), no discussion of alternatives
- No issues or PRs requesting additional SNC PSP versions
- PSP platform on decomp.me: 14 compilers total (1 SNC, 11 Metrowerks, 2 GCC)
- GitHub code search for "pspsnc": 0 results across all of GitHub

### archive.org

**Result: Every archive contains the same pspsnc 7503.**

| Archive | File | pspsnc Version |
|---------|------|----------------|
| `psp-sdk` (archive.org/details/psp-sdk) | `psp_sdk_660.7z` (868 MB) → `ProDGforPSPv1.4.1.exe` | 1.2.7503.0 (verified) |
| `sn_sys_consoles_2` (archive.org/details/sn_sys_consoles_2) | `ProDGforPSPv1.4.1.exe` (47.5 MB) | Same installer |
| `sn_sys_consoles_2` | `PSPother.zip` (42 MB) | Docs + ProView disc images only, no compiler |
| `sn_sys_consoles_2` | `ProViewforPSPv1.2.1.exe`, `TunerforPSPv1.6.0.exe` | Debugger/profiler, no compiler |
| `psp-sdk` | `psp_full_sdk_JSR.rar` (67.9 MB) | SDK 1.5.0 era, stripped, no ProDG |

No `sn_sys_consoles` (v1) or `sn_sys_consoles_3` exists on archive.org. No other ProDG PSP archives found.

### PSP Decomp Projects

**Result: No other PSP project uses SNC.**

- **sotn-decomp** (Castlevania: SotN PSP) — uses Metrowerks `mwccpsp`, not SNC
- **BakuganDotC-decomp** — very early stage, Ghidra only, no build system
- No matching-decomp projects exist for Patapon, Kingdom Hearts BBS, Dissidia, God of War CoO, or any ViciousEngine title

### ViciousEngine Titles

**Result: No decomp communities exist for any ViciousEngine game.**

Known ViciousEngine PSP titles with debug symbols (from retroreversing.com):
Alien Syndrome (2007), Ben 10: Alien Force (2008), Cart Kings (2011), Days of Thunder (2011), Desi Adda (2009), Despicable Me (2010), DON 2 The Game, PlayEnglish series, Pro Bull Riders, Top Gun.

None have decompilation projects.

### BetaArchive

Forum threads on PSP SDK returned HTTP 403 (require account). Snippets from web search describe the same SDK 6.60 + ProDG 1.4.1 we already have.

### ProDG Version Timeline

| ProDG PSP Version | Shipped With | SNC Build |
|-------------------|-------------|-----------|
| v1.4.0 | PSP SDK 6.2.0 | Unknown (not publicly archived) |
| v1.4.1 | PSP SDK 6.6.0 | 1.2.7503.0 (confirmed) |

VS Integration versions (separate numbering): v1.7.11 (Apr 2008), v1.8.0 (Jun 2008), v1.8.2 (Sep 2008), v1.8.3 (Apr 2009).

## Compiler Internals Analysis

### Compilation Pipeline

Revealed by `pspsnc -#`: `pspcfe` (EDG C/C++ frontend) → `pspcor` (optimizer/codegen) → `pspas` (assembler). `pspcq` is a "quick" mode that skips full optimization (no delay slot filler, no scheduler, no global register allocator). `pspipa` is for interprocedural analysis.

### Branch-Likely Mechanism

Branch-likely generation is a **side effect of delay slot filling** in `del_slot.c` (part of `pspcor`). When the scheduler (`-Xsched >= 1`) tries to fill a branch's delay slot with an instruction from the taken path, it converts the branch to its `-likely` variant (`bne` → `bnel`) because branch-likely annuls the delay slot when NOT taken, making speculative execution safe.

The filler has failure conditions that prevent filling (and thus prevent branch-likely):
- `"FAILED because store or call"`
- `"FAILED because float op"`
- `"FAILED because div op"`
- `"FAILED because of target reg"`
- `"FAILED because load"`
- `"FAILED because find_first_CGINS gave NULL"`

At `sched=0`, branches are always regular (`beq` + `nop`). At `sched=1` or `sched=2`, the filler runs and may produce branch-likely. The divergence is in the heuristics — a different `del_slot.c` would have different criteria for when filling is "safe."

### Complete Control Variables at -O2

| Control | Value | Range | Purpose |
|---------|-------|-------|---------|
| sched | 2 | 0-2 | Instruction scheduling + delay slot fill |
| mopt | 3 | 0-4 | Machine-level optimization |
| xopt | 2 | 0-5 | Extended optimization |
| alias | 3 | 0-5 | Alias analysis |
| reg | 1 | 0-3 | Register allocation aggressiveness |
| flow | 1 | 0-1 | Control flow optimization |
| constp | 2 | 0-2 | Constant propagation |
| copyp | 2 | 0-2 | Copy propagation |
| fcm | 1 | 0-3 | Function call modification |
| branchless | 0 | 0-1 | Convert branches to conditional moves |
| fltacc | 1 | 0-2 | Floating-point accuracy |
| sinllev | 3 | 0-5 | Static inline level |
| zone | 1 | 0-1 | Zone optimization |
| callmod | 1 | 0-10+ | Call modification |

Notable: `-Xbranchless=1` converts branches to `movn`/`movz` conditional moves. Not useful for our problem but previously undocumented.

### -O Level Comparison (codegen-relevant controls only)

```
Control   O0   O1   O2   O3   O4   O5
-------   --   --   --   --   --   --
sched      0    0    2    2    2    2
mopt       0    1    3    4    4    4
xopt       0    0    2    2    3    5
reg        0    0    1    1    3    3
fcm        0    0    1    2    2    2
alias      0    1    3    3    4    4
sinllev    0    1    3    3    4    5
```

Confirms -O2/-O3 differ only in `fcm` (1→2) and `mopt` (3→4).

## Conclusions

1. **pspsnc 1.2.7503.0 appears to be the only publicly available build** of the PSP SNC compiler. decomp.me (which actively collects compiler variants) has only this one. ProDG v1.4.1 (the only archived installer) contains it. GitHub has zero references to any other version.

2. **Our toolchain's PDB paths reference "ProDG_PSP_SDK550"**, but the game targets SDK 6.20. ProDG v1.4.0 shipped with SDK 6.2.0 — if that installer contained a different SNC build, it would be our target. However, no ProDG v1.4.0 installer has been found publicly.

3. **The branch-likely divergence is in del_slot.c heuristics**, not in any configurable flag. A different compiler build with different delay-slot-fill criteria would produce different branch-likely patterns. This is consistent with a minor version difference.

4. **The binary cannot tell us which SNC build was used.** The EBOOT is stripped, the .sym files lack SNC DWARF info, and the CRT compiler version (0x00030306) is SDK-provided, not SNC-specific.

## Remaining Avenues

| Avenue | Likelihood | Action Required |
|--------|-----------|-----------------|
| ProDG v1.4.0 installer (SDK 6.20 bundle) | Medium | Search archives, communities; not publicly found yet |
| decomp.me Discord | Medium | Ask if anyone has a different pspsnc build |
| PSP-RE community (psp-re.github.io) | Low-Medium | Ask about ProDG versions |
| BetaArchive forums (account required) | Low | Check PSP SDK threads with an account |
| Other ViciousEngine game binaries | Low | Compare branch-likely patterns to confirm compiler version hypothesis |
| SN Systems / Sony internal builds | Very Low | Would require industry contacts |
