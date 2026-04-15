# 008: SNC Name Mangling Scheme and Calling Conventions

**Date**: 2026-04-14
**Status**: Verified via systematic compiler testing with pspsnc 1.2.7503.0

## Overview

SNC (SN Systems ProDG) uses a proprietary C++ name mangling scheme that differs from both Itanium ABI and MSVC mangling. This document is the complete specification, reverse-engineered from compiled test cases and verified against the EBOOT symbol table.

---

## 1. Top-Level Symbol Structure

All mangled C++ symbols start with `__0` followed by a **category letter**:

| Prefix | Meaning | Example |
|--------|---------|---------|
| `__0f` | Member function (non-ctor/dtor) | `__0fFcBaseEReadR6FcFileP6IcMemPool` |
| `__0F` | Free function (non-member) | `__0FGf_voidv` |
| `__0o` | Constructor, destructor, or member operator | `__0oFcGUIDctv` |
| `__0O` | Global operator (new, delete) | `__0OnwUi` |
| `__0d` | Data (vtables, static members) | `__0dFVBaseG__vtbl` |

### 1.1 Member Functions (`__0f`)

```
__0f <class_name> <method_name> <params> [K] [T]
```

- `K` suffix = `const` method
- `T` suffix = `static` method
- No suffix = normal instance method

### 1.2 Free Functions (`__0F`)

```
__0F <function_name> <params>
```

### 1.3 Constructors/Destructors/Member Operators (`__0o`)

```
__0o <class_name> ct <params>    // constructor
__0o <class_name> dt <params>    // destructor
__0o <class_name> <op> <params>  // operator (see operator table)
```

### 1.4 Global Operators (`__0O`)

```
__0O nw <params>    // operator new
__0O dl <params>    // operator delete
```

---

## 2. Length-Prefixed Name Encoding

All identifiers (class names, method names) are encoded as a **length prefix character** followed by the raw name string.

### 2.1 Length Encoding Table

The length prefix is a single character from the sequence `A-Z, a-z`:

| Length | Char | Length | Char | Length | Char |
|--------|------|--------|------|--------|------|
| 1 | B | 14 | O | 27 | b |
| 2 | C | 15 | P | 28 | c |
| 3 | D | 16 | Q | 29 | d |
| 4 | E | 17 | R | 30 | e |
| 5 | F | 18 | S | 31 | f |
| 6 | G | 19 | T | 32 | g |
| 7 | H | 20 | U | ... | ... |
| 8 | I | 21 | V | 50 | y |
| 9 | J | 22 | W | 51 | z |
| 10 | K | 23 | X | 52 | 0A |
| 11 | L | 24 | Y | 53 | 0B |
| 12 | M | 25 | Z | ... | ... |
| 13 | N | 26 | a | 103 | 0z |

**Formula**: For length `n`:
- If `1 <= n <= 25`: character at position `n` in `A-Z` (i.e., `B`=1, `C`=2, ..., `Z`=25)
- If `26 <= n <= 51`: character at position `n-26` in `a-z` (i.e., `a`=26, `b`=27, ..., `z`=51)
- If `52 <= n <= 103`: `0` + single-char encoding of `n-51` (i.e., `0A`=52, `0B`=53, ..., `0z`=103)
- Larger values: additional `0` prefixes following the same pattern

**Note**: `A` (value 0) is reserved and never appears as a standalone length prefix.

### 2.2 Examples

| Name | Length | Encoding |
|------|--------|----------|
| `Foo` | 3 | `DFoo` |
| `cBase` | 5 | `FcBase` |
| `eCamera` | 7 | `HeCamera` |
| `cMemPool` | 8 | `IcMemPool` |
| `cTimeValue` | 10 | `KcTimeValue` |
| `cFileSystem` | 11 | `LcFileSystem` |
| `eHeightmapShape` | 15 | `PeHeightmapShape` |
| `cFileSystemPlatform` | 19 | `TcFileSystemPlatform` |
| `eKeyframedControllerTemplate` | 28 | `ceKeyframedControllerTemplate` |
| `gcDoEntitySendPartialControllerMessage` | 38 | `mgcDoEntitySendPartialControllerMessage` |

---

## 3. Type Encodings

### 3.1 Primitive Types

