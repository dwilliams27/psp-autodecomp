# Decompilation Phases

North-star roadmap for taking Days of Thunder from raw binary to fully matching C. Phases are sequential but overlap — type recovery runs continuously from Phase 2 onward. Mark each phase complete as it finishes.

## Codebase profile (from linker map analysis)

These numbers inform every phase's prioritization:

- **10,045 functions** total, **9,767 with real code** (272 zero-size stubs, 6 data mislabeled)
- **Only 8 .obj files**, 5 are unity builds (`*All_psp.obj` = 93% of functions, 97% of code)
- **gcAll_psp.obj** alone: 6,190 functions (49%), 2.2 MB
- **gMain_psp.obj** (816 functions, 112 KB) is SNC's C runtime library — not game code

### Size distribution

| Range | Functions | % of count | % of bytes |
|-------|-----------|------------|------------|
| ≤8 bytes | 1,043 | 10.7% | ~0.2% |
| 9-64 bytes | 1,004 | 10.3% | ~0.9% |
| 65-256 bytes | 3,736 | 38.2% | ~14% |
| 257-1024 bytes | 3,377 | 34.5% | ~40% |
| >1024 bytes | 613 | 6.3% | ~45% |

The 613 functions over 1 KB contain nearly half the binary's bytes. But the 2,047 functions under 64 bytes contribute only ~1% of bytes — they're cheap to match and build momentum.

### Architectural layers (by class prefix)

- **`c` (core)**: 188 classes, 585 functions. Memory, file I/O, serialization, type system.
- **`e` (engine)**: 281 classes, 3,141 functions. Collision, models, animation, rendering, physics.
- **`gc` (game classes)**: 458 classes, 5,129 functions. Game logic built on ViciousEngine.
  - **189 `gcDo*`/`gcVal*` template classes** (2,114 functions): Highly uniform action/value pattern with `VisitReferences`, `Evaluate`, `Read`, `Write`. Match a few exemplars → template the rest.
- **`m` (math)**: 14 classes, 64 functions, 22 KB. Small, self-contained. Ideal early target.
- **`nw` (network)**: 15 classes, 167 functions. Networking, voice chat.

## Compiler flags (determined)

**-O2 is confirmed.** Testing across 22 functions of varying complexity:
- -O2, -O3, -O4 produce **byte-identical output** on all tested functions
- -O5 differs only on functions with OR-condition loops and dead initializations (original binary matches -O2/-O3/-O4 patterns, not -O5)
- -O0 and -O1 produce dramatically larger code (~2x) and don't fill branch delay slots

The remaining flag question is whether any SNC `-X` control variables (108 exist) were used. This is best determined empirically: if a function won't match at -O2 despite correct C, investigate `-X` flags. No evidence yet that any are needed.

---

## Phase 0: Infrastructure [x]

Get the build system working and prove one byte-exact match on a real function.

### Deliverables (all complete)

1. **Symbol address file** — `config/symbol_addrs.txt` with 10,045 functions + 2,218 data symbols
2. **splat config** — `config/splat.yaml`, splits decrypted EBOOT into asm/data segments
3. **Makefile** — compiles C/C++ with SNC, assembles with GAS, links with GNU ld
4. **diff_settings.py** — asm-differ config for function-level .o comparison
5. **First real match** — `eWorld::LockWorld(bool)` (36 bytes, branch + delay slots), byte-exact

### Known issues discovered

- **Full binary rebuild blocked**: ~24K VFPU instructions unsupported by standard `mipsel-linux-gnu-as`. Needs PSP-aware assembler or `.word` encoding. Function-level .o comparison is unaffected.
- **splat asm post-processing required**: GAS can't handle `[]`, `~` in symbol names or `.ent`/`.end` on some symbols. Must reapply patches after any splat re-run.

## Phase 1: Matching Automation [x]

Build the tooling that lets agents work independently. Without this, nothing scales.

### Deliverables (all complete)

All tools live in `tools/`, third-party dependencies in `extern/` (gitignored).

