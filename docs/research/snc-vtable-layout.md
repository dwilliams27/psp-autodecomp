# SNC Vtable Layout and Virtual Dispatch

Reverse-engineered from SNC compilation experiments and EBOOT binary analysis (2026-04-14).

## Vtable Pointer Location

**Non-standard:** The vtable pointer is stored at the END of the object's data members, NOT at offset 0.

For a class like:
```cpp
struct eShape {
    int flags;        // offset 0x00
    void *vtable;     // offset 0x04 (after all data members)
    virtual void f();
};
```

The vtable pointer offset depends on how many data members precede it. This differs from Itanium ABI (vtable at offset 0) and MSVC (vtable at offset 0).

## Vtable Entry Format

Each vtable entry is **8 bytes**:
```
struct VtableEntry {
    int16_t this_adjust;   // signed halfword — this pointer adjustment
    int16_t padding;       // unused
    uint32_t func_ptr;     // function pointer (relocated)
};
```

## Virtual Call Codegen Pattern

```asm
lw    $a0, 0(ptr)          // load object pointer
lw    $a1, VTABLE_OFF($a0) // load vtable pointer (offset = after data members)
addiu $a1, $a1, ENTRY_OFF  // index into vtable (entry_index * 8)
lh    $a2, 0($a1)          // load this-adjust (signed halfword)
addu  $a0, $a0, $a2        // adjust this pointer
lw    $a3, 4($a1)          // load function pointer
jalr  $a3                  // call
```

- `VTABLE_OFF`: offset of vtable pointer in the object (depends on class layout)
- `ENTRY_OFF`: `virtual_method_index * 8`

## eShape Vtable (42 entries, 336 bytes)

The eShape class hierarchy has 42 virtual methods. SNC groups overloads together. Key offsets:
- GetAABB: entry at offset known from eCompoundShape dispatch
- GetSupport: offset 0x00
- GetSweptContacts: offset 0xA8
- GetEmbedContacts: offset 0xB0

## Multiple Inheritance

With multiple inheritance, objects have multiple vtable pointers (one per base class with virtuals). The `this_adjust` field in vtable entries handles pointer adjustment when calling through a secondary base's vtable.

## Implications for Matching

1. Must determine the vtable pointer offset for each class (count data members)
2. Must determine the vtable entry offset for each virtual method (count preceding virtuals × 8)
3. The `lh` (signed halfword load) for this-adjust is distinctive — look for it to identify virtual calls
4. Virtual calls through secondary bases have non-zero this-adjust
