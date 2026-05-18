# Battle Packet: `eStaticPointLight::GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const`

## Target

- Address: `0x0005f07c`
- Size: `224` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eStaticPointLight`
- Method family: `GetDirectLight`
- Leaf: `True`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 23B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `23`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `23`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Placement

- Canonical source: `src/eStaticPointLight.cpp`
- Header(s): `include/eStaticPointLight.h`
- Allowed source(s): `src/eStaticPointLight.cpp`
- Split-TU prefix: `src/eStaticPointLight_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eStaticPointLight.h`

Not found.

## Matched Same-Class Neighbors

- `0x0005eec4` 76B `eStaticPointLight::Write(cFile &) const` — `src/eStaticPointLight.cpp`
- `0x0005ef10` 188B `eStaticPointLight::Read(cFile &, cMemPool *)` — `src/eStaticPointLight.cpp`
- `0x0005efcc` 52B `eStaticPointLight::eStaticPointLight(cBase *)` — `src/eStaticPointLight.cpp`
- `0x0005f000` 124B `eStaticPointLight::~eStaticPointLight(void)` — `src/eStaticPointLight.cpp`
- `0x0005f15c` 72B `eStaticPointLight::GetSampleRay(mRay *, mVec3 *, const mVec3 &, const mVec3 &) const` — `src/eStaticPointLight.cpp`

## Matched Method Exemplars

### Exemplar 1: `eStaticSkyLight::GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const`

- Address: `0x0005f388`
- Size: `184` bytes
- Source: `src/eStaticSkyLight.cpp`

```cpp
void eStaticSkyLight::GetDirectLight(mVec3 *out, const mVec3 &, const mVec3 &normal, const mRay &ray, const mVec3 &) const {
    volatile float color[8];

    float zero, dot;
    __asm__ volatile(
        "lv.q C120, 0x10(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a2, S100\n"
        "mtc1 $a2, %1\n"
        "mtc1 $zero, %0\n"
        : "=f"(zero), "=f"(dot)
        : "r"(&ray), "r"(&normal)
        : "$a2"
    );

    if (dot > zero) {
        float scale = 1.0f / 255.0f;
        color[0] = (float)colorR * scale;
        color[1] = (float)colorG * scale;
        color[2] = (float)colorB * scale;
        float factor = dot * intensity;

        __asm__ volatile(
            "mfc1 $a0, %0\n"
            "mtv $a0, S100\n"
            "lv.q C120, 0($sp)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0(%1)\n"
            :: "f"(factor), "r"(out)
            : "$a0", "memory"
        );
        return;
    }

    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a2, %0\n"
        "mfc1 $a3, %0\n"
        "mtv $a0, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(zero), "r"(out)
        : "$a0", "$a2", "$a3", "memory"
    );
}
```

### Exemplar 2: `eStaticSunLight::GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const`

- Address: `0x0005fc30`
- Size: `184` bytes
- Source: `src/eStaticSunLight.cpp`

```cpp
void eStaticSunLight::GetDirectLight(mVec3 *out, const mVec3 &, const mVec3 &normal, const mRay &ray, const mVec3 &) const {
    volatile float color[8];

    float zero, dot;
    __asm__ volatile(
        "lv.q C120, 0x10(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a2, S100\n"
        "mtc1 $a2, %1\n"
        "mtc1 $zero, %0\n"
        : "=f"(zero), "=f"(dot)
        : "r"(&ray), "r"(&normal)
        : "$a2"
    );

    if (dot > zero) {
        float scale = 1.0f / 255.0f;
        color[0] = (float)*(const unsigned char *)((const char *)this + 0x4A) * scale;
        color[1] = (float)*(const unsigned char *)((const char *)this + 0x49) * scale;
        color[2] = (float)*(const unsigned char *)((const char *)this + 0x48) * scale;
        float factor = dot * *(const float *)((const char *)this + 0x44);

        __asm__ volatile(
            "mfc1 $a0, %0\n"
            "mtv $a0, S100\n"
            "lv.q C120, 0($sp)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0(%1)\n"
            :: "f"(factor), "r"(out)
            : "$a0", "memory"
        );
        return;
    }

    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a2, %0\n"
        "mfc1 $a3, %0\n"
        "mtv $a0, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(zero), "r"(out)
        : "$a0", "$a2", "$a3", "memory"
    );
}
```


## Prior Failure Notes

### Note 1

- session `94289255`; src `src/eStaticPointLight.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/94289255/0005f07c__agent_self_reported_failure__src_eStaticPointLight.cpp`

Closest attempt is a C++ adaptation of the matched eStaticSunLight/eStaticSkyLight GetDirectLight pattern, with the point-light inverse-square attenuation added as 100.0f / ((ray[0x20] * ray[0x20]) * 3.1415927f). The scalar math, constants, color loads, stack frame size, and color stores at 0x10/0x14/0x18(sp) match. The remaining mismatch is 23/224 bytes: expected uses bc1tl with mfc1 a0,$f12 in the branch-likely delay slot for the zero-output path, while our SNC output uses bc1t plus a nop and starts the zero-output asm at the branch target. Tried sched=1 and sched=2, positive-first and zero-first control flow, and splitting the zero-path mfc1 instructions into separate one-instruction asm statements; splitting did not allow SNC/GAS to place the asm into the bc1tl delay slot. A short permuter run could not start because tools/permuter.py could not isolate this method in the multi-function eStaticPointLight.cpp object.


## Disassembly

```asm
5f07c:	27bdffe0 	addiu	sp,sp,-32
5f080:	d9060010 	lv.q        C120, 0x10($t0)
5f084:	d8e70000 	lv.q        C130, 0x0($a3)
5f088:	64878604 	vdot.t      S100, C120, C130
5f08c:	48660004 	mfv         $a2, S100
5f090:	44866800 	mtc1	a2,$f13
5f094:	44806000 	mtc1	zero,$f12
5f098:	460c683e 	c.le.s	$f13,$f12
5f09c:	00000000 	nop
5f0a0:	45030026 	bc1tl	0x5f13c
5f0a4:	44046000 	mfc1	a0,$f12
5f0a8:	c50c0020 	lwc1	$f12,32(t0)
5f0ac:	460c6302 	mul.s	$f12,$f12,$f12
5f0b0:	3c064049 	lui	a2,0x4049
5f0b4:	34c60fdb 	ori	a2,a2,0xfdb
5f0b8:	44867000 	mtc1	a2,$f14
5f0bc:	460e6302 	mul.s	$f12,$f12,$f14
5f0c0:	3c0642c8 	lui	a2,0x42c8
5f0c4:	44867800 	mtc1	a2,$f15
5f0c8:	460c7b03 	div.s	$f12,$f15,$f12
5f0cc:	3c063b80 	lui	a2,0x3b80
5f0d0:	34c68081 	ori	a2,a2,0x8081
5f0d4:	44868000 	mtc1	a2,$f16
5f0d8:	9086004a 	lbu	a2,74(a0)
5f0dc:	44867000 	mtc1	a2,$f14
5f0e0:	468073a0 	cvt.s.w	$f14,$f14
5f0e4:	46107382 	mul.s	$f14,$f14,$f16
5f0e8:	e7ae0010 	swc1	$f14,16(sp)
5f0ec:	90860049 	lbu	a2,73(a0)
5f0f0:	44867000 	mtc1	a2,$f14
5f0f4:	468073a0 	cvt.s.w	$f14,$f14
5f0f8:	46107382 	mul.s	$f14,$f14,$f16
5f0fc:	e7ae0014 	swc1	$f14,20(sp)
5f100:	90860048 	lbu	a2,72(a0)
5f104:	44867000 	mtc1	a2,$f14
5f108:	468073a0 	cvt.s.w	$f14,$f14
5f10c:	46107382 	mul.s	$f14,$f14,$f16
5f110:	e7ae0018 	swc1	$f14,24(sp)
5f114:	c48e0044 	lwc1	$f14,68(a0)
5f118:	460e6b42 	mul.s	$f13,$f13,$f14
5f11c:	460c6b02 	mul.s	$f12,$f13,$f12
5f120:	44046000 	mfc1	a0,$f12
5f124:	48e40004 	mtv         $a0, S100
5f128:	dba60010 	lv.q        C120, 0x10($sp)
5f12c:	65048606 	vscl.t      C120, C120, S100
5f130:	f8a60000 	sv.q        C120, 0x0($a1)
5f134:	03e00008 	jr	ra
5f138:	27bd0020 	addiu	sp,sp,32
5f13c:	44066000 	mfc1	a2,$f12
5f140:	44076000 	mfc1	a3,$f12
5f144:	48e40006 	mtv         $a0, S120
5f148:	48e60026 	mtv         $a2, S121
5f14c:	48e70046 	mtv         $a3, S122
5f150:	f8a60000 	sv.q        C120, 0x0($a1)
5f154:	03e00008 	jr	ra
5f158:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eStaticPointLight::GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const
// Address: 0x0005f07c, Size: 224B
// Obj: eAll_psp.obj

void eStaticPointLight__GetDirectLight_mVec3ptr_constmVec3ref_constmVec3ref_constmRayref_constmVec3refconst(void *arg0, f32 arg2) {
    f32 sp10;
    f32 sp14;
    f32 sp18;

    __asm__ volatile("lv.q C120, 0x10($t0)");
    __asm__ volatile("lv.q C130, ($a3)");
    __asm__ volatile("vdot.t S100, C120, C130");
    __asm__ volatile("mfv $a2, S100");
    if (arg2 <= 0.0f) {
        __asm__ volatile("mtv $a0, S120");
        __asm__ volatile("mtv $a2, S121");
        __asm__ volatile("mtv $a3, S122");
        __asm__ volatile("sv.q C120, ($a1)");
        return;
    }
    sp10 = (f32) arg0->unk4A * 0.003921569f;
    sp14 = (f32) arg0->unk49 * 0.003921569f;
    sp18 = (f32) arg0->unk48 * 0.003921569f;
    __asm__ volatile("mtv $a0, S100");
    __asm__ volatile("lv.q C120, 0x10($sp)");
    __asm__ volatile("vscl.t C120, C120, S100");
    __asm__ volatile("sv.q C120, ($a1)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
