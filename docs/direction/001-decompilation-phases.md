# Decompilation Phases

North-star roadmap for taking Days of Thunder from raw binary to fully matching C. Phases are sequential but overlap — type recovery (Phase 2) runs continuously alongside matching (Phase 3). Mark each phase complete as it finishes.

## Phase 0: Infrastructure [x]

Get the build system working and prove one byte-exact match on a real function.

### Deliverables

1. **Symbol address file** — parse the .map into splat-compatible `config/symbol_addrs.txt` (9,047 function entries: `FunctionName = 0xADDR; // size:0xNN`)
2. **splat config** — YAML config for the decrypted EBOOT that splits the binary into asm/data segments
3. **Makefile** — compile .c with SNC, assemble .s with mipsel-linux-gnu-as, link with mipsel-linux-gnu-ld, compare against original
4. **diff_settings.py** — asm-differ config for this project
5. **First real match** — pick a non-trivial function (not just `return 0`), write C, compile, verify byte-exact match end-to-end through the build system

### What this proves

The entire pipeline works: split -> edit -> compile -> link -> diff -> verify. Without this, nothing else can proceed.

## Phase 1: Compiler Flags + Tool Validation [ ]

Pin down the exact SNC flags and validate every tool the agents will use.

### Compiler flag determination

Match ~20-50 simple functions across different compilation units to determine:
- Exact optimization level (-O2? -O3? -O4?)
- Key `-X` control variables (inline level, unroll settings, scheduling, etc.)
- Whether all compilation units use the same flags or if some differ

Strategy: start with the smallest leaf functions (8-16 bytes), try each -O level, find the one where multiple functions match simultaneously. Then test progressively larger functions to validate.

### Tool validation

Before agents run unattended, every tool in the context-generation pipeline must be tested end-to-end and proven reliable. Specifically:

**Context generators (things agents will consume):**
- [ ] **Disassembler** — `mipsel-linux-gnu-objdump` correctly disassembles any function by address/name, including Allegrex extensions (CLZ, EXT/INS, SEB/SEH, rotates). Verify on functions that contain these instructions.
- [ ] **m2c** — produces usable initial C from a function's disassembly. Test on 10+ functions of varying complexity. Document which instruction patterns it handles well and which it struggles with.
- [ ] **Ghidra headless** — bulk decompilation of all 9,047 functions to pseudo-C files. Install Ghidra + ghidra-allegrex, write the analysis script, run it, verify output quality on a sample.
- [ ] **Symbol/map parser** — parse the .sym ELF and .map to extract: function names, addresses, sizes, source .obj file paths, class relationships. Output a structured format agents can query (e.g., "give me all methods on cFastMemAllocator" or "what functions does Reset() call?").
- [ ] **Call graph extractor** — for any function, identify its callees (from `jal` targets) and callers (from cross-references). Agents need to know what a function calls and what calls it.
- [ ] **Struct field tracker** — as functions are matched and struct offsets are discovered, maintain a header file per class. Agents need to read current struct definitions and propose additions.

**Build/verify tools (things agents will invoke):**
- [ ] **Compile skill** — `wibo pspsnc.exe -c` with the determined flags, returns success or errors
- [ ] **Diff skill** — `asm-differ` for a named function, returns match/mismatch with instruction-level details
- [ ] **Verify skill** — compile + diff + report: matched bytes, mismatched bytes, total bytes

Each tool must fail loudly on bad input (no silent fallbacks). If objdump can't disassemble an address, it errors. If m2c can't handle an instruction, it says so. If compilation fails, the full error is returned.

## Phase 2: Type Recovery [ ]

Reconstruct struct/class definitions from matched functions. This phase runs continuously alongside Phase 3 — every matched function potentially reveals new type information.

### What we already know from symbols

- All 9,047 function names with C++ demangled signatures (parameter types, const qualifiers, etc.)
- Class hierarchy implied by method names (cFastMemAllocator inherits from cMemAllocator)
- Virtual tables (addresses in .map)
- Compilation unit boundaries (which functions were in the same .obj file)

### What we must reconstruct

