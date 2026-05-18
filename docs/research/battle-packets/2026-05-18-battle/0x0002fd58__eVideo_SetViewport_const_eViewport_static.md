# Battle Packet: `eVideo::SetViewport(const eViewport *) static`

## Target

- Address: `0x0002fd58`
- Size: `448` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eVideo`
- Method family: `SetViewport`
- Leaf: `False`

## Queue Metadata

- score: `131`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 18B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `18`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `18`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0002ff44` `matched` 268B `eVideo::SetScissor(const eViewport *) static`

Callers:
- `0x00030898` `untried` 1108B `eCameraBins::Draw(bool)`
- `0x00031570` `untried` 436B `eRenderTarget::Select(cHandleT<eTexture>, bool, cHandleT<eTexture>, unsigned int, eViewport *)`
- `0x00031724` `failed` 128B `eRenderTarget::Unselect(void)`
- `0x00099774` `untried` 1632B `eCameraEffectGeom::Draw(const eDrawInfo &) const`
- `0x000b995c` `untried` 4084B `eVideoPlatform::Initialize(void) static`
- `0x000e2720` `failed` 372B `gcUI::gcFader::FadeImmediate(bool, cTimeValue, cHandleT<eMaterial>) static`
- `0x000e4e34` `matched` 352B `gcUIDialog::Draw(unsigned int)`
- `0x000f31a4` `untried` 608B `gcMap::Draw(void)`
- `0x000fe500` `failed` 860B `gcViewport::Draw(void) static`
- `0x000fe85c` `untried` 328B `gcViewport::DrawMouse(void) static`
- `0x001096c8` `matched` 116B `gcUIStackDialog::Draw(void)`
- `0x00222078` `untried` 388B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcBackgroundLoader> >(gcRenderLoopT *)`
- `0x00225cc8` `untried` 456B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<eMoviePauseExitCheck> >(gcRenderLoopT *)`
- `0x00225e90` `untried` 400B `void gcRenderLoopT<fileSystemErrorLoop>(gcRenderLoopT *)`
- `0x00226020` `untried` 412B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcUIOpenDialogExitCheck> >(gcRenderLoopT *)`
- `0x00226b00` `untried` 516B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcMapServerReadyExitCheck> >(gcRenderLoopT *)`
- `0x00226ee8` `untried` 524B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcRegionLoaderExitCheck> >(gcRenderLoopT *)`

## Placement

- Canonical source: `src/eVideo.cpp`
- Header(s): `include/eVideo.h`
- Allowed source(s): `src/eVideo.cpp`
- Split-TU prefix: `src/eVideo_*.cpp`

## Class Header: `include/eVideo.h`

```cpp
#ifndef EVIDEO_H
#define EVIDEO_H

class eCamera;
class eVertexShader;

class eVideo {
public:
    enum eVideoMode {};

