# ADR-014: VisitReferences function-pointer backref mangling divergence

**Status:** Verified finding (point-in-time). Fix path is an open operator decision (A vs B below).
**Date:** 2026-05-31
**Relates to:** ADR-006 (bnel compiler divergence), ADR-008 (snc-name-mangling), ADR-009 (compiler-version search), ADR-011 (bnel compiler-patch design).

## Summary

All **346** `VisitReferences(unsigned int, cBase *, void (*)(cBase*, unsigned int, void*), void *)`
methods in the binary share a single, systematic **name-mangling-algorithm divergence**
between the original SNC mangler and our `pspsnc 1.2.7503.0`. The divergence is
**purely symbolic — there is ZERO codegen divergence.** The compiled instruction
bytes are byte-for-byte identical to the original; the functions cannot be
*credited* only because the emitted symbol string differs from the DB's recorded
mangled symbol, so the verifier's symbol gate never pairs them to their DB entry.

This is the largest single mangling-blocked family found to date.

## The divergence (exact)

For `cObject::VisitReferences` (0x00009cb0, 120B):

| | mangled symbol |
|---|---|
| DB / original SNC | `__0fHcObjectPVisitReferencesUiP6FcBasePFP6FcBase`**`TB`**`Pv_vPvTB` |
| our pspsnc emits  | `__0fHcObjectPVisitReferencesUiP6FcBasePFP6FcBase`**`Ui`**`Pv_vPvTB` |

The only difference is the inner `unsigned int` of the **nested function-pointer
parameter** `void (*)(cBase*, unsigned int, void*)`:

- **Original SNC:** backreferences inside a function-pointer parameter (`PF … _ ret`)
  draw from the **enclosing function's cumulative parameter type table**. The inner
  `unsigned int` is therefore encoded `TB` = "same type as outer param B" (the outer
  `flags` arg). The mangler keeps **one shared/global backref table** across the
  enclosing signature and its nested fn-ptr parameter lists.
- **pspsnc 1.2.7503.0:** keeps a **separate, fn-ptr-local backref table**. Backrefs
  inside a `PF` may only point at earlier params *within that fn-ptr*. The inner
  `unsigned int` has no local predecessor of that type, so it is spelled literally `Ui`.

The trailing outer `unsigned int` (the final param `…Pv`**`TB`**) correctly emits
`TB` in **both** compilers, because that is an outer→outer backref and both share
one table for the top level. The asymmetry is exactly the **cross-scope
(outer ↔ fn-ptr) backref** that this compiler version dropped.

### Decisive disambiguating probes (free functions, -O2)

| Source | our emitted symbol | interpretation |
|--------|--------------------|----------------|
| `f(int, void(*)(float,int))`     | `…iPF f i _v`        | inner `int` → `i`, NOT `TB`: no outer ref |
| `f(int, void(*)(float,float))`   | `…iPF f TB _v`       | inner 2nd `float` → `TB` = 1st *inner* float: local-only table |
| `f(uint, uint, void(*)(uint))`   | `…Ui TB PFUi_v`      | outer dup refs work; inner `Ui` never refs outer |

## What does NOT fix it

Exhaustively tested, all still emit `…P6FcBaseUiPv…`:
literal `unsigned int`; a shared `typedef u32`; a named `typedef VisitFn`; inner
param reordering; `const` qualification; and every codegen flag
(`-Xsched=0/1/2`, `-Xmopt=0`, `-Xxopt=0/5`). Mangling-adjacent flags `-Xcompat`
(rejects all tried integer/string values) and `-Xtmpl` (rejects 0–3) do not expose
the cumulative-backref behavior. **No source-level change can fix this** — it is a
compiler-version mangling-algorithm difference, in the same class as the bnel
branch-likely divergence (ADR-006).

## Verification that it is bytes-identical (not codegen)

- `cObject::VisitReferences` (120B): all 30 instructions identical between
  `build/src/cObject.cpp.o` (`…Ui…`) and `expected/bin/00009cb0.bin` (`…TB…`).
- `cBase::VisitReferences` (60B): byte-identical, independently confirmed.
- `ePortal::VisitReferences` (28B): trivial tail-call body, byte-identical.