1. **`func_db.py`** — parses linker map into `config/functions.json` (9,966 functions). Queryable by class, size, .obj file, match status, leaf status. Tracks match progress.
2. **`extract_func.py`** — extracts function bytes from original binary, creates expected .o files via `.incbin` (handles VFPU). Supports single function or batch mode.
3. **`compare_func.py`** — compiles source with SNC, compares .o bytes against expected, reports match/mismatch. Size-indexed lookup for O(1) candidate matching. Auto-updates match status in functions.json.
4. **`decompile_func.py`** — objdump-to-m2c format converter (register names, branch labels, JAL symbol resolution) + m2c invocation. Produces initial C for any function.
5. **`call_graph.py`** — raw binary scan for JAL instructions (no objdump needed). 7,531 functions make calls, 2,229 are leaf functions. Enriches functions.json with callers/callees.
6. **`common.py`** — shared utilities: `load_db`, `save_db`, `find_function`, `filter_functions`, `build_addr_map`, constants.

### Agent workflow

```bash
python3 tools/func_db.py query --class mSphere --size-max 64    # find target
python3 tools/extract_func.py "mSphere::SomeMethod"              # create expected .o
python3 tools/decompile_func.py "mSphere::SomeMethod"            # get initial C
# edit the C in src/mSphere.cpp
python3 tools/compare_func.py src/mSphere.cpp                    # compile + compare
python3 tools/call_graph.py show "mSphere::SomeMethod"           # check dependencies
```

### -X flag investigation (if needed)

Monitor for functions that produce correct logic but wrong instruction scheduling or register allocation at -O2. These may indicate `-X` control variables. No evidence of any needed yet — revisit if matching failures cluster around specific patterns.

## Phase 2: Type Bootstrap + Early Matching [ ]

Interleave type discovery with matching — you can't do one without the other. Start with the easiest functions to build momentum and struct knowledge.

### Wave 1: Trivial functions (≤8 bytes, ~1,000 functions)

These are single-instruction functions: return a constant, load a field, call a trampoline. Match rate should approach 100% with correct flags. Each match is cheap but reveals:
- Return types (what does this getter return?)
- Field offsets (which struct field at which offset?)
- Call patterns (what does this trampoline forward to?)

### Wave 2: Math library (64 functions, 22 KB)

The `m` prefix classes (`mSphere`, `mRay`, `mBasis`, `mFrustum`, `mQuat`, etc.) are self-contained with no dependencies on game logic. Pure computation. Ideal for:
- Validating the matching pipeline at scale
- Building confidence in compiler flag settings
- Producing reusable math headers

### Wave 3: Core class constructors + simple methods

For each class, the constructor reveals the struct layout (field initialization order, vtable pointer, default values). Strategy:
1. Match the constructor → write the class header with field offsets
2. Match simple methods (getters, setters, flag tests) → confirm field types
3. Use the header to match progressively harder methods

Priority classes: `cMemAllocator` family (already started), `cListSubscriber`, `cFile`, `cObject` — the foundation classes that many others inherit from.

### Snowball effect

Early functions are hard because types are unknown. But each match adds type info that constrains subsequent functions. By ~30% matched, ~70%+ of struct layouts should be known.

### Phase 2 is complete when

- All ≤8 byte functions are matched
- Math library is matched
- At least 10 class headers exist with verified struct layouts
- Matching rate on attempted functions exceeds 50%

## Phase 3: Systematic Matching [ ]

The main grind. Work through all functions systematically using the tooling and type knowledge from Phases 1-2.

### Priority ordering

1. **Leaf functions** — call nothing, pure computation. No type dependencies.
2. **Functions whose callees are all matched** — types flowing through are known.
3. **Functions in well-understood classes** — struct layouts known, method patterns familiar.
4. **Template classes (gcDo*/gcVal*)** — 189 classes, 2,114 functions following uniform patterns. Match 3-5 exemplars per pattern, then template-generate the rest.
5. **Large/complex functions** — saved for last when maximum context is available.

### Agent context package

Each agent receives:
- **Function identity**: name, address, size, .obj file, class
- **Disassembly**: from the original binary
- **m2c output**: first-pass C (pre-generated)
- **Type context**: current class headers for all referenced types
- **Neighbor context**: already-matched C for same class / same compilation unit
- **Call graph**: callees (with signatures) and callers