| Type | Code |
|------|------|
| `void` | `v` |
| `bool` | `b` |
| `char` | `c` |
| `signed char` | `Sc` |
| `unsigned char` | `Uc` |
| `short` | `s` |
| `unsigned short` | `Us` |
| `int` | `i` |
| `unsigned int` | `Ui` |
| `long` | `l` |
| `unsigned long` | `Ul` |
| `long long` | `L` |
| `unsigned long long` | `UL` |
| `float` | `f` |
| `double` | `d` |
| `wchar_t` | `w` |

### 3.2 Type Qualifiers and Modifiers

| Modifier | Prefix | Example |
|----------|--------|---------|
| Pointer | `P` | `Pi` = `int*` |
| Const pointer | `PC` | `PCi` = `const int*` |
| Volatile pointer | `PV` | `PVi` = `volatile int*` |
| Const volatile pointer | `PCV` | (not tested, likely `PCVi`) |
| Reference | `R` | `Ri` = `int&` |
| Const reference | `RC` | `RCi` = `const int&` |
| Volatile reference | `RV` | `RVi` = `volatile int&` |
| Pointer to pointer | `PP` | `PPi` = `int**` |
| Top-level const pointer | `CP` | `CPCc` = `const char* const` |

Modifiers are applied as prefixes and can be stacked: `PCi` = `const int*`, `PPi` = `int**`.

### 3.3 Class/Struct/Enum Types

User-defined types are encoded with the `6` prefix followed by the length-prefixed name:

```
6 <length_char> <name>
```

| Type | Encoding |
|------|----------|
| `cBase*` | `P6FcBase` |
| `const mVec3&` | `RC6FmVec3` |
| `cTimeValue` (by value) | `6KcTimeValue` |
| `eCollisionContactInfo*` | `P6VeCollisionContactInfo` |

### 3.4 Nested Types (Enums in Classes, Namespaces)

Types nested within classes or namespaces use the `5` prefix for scope qualification:

```
6 <scope_encoding> <hash_char> __ <qualified_type_encoding>
```

Where `<qualified_type_encoding>` is:
```
5 <scope1_len_name> [5 <scope2_len_name> ...] <type_len_name>
```

The `<hash_char>` before `__` is approximately `len(qualified_type_encoding) + 3` (varies slightly for deep nesting). It serves as an internal consistency field.

**Examples**:

| Type | Encoding |
|------|----------|
| `Outer::Mode` (enum) | `65FOuterO__5FOuterEMode` |
| `eVideo::eVideoMode` | `65GeVideoV__5GeVideoKeVideoMode` |
| `eInputJoystick::eButton` | `65OeInputJoysticka__5OeInputJoystickHeButton` |
| `A::B::Color` | `65BA5BBN__5BA5BBFColor` |

### 3.5 Function Pointer Types

```
PF <param_types> _ <return_type>
```

| Type | Encoding |
|------|----------|
| `void (*)(int)` | `PFi_v` |
| `int (*)(float, float)` | `PFfTB_i` |
| `void (*)()` | `PFv_v` |
| `void (*)(void*)` returning `void` | `PFPv_v` |

Note: The `_` inside function pointer types separates parameters from return type. Backreferences (T-codes) work within the function pointer signature.

### 3.6 Pointer-to-Member

```
PM <class_len_name> <member_type>
```

Example: `int Baz::*` = `PMDBazDi` (PM + class `DBaz` + type `Di`... wait, actually `Di` = length-4 name "i"? No, `i` is just the `int` type code. The `D` here is part of `DBaz` which is the class encoding.)

Corrected: `PMDBazi` where `DBaz` = class Baz (3 chars), `i` = int.

---

## 4. Backreferences

SNC uses backreferences to avoid repeating identical type encodings in parameter lists.

### 4.1 Parameter Indexing

Parameters are indexed with letters starting from `B`:

- `A` = reserved (implicit `this` for member functions, unused for free functions)
- `B` = 1st explicit parameter
- `C` = 2nd explicit parameter
- `D` = 3rd explicit parameter
- etc.

### 4.2 Single Backreference: `T<index>`

`T<index>` means "same type as parameter `<index>`".

| Signature | Encoding |
|-----------|----------|
| `f(int, int)` | `iTB` |
| `f(int, float, int)` | `ifTB` |
| `f(int, float, int, float)` | `ififTBTC` (TB=same as B=int, TC=same as C=float) |
| `f(Alpha*, Alpha*)` | `P6FAlphaTB` |
| `f(Alpha*, int, Alpha*)` | `P6FAlphaiTB` |

### 4.3 Range Backreference: `N<end><start>`

`N<end><start>` means "parameters from `<start>+1` through `<end>` are all the same type as `<start>`". Used when 3+ consecutive parameters share the same type.

