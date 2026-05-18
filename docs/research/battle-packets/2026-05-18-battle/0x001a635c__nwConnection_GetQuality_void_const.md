# Battle Packet: `nwConnection::GetQuality(void) const`

## Target

- Address: `0x001a635c`
- Size: `136` bytes
- Status: `failed`
- Object: `nwAll_psp.obj`
- Class: `nwConnection`
- Method family: `GetQuality`
- Leaf: `True`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 24B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `24`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `constant_cse_codegen`
- Near miss: `24`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x0035309c` `untried` 1428B `gcValNetworkState::Evaluate(void) const`

## Placement

- Canonical source: `src/nwConnection.cpp`
- Header(s): `include/nwConnection.h`
- Allowed source(s): `src/nwConnection.cpp`
- Split-TU prefix: `src/nwConnection_*.cpp`

## Class Header: `include/nwConnection.h`

```cpp
#pragma once

class cTimeValue;
class cInStream;
class cOutStream;
class nwAddress;
class nwMsg;
class nwSocket;
class nwInPacket;
class nwOutPacket;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

// nwConnection: network connection object.
// Constructor (0x1a6420) takes (nwSocket*, nwConnectionHandle, int, const nwAddress&,
//   void (*)(nwConnectionHandle, nwConnection::nwConnectionError)).
// Ctor field inits:
//   +0x00: nwSocket* (socket)
//   +0x04: nwConnectionHandle (handle)
//   +0x08: int (from address arg)
//   +0x0C..0x18: nwAddress fields (4 ints copied from arg, 16 bytes)
//   +0x1C..0x3C: zeroed (8 ints)
//   +0x40: int = 0
//   +0x44: int = -1
//   +0x48: int = 0
//   +0x4C: float = 0.0 (ping/RTT)
//   +0x50: byte = 0 (flag)
//   +0x58: void* = error callback
//   +0x5C..0xCD4: nwMsgsBlock array (3 blocks of 0x428 bytes each, initialized by subroutine)
//   +0xCD4: void* (error callback stored again)
//   +0xCD8..0xCEC: zeroed fields
//   +0xCEC: float = 0.0 (send rate)
// UpdatePing (0x1a69e8) reads:
//   +0x2C: int (seq number / tick count)
//   +0x40: int (last ack tick)
//   +0x48: int (accumulated ping samples)
//   +0x4C: float (smoothed ping)
// Struct size: at least 0xCF0 bytes
class nwConnection {
public:
    enum nwSendMethod {
        SEND_UNRELIABLE = 0,
        SEND_RELIABLE = 1,
        SEND_ORDERED = 2
    };

    enum nwConnectionError {
        ERROR_NONE = 0,
        ERROR_TIMEOUT = 1,
        ERROR_REJECTED = 2
    };

    struct nwMsgsBlock {
        nwMsgsBlock();
        char _pad0[0x41C];
        unsigned int mLastSeq;  // 0x41C
        char _padEnd[0x8];      // 0x420..0x428
    };

    struct nwOutBlockInfo {
        void Next();
        int Ack(unsigned int, unsigned int) const;
    };

    struct nwInBlockInfo {
        void Next();
    };

    nwConnection(nwSocket *, nwConnectionHandle, int, const nwAddress &, void (*)(nwConnectionHandle, nwConnectionError));
    ~nwConnection();

    void Send(nwSendMethod, const nwMsg &, unsigned int, cOutStream *);
    void Close();
    void FlushAll();
    int GetQuality() const;
    void SetError(nwConnectionError);
    void Connect(const char *, bool);
    void StartConnection();
    void Accept();
    void OnConnectionAccepted();
    void AllocateBuffers();
    void Update(cTimeValue);
    void UpdatePing(cTimeValue);
    void UpdateSendRate(cTimeValue);
    void Flush(nwSendMethod, nwOutPacket &);
    void Flush(nwSendMethod);
    void WriteBlock(nwSendMethod, nwOutPacket &, unsigned int, unsigned int, unsigned char *);
    void SendAckAndPing(nwOutPacket &);
    void BufferOutBlock(unsigned int, int, unsigned char *);
    void ResendReliable(nwOutPacket &, int);
    void Receive(int, nwInPacket &);
    void ReceiveAck(nwInPacket &);
    void BufferInBlock(unsigned int, unsigned char *, int);
    void ResendConnect();
    void TransportSend(const nwAddress &, const nwOutPacket &);
    bool IsBlockIdOld(unsigned int blockId, nwSendMethod method) const;

