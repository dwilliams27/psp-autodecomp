# Battle Packet: `gcValNavMeshDirection::AssignCopy(const cBase *)`

## Target

- Address: `0x00350b88`
- Size: `784` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValNavMeshDirection`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 4B; codegen-specific diagnosis; AssignCopy retry family`
- failure_action: `research`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `tagged_pointer_shape`
- Near miss: `4`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x0011d404` `untried` 632B `gcDesiredEntity::operator=(const gcDesiredEntity &)`

## Placement

- Canonical source: `src/gcValNavMeshDirection.cpp`
- Header(s): `include/gcValNavMeshDirection.h`
- Allowed source(s): `src/gcValNavMeshDirection.cpp`, `src/gcValNavMeshDirection_AssignCopy.cpp`
- Split-TU prefix: `src/gcValNavMeshDirection_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcValNavMeshDirection.h`

Not found.

## Matched Same-Class Neighbors

- `0x00350e98` 300B `gcValNavMeshDirection::New(cMemPool *, cBase *) static` — `src/gcValNavMeshDirection.cpp`
- `0x00350fc4` 280B `gcValNavMeshDirection::GetType(void) const` — `src/gcValMouseState.cpp`
- `0x003510dc` 228B `gcValNavMeshDirection::Write(cFile &) const` — `src/gcValNavMeshDirection.cpp`
- `0x003511c0` 536B `gcValNavMeshDirection::Read(cFile &, cMemPool *)` — `src/eSurfacePropertyTable.cpp`
- `0x00352abc` 448B `gcValNavMeshDirection::~gcValNavMeshDirection(void)` — `src/gcValNavMeshDirection_dtor.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValGameSetting::AssignCopy(const cBase *)`

- Address: `0x00344658`
- Size: `480` bytes
- Source: `src/gcValGameSetting.cpp`

```cpp
// 0x00344658 - gcValGameSetting::AssignCopy(const cBase *)
void gcValGameSetting::AssignCopy(const cBase *base) {
    const gcValGameSetting *other = 0;

    if (base != 0) {
        if (!type_gcValGameSetting) {
            if (!type_variable) {
                if (!type_value) {
                    if (!type_expression) {
                        if (!type_base) {
                            type_base = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        type_expression = cType::InitializeType(
                            0, 0, 0x6A, type_base, 0, 0, 0, 0);
                    }
                    type_value = cType::InitializeType(
                        0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
                }
                type_variable = cType::InitializeType(
                    0, 0, 0x6D, type_value, 0, 0, 0, 0);
            }
            type_gcValGameSetting = cType::InitializeType(
                0, 0, 0x1B4, type_variable, gcValGameSetting::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValGameSetting;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValGameSetting *)base;
        }
    }

    operator=(*other);
}
```

### Exemplar 2: `gcValStatsTracking::AssignCopy(const cBase *)`

- Address: `0x0035bbac`
- Size: `480` bytes
- Source: `src/gcValStatsTracking.cpp`

```cpp
void gcValStatsTracking::AssignCopy(const cBase *base) {
    const gcValStatsTracking *other = 0;

    if (base != 0) {
        if (!type_gcValStatsTracking) {
            if (!type_variable) {
                if (!type_value) {
                    if (!type_expression) {
                        if (!type_base) {
                            type_base = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        type_expression = cType::InitializeType(
                            0, 0, 0x6A, type_base, 0, 0, 0, 0);
                    }
                    type_value = cType::InitializeType(
                        0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
                }
                type_variable = cType::InitializeType(
                    0, 0, 0x6D, type_value, 0, 0, 0, 0);
            }
            type_gcValStatsTracking = cType::InitializeType(
                0, 0, 0xE8, type_variable, gcValStatsTracking::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *wanted = type_gcValStatsTracking;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
        fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValStatsTracking *)base;
        }
    }

    operator=(*other);
}
```

### Exemplar 3: `gcValTableEntry::AssignCopy(const cBase *)`

- Address: `0x0035efa8`
- Size: `480` bytes
- Source: `src/gcValTableEntry.cpp`

```cpp
void gcValTableEntry::AssignCopy(const cBase *base) {
    const gcValTableEntry *other = 0;

    if (base != 0) {
        if (!type_gcValTableEntry) {
            if (!type_variable) {
                if (!type_value) {
                    if (!type_expression) {
                        if (!type_base) {
                            type_base = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        type_expression = cType::InitializeType(
                            0, 0, 0x6A, type_base, 0, 0, 0, 0);
                    }
                    type_value = cType::InitializeType(
                        0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
                }
                type_variable = cType::InitializeType(
                    0, 0, 0x6D, type_value, 0, 0, 0, 0);
            }
            type_gcValTableEntry = cType::InitializeType(
                0, 0, 0x218, type_variable, gcValTableEntry::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *wanted = type_gcValTableEntry;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValTableEntry *)base;
        }
    }

    operator=(*other);
}
```

### Exemplar 4: `gcValCinematicInfo::AssignCopy(const cBase *)`

- Address: `0x00323978`
- Size: `420` bytes
- Source: `src/gcValCinematicInfo.cpp`

```cpp
void gcValCinematicInfo::AssignCopy(const cBase *base) {
    const gcValCinematicInfo *other = 0;

    if (base != 0) {
        if (!type_gcValCinematicInfo) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_gcValCinematicInfo = cType::InitializeType(
                0, 0, 0x1DD, type_value, gcValCinematicInfo::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValCinematicInfo;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
        fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValCinematicInfo *)base;
        }
    }

    operator=(*other);
}
```


## Prior Failure Notes

### Note 1

- session `5fc0bdc7`; src `src/gcValNavMeshDirection_AssignCopy.cpp`; snapshot `logs/failure_snapshots/match_20260510_175602/5fc0bdc7/00350b88__agent_self_reported_failure__src_gcValNavMeshDirection_AssignCopy.cpp`

Closest attempt is 780B vs expected 784B (4-byte / 1-instruction body diff after adding trailing b./nop via __asm__(".size ...") directive). Source structure follows the gcValEntityPathDistance_AssignCopy pattern with field-offset shift (destruct/clone block at +0x34 instead of +0x40, plus 24-byte cName memcpy at +0x40 and three int copies at +0x58/5C/60).

The remaining 4-byte diff is a branch-direction divergence at 0x188. Expected uses jump-in pattern: `bnel a0,s7,0x198; lw a2,this->unk34 (delay); b 0x280; lw a0,other->unk38 (delay); ... body ...; merge`. Our SNC emits skip-over pattern: `beql a0,s3,0x27c; lw a0,other->unk38 (delay); ... body ... (with explicit `lw a0,this->unk34` inside body)`. Same semantics, 1 fewer instruction. The expected pre-loads this->unk34 via the bnel delay slot into a2 and uses a2 throughout the body; ours allocates a0 for this->unk34 with a regular load at the body's start.

This is essentially the inverse of docs/decisions/006-bnel-compiler-divergence.md — our SNC picks the skip-over (likely-beql) form where original picked the jump-in (bnel + b merge) form. Same fundamental heuristic divergence, just a different code shape. The sibling file src/gcValEntitySoundFrequency_AssignCopy.cpp has the identical issue with the identical source pattern (16 bytes off there because that file also lacks the .size directive).

Source restructurings tested without effect:
  1. Plain `if (cond) { body } rest` (4-byte diff)
  2. `int oldValue = ...; if (cond) { ... }` with the load hoisted out (made worse — 16-byte diff because SNC removed the redundant load entirely but also didn't add b merge)
  3. goto-based `if (cond == eq) goto skip; { body } skip:` (4-byte diff, same as 1)
  4. `__asm__ volatile("" :: "r"(srcEntity) : "memory")` barrier before operator= (no effect)
  5. `char *otherKey = ...; if (otherKey != destValue)` temporary (no effect)

Permuter run aborted with PERMUTER_SKIPPED_NOT_LAST_MILE because size delta is 4B. Permuter is gated on size-match.

Root cause: SNC 1.2.7503.0 register allocator places `this->unk34` in `a0` (caller-saved, also used for branch compare), preventing the bnel-delay-slot pre-load optimization the original compiler used. To match, we'd need either (a) a way to coerce SNC's RA to use `a2` for `this->unk34`, or (b) compiler-side patching of pspcor.exe to flip the branch heuristic for this pattern. Neither is straightforward from source.

Suggested next steps: (i) try `register int oldValue asm("a2")` hint inside the if-body; (ii) try splitting the destruct/clone into a separate inline helper to push register pressure; (iii) compare with a fully-matched sibling that has same dual-section structure (destruct-then-copies) if one exists in the corpus.


## Disassembly

```asm
350b88:	27bdffd0 	addiu	sp,sp,-48
350b8c:	afb00000 	sw	s0,0(sp)
350b90:	afb20008 	sw	s2,8(sp)
350b94:	afb7001c 	sw	s7,28(sp)
350b98:	34120000 	li	s2,0x0
350b9c:	24970034 	addiu	s7,a0,52
350ba0:	00808025 	move	s0,a0
350ba4:	afb10004 	sw	s1,4(sp)
350ba8:	afb3000c 	sw	s3,12(sp)
350bac:	afb40010 	sw	s4,16(sp)
350bb0:	afb50014 	sw	s5,20(sp)
350bb4:	afb60018 	sw	s6,24(sp)
350bb8:	afbf0020 	sw	ra,32(sp)
350bbc:	10a00050 	beqz	a1,0x350d00
350bc0:	00a08825 	move	s1,a1
350bc4:	3c13000a 	lui	s3,0xa
350bc8:	8e64f8b0 	lw	a0,-1872(s3)
350bcc:	14800035 	bnez	a0,0x350ca4
350bd0:	3c14000a 	lui	s4,0xa
350bd4:	8e84f3e8 	lw	a0,-3096(s4)
350bd8:	14800028 	bnez	a0,0x350c7c
350bdc:	3c080035 	lui	t0,0x35
350be0:	3c150004 	lui	s5,0x4
350be4:	8ea485d8 	lw	a0,-31272(s5)
350be8:	14800019 	bnez	a0,0x350c50
350bec:	3c160004 	lui	s6,0x4
350bf0:	8ec485dc 	lw	a0,-31268(s6)
350bf4:	1480000c 	bnez	a0,0x350c28
350bf8:	3c040037 	lui	a0,0x37
350bfc:	3c050037 	lui	a1,0x37
350c00:	34060001 	li	a2,0x1
350c04:	00003825 	move	a3,zero
350c08:	00004025 	move	t0,zero
350c0c:	00004825 	move	t1,zero
350c10:	00005025 	move	t2,zero
350c14:	00005825 	move	t3,zero
350c18:	2484d894 	addiu	a0,a0,-10092
350c1c:	0c001d4d 	jal	0x7534
350c20:	24a5d89c 	addiu	a1,a1,-10084
350c24:	aec285dc 	sw	v0,-31268(s6)
350c28:	8ec785dc 	lw	a3,-31268(s6)
350c2c:	00002025 	move	a0,zero
350c30:	00002825 	move	a1,zero
350c34:	3406006a 	li	a2,0x6a
350c38:	00004025 	move	t0,zero
350c3c:	00004825 	move	t1,zero
350c40:	00005025 	move	t2,zero
350c44:	0c001d4d 	jal	0x7534
350c48:	00005825 	move	t3,zero
350c4c:	aea285d8 	sw	v0,-31272(s5)
350c50:	8ea785d8 	lw	a3,-31272(s5)
350c54:	00002025 	move	a0,zero
350c58:	00002825 	move	a1,zero
350c5c:	3406006c 	li	a2,0x6c
350c60:	00004025 	move	t0,zero
350c64:	00004825 	move	t1,zero
350c68:	00005025 	move	t2,zero
350c6c:	0c001d4d 	jal	0x7534
350c70:	340b0080 	li	t3,0x80
350c74:	ae82f3e8 	sw	v0,-3096(s4)
350c78:	3c080035 	lui	t0,0x35
350c7c:	8e87f3e8 	lw	a3,-3096(s4)
350c80:	00002025 	move	a0,zero
350c84:	00002825 	move	a1,zero
350c88:	340600fa 	li	a2,0xfa
350c8c:	00004825 	move	t1,zero
350c90:	00005025 	move	t2,zero
350c94:	00005825 	move	t3,zero
350c98:	0c001d4d 	jal	0x7534
350c9c:	25080e98 	addiu	t0,t0,3736
350ca0:	ae62f8b0 	sw	v0,-1872(s3)
350ca4:	8e240004 	lw	a0,4(s1)
350ca8:	8e73f8b0 	lw	s3,-1872(s3)
350cac:	24840008 	addiu	a0,a0,8
350cb0:	84850000 	lh	a1,0(a0)
350cb4:	8c860004 	lw	a2,4(a0)
350cb8:	00c0f809 	jalr	a2
350cbc:	02252021 	addu	a0,s1,a1
350cc0:	16600003 	bnez	s3,0x350cd0
350cc4:	00402025 	move	a0,v0
350cc8:	1000000a 	b	0x350cf4
350ccc:	34040000 	li	a0,0x0
350cd0:	50800008 	beqzl	a0,0x350cf4
350cd4:	34040000 	li	a0,0x0
350cd8:	54930003 	bnel	a0,s3,0x350ce8
350cdc:	8c84001c 	lw	a0,28(a0)
350ce0:	10000004 	b	0x350cf4
350ce4:	34040001 	li	a0,0x1
350ce8:	1480fffb 	bnez	a0,0x350cd8
350cec:	00000000 	nop
350cf0:	34040000 	li	a0,0x0
350cf4:	10800003 	beqz	a0,0x350d04
350cf8:	26450008 	addiu	a1,s2,8
350cfc:	02209025 	move	s2,s1
350d00:	26450008 	addiu	a1,s2,8
350d04:	0c047501 	jal	0x11d404
350d08:	26040008 	addiu	a0,s0,8
350d0c:	26440034 	addiu	a0,s2,52
350d10:	54970003 	bnel	a0,s7,0x350d20
350d14:	8e060034 	lw	a2,52(s0)
350d18:	1000003b 	b	0x350e08
350d1c:	8e440038 	lw	a0,56(s2)
350d20:	34050001 	li	a1,0x1
350d24:	30c40001 	andi	a0,a2,0x1
350d28:	54800001 	bnezl	a0,0x350d30
350d2c:	34050000 	li	a1,0x0
350d30:	10a00015 	beqz	a1,0x350d88
350d34:	00803825 	move	a3,a0
350d38:	00c02025 	move	a0,a2
350d3c:	34050000 	li	a1,0x0
350d40:	54e00001 	bnezl	a3,0x350d48
350d44:	34050001 	li	a1,0x1
350d48:	10a00004 	beqz	a1,0x350d5c
350d4c:	2405fffe 	li	a1,-2
350d50:	00c53024 	and	a2,a2,a1
350d54:	10000003 	b	0x350d64
350d58:	34c60001 	ori	a2,a2,0x1
350d5c:	8cc60000 	lw	a2,0(a2)
350d60:	34c60001 	ori	a2,a2,0x1
350d64:	10800008 	beqz	a0,0x350d88
350d68:	ae060034 	sw	a2,52(s0)
350d6c:	8c860004 	lw	a2,4(a0)
350d70:	34050003 	li	a1,0x3
350d74:	24c60050 	addiu	a2,a2,80
350d78:	84c70000 	lh	a3,0(a2)
350d7c:	8cc60004 	lw	a2,4(a2)
350d80:	00c0f809 	jalr	a2
350d84:	00872021 	addu	a0,a0,a3
350d88:	8e440034 	lw	a0,52(s2)
350d8c:	34050001 	li	a1,0x1
350d90:	30860001 	andi	a2,a0,0x1
350d94:	54c00001 	bnezl	a2,0x350d9c
350d98:	34050000 	li	a1,0x0
350d9c:	10a00019 	beqz	a1,0x350e04
350da0:	00808825 	move	s1,a0
350da4:	8e250004 	lw	a1,4(s1)
350da8:	02e02025 	move	a0,s7
350dac:	24b30010 	addiu	s3,a1,16
350db0:	86650000 	lh	a1,0(s3)
350db4:	0c0011e8 	jal	0x47a0
350db8:	02258821 	addu	s1,s1,a1
350dbc:	8e170034 	lw	s7,52(s0)
350dc0:	00402025 	move	a0,v0
350dc4:	34050000 	li	a1,0x0
350dc8:	32e70001 	andi	a3,s7,0x1
350dcc:	8e660004 	lw	a2,4(s3)
350dd0:	54e00001 	bnezl	a3,0x350dd8
350dd4:	34050001 	li	a1,0x1
350dd8:	10a00003 	beqz	a1,0x350de8
350ddc:	2405fffe 	li	a1,-2
350de0:	10000002 	b	0x350dec
350de4:	02e5b824 	and	s7,s7,a1
350de8:	8ef70000 	lw	s7,0(s7)
350dec:	00c03825 	move	a3,a2
350df0:	00802825 	move	a1,a0
350df4:	02202025 	move	a0,s1
350df8:	00e0f809 	jalr	a3
350dfc:	02e03025 	move	a2,s7
350e00:	ae020034 	sw	v0,52(s0)
350e04:	8e440038 	lw	a0,56(s2)
350e08:	ae040038 	sw	a0,56(s0)
350e0c:	8e44003c 	lw	a0,60(s2)
350e10:	26450040 	addiu	a1,s2,64
350e14:	ae04003c 	sw	a0,60(s0)
350e18:	8ca60000 	lw	a2,0(a1)
350e1c:	8ca70004 	lw	a3,4(a1)
350e20:	26040040 	addiu	a0,s0,64
350e24:	8ca80008 	lw	t0,8(a1)
350e28:	ac860000 	sw	a2,0(a0)
350e2c:	8ca6000c 	lw	a2,12(a1)
350e30:	ac870004 	sw	a3,4(a0)
350e34:	8ca70010 	lw	a3,16(a1)
350e38:	ac880008 	sw	t0,8(a0)
350e3c:	8ca50014 	lw	a1,20(a1)
350e40:	ac86000c 	sw	a2,12(a0)
350e44:	ac870010 	sw	a3,16(a0)
350e48:	ac850014 	sw	a1,20(a0)
350e4c:	8e440058 	lw	a0,88(s2)
350e50:	ae040058 	sw	a0,88(s0)
350e54:	8e44005c 	lw	a0,92(s2)
350e58:	ae04005c 	sw	a0,92(s0)
350e5c:	8e440060 	lw	a0,96(s2)
350e60:	ae040060 	sw	a0,96(s0)
350e64:	8fb00000 	lw	s0,0(sp)
350e68:	8fb10004 	lw	s1,4(sp)
350e6c:	8fb20008 	lw	s2,8(sp)
350e70:	8fb3000c 	lw	s3,12(sp)
350e74:	8fb40010 	lw	s4,16(sp)
350e78:	8fb50014 	lw	s5,20(sp)
350e7c:	8fb60018 	lw	s6,24(sp)
350e80:	8fb7001c 	lw	s7,28(sp)
350e84:	8fbf0020 	lw	ra,32(sp)
350e88:	03e00008 	jr	ra
350e8c:	27bd0030 	addiu	sp,sp,48
350e90:	1000ffff 	b	0x350e90
350e94:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcValNavMeshDirection::AssignCopy(const cBase *)
// Address: 0x00350b88, Size: 784B
// Obj: gcAll_psp.obj

s32 cMemPool__GetPoolFromPtr_constvoidptrstatic(s32, s16, s32); /* extern */
s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? gcDesiredEntity__operator_eq_constgcDesiredEntityref(s32, void *); /* extern */

void gcValNavMeshDirection__AssignCopy_constcBaseptr(void *arg0, void *arg1) {
    ? (*temp_a2_4)(s32, ?, ?, s16);
    s16 temp_a1;
    s16 temp_a1_3;
    s16 temp_a3;
    s32 (*temp_a2_6)(s32, s32, s32, ?);
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_a2_2;
    s32 temp_a2_5;
    s32 temp_s3;
    s32 temp_s7;
    s32 temp_s7_2;
    s32 var_a0_2;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a1_4;
    s32 var_a1_5;
    s32 var_a2;
    s32 var_s7;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *temp_a0_5;
    void *temp_a1_2;
    void *temp_a1_4;
    void *temp_a2_3;
    void *var_a0;
    void *var_a1;
    void *var_s2;

    var_s2 = NULL;
    temp_s7 = arg0 + 0x34;
    if (arg1 != NULL) {
        if (*(s32 *)0x9F8B0 == 0) {
            if (*(s32 *)0x9F3E8 == 0) {
                if (*(s32 *)0x385D8 == 0) {
                    if (*(s32 *)0x385DC == 0) {
                        *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36D894, 0x36D89C, 1, 0);
                    }
                    *(s32 *)0x385D8 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x6A, *(s32 *)0x385DC);
                }
                *(s32 *)0x9F3E8 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x6C, *(s32 *)0x385D8);
            }
            *(s32 *)0x9F8B0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0xFA, *(s32 *)0x9F3E8);
        }
        temp_a0 = arg1->unk4;
        temp_s3 = *(s32 *)0x9F8B0;
        temp_a1 = temp_a0->unk8;
        temp_a2 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a2(arg1 + temp_a1, temp_a1, temp_a2);
        if (temp_s3 == 0) {
            goto block_17;
        }
        if (var_a0 != NULL) {
loop_12:
            if (var_a0 != temp_s3) {
                var_a0 = var_a0->unk1C;
                if (var_a0 == NULL) {
                    goto block_17;
                }
                goto loop_12;
            }
            var_a0_2 = 1;
        } else {
block_17:
            var_a0_2 = 0;
        }
        var_a1 = (void *)8;
        if (var_a0_2 != 0) {
            var_s2 = arg1;
            goto block_20;
        }
    } else {
block_20:
        var_a1 = var_s2 + 8;
    }
    gcDesiredEntity__operator_eq_constgcDesiredEntityref(arg0 + 8, var_a1);
    if ((var_s2 + 0x34) != temp_s7) {
        temp_a2_2 = arg0->unk34;
        var_a1_2 = 1;
        temp_a0_2 = temp_a2_2 & 1;
        if (temp_a0_2 != 0) {
            var_a1_2 = 0;
        }
        if (var_a1_2 != 0) {
            var_a1_3 = 0;
            if (temp_a0_2 != 0) {
                var_a1_3 = 1;
            }
            if (var_a1_3 != 0) {
                var_a2 = temp_a2_2 & ~1;
            } else {
                var_a2 = temp_a2_2->unk0;
            }
            arg0->unk34 = (s32) (var_a2 | 1);
            if (temp_a2_2 != 0) {
                temp_a2_3 = temp_a2_2->unk4;
                temp_a3 = temp_a2_3->unk50;
                temp_a2_4 = (temp_a2_3 + 0x50)->unk4;
                temp_a2_4(temp_a2_2 + temp_a3, 3, temp_a2_4, temp_a3);
            }
        }
        temp_a0_3 = var_s2->unk34;
        var_a1_4 = 1;
        temp_a2_5 = temp_a0_3 & 1;
        if (temp_a2_5 != 0) {
            var_a1_4 = 0;
        }
        if (var_a1_4 != 0) {
            temp_a1_2 = temp_a0_3->unk4;
            temp_a1_3 = temp_a1_2->unk10;
            temp_s7_2 = arg0->unk34;
            temp_a0_4 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s7, temp_a1_3, temp_a2_5);
            var_a1_5 = 0;
            temp_a2_6 = (temp_a1_2 + 0x10)->unk4;
            if (temp_s7_2 & 1) {
                var_a1_5 = 1;
            }
            if (var_a1_5 != 0) {
                var_s7 = temp_s7_2 & ~1;
            } else {
                var_s7 = temp_s7_2->unk0;
            }
            arg0->unk34 = temp_a2_6(temp_a0_3 + temp_a1_3, temp_a0_4, var_s7, temp_a2_6);
        }
    }
    arg0->unk38 = (s32) var_s2->unk38;
    temp_a1_4 = var_s2 + 0x40;
    arg0->unk3C = (s32) var_s2->unk3C;
    temp_a0_5 = arg0 + 0x40;
    arg0->unk40 = (s32) var_s2->unk40;
    temp_a0_5->unk4 = (s32) temp_a1_4->unk4;
    temp_a0_5->unk8 = (s32) temp_a1_4->unk8;
    temp_a0_5->unkC = (s32) temp_a1_4->unkC;
    temp_a0_5->unk10 = (s32) temp_a1_4->unk10;
    temp_a0_5->unk14 = (s32) temp_a1_4->unk14;
    arg0->unk58 = (s32) var_s2->unk58;
    arg0->unk5C = (s32) var_s2->unk5C;
    arg0->unk60 = (s32) var_s2->unk60;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