| Signature | Encoding | Explanation |
|-----------|----------|-------------|
| `f(int, int, int)` | `iNCB` | B=int, C=int (same as B) |
| `f(int, int, int, int)` | `iNDB` | B=int, C,D=int (same as B) |
| `f(int, int, int, int, int)` | `iNEB` | B=int, C,D,E=int (same as B) |
| `f(float, float, float)` | `fNCB` | same pattern |

---

## 5. Namespaces

Namespaces use the `5` prefix followed by the length-prefixed namespace name:

```
5 <namespace_len_name>
```

| Construct | Encoding |
|-----------|----------|
| `MyNS::func()` | `__0F5EMyNSEfuncv` |
| `MyNS::Bar::method()` | `__0f5EMyNSDBarGmethodv` |
| `MyNS::Inner::innerFunc()` | `__0F5EMyNS5FInnerJinnerFuncv` |

Nested namespaces stack `5`-prefixed segments.

### 5.1 Nested Classes (without namespaces)

Nested classes also use `5` prefixes:

| Construct | Encoding |
|-----------|----------|
| `Outer::Inner::method()` | `__0f5FOuterFInnerGmethodv` |
| `A::B::C::deep()` | `__0f5BA5BBBCEdeepv` |

---

## 6. Operators

### 6.1 Operator Name Codes

Member operators use the `__0o` prefix. The operator is encoded between the class name and parameters:

| Operator | Code | Example |
|----------|------|---------|
| `operator+` | `pl` | `__0oDFooplRC6DFoo` |
| `operator-` (binary & unary) | `mi` | `__0oDFoomiRC6DFoo` |
| `operator*` (multiply & deref) | `ml` | `__0oDFoomlRC6DFoo` |
| `operator/` | `dv` | `__0oDFoodvRC6DFoo` |
| `operator%` | `md` | `__0oDFoomdRC6DFoo` |
| `operator=` | `as` | `__0oDFooasRC6DFoo` |
| `operator==` | `eq` | `__0oDFooeqRC6DFoo` |
| `operator!=` | `ne` | `__0oDFooneRC6DFoo` |
| `operator<` | `lt` | `__0oDFooltRC6DFoo` |
| `operator>` | `gt` | `__0oDFoogtRC6DFoo` |
| `operator<=` | `le` | `__0oDFooleRC6DFoo` |
| `operator>=` | `ge` | `__0oDFoogeRC6DFoo` |
| `operator+=` | `apl` | `__0oDFooaplRC6DFoo` |
| `operator-=` | `ami` | `__0oDFooamiRC6DFoo` |
| `operator*=` | `amu` | `__0oDFooamuRC6DFoo` |
| `operator/=` | `adv` | `__0oDFooadvRC6DFoo` |
| `operator\|` | `or` | `__0oDFooorRC6DFoo` |
| `operator&` (bitwise) | `ad` | `__0oDFooadRC6DFoo` |
| `operator^` | `er` | `__0oDFooerRC6DFoo` |
| `operator\|=` | `aor` | `__0oDFooaorRC6DFoo` |
| `operator&=` | `aad` | `__0oDFooaadRC6DFoo` |
| `operator^=` | `aer` | `__0oDFooaerRC6DFoo` |
| `operator<<` | `ls` | `__0oDFoolsi` |
| `operator>>` | `rs` | `__0oDFoorsi` |
| `operator<<=` | `als` | `__0oDFooalsi` |
| `operator>>=` | `ars` | `__0oDFooarsi` |
| `operator++` | `pp` | `__0oDFooppv` (prefix), `__0oDFooppi` (postfix) |
| `operator--` | `mm` | `__0oDFoommv` (prefix), `__0oDFoommi` (postfix) |
| `operator~` | `co` | `__0oDFoocov` |
| `operator!` | `nt` | `__0oDFoontv` |
| `operator&` (address-of) | `ad` | `__0oDFooadv` |
| `operator->` | `rf` | `__0oDFoorfv` |
| `operator[]` | `vc` | `__0oDFoovci` |
| `operator()` | `cl` | `__0oDFooclv`, `__0oDFoocli` |
| `operator,` | `cm` | `__0oDFoocmR6DFoo` |

### 6.2 Conversion Operators

```
op <type_encoding>
```

| Conversion | Code |
|------------|------|
| `operator int()` | `opi` |
| `operator float()` | `opf` |
| `operator bool()` | `opb` |
| `operator X*()` | `opP6BX` |