    static void DispatchMessages(unsigned char *, int, nwSocketHandle, const nwAddress &, nwConnectionHandle, bool);
    static void DispatchMessage(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle, bool);
    static void BuildMessage(const nwMsg &, cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);

    nwSocket *mSocket;              // 0x00
    int mHandle;                    // 0x04
    int mField08;                   // 0x08
    char _padAddress[0x10];         // 0x0C (nwAddress, 16 bytes)
    char _pad1C[0x10];             // 0x1C
    int mSeqNumber;                 // 0x2C
    char _pad30[0x10];             // 0x30
    int mLastAckTick;               // 0x40
    int mField44;                   // 0x44
    int mPingSamples;               // 0x48
    float mSmoothedPing;            // 0x4C
    unsigned char mFlag50;          // 0x50
    char _pad51[3];                 // 0x51
    char _pad54[4];                 // 0x54
    void *mErrorCallback;           // 0x58
    nwMsgsBlock mBlocks[3];         // 0x5C (3 * 0x428 = 0xC78 bytes)
    void *mErrorCallback2;          // 0xCD4
    int mFieldCD8;                  // 0xCD8
    int mFieldCDC;                  // 0xCDC
    int mFieldCE0;                  // 0xCE0
    int mFieldCE4;                  // 0xCE4
    int mFieldCE8;                  // 0xCE8
    float mSendRate;                // 0xCEC
};
```

## Matched Same-Class Neighbors

- `0x001a6514` 240B `nwConnection::~nwConnection(void)` — `src/nwConnection.cpp`
- `0x001a671c` 48B `nwConnection::StartConnection(void)` — `src/nwConnection.cpp`
- `0x001a674c` 124B `nwConnection::Accept(void)` — `src/nwConnection.cpp`
- `0x001a67c8` 28B `nwConnection::OnConnectionAccepted(void)` — `src/nwMsgType.cpp`
- `0x001a6ac0` 144B `nwConnection::UpdateSendRate(cTimeValue)` — `src/nwConnection.cpp`

## Prior Failure Notes

### Note 1

- session `f182e059`; src `src/nwConnection.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/f182e059/001a635c__agent_self_reported_failure__src_nwConnection.cpp`

Closest attempt is 136B with 24/136 bytes differing. It keeps the high-level target structure: separate 0.25f/0.6f threshold materialization, early bc1t path with sub.s one-one in the jr delay slot, and final sub.s before jr with a nop delay. The remaining diff is primarily FPU register allocation and branch-likely shape in the second clamp: current code uses f16 for zero, f14 for range, and f13 for the first clamped value, while expected uses f14 for zero, f13 for range, and f16 for the first clamped value. Current also emits bc1fl+mov in the second clamp where expected has bc1t+nop then b+mov.

Tried simple m2c form, separate low/high constants to prevent range folding, named zero reassignment, literal 0.0f clamps, positive and negative middle range checks, early-return temp to force one-one subtraction, empty-branch memory barrier, local mopt/fprreserve pragmas, explicit register variables, and declaration-order permutations around low/zero/a/high/diff/range. A 300s permuter run generated 9792 candidates, compiled 5283, and improved 58 differing bytes to 24 but did not match. Root cause appears to be SNC allocator/branch-likely heuristic selection around the clamped zero value, not a remaining arithmetic issue.

### Note 2

- session `a8acb24c`

GetQuality (const, 136B). Compiles to 128B — 8 bytes short and structurally divergent from the original. The expected function uses a bc1f + branch + final sub.s pattern with FPU registers f16 (one), f15 (a), f14 (zero/diff) and an early return that places the sub in a delay slot of a separate jr. My best version (with `one = 1.0f` hoisted via being assigned in BOTH if branches so SNC pulls it into the bc1f delay slot) gets the high-level shape right but SNC still allocates f15/f14/f0 instead of f16/f15/f14, fuses the early-return path into 3 instructions instead of 4, and omits the final sub fixup. Tested: simple ternary forms, nested if/else with shared assignments, fmaxf-style helper, splitting the divide into a pre-comparison temp, swapping operand order on `1.0f - b/range`, -Xsched=1, -Xmopt=0 (mopt=0 produced 137B with correct register coloring but cannot be used file-wide without breaking the other matched functions in this TU). Permuter rejected the function because compile-size never reached the expected 136B in any seed. Root cause appears to be a register-allocator divergence in const-method codegen tied to how the comparator (`a/range <= one`) is materialized — likely needs a different comparator-source structure or a per-file -Xmopt override that we can't safely set without re-validating the rest of nwAll_psp.obj.


## Disassembly

```asm
1a635c:	c48f004c 	lwc1	$f15,76(a0)
1a6360:	3c043e80 	lui	a0,0x3e80
1a6364:	44846800 	mtc1	a0,$f13
1a6368:	460d7bc1 	sub.s	$f15,$f15,$f13
1a636c:	3c043f19 	lui	a0,0x3f19
1a6370:	3484999a 	ori	a0,a0,0x999a
1a6374:	44807000 	mtc1	zero,$f14
1a6378:	44846000 	mtc1	a0,$f12
1a637c:	3c043f80 	lui	a0,0x3f80
1a6380:	460d6341 	sub.s	$f13,$f12,$f13
1a6384:	460e783c 	c.lt.s	$f15,$f14
1a6388:	00000000 	nop
1a638c:	45000003 	bc1f	0x1a639c
1a6390:	44846000 	mtc1	a0,$f12
1a6394:	10000002 	b	0x1a63a0
1a6398:	46007406 	mov.s	$f16,$f14
1a639c:	46007c06 	mov.s	$f16,$f15
1a63a0:	460d8403 	div.s	$f16,$f16,$f13
1a63a4:	460c803e 	c.le.s	$f16,$f12
1a63a8:	00000000 	nop
1a63ac:	45010003 	bc1t	0x1a63bc
1a63b0:	00000000 	nop
1a63b4:	03e00008 	jr	ra
1a63b8:	460c6001 	sub.s	$f0,$f12,$f12
1a63bc:	460e783c 	c.lt.s	$f15,$f14
1a63c0:	00000000 	nop
1a63c4:	45010003 	bc1t	0x1a63d4
1a63c8:	00000000 	nop
1a63cc:	10000001 	b	0x1a63d4
1a63d0:	46007b86 	mov.s	$f14,$f15
1a63d4:	460d7003 	div.s	$f0,$f14,$f13
1a63d8:	46006001 	sub.s	$f0,$f12,$f0
1a63dc:	03e00008 	jr	ra
1a63e0:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: nwConnection::GetQuality(void) const
// Address: 0x001a635c, Size: 136B
// Obj: nwAll_psp.obj

f32 nwConnection__GetQuality_voidconst(void *arg0) {
    f32 temp_f13;
    f32 temp_f15;
    f32 var_f14;
    f32 var_f16;

    temp_f15 = arg0->unk4C - 0.25f;
    var_f14 = 0.0f;
    temp_f13 = 0.6f - 0.25f;
    if (temp_f15 < 0.0f) {
        var_f16 = 0.0f;
    } else {
        var_f16 = temp_f15;
    }
    if (!((var_f16 / temp_f13) <= 1.0f)) {
        return 1.0f - 1.0f;
    }
    if (!(temp_f15 < 0.0f)) {
        var_f14 = temp_f15;
    }
    return 1.0f - (var_f14 / temp_f13);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