`compare_func.py` reports `NO DB ENTRY: no database function has this mangled name`
for all three — the gate is `find_db_func_for_sym` in `tools/byte_match.py`, which
requires `sym_name == mangled_symbol` exactly. The `…Ui…` symbol never equals the
DB's `…TB…`, so byte comparison is never reached.

## Fix paths (OPEN — operator decision)

These 346 are byte-exact-correct functions blocked solely by an encoding artifact.
Two routes can credit them; they have different norm implications.

- **(A) Patch the SNC mangler binary** — change the backref-table scoping inside
  function-pointer parameter mangling to be cumulative (outer-inclusive), so emitted
  symbols become `…TB…`. This is the **norms-blessed route** (CLAUDE.md: "compiler
  divergences are fixed by patching pspcor.exe, not post-processing"; precedent in
  ADR-011 bnel patch). Blast radius: changes emitted symbols for *every* function
  with a nested fn-ptr param whose inner type repeats an outer type — **must be
  regression-tested against all currently-matched functions before deploy**
  (zero-breakage gate). High prize (one patch unlocks the whole family at the
  source), uncertain RE effort.

- **(B) Normalize the verifier symbol gate** — teach `find_db_func_for_sym` to treat
  `PF…<literal-type>…` and `PF…<backref-to-equal-outer-type>…` as equivalent when
  the literal type equals the outer-referenced type. Must be a precise
  demangle-and-compare equivalence (not textual munging) to avoid mispairing.
  **Byte comparison still runs unchanged afterward**, so it cannot manufacture a
  false byte match — the only theoretical risk (two different functions whose names
  normalize identically *and* whose bytes coincide) is astronomically unlikely.
  Lower effort / lower technical risk, but it handles a compiler divergence in the
  verifier rather than the compiler, which is in tension with the no-post-processing
  norm. Operator's call.

## Classification guidance

These functions must **not** be marked `failed` for codegen reasons — the codegen is
already exact. The accurate status is `unmatchable_symbol_mangling` (as 26 already
are). Hold reclassification of the remaining 302 untried + 18 failed until the A/B
decision lands: if path A or B is taken, they become creditable and many of the
small/medium bodies will match immediately. The large ones (cFactory 628B, cGroup
648B, gcEntityTemplate 6956B, …) still require real per-function body decompilation
and will hit the usual ADR-012 whole-TU register-coloring wall; the *mangling*
blocker, however, lifts uniformly across all 346.

## Mangler RE findings (2026-05-31 — path-A investigation)

A 3-phase RE workflow (recon ×4 → synthesis → prototype) investigated the path-A
compiler-mangler patch. Result: **diagnosis confirmed, exact patch site NOT pinned;
synthesis returned feasible=false (no blind patch).**

**Confirmed:**
- **The mangler is `extern/snc/pspcfe.exe`** (the SN-customized EDG C/C++ Front End,
  PE32 x86, ImageBase 0x400000, md5 `0ef5d17bf2e033885380b6663399849e`) — proven
  empirically: the buggy literal-`Ui` symbol appears in pspcfe's `.B` intermediate
  *before* pspcor/pspas run. (`pspname.exe` is a RED HERRING — it is an Itanium `_Z`
  demangler via `pspdemangle.dll`; the SN `__0f` *demangler* is `demangle.dll`.)
- **The substitution scheme is CUMULATIVE by design.** The `demangle.dll` decoder
  (entry `demangle`@0x1001adc0 → driver fcn.1001aa80 → recursive type parser
  fcn.10015f90, 50-way type-letter switch via tables 0x100166e4/0x10016714) keeps a
  single shared type-array at ctx `+0x2830` for the *entire* signature. In its
  function-type (`F`) case (0x100164e2–0x10016562) it SAVES+ZEROES only the two walk
  cursors `+0x2880`/`+0x2870` around the recursive param parse and RESTORES them after
  — it never touches `+0x2830`. So inside a function-pointer param, the substitution
  *table* stays cumulative; only per-subtype walk bookkeeping is locally framed. The
  original game's `TB` backref is exactly what this decoder expects.