    static void RegisterInit(class eIVideoRegisterInit *);
    static void SetVideoMode(eVideoMode);
    static void SetDefaultVideoMode(void);
    static void Flush(void);
    static void BeginFrame(void);
    static void Flip(void);
    static void EndFrame(void);
    static void InvalidateTextureCache(void);
    static void PreFlip(void);
    static void Clear(bool, bool, unsigned int, float, const struct eViewport *);
    static void BeginStencil(int);
    static void EndStencil(void);
    static void ApplyVertexShader(const eCamera *, const eVertexShader *);
    static void SetAlphaWriteEnable(bool);
    static void SetAlphaBlendEnable(bool);
    static void SetFlippedBFCMode(bool);
    static int IsWidescreen(void);
    static struct eViewport *GetFullscreenViewport(void);
    static void GetMaxWidthHeight(int *, int *);
    static void GetVirtualWidthHeight(int *, int *);
    static void GetAspectWidthHeight(int, int, int *, int *);
    static void GetViewport(struct eViewport *);
    static void GetScissor(struct eViewport *);
    static void SetScissor(const struct eViewport *);
    static void SetFullscreenViewport(const struct eViewport &);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0002f68c` 20B `eVideo::GetMaxWidthHeight(int *, int *) static` — `src/eVideo_GetMaxWidthHeight.cpp`
- `0x0002f6a0` 132B `eVideo::GetVirtualWidthHeight(int *, int *) static` — `src/eVideo.cpp`
- `0x0002f724` 140B `eVideo::GetAspectWidthHeight(int, int, int *, int *) static` — `src/eVideo.cpp`
- `0x0002f848` 64B `eVideo::EndFrame(void) static` — `src/eVideo.cpp`
- `0x0002f888` 8B `eVideo::PreFlip(void) static` — `src/eCollision.cpp`

## Matched Method Exemplars

### Exemplar 1: `eCamera::SetViewport(const eViewport *, bool)`

- Address: `0x0002d8ec`
- Size: `128` bytes
- Source: `src/eCamera.cpp`

```cpp
void eCamera::SetViewport(const eViewport *vp, bool adjustFrustum) {
    if (vp == 0) {
        *(int *)((char *)this + 4) = 0;
        *(int *)((char *)this + 8) = 0;
        GetMaxWidthHeight((int *)((char *)this + 0xC), (int *)((char *)this + 0x10));
    } else {
        *(eViewport *)((char *)this + 4) = *vp;
    }
    if (adjustFrustum) {
        AdjustFrustum();
    }
}
```


## Prior Failure Notes

### Note 1

- session `3f3fb71a`; src `src/eVideo.cpp`; snapshot `logs/failure_snapshots/match_20260507_103437/3f3fb71a/0002fd58__agent_self_reported_failure__src_eVideo.cpp`

Closest attempt is 18/448 bytes different after a 300s permuter pass that improved the baseline from 133 bytes to 18 bytes. The remaining diff is localized to FPU/integer scheduling in the viewport command emission: at 0x124 the compiler swaps `cvt.s.w $f14,$f16` with the following integer `or`, and around 0x158 it delays the `p[1]` store until after the first width-bit `srl`. Manual attempts included plain float bitcasts, volatile stack float temporaries, union-backed bitcasts, moving command-list pointer setup before/after the float expressions, interleaving the union stores to mirror m2c, and adding/removing memory barriers; the best source is the permuter-saved union/interleaved version. Likely SNC scheduler/source-order sensitivity around stack-backed float bit extraction rather than control-flow or register-allocation drift. All existing matched sibling eVideo functions in this TU still match.


## Disassembly

```asm
2fd58:	27bdffd0 	addiu	sp,sp,-48
2fd5c:	afb00020 	sw	s0,32(sp)
2fd60:	afbf0024 	sw	ra,36(sp)
2fd64:	0c00bfd1 	jal	0x2ff44
2fd68:	00808025 	move	s0,a0
2fd6c:	afa00000 	sw	zero,0(sp)
2fd70:	340401e0 	li	a0,0x1e0
2fd74:	afa00004 	sw	zero,4(sp)
2fd78:	34050110 	li	a1,0x110
2fd7c:	afa40008 	sw	a0,8(sp)
2fd80:	afa5000c 	sw	a1,12(sp)
2fd84:	3c04000a 	lui	a0,0xa
2fd88:	52000001 	beqzl	s0,0x2fd90
2fd8c:	03a08025 	move	s0,sp
2fd90:	8e050000 	lw	a1,0(s0)
2fd94:	8c878358 	lw	a3,-31912(a0)
2fd98:	14a7000f 	bne	a1,a3,0x2fdd8
2fd9c:	34060000 	li	a2,0x0
2fda0:	24858358 	addiu	a1,a0,-31912
2fda4:	8e070004 	lw	a3,4(s0)
2fda8:	8ca80004 	lw	t0,4(a1)
2fdac:	54e8000b 	bnel	a3,t0,0x2fddc
2fdb0:	30c500ff 	andi	a1,a2,0xff
2fdb4:	8e070008 	lw	a3,8(s0)
2fdb8:	8ca80008 	lw	t0,8(a1)
2fdbc:	54e80007 	bnel	a3,t0,0x2fddc
2fdc0:	30c500ff 	andi	a1,a2,0xff
2fdc4:	8e07000c 	lw	a3,12(s0)
2fdc8:	8ca5000c 	lw	a1,12(a1)
2fdcc:	14e50003 	bne	a3,a1,0x2fddc
2fdd0:	30c500ff 	andi	a1,a2,0xff
2fdd4:	34060001 	li	a2,0x1
2fdd8:	30c500ff 	andi	a1,a2,0xff
2fddc:	14a0004a 	bnez	a1,0x2ff08
2fde0:	00000000 	nop
2fde4:	8e050000 	lw	a1,0(s0)
2fde8:	8e060004 	lw	a2,4(s0)
2fdec:	8e070008 	lw	a3,8(s0)
2fdf0:	24888358 	addiu	t0,a0,-31912
2fdf4:	ac858358 	sw	a1,-31912(a0)
2fdf8:	8e04000c 	lw	a0,12(s0)
2fdfc:	ad060004 	sw	a2,4(t0)
2fe00:	ad070008 	sw	a3,8(t0)
2fe04:	ad04000c 	sw	a0,12(t0)
2fe08:	8e040008 	lw	a0,8(s0)
2fe0c:	8e05000c 	lw	a1,12(s0)
2fe10:	44846000 	mtc1	a0,$f12
2fe14:	3c063f00 	lui	a2,0x3f00
2fe18:	8e040000 	lw	a0,0(s0)
2fe1c:	44867000 	mtc1	a2,$f14
2fe20:	44856800 	mtc1	a1,$f13
2fe24:	3c07bf00 	lui	a3,0xbf00
2fe28:	3c06000a 	lui	a2,0xa
2fe2c:	44877800 	mtc1	a3,$f15
2fe30:	8e050004 	lw	a1,4(s0)
2fe34:	24840800 	addiu	a0,a0,2048
2fe38:	24c684d0 	addiu	a2,a2,-31536
2fe3c:	44848000 	mtc1	a0,$f16
2fe40:	8cc70008 	lw	a3,8(a2)
2fe44:	46806320 	cvt.s.w	$f12,$f12
2fe48:	24a40800 	addiu	a0,a1,2048
2fe4c:	46806b60 	cvt.s.w	$f13,$f13
2fe50:	24e50018 	addiu	a1,a3,24
2fe54:	44848800 	mtc1	a0,$f17
2fe58:	acc50008 	sw	a1,8(a2)
2fe5c:	8e040008 	lw	a0,8(s0)
2fe60:	460e6302 	mul.s	$f12,$f12,$f14
2fe64:	34051000 	li	a1,0x1000
2fe68:	460f6b42 	mul.s	$f13,$f13,$f15
2fe6c:	00a42023 	subu	a0,a1,a0
2fe70:	00042043 	sra	a0,a0,0x1
2fe74:	00042100 	sll	a0,a0,0x4
2fe78:	3c064c00 	lui	a2,0x4c00
2fe7c:	00862025 	or	a0,a0,a2
2fe80:	468083a0 	cvt.s.w	$f14,$f16
2fe84:	ace40000 	sw	a0,0(a3)
2fe88:	8e04000c 	lw	a0,12(s0)
2fe8c:	46808c60 	cvt.s.w	$f17,$f17
2fe90:	e7ac0010 	swc1	$f12,16(sp)
2fe94:	00a42023 	subu	a0,a1,a0
2fe98:	00042043 	sra	a0,a0,0x1
2fe9c:	00042100 	sll	a0,a0,0x4
2fea0:	3c054d00 	lui	a1,0x4d00
2fea4:	8fa60010 	lw	a2,16(sp)
2fea8:	00852025 	or	a0,a0,a1
2feac:	e7ad0014 	swc1	$f13,20(sp)
2feb0:	ace40004 	sw	a0,4(a3)
2feb4:	00062202 	srl	a0,a2,0x8
2feb8:	3c054200 	lui	a1,0x4200
2febc:	8fa60014 	lw	a2,20(sp)
2fec0:	00852025 	or	a0,a0,a1
2fec4:	e7ae0018 	swc1	$f14,24(sp)
2fec8:	ace40008 	sw	a0,8(a3)
2fecc:	00062202 	srl	a0,a2,0x8
2fed0:	3c054300 	lui	a1,0x4300
2fed4:	8fa60018 	lw	a2,24(sp)
2fed8:	00852025 	or	a0,a0,a1
2fedc:	e7b1001c 	swc1	$f17,28(sp)
2fee0:	ace4000c 	sw	a0,12(a3)
2fee4:	00062202 	srl	a0,a2,0x8
2fee8:	3c054500 	lui	a1,0x4500
2feec:	8fa6001c 	lw	a2,28(sp)
2fef0:	00852025 	or	a0,a0,a1
2fef4:	ace40010 	sw	a0,16(a3)
2fef8:	00062202 	srl	a0,a2,0x8
2fefc:	3c054600 	lui	a1,0x4600
2ff00:	00852025 	or	a0,a0,a1
2ff04:	ace40014 	sw	a0,20(a3)
2ff08:	8fb00020 	lw	s0,32(sp)
2ff0c:	8fbf0024 	lw	ra,36(sp)
2ff10:	03e00008 	jr	ra
2ff14:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: eVideo::SetViewport(const eViewport *) static
// Address: 0x0002fd58, Size: 448B
// Obj: eAll_psp.obj

? eVideo__SetScissor_consteViewportptrstatic();     /* extern */

void eVideo__SetViewport_consteViewportptrstatic(void *arg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    f32 sp1C;
    s32 var_a1;
    s32 var_a2;
    void *temp_a3;
    void *var_s0;

    var_s0 = arg0;
    eVideo__SetScissor_consteViewportptrstatic();
    if (var_s0 == NULL) {
        var_s0 = sp;
    }
    var_a2 = 0;
    if ((var_s0->unk0 == *(s32 *)0x98358) && (var_s0->unk4 == -0x7CA8->unkA0004) && (var_s0->unk8 == -0x7CA8->unkA0008)) {
        var_a1 = 0 & 0xFF;
        if (var_s0->unkC == -0x7CA8->unkA000C) {
            var_a2 = 1;
            goto block_7;
        }
    } else {
block_7:
        var_a1 = var_a2 & 0xFF;
    }
    if (var_a1 == 0) {
        *(s32 *)0x98358 = var_s0->unk0;
        -0x7CA8->unkA0004 = (s32) var_s0->unk4;
        -0x7CA8->unkA0008 = (s32) var_s0->unk8;
        -0x7CA8->unkA000C = (s32) var_s0->unkC;
        temp_a3 = (void *)0x984D0->unk8;
        (void *)0x984D0->unk8 = (void *) (temp_a3 + 0x18);
        temp_a3->unk0 = (s32) ((((s32) (0x1000 - var_s0->unk8) >> 1) * 0x10) | 0x4C000000);
        sp10 = (f32) var_s0->unk8 * 0.5f;
        sp14 = (f32) var_s0->unkC * -0.5f;
        temp_a3->unk4 = (s32) ((((s32) (0x1000 - var_s0->unkC) >> 1) * 0x10) | 0x4D000000);
        sp18 = (f32) (var_s0->unk0 + 0x800);
        temp_a3->unk8 = (s32) (((bitwise u32) sp10 >> 8) | 0x42000000);
        sp1C = (f32) (var_s0->unk4 + 0x800);
        temp_a3->unkC = (s32) (((bitwise u32) sp14 >> 8) | 0x43000000);
        temp_a3->unk10 = (s32) (((bitwise u32) sp18 >> 8) | 0x45000000);
        temp_a3->unk14 = (s32) (((bitwise u32) sp1C >> 8) | 0x46000000);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
