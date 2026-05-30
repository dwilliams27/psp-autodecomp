export const meta = {
  name: 'overnight-match-parallel',
  description: 'Attack a slice of independent PSP functions for byte-exact matches IN PARALLEL (high compute; for split-TU / non-conflicting targets)',
  whenToUse: 'Driven by the overnight conductor when targets are independent (distinct src files, no shared header edits). args = array of function target objects.',
  phases: [{ title: 'Match', detail: 'concurrent deep agents, one per function' }],
}

const SCHEMA = {
  type: 'object', additionalProperties: false,
  properties: {
    address: { type: 'string' }, name: { type: 'string' },
    status: { type: 'string', enum: ['matched', 'near_miss', 'failed', 'blocked'] },
    srcPath: { type: 'string' }, symbol: { type: 'string' },
    diffBytes: { type: 'number' }, exactSize: { type: 'boolean' },
    structurallyComplete: { type: 'boolean' }, asmRatioPct: { type: 'number' },
    anglesTried: { type: 'number' }, notes: { type: 'string' },
  },
  required: ['address', 'name', 'status', 'srcPath', 'diffBytes', 'exactSize', 'structurallyComplete', 'asmRatioPct', 'notes'],
}
const ROOT = '/Users/dwilliams/proj/psp-autodecomp'
const PHASE = 'Match'

function blockedResult(t, notes) {
  return { address: t.address, name: t.name, status: 'blocked', srcPath: '', symbol: '', diffBytes: 999, exactSize: false, structurallyComplete: false, asmRatioPct: 0, anglesTried: 0, notes }
}
function priorNotes(t) {
  const fn = t.failure_notes || []
  if (!fn.length) return 'none — fresh target.'
  return fn.map((n, i) => `  [attempt ${i + 1}] ${typeof n === 'string' ? n : (n.notes || JSON.stringify(n))}`).join('\n')
}
function buildPrompt(t) {
  const method = t.method_name || t.name
  const sym = t.mangled_symbol || '(derive via docs/research/snc-name-mangling.md)'
  return [
    'You are doing BYTE-EXACT decompilation of one PSP function for Days of Thunder (SNC / SN Systems ProDG compiler). Working dir: ' + ROOT + '. Byte-exact (masked-byte-identical) is the ONLY standard.',
    '',
    'TARGET',
    '  name:    ' + method,
    '  address: ' + t.address,
    '  size:    ' + t.size + ' bytes',
    '  obj:     ' + t.obj_file,
    '  mangled: ' + sym,
    '  is_leaf: ' + t.is_leaf,
    'PRIOR ATTEMPTS (do NOT repeat these dead ends):',
    priorNotes(t),
    '',
    'MANDATORY PROCESS',
    '1. Research: read CLAUDE.md "Matching workflow" + the docs/research/*.md fitting this pattern (VFPU->snc-struct-vfpu-codegen.md+007; FPU->snc-fpu-scheduling.md; loops/switch->snc-loop-switch.md; branch-likely->snc-branch-likely.md+006; ctors->snc-constructor-codegen.md; vtable->snc-vtable-layout.md; mangling->snc-name-mangling.md; reg-alloc->snc-register-allocation.md+sched docs).',
    '2. Expected: python3 tools/extract_func.py "' + method + '"   3. m2c seed: python3 tools/decompile_func.py "' + method + '"',
    '4. Write src/' + t.safe_name + '.cpp emitting symbol ' + sym + '.',
    '   - Class methods: #include the existing header OR (if the method is missing from it) declare the class LOCALLY in the .cpp (split-TU). NEVER add new method declarations to include/*.h (BANNED).',
    '   - Template/handle ctors: declare a template class; define the ctor OUT-OF-LINE (an in-class inline ctor emits NO text symbol under explicit instantiation), then add an explicit ctor instantiation for the concrete type. compare_func requires the emitted symbol to EXACTLY equal the DB mangled symbol (no extern "C" wrappers — those get no DB entry).',
    '   - Only create/modify src/*.cpp, src/*.c, include/*.h. NEVER touch tools/, config/, Makefile.',
    '5. Verify: python3 tools/compare_func.py src/' + t.safe_name + '.cpp --no-update-db . Drive the masked byte diff to 0.',
    '6. If stuck, try MULTIPLE restructurings (expression/eval order, loop/branch idiom, local declaration ORDER — it drives SNC register allocation/delay-slot fill — register-forcing locals, #pragma control sched=N, __asm__ volatile("" ::: "memory") barriers). Reuse matched-sibling structure when one exists.',
    '',
    'ASSEMBLY BAN: inline asm ONLY for VFPU/FPU ops with no C equivalent, inside a real C/C++ body. >50% inline-asm by instruction count, or wrapped integer-MIPS disassembly => restructure or status=failed.',
    'HONESTY: the conductor re-verifies every claimed match with byte_match. Never fabricate; on tool/env error return status=blocked with the verbatim error.',
    '',
    'Return the structured result (status=matched ONLY if compare_func reports 0-diff for ' + sym + '; near_miss = exact size + small register/scheduling residual; failed = exhausted/cannot reach exact size).',
  ].join('\n')
}

// ---- parallel driver: independent functions, concurrent agents (cap auto-applied) ----
let raw = args
if (typeof raw === 'string') { try { raw = JSON.parse(raw) } catch (e) { log('FATAL: args not JSON: ' + String(e).slice(0, 160)); return { results: [], bailed: true, reason: 'bad_args' } } }
const targets = Array.isArray(raw) ? raw : (raw && Array.isArray(raw.targets) ? raw.targets : [])
if (!targets.length) { return { results: [], bailed: false, reason: 'no targets (args type=' + (typeof args) + ')' } }

phase(PHASE)
log('parallel match: ' + targets.length + ' independent targets (concurrency auto-capped)')
const raw_results = await parallel(targets.map((t) => () =>
  agent(buildPrompt(t), { schema: SCHEMA, label: 'match:' + (t.method_name || t.name).slice(0, 38), phase: PHASE })
))
const results = raw_results.map((r, i) => r || blockedResult(targets[i], 'agent returned null (error/skip/rate-limit)'))
const blocked = results.filter((r) => r.status === 'blocked').length
const matched = results.filter((r) => r.status === 'matched').length
log('done: ' + matched + ' matched, ' + blocked + ' blocked of ' + targets.length)
const bailed = blocked >= Math.ceil(targets.length * 0.6)
return { results, bailed, blocked, matched, done: targets.length }