- **Struct/class member layouts** — field names, offsets, sizes, types. Discovered from assembly: `sw a0, 0x1C(v0)` means a field at offset 0x1C. The constructor is usually the best source since it initializes every field.
- **Enum values** — immediate constants in the assembly that represent enum members. Named from context.
- **Typedefs and forward declarations** — inferred from how types are used across functions.
- **Inheritance and vtable layout** — vtable entries map to virtual function addresses, which we can resolve from the symbol table.

### How it works

1. Match a constructor → discover the struct layout (fields and their initial values)
2. Match simple methods → confirm field types from how they're used
3. Write a header file for the class
4. Use that header to match more complex methods on the same class
5. Repeat — each class header makes the next one easier

### Snowball effect

Early functions are hard because types are unknown. But each matched function adds type information that makes subsequent functions easier. By the time we've matched 30% of functions, we likely know 70%+ of the struct layouts. This is the core feedback loop that makes autonomous decompilation tractable.

## Phase 3: Bulk Matching [ ]

The main grind. Work through all 9,047 functions systematically.

### Priority ordering

1. **Trivial functions** (≤16 bytes) — getters, setters, `return 0` stubs. Match rate should be near 100%. Proves the pipeline and builds confidence.
2. **Leaf functions** — call nothing, pure computation. No dependency on understanding other functions.
3. **Functions whose callees are all matched** — we understand the types flowing through, making the C more constrained.
4. **Functions in well-understood classes** — struct layouts are known, method patterns are familiar.
5. **Large/complex functions** — saved for last when maximum context is available.

### What an agent starts with for each function

The agent receives a context package:

- **Function identity**: name, address, size, source .obj file, class (if method)
- **Disassembly**: full MIPS assembly from the original binary
- **Initial C attempts**: Ghidra pseudo-C and m2c output (pre-generated)
- **Type context**: current struct/class header files for all types referenced by this function
- **Neighbor context**: already-matched C source for functions in the same class and same compilation unit
- **Call graph**: list of callees (with their signatures) and callers
- **Compiler info**: SNC version, optimization level, flags

### The agent's iteration loop

1. Read disassembly and initial C attempts
2. Write C incorporating known types and struct definitions
3. Compile with `/compile`
4. Check with `/diff`
5. If match → done, submit result
6. If mismatch → analyze the diff:
   - Wrong register allocation? → reorder variable declarations
   - Wrong branch structure? → change control flow idiom (if→switch, etc.)
   - Wrong instruction scheduling? → adjust expression evaluation order
   - Off by a few instructions? → try decomp-permuter for brute-force variation search
7. Repeat up to N attempts, then report failure with best diff achieved

### Progress tracking

The orchestrator maintains:
- Total functions: 9,047
- Matched: N (with matched byte count)
- Failed: M (with best diff score for each)
- In progress: K
- Percentage complete (by function count and by byte count)

## Phase 4: Hard Functions [ ]

Functions that resist automated matching after Phase 3. These require deeper analysis:

- **Complex control flow** — deeply nested loops, switch statements with fallthrough, where SNC's optimizer makes non-obvious choices
- **Compiler-specific idioms** — patterns where SNC generates code that no straightforward C would produce (requires knowing SNC's specific optimization quirks)
- **VFPU code** — vector floating-point operations, if any exist in game logic (unlikely for a racing game's non-rendering code, but possible)
- **Functions that depend on unmatched functions** — circular dependencies where two functions reference each other's types

Strategies:
- **decomp-permuter** — brute-force C variations to find matching output
- **Cross-reference with other ViciousEngine games** — if we can find similar functions in Alien Syndrome or other ViciousEngine titles, the same C patterns may apply
- **Manual human review** — for the truly stubborn cases, a human looks at the diff and provides direction

## Phase 5: Completion + Validation [ ]

All 9,047 functions match byte-exact.

### Final verification
- Full binary rebuild produces byte-identical output to the original EBOOT
- Every function individually verified via asm-differ

### Artifacts
- Complete C source tree that compiles to a matching binary
- Header files with all struct/class definitions
- Build system that reproduces the binary from source
- Documentation of SNC compiler behavior and flag choices

### What carries forward to Milestone 2
- SNC compiler flag knowledge
- ViciousEngine struct/class definitions (shared across games)
- Agent skills and orchestration system
- Type recovery patterns and heuristics
- All tooling (splat configs need updating, but the tools transfer)