### Agent iteration loop

1. Read disassembly + m2c output
2. Write C using known types and struct definitions
3. Compile with SNC
4. Diff against expected
5. If match → done
6. If mismatch → analyze: register allocation (reorder declarations), branch structure (if↔switch), scheduling (expression order), and retry
7. After N attempts, report failure with best diff

### Template class acceleration

The `gcDo*`/`gcVal*` classes are ~21% of all functions. They share:
- Same set of virtual methods (`VisitReferences`, `Evaluate`, `Read`, `Write`, etc.)
- Same serialization patterns
- Same memory layout conventions

Match 3-5 complete exemplar classes, extract the pattern, and generate C for the remaining ~184 classes. Validate each with the comparison pipeline.

### Progress tracking

- Total functions: ~9,767 (excluding zero-size stubs)
- Track: matched / failed / in-progress / untried
- Track by byte count (since 613 large functions = 45% of bytes)
- Track by class (how complete is each class?)

### Phase 3 is complete when

- ≥90% of functions matched by count
- ≥95% of bytes matched

## Phase 4: Hard Functions [ ]

Functions that resist automated matching. These require deeper analysis:

- **Complex control flow** — deeply nested loops, switch with fallthrough, non-obvious optimizer choices
- **SNC-specific idioms** — patterns where SNC generates code that no straightforward C produces
- **VFPU code** — vector floating-point operations (uncommon in game logic, but possible in engine math)
- **Large functions** — the 613 functions >1 KB, especially `gcGame::VisitReferences` (30 KB), `gcEntity::Replicate` (11 KB)
- **Static initializers** — `__sti__gcAll_psp_cpp` (124 KB) and `__sti__eAll_psp_cpp` (74 KB) are compiler-generated type registration chains
- **C runtime** — gMain_psp.obj (816 functions) is SNC's libc. May be matchable by finding the exact SNC runtime library source, or may need function-by-function work.

Strategies:
- **decomp-permuter** — brute-force C variations
- **Cross-reference with other ViciousEngine games** — Alien Syndrome and others may share engine code
- **Manual human review** — for the truly stubborn cases

## Phase 5: Completion + Validation [ ]

All ~9,767 real functions match byte-exact.

### Full binary rebuild

Requires solving the VFPU assembler blocker:
- Option A: Build a PSP-aware GAS (from pspdev toolchain) that handles VFPU instructions
- Option B: Encode all VFPU instructions in asm/0.s as `.word` directives (automatable)
- Option C: Use SNC's own assembler (pspas.exe, which works under wibo) for VFPU sections

### Final verification

- Full binary rebuild produces SHA1-identical output to original EBOOT
- Every function individually verified via asm-differ
- Clean build from source with no manual patches

### Artifacts

- Complete C/C++ source tree
- Header files with all struct/class definitions
- Build system that reproduces the binary
- Documentation of SNC compiler behavior

### What carries forward to Milestone 2

- SNC compiler flag knowledge
- ViciousEngine struct/class definitions (shared across games)
- Agent orchestration and matching skills
- Type recovery patterns and heuristics
- Tooling (splat configs need updating, but tools transfer)

---

## Open questions (revisit as we learn more)

1. **Are any `-X` flags needed?** No evidence yet. Will surface as matching failures with correct C logic but wrong codegen. Revisit during Phase 2/3.
2. **Ghidra headless: worth the setup cost?** m2c handles the tested functions well. Ghidra may add value for large functions (>1 KB) where its data flow analysis is stronger. Evaluate during Phase 3 if m2c hit rate drops.
3. **C runtime strategy**: Is gMain_psp.obj worth decompiling function-by-function, or can we find the original SNC runtime source? 816 functions is significant effort for non-game code.
4. **Template class automation**: The gcDo*/gcVal* hypothesis (match a few → generate the rest) needs validation. If the classes are less uniform than they appear, this becomes manual work on 2,114 functions.
5. **VFPU assembler solution**: Three options listed in Phase 5. Need to evaluate before full binary rebuild is attempted.
