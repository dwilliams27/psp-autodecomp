export const meta = {
  name: 'overnight-match-engine',
  description: 'Serially attack a slice of PSP functions for byte-exact matches (deep, research-driven, one function at a time)',
  whenToUse: 'Driven by the overnight conductor loop. args = array of function target objects from func_db.py.',
  phases: [
    { title: 'Match', detail: 'one deep agent per function, serial' },
  ],
}

// --- Structured result contract every per-function agent must return ---
const SCHEMA = {
  type: 'object',
  additionalProperties: false,
  properties: {
    address: { type: 'string' },
    name: { type: 'string' },
    status: {
      type: 'string',
      enum: ['matched', 'near_miss', 'failed', 'blocked'],
      description: 'matched=byte-exact (diffBytes 0). near_miss=exact size + small/register-only residual (permuter candidate). failed=exhausted angles, not close. blocked=tool/env error stopped progress (report verbatim, never fake success).',
    },
    srcPath: { type: 'string', description: 'the source file you wrote, e.g. src/<safe_name>.cpp' },
    symbol: { type: 'string', description: 'the emitted mangled symbol you matched, or "" ' },
    diffBytes: { type: 'number', description: 'residual masked byte diff from compare_func; 0 means exact' },
    exactSize: { type: 'boolean', description: 'compiled .text size == target size' },
    structurallyComplete: { type: 'boolean', description: 'exactSize AND residual is register-allocation/scheduling only (0 structural diffs)' },
    asmRatioPct: { type: 'number', description: 'approx % of function body that is inline-asm instructions (assembly ban: >50 is illegal)' },
    anglesTried: { type: 'number' },
    notes: { type: 'string', description: 'concise: what worked / what residual remains / next idea. This is persisted to the function DB.' },
  },
  required: ['address', 'name', 'status', 'srcPath', 'diffBytes', 'exactSize', 'structurallyComplete', 'asmRatioPct', 'notes'],
}

const ROOT = '/Users/dwilliams/proj/psp-autodecomp'
const PHASE = 'Match'

// Canonical "could not match" result — one definition so it can't drift from SCHEMA.
function blockedResult(t, notes) {
  return { address: t.address, name: t.name, status: 'blocked', srcPath: '', symbol: '', diffBytes: 999, exactSize: false, structurallyComplete: false, asmRatioPct: 0, anglesTried: 0, notes }
}

function priorNotes(t) {
  const fn = t.failure_notes || []
  if (!fn.length) return 'none — this is a fresh target.'
  return fn.map((n, i) => `  [attempt ${i + 1}] ${typeof n === 'string' ? n : (n.notes || JSON.stringify(n))}`).join('\n')
}

