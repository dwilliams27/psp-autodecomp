# This file is used by modern gas.
# Defines the expected assembly macros

# Evaluate this file only once in case it's included more than once
.ifndef _MACRO_INC_GUARD
.internal _MACRO_INC_GUARD
.set _MACRO_INC_GUARD, 1

# A function symbol.
.macro .globl label, visibility=global
    .\visibility "\label"
    "\label":
.endm

# Note: .ent/.end directives were stripped from asm/0.s because GAS MIPS
# auto-generates .size from them, which fails on some splat symbol names
# ([], ~, etc). GAS does not allow macro overrides of .ent/.end.

# A label referenced by an error handler table.
.macro ehlabel label, visibility=global
    .\visibility "\label"
    "\label":
.endm




.endif
