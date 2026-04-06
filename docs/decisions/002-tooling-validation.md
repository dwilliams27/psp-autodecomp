# 002: Tooling Validation

Research sprint results validating all assumptions about the Days of Thunder target binary, compiler toolchain, and decompilation pipeline tools. All findings are verified on macOS Apple Silicon (M2 Pro).

## Binary Analysis

### Days of Thunder structure

ISO: `Days of Thunder (USA) (minis) (PSN).iso` (214 MB)
- **DISC_ID**: NPUZ00185 (confirmed via PARAM.SFO)
- **Developer**: Gameshastra (internal codename: "bcracing")
- **Engine**: ViciousEngine
- **Compiler**: SNC (confirmed by `.cplinit` section and `C:\Program Files\SN Systems\PSP\lib\` in linker map)

Binary layout (`PSP_GAME/SYSDIR/EBOOT.BIN.dec`, after decryption):
- **Format**: ELF32 PRX (type 0xFFA0, relocatable) — standard for PSP Minis
- **ISA**: MIPS-II, EABI32, little-endian
- **Entry point**: 0x1A98A0
- **.text**: 3.57 MB (0x36BAB4 bytes)
- **.rodata**: 26 KB
- **.data**: 107 KB
- **.bss**: 598 KB
- **52 ELF sections** including 22 `.sceStub.text` sections (OS import stubs), `.cplinit`, `.ctors`, `.dtors`
- **No PRX module loading** — monolithic binary, no `sceKernelLoadModule` calls
- **EBOOT.BIN** is encrypted (`~PSP` signed format), decrypted with pspdecrypt (tag D9160BF0, type 2)
- **BOOT.BIN** is also encrypted (all zeros header), not an unencrypted fallback

### PRX format implications

The binary is a PRX (PlayStation Relocatable Executable), not a static ELF. This is standard for PSP Minis and has positive implications:
- Full relocation entries (`.rel.*` sections) identify every address-dependent instruction, aiding disassembly and reference resolution
- splat can use relocation info for correct pointer/reference identification
- Matching comparison works on relocatable `.o` sections — the relocations handle address binding
- The SNC toolchain includes `pspprxgen.exe` for producing PRX output

### DATA.PAK format

Custom ViciousEngine archive format at `PSP_GAME/USRDIR/DATA.PAK` (95 MB):
- **Header**: 16 bytes — `table_total_size(u32)`, `entries_size(u32)`, `version(u32)`, `table_offset(u32)`
- **File table at offset 0x800**: depth-first directory tree encoded as u32 arrays
- **2048-byte sector alignment** for file data (matching PSP UMD sector size)
- **50 files total** including game data, assets, and debug symbols
- **Extraction script**: `tools/extract_pak.py`

Notable contents:
- `Game-dvd.sym` (5.6 MB) — ELF with full debug symbol table
- `Game-dvd-pal.sym` (5.6 MB) — PAL version symbols
- `Game-dvd.map` (2.6 MB) — SN Systems linker map
- `Modules/.svn/` — developer left Subversion metadata in the build
- `pspnet_ap_dialog_dummy.prx` — networking dialog stub (dev leftover, not loaded at runtime)
- Game data: Configurations, EntityTemplates, Games, LoadingScreens, Maps (11 levels), Sounds (5 music files)

### Symbol files

**Game-dvd.sym**: ELF 32-bit LSB MIPS with `.symtab` and `.debug_str` sections. Contains **12,614 symbols** (9,047 functions) with full names.

**Game-dvd.map**: SN Systems linker map (text format). Contains **9,287 C++ demangled function signatures** with:
- Addresses and sizes
- Source `.obj` file paths (e.g., `c:\work\bcracing\build\psp\FinalDVD\gGame\cAll_psp.obj`)
- Compilation unit boundaries

**Format note**: These are NOT PPSSPP `.sym` format. The `.sym` files are ELF binaries with debug info (likely SN Systems ProDG format). The `.map` is a standard SN Systems linker map. ghidra-allegrex's `PpssppImportSymFile` script cannot import these directly — a custom converter is needed.

### Visible code architecture

From the linker map, the ViciousEngine class hierarchy includes:
- **Core**: `cThread`, `cMemAllocator`, `cFastMemAllocator`, `cDynamicMemAllocator`, `cMemPool`, `cBase`
- **File I/O**: `cFile`, `cFileSystem`, `cFileSystemPlatform`, `cBufferedFile`
- **Game**: `gcDoReadFile`, `gcDoRecord`, `cListSubscriber`, `cLanguage`
- **Major compilation units**: `cAll_psp.obj` (0x106,444 bytes), `eAll_psp.obj`, `gcAll_psp.obj`, `mAll_psp.obj`, `nwAll_psp.obj`, `gMain_psp.obj`

## Compiler Toolchain

### SNC compiler (verified working on macOS ARM)

**Binary**: `pspsnc 1.2.7503.0` from decompme/compilers. Windows PE32 executables run via wibo under Rosetta 2.

**Execution chain verified**: macOS ARM64 -> Rosetta 2 -> wibo (x86_64 Mach-O) -> pspsnc.exe (PE32)

**Archive contents** (21 files in `tools/snc/`):

| Tool | Version | Status | Purpose |
|------|---------|--------|---------|
| `pspsnc.exe` | 1.2.7503.0 | Working | Compiler driver |
| `pspcfe.exe` | — | Working | C/C++ frontend |
| `pspcor.exe` | — | Working | Optimizer/register allocator |
| `pspcq.exe` | — | Working | Code quality / IPA backend |
| `pspipa.exe` | — | Partial | Inter-procedural analysis |
| `pspas.exe` | 1.1.1737.0 | Working | MIPS assembler |
| `pspld.exe` | 2.7.63.0 | Broken | Linker (missing wibo DLL imports) |
| `pspprxgen.exe` | 1.7.174.0 | Working | PRX generator |
| `snarl.exe` | 1.5.143.0 | Working | Archive librarian |
| `snbin.exe` | — | Broken | Binary utility (missing wibo imports) |
| `pspname.exe` | — | Working | C++ name demangler |

**The core compilation pipeline works**: pspsnc -> pspcfe -> pspcor -> pspas. The linker is broken under wibo but can be replaced by GNU ld from mipsel-linux-gnu-binutils.

**Compiler flags**:
- Optimization: `-O0` through `-O5` (6 levels). No `-Os`.
- GP control: `-G0`, `-G4`, `-G8`
- Debug: `-g`
- Language: `-Xc=mixed+gnu_ext` (default), `-Xc=ansi`
- 108 `-X` control variables for fine-tuning (inline levels, unroll, scheduling, etc.)
- Predefined macros: `__SNC__`, `__PSP__`, `__psp__`, `_mips=2`, `__mips=2`, `MIPSEL`, `__MIPSEL__`
- Expected include paths: `../include_snc/` and `../include/` relative to pspsnc.exe

### SNC version matching (preliminary)

**First match confirmed**: `cGetConfigTerritory(void)` (returns 0) compiles to byte-identical output at -O2 through -O5:
```
Original:  03e00008 00001025  (jr ra; move v0,zero)
SNC -O2+:  03e00008 00001025  (jr ra; move v0,zero)
```

At -O0 and -O1, SNC does NOT use the branch delay slot (produces 3 instructions instead of 2), so the game was compiled at **-O2 or higher**.

Full matching test with complex functions requires the build environment (headers, linker script). The optimization level and exact `-X` flag combination will be determined empirically during build system setup.

### SDK headers

**pspdev/pspsdk** (open source, BSD licensed) provides comprehensive PSP OS API headers:
- Kernel/threading, file I/O, memory management, graphics (GU/GUM/GE), audio (PCM, AT3, AAC, MPEG), networking, controller input, utilities, UMD, USB
- Struct definitions are reverse-engineered but battle-tested across thousands of homebrew applications
- Ghidra type archive (`pspsdk.gdt`) exists for analysis
- **Installation**: `brew install pspdev/psp/psp-dev` (native ARM64 macOS builds available)

**Limitation**: pspdev headers use GCC extensions (`__attribute__`, etc.) and cannot be compiled directly with SNC. For byte-exact matching compilation, the official Sony SDK headers are needed. The pspdev headers serve analysis and understanding; the official headers serve compilation.

**Action needed**: The official Sony PSP SDK headers must be obtained separately for SNC compilation. pspdev headers are sufficient for all analysis work.

## Pipeline Tools

### splat (binary splitter)

PSP support is first-class (`platform: psp`). Key configuration:
- `compiler: SNC` — recognized, affects function boundary detection
- `gp_value` — critical for `$gp`-relative reference resolution, must be extracted from binary
- `section_order` — must match actual ELF layout (verify: `.text`, `.rodata`, `.data`, `.bss`)
- `vram` — for PRX, this is the base address after relocation
- Handles PRX-format ELFs with relocation info

Reference configs available from sotn-decomp project (MWCCPSP, adaptable to SNC).

### m2c (MIPS-to-C decompiler)

**Installed and working** on macOS ARM (Python-based). Supports `mipsel` target architecture. Does NOT list SNC as a compiler — GCC or MWCC compiler flags may produce reasonable starting C. Allegrex-specific instruction support untested but standard MIPS II should work.

### asm-differ

Straightforward setup. Configured via `diff_settings.py` with arch `mips`, pointing to original and rebuilt binaries. Uses mipsel objdump for disassembly comparison. Reference config available from sotn-decomp.

### mipsel-linux-gnu-binutils

**Installed via Homebrew**: `brew install mipsel-linux-gnu-binutils` — provides `mipsel-linux-gnu-objdump`, `mipsel-linux-gnu-readelf`, `mipsel-linux-gnu-as`, `mipsel-linux-gnu-ld` at `/opt/homebrew/bin/`. Version 2.46 includes upstream Allegrex support.

### pspdecrypt

**Built and working** on macOS ARM. Requires OpenSSL (`brew install openssl@3`) and build flags:
```
CFLAGS="-O3 -I$(brew --prefix openssl@3)/include" \
CXXFLAGS="-O3 -I$(brew --prefix openssl@3)/include" \
EXTRA_FLAG="-L$(brew --prefix openssl@3)/lib" make
```
Successfully decrypted EBOOT.BIN (tag D9160BF0, type 2).

## Runtime and Analysis Tools

### PPSSPP WebSocket API

Comprehensive debugger API over WebSocket (default port 12345, protocol `debugger.ppsspp.org`):

| Subscriber | Capabilities |
|------------|-------------|
| CPUCore | Stepping, resume, step-into/over/out, register read/write, expression eval |
| Disasm | Disassemble address ranges, search for patterns |
| Breakpoint | Execution breakpoints, memory watchpoints (read/write/change), conditional |
| Memory | Read/write arbitrary memory (u8/u16/u32/bulk base64), read strings |
| MemoryInfo | Memory mapping, region tagging |
| HLE | Function list, module list, thread list, backtraces |
| Game | Status, reset, lifecycle notifications |
| Input | Button/analog injection |
| Replay | Save states, input recording/playback |
| GPU | Framebuffer screenshots, render target inspection |

**Headless mode**: PPSSPPHeadless with `--debugger=PORT --timeout=N --graphics=null` runs without display. Native macOS ARM builds available (`brew install --cask ppsspp` for GUI; build from source for headless).

**Automation viable**: Set breakpoints, capture register/memory state, inject code patches, save/load states — all programmatically via WebSocket.

### Ghidra headless + ghidra-allegrex

`analyzeHeadless` supports full non-interactive workflow: import -> analyze -> run scripts -> output. ghidra-allegrex provides Allegrex processor module, PSP ELF/PRX loader with relocation support, VFPU instruction handling.

**Key scripts**:
- `PpssppImportSymFile` / `PpssppExportSymFile` — for PPSSPP-format `.sym` files only (NOT ProDG format)
- `SonyPSPResolveNIDs.py` — resolves OS function import stubs to names
- Custom bulk decompilation via `DecompInterface.decompileFunction()` loop

**ProDG .sym import**: Requires custom parser. The `.sym` files are ELF binaries with debug info, not text. A converter to PPSSPP format or direct Ghidra `createFunction()`/`setName()` API calls is needed. Reference: `mefistotelis/psx_mnd_sym` for PS1 variant of MND format.

## Build System Design

### Reference: sotn-decomp patterns

The sotn-decomp project (Castlevania PSP, using MWCCPSP) provides a directly transferable build pattern:

```
Extract:  splat split config.yaml  →  asm/, src/ stubs, linker scripts
Compile:  .c  →  wibo pspsnc.exe  →  .o
Assemble: .s  →  mipsel-linux-gnu-as  →  .o
Link:     .ld + .o files  →  mipsel-linux-gnu-ld  →  .elf
PRX:      .elf  →  wibo pspprxgen.exe  →  .prx (if needed)
Compare:  diff rebuilt vs. original
```

**~70-80% of sotn-decomp infrastructure is directly reusable.** What transfers: splat configs, asm-differ setup, directory conventions, `INCLUDE_ASM` macro pattern, Allegrex binutils, Makefile structure. What changes: compiler binary (pspsnc instead of mwccpsp), compiler flags, wrapper script.

### Proposed SNC pipeline

1. **Compiler wrapper**: Simple script invoking `wibo pspsnc.exe -c -O2 [flags] -o output.o input.c`
2. **Assembler**: `mipsel-linux-gnu-as -march=allegrex -mabi=eabi -EL`
3. **Linker**: `mipsel-linux-gnu-ld` with generated linker script (pspld.exe is broken under wibo)
4. **PRX generation**: `wibo pspprxgen.exe` if PRX comparison is needed; otherwise compare at .o level
5. **Diff**: `asm-differ` with mipsel-linux-gnu-objdump

## Agent Skill Design

Each tool maps to a Claude Code skill wrapper:

| Skill | Wraps | Purpose |
|-------|-------|---------|
| `compile` | `wibo pspsnc.exe -c` | Compile C to .o |
| `assemble` | `mipsel-linux-gnu-as` | Assemble .s to .o |
| `link` | `mipsel-linux-gnu-ld` | Link .o files |
| `split` | `splat split` | Split binary into asm/data segments |
| `diff` | `asm-differ diff.py` | Compare function assembly |
| `decompile-m2c` | `m2c` | Generate initial C from assembly |
| `decrypt` | `pspdecrypt` | Decrypt PSP EBOOT.BIN |
| `extract-pak` | `tools/extract_pak.py` | Extract DATA.PAK contents |
| `ghidra-analyze` | `analyzeHeadless` | Headless Ghidra analysis |
| `ppsspp-debug` | WebSocket client | Runtime testing via PPSSPP |
| `permute` | `decomp-permuter` | Brute-force C variations for matching |

## User Action Required

1. **Official Sony PSP SDK headers** — needed for SNC compilation. pspdev/pspsdk headers work for analysis but use GCC-specific syntax incompatible with SNC. The leaked SDK v6.6.0 contains the official include directory. Source this when ready to begin compilation matching.

## Unresolved Items

1. **Exact SNC optimization level**: Confirmed >= -O2, but could be -O2, -O3, -O4, or -O5. Requires testing complex functions with the full build environment to determine.
2. **SNC `-X` flag combination**: 108 control variables exist. The exact combination used by ViciousEngine must be determined empirically.
3. **ProDG .sym parser**: The Game-dvd.sym file is an ELF with debug info, not a text file. A parser is needed to extract the symbol table for import into Ghidra and for generating splat symbol address files.
4. **Linker replacement**: pspld.exe is broken under wibo. GNU ld should work but may produce different section layout. Needs testing.
5. ~~**PRX comparison strategy**~~: **Resolved.** Function-level .o comparison via asm-differ `-o` mode is the primary workflow. Full binary rebuild is blocked by VFPU assembler support (see below).
6. **SDK header compatibility**: Exact differences between pspdev headers and official SDK headers for the APIs this game uses.
7. **VFPU assembler support**: Standard `mipsel-linux-gnu-as` (binutils 2.46) does not support PSP VFPU instructions (~24K occurrences in asm/0.s: sv.q, lv.q, vdot.t, vsqrt.s, mfv, etc.). Full binary rebuild requires either a PSP-aware GAS build or encoding these as raw `.word` directives. Function-level matching at .o granularity is unaffected.
8. **splat asm post-processing**: GAS cannot handle `[]` or `~` in symbol names, and auto-generates broken `.size` from `.ent`/`.end` pairings on some symbols. After any splat re-run, apply: `[]` → `_arr_`, `~` → `_dtor_`, strip `.ent`/`.end` lines.