function buildPrompt(t) {
  const method = t.method_name || t.name
  return `You are doing BYTE-EXACT decompilation of one PSP function for the game Days of Thunder (SNC / SN Systems ProDG compiler). Working directory: ${ROOT}. Byte-exact is the ONLY standard — masked-byte-identical to the original, no post-processing.

TARGET
  name:    ${method}
  address: ${t.address}
  size:    ${t.size} bytes
  obj:     ${t.obj_file}
  mangled: ${t.mangled_symbol || '(derive via docs/research/snc-name-mangling.md)'}
  is_leaf: ${t.is_leaf}
PRIOR ATTEMPTS (do NOT repeat these dead ends — they failed for the reasons stated):
${priorNotes(t)}

MANDATORY PROCESS
1. Research FIRST. Read CLAUDE.md "Matching workflow", then the docs/research/*.md that fits this function's pattern:
   - VFPU (lv.q/sv.q, matrices) -> snc-struct-vfpu-codegen.md + docs/decisions/007-vfpu-native-types.md
   - FPU arithmetic/order -> snc-fpu-scheduling.md
   - loops / switch -> snc-loop-switch.md ;  branch-likely (beql/bnel/bnezl) -> snc-branch-likely.md + decisions/006
   - constructors w/ float members -> snc-constructor-codegen.md ;  virtual dispatch -> snc-vtable-layout.md
   - delay-slot inline-asm issues -> snc-delay-slots.md ;  name mangling -> snc-name-mangling.md
   - If prior notes mention register allocation or sched: read snc-register-allocation.md AND docs/research/snc-transition-zone-sched.md.
2. Build the expected object:  python3 tools/extract_func.py "${method}"
3. Seed C with m2c:           python3 tools/decompile_func.py "${method}"
4. Write a self-contained source file at  src/${t.safe_name}.cpp  that emits symbol ${t.mangled_symbol || '(the target symbol)'}.
   - For class methods, #include the existing class header from include/. DO NOT add new method declarations to include/*.h — that is BANNED (use the split-TU pattern; declare locally in the .cpp if needed).
   - Only create/modify files under src/*.cpp, src/*.c, include/*.h. Never touch tools/, config/, Makefile.
5. Compile + compare (read-only):  python3 tools/compare_func.py src/${t.safe_name}.cpp --no-update-db
   Read its diff report and drive the masked byte diff to 0.
6. When stuck, try MULTIPLE genuinely-different restructurings — expression/evaluation order, loop form (for/while/do), branch idiom, char-vs-pointer compares, register-forcing locals, #pragma control sched=N per the transition-zone doc, __asm__ volatile("" ::: "memory") barriers when the scheduler reorders within a block. Maximum effort: exhaust approaches before giving up. The permuter (CPU, not your job) will finish register/scheduling near-misses, so your goal on a hard one is to reach EXACT SIZE with only register/scheduling residual.

ASSEMBLY BAN (hard rule): never wrap disassembly in __asm__ as a "match". Inline asm is allowed ONLY for small VFPU ops that have no C/C++ equivalent, inside a real C/C++ body. If your file would be >50% inline-asm by instruction count, restructure or report status=failed. A .cpp that is mostly __asm__ is auto-rejected and wastes the run.

HONESTY: The conductor independently re-verifies every claimed match with the canonical byte_match verifier. A fabricated "matched" is detected and poisons the pipeline. If a tool errors or the environment blocks you, return status=blocked with the verbatim error — never paper over it.

Return the structured result. status=matched ONLY if compare_func reports an exact (0-diff) match for ${t.mangled_symbol || 'the target symbol'}.`
}

// ---- serial driver: one deep agent per function, in order ----
let raw = args
if (typeof raw === 'string') {
  // A string arg was MEANT to be JSON; a parse failure is a real bug (truncation,
  // double-encoding, bad quoting), not an empty batch. Fail loud per the no-silent-
  // fallback norm so the conductor backs off instead of silently skipping the slice.
  try { raw = JSON.parse(raw) }
  catch (e) {
    log(`FATAL: targets arg is not valid JSON: ${String(e).slice(0, 200)}`)
    return { results: [], bailed: true, reason: `malformed targets arg (JSON.parse failed: ${String(e).slice(0, 160)})` }
  }
}
const targets = Array.isArray(raw) ? raw : (raw && Array.isArray(raw.targets) ? raw.targets : [])
if (!targets.length) { return { results: [], bailed: false, reason: `no targets supplied (args type=${typeof args})` } }

phase(PHASE)
const results = []
let consecutiveErrors = 0

for (let i = 0; i < targets.length; i++) {
  const t = targets[i]
  let r = null
  try {
    r = await agent(buildPrompt(t), { schema: SCHEMA, label: `match:${(t.method_name || t.name).slice(0, 40)}`, phase: PHASE })
  } catch (e) {
    log(`agent error on ${t.name}: ${String(e).slice(0, 200)}`)
  }

  if (!r) r = blockedResult(t, 'agent returned no result (possible rate-limit or skip)')
  results.push(r)
  log(`${t.name}: ${r.status}  diff=${r.diffBytes}B  exactSize=${r.exactSize}  asm=${r.asmRatioPct}%`)

  // A thrown error AND an agent-reported `blocked` both mean the environment may be
  // unhealthy (rate-limit / tool outage) — count both toward the bail streak. Any real
  // result (matched/near_miss/failed) means the agent is working, so reset.
  if (r.status === 'blocked') {
    consecutiveErrors++
    if (consecutiveErrors >= 2) {
      log(`two consecutive blocked/error results after ${i + 1}/${targets.length} — bailing so the conductor can back off (rate-limit?) and let the CPU permuter pool run.`)
      return { results, bailed: true, reason: 'consecutive_blocked_results', done: i + 1 }
    }
  } else {
    consecutiveErrors = 0
  }
}

return { results, bailed: false, done: targets.length }