- **The bug:** pspcfe's *encoder* analog resets the substitution LIST (not just the
  cursors) when entering a `PF…_` param list, so an inner type duplicating an outer arg
  is spelled literally instead of as `Tn`.

**Not pinned:** the exact encoder routine + list-reset instruction inside the 1.69 MB
pspcfe.exe. Recon converged on `0x51296f`, but synthesis identified that as the global
**type-canonicalizer** (touching type-identity globals 0x5be8e0/0x5c1b80) — NOT the
backref emitter; patching it would corrupt all type processing. **Do not patch blind.**

**Firsthand repro** (project toolchain): a free-fn probe with a
`(uint,cBase*,void(*)(cBase*,uint,void*),void*)` arg emits undefined symbol
`__0FBgUiP6FcBasePFP6FcBaseUiPv_vPv` — inner `Ui` literal, confirming the bug outside the
VisitReferences family.

**Validation plan for any future path-A patch:** (1) patch a COPY in /tmp; (2) assert the
probe's inner `Ui`→`TB`; (3) the 3 disambiguating probes keep their behavior
(`f(int,void(*)(float,int))` inner stays `i`; `f(int,void(*)(float,float))` inner-2nd stays
`TB`; `f(uint,uint,void(*)(uint))` outer-dup `TB` preserved); (4) the DB symbol
`__0fHcObjectPVisitReferencesUiP6FcBasePFP6FcBaseTBPv_vPvTB` reproduces; (5) ZERO-BREAKAGE
gate — rebuild every currently-matched function, any `.o` byte change aborts.

**Pinning the encoder needs** either dynamic tracing (watch which pspcfe code writes the
substitution-list head while mangling the probe) or anchored static RE on the `T`-token
emission + list append/search — a deeper, possibly multi-iteration RE pass.

### Deeper RE (2026-05-31, pass 2) — mangler routines PINNED, decisive probe identified

A second workflow (static anchors + a patch-and-test loop validated against real DB
symbols) pinned the actual mangler machinery in pspcfe.exe (non-relocatable, fixed load
0x400000, so guest VAs are stable):
- **0x4a5dc8** — central recursive per-type ENCODER. Dispatches on type tag `[node+0x32]`;
  function-type case is tag==8 @0x4a61ba → calls the F-helper.
- **0x4a3582** — FUNCTION-TYPE (`F`) helper. Emits `F`@0x4a3593, calls the walker@0x4a35bb,
  emits `_`@0x4a35ca, then recurses into params via 0x4a5dc8@0x4a35d7 using node `[esi+0x50]`.
- **0x4a345c** — SUBSTITUTION-LIST WALKER / backref emitter. Reads list head from
  `[eax+0x54]`@0x4a3464, walks a circular list, emits `T`@0x4a3550 on a head match (append
  via 0x4a5dc8@0x4a34e7). **The `T`-vs-literal decision lives here.**
- Leaf helpers (NOT the bug): char-emit 0x4a32f4, string 0x4a3324, numeric/index 0x4a334a,
  list-register 0x4a341f.

**Key disambiguation (why static RE kept mis-converging):** node field `+0x54` is OVERLOADED.
In the encoder 0x4a5dc8 it is the typedef/canonicalization redirect link (ties to the
0x51296f canonicalizer red herring — do NOT patch). In the walker 0x4a345c, `[eax+0x54]` is
read off the CONTEXT object and yields the substitution-LIST head — a different use of the
same displacement.

**All 10 emit-path candidate patches returned `no_effect`** → the bug is NOT a hardcoded
reset on the emit path. The remaining decisive question: when mangling the OUTER `unsigned
int` vs the INNER `unsigned int` (inside the PF), do the walker invocations present the
**same list-head object** (→ the list is reset on entering the PF param recursion at
0x4a35d7; an emit-/recursion-framing patch is viable) or **distinct list-head objects** (→
the substitution scope is rebuilt per-parameter-list-context UPSTREAM during recursive
descent; the patch must target where that context/scope is allocated — which would explain
the 10 no-effect results)?

