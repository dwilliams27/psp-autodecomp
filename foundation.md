# Autonoma

Autonomous decompilation of PSP games using coding agent swarms.

## Goal

Build a system that can take an arbitrary PSP game binary and produce a fully matching C decompilation with no human involvement in the decompilation work itself. All code in this repository — the tooling, the orchestration, the decompilation output — is written and maintained by coding agents.

## Why PSP

The PSP is the most favorable target platform for autonomous decompilation across every dimension that matters:

- **ISA**: The Allegrex CPU is MIPS II at its base, with cherry-picked MIPS32R2 instructions (CLZ, EXT/INS, SEB/SEH, rotates) and proprietary extensions (VFPU, custom min/max). MIPS is the best-understood ISA in the decompilation ecosystem and in LLM training data. The entire N64/PS1 decomp community's tooling was built for MIPS, and tools like ghidra-allegrex handle the Allegrex-specific instructions explicitly.
- **Binary format**: PSP executables are ELF-based. EBOOT.BIN files are encrypted and require a decryption step (tools like pspdecrypt handle this), after which they are standard ELF. PRX (PlayStation Relocatable Executable) files use ELF with Sony-specific extensions (custom type values, relocation sections). The format is well-understood and well-tooled.
- **Compilers**: Commercial PSP games were built with one of three toolchains: Sony's official SDK (GCC-based), SN Systems ProDG (SNC, a proprietary compiler — used by the majority of commercial titles), or Metrowerks CodeWarrior (MWCCPSP). Leaked SDK versions (1.5.0, 6.6.0) exist, though compiler binary availability varies by toolchain and version. Byte-exact matching requires identifying the correct compiler for each target — this is an empirical question per game.
- **Existing tools**: `splat` (binary splitter) supports PSP as a first-class target. `m2c` (machine-code-to-C decompiler) produces initial C from MIPS disassembly. `asm-differ`, `decomp-permuter`, and `decomp.me` all work with MIPS — decomp.me has explicit PSP platform support. `ghidra-allegrex` is an actively maintained Ghidra processor module for the Allegrex CPU. There is a dedicated PSP RE community (psp-re.github.io) with a quick start guide.
- **Memory model**: 32MB baseline RAM (of which ~24MB is available to user code after kernel reservation), no ASLR, no MMU-based randomization. Memory addresses for the main executable are deterministic across sessions. The PSP does have a dynamic module system (PRX loading via `sceKernelLoadModule()`), and some games use it, but most commercial titles ship as a single monolithic ELF — far simpler than PS1's 2MB + constant overlay swapping.
- **Emulator**: PPSSPP has a WebSocket-based debugger API for programmatic control — breakpoints, stepping, memory read/write, register inspection, disassembly, and log streaming. It also has a built-in GUI debugger with disassembly view and memory breakpoints. Ghidra integration via the WebSocket API existed historically through ghidra-allegrex but was removed when Ghidra dropped the underlying API; static symbol exchange between the tools still works.
- **Catalog**: ~2,000 commercial titles across all regions. Almost none have been decompiled. Enormous opportunity.

## Milestones

### Milestone 1: Days of Thunder (PSP Mini)

Days of Thunder (USA, 2009) is a ViciousEngine PSP Mini. Like all ViciousEngine games, it ships with `.sym` and `.map` files (containing all original function and variable names) inside its `DATA.PAK`. It was compiled with SNC (SN Systems ProDG). As a Mini it is hard-capped at 100MB with no multiplayer or DLC — a small, constrained target with full ground truth available.

This milestone validates every component of the pipeline: EBOOT decryption, binary splitting, disassembly, initial C generation, compiler flag identification, byte-exact matching, and end-to-end orchestration. Every function match can be verified against known symbol names.

Deliverable: a fully matching C decompilation of Days of Thunder, produced entirely by agents.

### Milestone 2: Full Commercial Game (with symbols)

Graduate to a full-size ViciousEngine commercial game that also ships with debug symbols. The leading candidate is Alien Syndrome (USA, 2007), a ~274MB action RPG published by Sega. The specific target will be decided after Milestone 1 validates the pipeline, but the key constraint is shared engine lineage — same ViciousEngine, same SNC compiler, same symbol format — so the pipeline transfers directly. The only change is scale.

This is an unusual opportunity: a real commercial game where the system still has ground truth. The jump is from "small with answers" to "big with answers."

Deliverable: a fully matching C decompilation of a complete commercial PSP game, produced entirely by agents.

### Milestone 3: Full Commercial Game (without symbols)

Graduate to a ViciousEngine game that does not ship with debug symbols. Several exist (Dead Head Fred, 300: March to Glory, Puzzle Quest). Same engine, same compiler, but no ground truth — the system must identify function boundaries, infer names, and produce matching C with no answers to check against.

This is the real test of autonomous decompilation. Everything learned about ViciousEngine's code patterns and the SNC compiler's behavior from Milestones 1 and 2 informs this attempt, but the system must work without a safety net.

Deliverable: a fully matching C decompilation of a commercial PSP game with no debug symbols, produced entirely by agents.

## Principles

- **Agents do the work.** Every line of decompiled C, every tool script, every piece of research is produced by coding agents. The human role is architectural direction and unblocking.
- **Byte-exact matching is the only standard.** A function is done when it compiles to identical bytes. No "close enough."
- **Verify everything.** The ViciousEngine milestone exists specifically to validate the system against known ground truth before attempting a target without answers.
- **Research before action.** Agents should investigate tooling, compiler behavior, and prior art thoroughly before writing code. Many details in this document are intentionally high-level — agents are expected to research specifics independently.