### 6.3 Constructors and Destructors

| Special | Code |
|---------|------|
| Constructor | `ct` |
| Destructor | `dt` |

Examples:
- `cGUID::cGUID()` = `__0oFcGUIDctv`
- `cGUID::cGUID(int)` = `__0oFcGUIDcti`
- `VBase::~VBase()` = `__0oFVBasedtv`

### 6.4 Global Operators

| Operator | Code |
|----------|------|
| `operator new(unsigned int)` | `__0OnwUi` |
| `operator delete(void*)` | `__0OdlPv` |

---

## 7. Templates

### 7.1 Template Class Encoding

Template arguments are introduced with `7` and terminated with `_`:

```
<class_len_name> 7 <template_args> _
```

Template args use standard type encodings: `i`=int, `f`=float, `6<len><name>`=struct, etc.

| Template | Encoding |
|----------|----------|
| `Vec<int>` | `DVec7i_` |
| `Vec<float>` | `DVec7f_` |
| `Vec<MyData>` | `DVec76GMyData_` |
| `Pair<int, float>` | `EPair7if_` |
| `cHandleT<eMesh>` | `IcHandleT76FeMesh_` |
| `cRedBlackTreeNode<eCollisionPair>` | `RcRedBlackTreeNode76OeCollisionPair_` |

Nested templates nest `7..._` sections:
```
cFixedSizeAllocatorBase<cRedBlackTreeNode<eCollisionPair>>
= XcFixedSizeAllocatorBase76RcRedBlackTreeNode76OeCollisionPair__
```

Note the `__` (double underscore) which closes two template levels at once.

### 7.2 Template Parameter References in Method Signatures

When a method parameter uses a template type parameter, it is encoded as:

```
9 <param_letter> A
```

Where `<param_letter>` identifies the template parameter: `B`=first, `C`=second, etc.

The `_` terminator appears at the end of the parameter/return-type zone for template methods.

| Method | Encoding |
|--------|----------|
| `Vec<int>::set(int)` | `__0fDVec7i_Dset9BA_v` |
| `Vec<float>::set(float)` | `__0fDVec7f_Dset9BA_v` |
| `Pair<int,float>::swap(int&,float&)` | `__0fEPair7if_EswapR9BAR9CA_v` |
| `Box<float>::set(float, int)` | `__0fDBox7f_Dset9BAi_v` |
| `Box<float>::get(int, float*)` | `__0fDBox7f_DgetiP9BA_v` |
| `Ptr<int>::set(int*)` | `__0fDPtr7i_DsetP9BA_v` |

### 7.3 Template Return Types

Template class methods **encode the return type** after a `_` separator (unlike non-template methods which omit return types):

```
<params> _ <return_type>
```

| Method | Encoding | Return |
|--------|----------|--------|
| `Getter<int>::get()` returns `int` | `Dgetv_9BA` | template param (=int) |
| `Getter<float>::getInt()` returns `int` | `GgetIntv_i` | int |
| `Fixed<10>::clear()` returns `void` | `Fclearv_v` | void |

### 7.4 Non-Type Template Parameters

Integer template parameters use `4` as a type marker, followed by the value encoding:

**Value encoding** (same alphabet as length encoding but 0-indexed):
- `A`=0, `B`=1, `C`=2, ..., `Z`=25, `a`=26, ..., `z`=51
- `0A`=52, `0B`=53, ..., `0z`=103
- `00A`=104, ..., `0000z`=259
- For values >= 260: `8` + length-prefixed decimal string (e.g., `8D260`, `8E9999`)
- Negative values: append `n` (e.g., `Bn`=-1, `Fn`=-5, `0wn`=-100)

| Template | Encoding |
|----------|----------|
| `Fixed<0>` | `FFixed74A_` |
| `Fixed<1>` | `FFixed74B_` |
| `Fixed<10>` | `FFixed74K_` |
| `Fixed<256>` | `FFixed740000w_` |
| `Fixed<1000>` | `FFixed748E1000_` |
| `Fixed<-1>` | `BV74Bn_` |
| `Fixed<-5>` | `BV74Fn_` |

---

## 8. Special Symbols

### 8.1 Virtual Tables

```
__0d <class_len_name> G__vtbl
```

Example: `__0dFVBaseG__vtbl` = vtable for VBase.

### 8.2 Static Initializers

```
__sti__<source_file_without_extension>_cpp
```

These are NOT mangled with the `__0` scheme. They are emitted as literal symbol names.