**Next probe (concrete):** an in-binary TRAMPOLINE LOGGER at the walker prologue 0x4a345c
(5-byte `E8` call into a code cave in .text slack) that logs, per invocation while mangling
the firsthand probe: the context ptr (`eax`), the derived list head (`[eax+0x54]`), and the
emitted token (`T` vs literal). Compare the outer-`uint` call against the inner-`uint` call.
Prefer the in-binary trampoline over lldb (wibo+Rosetta2 makes lldb VA-mapping to translated
code unreliable). The answer directly localizes the patch.

### Deeper RE (2026-05-31, pass 3) — MECHANISM cracked

Dynamic JMP-trampoline loggers on /tmp copies (originals verified intact, md5
0ef5d17b…; the buggy `__0FBgUiP6FcBasePFP6FcBaseUiPv_vPv` reproduced) settled it — and
**refuted** the pass-3 trace agents' "distinct" guess:

- **Relationship is SAME.** Encoder-entry logger (0x4a5dc8, node=[ebp+8], ctx=[ebp+0xc]):
  every type in the probe signature shares the SAME context (0x7dff0c84); the outer `uint`
  and inner `uint` are the IDENTICAL canonical node (0x6c241820); both `cBase*` share one
  node, both `void*` share one. So the shared context + shared type identity a `TB` needs
  ARE present — refuting both "distinct context" AND "list reset."
- **Actual mechanism:** the `T` backref char is emitted at exactly one site, 0x4a3550,
  inside the walker 0x4a345c. The walker fires ONLY for the function-type node (dedup),
  **never for a scalar param**. The scalar inner `uint` is gated out at **0x4a6126**
  (`mov ecx,[node+0x54]; cmp dword[ecx],0; jne 0x4a6134; test byte[ecx+8],1; je …`) — only
  function-type/qualified paths consult the cumulative list. So the inner `uint` never runs
  the substitution SEARCH. THIS is why all 10 emit-path patches were no-ops: they edited a
  decision the inner `uint` never reaches.
- Combined EN+WK logger trace of the probe confirmed the walker fires once (for the PF/
  function-type node), and the scalar `uint` (outer and inner) never enters it.

**Toolchain intel (critical for any patch/logger):** JMP (`E9`) trampolines run cleanly
under wibo; CALL (`E8`) and `pushad` SIGBUS. Zero-filled `.text` caves at 0x53d4c0 /
0x53da00 / 0x53dc00. Oracle harness pattern: `/tmp/pspcfe.<…>/oracle.sh` (arg-vector via
extern/wibo + pspcfe.exe). Gotcha: a stray `/tmp/select.py` shadows stdlib — run python with
`PYTHONPATH=` and cwd outside /tmp.

**Next step + PRIME patch candidate:** (1) a walker-internal-loop logger over
0x4a3492–0x4a34c3 (log candidate [ebx+4], queried [edi+4], the type-equality call 0x52911d
@0x4a34a6, and the qualifier check `[ebx+0x11]^[edi+0x11]&3` @0x4a34b8) to decide whether the
inner `uint` is (a) never appended to the cumulative list or (b) appended but rejected by the
equality/qualifier test. (2) Prime same-size in-place candidate: the gate at **0x4a612c**
(`74 07` → `90 90`, or invert the `jne` @0x4a612c) so the scalar inner `uint` also consults
the already-cumulative list. Alternatives: the match test (`je` @0x4a34a0 / @0x4a34ba) or the
append (`call 0x4a5dc8` @0x4a34e7). Validate any candidate against the exact DB symbol +
3 sanity probes + zero-regression gate — making scalars search risks over-eager backrefs, so
the disambiguating probes matter here.

### Bearing on the A-vs-B decision
This RE *strengthens path B*: `demangle.dll` — SN's own decoder — treats the cumulative
form as canonical and would resolve our `Ui` and the DB's `TB` to the **identical type**.
So a verifier-side equivalence (path B) is recognizing two spellings of one type, not
loosening byte-exactness (the masked byte-compare still fully gates the match). Path B
remains hours of work; path A is now a confirmed multi-day RE with corruption risk and no
pinned site. Operator decision refreshed.