### 8.3 Thunks / Wrappers

Some functions in the codebase use a non-mangled naming convention for C-linkage wrappers:

```
ClassName__MethodName_paramtype1_paramtype2...
```

Example: `eShape__GetSupport_constmVec3ref_constmOCSref_eCollisionSupportptrconst`

These are typically C-linkage shims and don't follow the `__0` mangling scheme.

---

## 9. Calling Conventions

SNC on PSP uses an **extended o32** calling convention with more register arguments than standard MIPS o32.

### 9.1 Integer/Pointer Arguments

Up to **8 integer/pointer arguments** passed in registers:

| Register | MIPS Name | Argument |
|----------|-----------|----------|
| `$4` | `a0` | 1st int/ptr arg |
| `$5` | `a1` | 2nd int/ptr arg |
| `$6` | `a2` | 3rd int/ptr arg |
| `$7` | `a3` | 4th int/ptr arg |
| `$8` | `t0` | 5th int/ptr arg |
| `$9` | `t1` | 6th int/ptr arg |
| `$10` | `t2` | 7th int/ptr arg |
| `$11` | `t3` | 8th int/ptr arg |

Additional arguments go on the stack.

### 9.2 Float Arguments

Float arguments use a **separate register track** from integers. They do NOT consume integer register slots.

| Register | Argument |
|----------|----------|
| `$f12` | 1st float arg |
| `$f13` | 2nd float arg |
| `$f14` | 3rd float arg |
| `$f15` | 4th float arg |
| `$f16` | 5th float arg |
| `$f17` | 6th float arg |
| `$f18` | 7th float arg |
| `$f19` | 8th float arg |

Up to 8 single-precision floats in consecutive FPU registers starting at `$f12`.

**Important**: This differs from standard MIPS o32, which uses only `$f12` and `$f14` and interleaves with integer slots.

### 9.3 Mixed Integer/Float Parameters

Integers and floats use **independent register tracks**:

```cpp
void f(int a, float b, int c);
// a -> a0, b -> $f12, c -> a1
// (NOT a0, skip, a2 like standard o32)
```

### 9.4 Member Functions

`this` pointer is always passed in `a0`. Explicit parameters start at `a1` (for integers/pointers) or `$f12` (for floats).

### 9.5 Return Values

| Type | Register(s) |
|------|-------------|
| `int` / pointer | `v0` (`$2`) |
| `float` | `$f0` |
| `double` | `v0:v1` (`$2:$3`) as integer pair |
| struct by value | Hidden pointer param in `a0`; caller allocates space |

For struct return by value, the caller passes a hidden first argument (pointer to return space) in `a0`. All other parameters shift to the next registers.

### 9.6 Double Arguments

Doubles are passed as 64-bit values in **pairs of integer registers** (NOT FPU register pairs):

```cpp
void f(double a, double b);
// a -> a0:a1 (low:high), b -> a2:a3
```

---

## 10. Quick Reference: Mangling a Symbol

To mangle `eCamera::Update(cTimeValue)`:

1. Category: member function = `__0f`
2. Class name: `eCamera` = 7 chars = `H` prefix = `HeCamera`
3. Method name: `Update` = 6 chars = `G` prefix = `GUpdate`
4. Parameter: `cTimeValue` = struct by value = `6` + length `K`(10) + name = `6KcTimeValue`
5. No const/static suffix
6. Result: `__0fHeCameraGUpdate6KcTimeValue`

To mangle `eHeightmapShape::GetAABB(mBox*, const mOCS&) const`:

1. Category: member function = `__0f`
2. Class: `eHeightmapShape` = 15 chars = `P` = `PeHeightmapShape`
3. Method: `GetAABB` = 7 chars = `H` = `HGetAABB`
4. Param 1: `mBox*` = `P6EmBox` (E=4 chars, pointer to struct)
5. Param 2: `const mOCS&` = `RC6EmOCS` (const ref to struct)
6. Const: `K`
7. Result: `__0fPeHeightmapShapeHGetAABBP6EmBoxRC6EmOCSK`

To verify any mangling, compile a stub and check with `nm`:
```bash
echo 'struct eCamera {}; struct cTimeValue {}; void eCamera::Update(cTimeValue) {}' > /tmp/t.cpp
extern/wibo extern/snc/pspsnc.exe -c -O0 -G0 -Iextern/include -Iinclude -o /tmp/t.o /tmp/t.cpp
mipsel-linux-gnu-nm /tmp/t.o | grep " T "
```
