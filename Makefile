# Days of Thunder (PSP) — decompilation build system
# Compiler: SNC (SN Systems) via wibo
# Target: PRX (relocatable ELF), VMA 0x0

WIBO     := extern/wibo
SNC      := extern/snc/pspsnc.exe
CC       := $(WIBO) $(SNC)
AS       := mipsel-linux-gnu-as
LD       := mipsel-linux-gnu-ld
OBJCOPY  := mipsel-linux-gnu-objcopy

CFLAGS   := -c -O2 -G0 -Xsched=2 -Xvfpumatrix=1 -Xvfpuscalar=8 -Iextern/include -Iinclude
ECFLAGS  := -c -O2 -G0 -Xsched=1 -Xvfpumatrix=1 -Xvfpuscalar=8 -Iextern/include -Iinclude
ASFLAGS  := -march=allegrex -mabi=eabi -EL -Iinclude
LDFLAGS  := -T build/EBOOT.ld --Map build/EBOOT.map

TARGET      := extern/iso_extract/PSP_GAME/SYSDIR/EBOOT.BIN.dec
SHA1        := 952cbd11eb99fe986662e83d2d3512d207f9b979
BUILD_DIR   := build
TARGET_ELF  := $(BUILD_DIR)/EBOOT.elf
TARGET_BIN  := $(BUILD_DIR)/EBOOT.bin

# Collect source files
_ALL_SRCS  := $(shell find src \( -name '*.c' -o -name '*.cpp' \) 2>/dev/null)
C_SRCS     := $(filter %.c,$(_ALL_SRCS))
CXX_SRCS   := $(filter %.cpp,$(_ALL_SRCS))
ASM_SRCS   := $(shell find asm -name '*.s' 2>/dev/null)
ASSET_BINS := $(wildcard assets/*.bin)

# Derive object paths
C_OBJS     := $(patsubst src/%.c,$(BUILD_DIR)/src/%.c.o,$(C_SRCS))
CXX_OBJS   := $(patsubst src/%.cpp,$(BUILD_DIR)/src/%.cpp.o,$(CXX_SRCS))
ASM_OBJS   := $(patsubst asm/%.s,$(BUILD_DIR)/asm/%.s.o,$(ASM_SRCS))
ASSET_OBJS := $(patsubst assets/%.bin,$(BUILD_DIR)/assets/%.bin.o,$(ASSET_BINS))

ALL_OBJS := $(ASM_OBJS) $(ASSET_OBJS) $(C_OBJS) $(CXX_OBJS)

# ──────────────────────────────────────────
# Targets
# ──────────────────────────────────────────

all: verify

clean:
	rm -rf $(BUILD_DIR)/src $(BUILD_DIR)/asm $(BUILD_DIR)/assets
	rm -f $(TARGET_ELF) $(TARGET_BIN) $(BUILD_DIR)/EBOOT.map

# Link all objects into ELF
$(TARGET_ELF): $(ALL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Extract raw binary from ELF
$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) -O binary $< $@

# SHA1 verification against original
verify: $(TARGET_BIN)
	@echo "$(SHA1)  $<" | shasum -a 1 -c

# ──────────────────────────────────────────
# Per-file flag overrides
#
# Almost all code uses -Xsched=2 (SNC default at -O2). Within eAll_psp.obj,
# some engine classes (addr range ~0x6e000-0x8f000) use -Xsched=1 instead:
#   sched=1: eTextureMap, eBumpOffsetMap, eDynamicMeshMorphTarget,
#            eCollisionConstraint, eInputKeyboard
#   sched=2: everything else (eAudio, eCamera, eRenderTarget, eShape,
#            ePortal, ePhysics, plus all gc/c/nw/gMain/m code)
#
# See docs/decisions/003-compiler-flags.md for the full analysis.
# ──────────────────────────────────────────

# Engine classes confirmed sched=1 (per prologue byte-matching)
#
# WARNING: eAll_psp.obj is mixed at class/method granularity. Prefer local
# `#pragma control sched=N` while a class has known sched=1 and sched=2
# methods. Add broad Makefile overrides only after multiple non-trivial
# matches prove a class is consistently sched=1. See
# docs/research/snc-transition-zone-sched.md.
$(BUILD_DIR)/src/eTextureMap%.o: CFLAGS := $(ECFLAGS)
$(BUILD_DIR)/src/eBumpOffsetMap%.o: CFLAGS := $(ECFLAGS)
$(BUILD_DIR)/src/eDynamicMeshMorphTarget%.o: CFLAGS := $(ECFLAGS)
$(BUILD_DIR)/src/eCollisionConstraint%.o: CFLAGS := $(ECFLAGS)
$(BUILD_DIR)/src/eCompoundShape%.o: CFLAGS := $(ECFLAGS)
$(BUILD_DIR)/src/eInputKeyboard%.o: CFLAGS := $(ECFLAGS)
$(BUILD_DIR)/src/eDynamicModel%.o: CFLAGS := $(ECFLAGS)

# Other per-file overrides
$(BUILD_DIR)/src/gcLoadingScreen_Read.cpp.o: CFLAGS += -Xxopt=5
$(BUILD_DIR)/src/gcUIWidget_InsertIntoDialog.cpp.o: CFLAGS := -c -O2 -G0 -Xsched=2 -Xvfpumatrix=1 -Xvfpuscalar=8 -Xmopt=0 -Iextern/include -Iinclude

# VFPU register allocation flags — controls which VFPU registers SNC uses
# for auto-vectorized struct copies (lv.q/sv.q). All obj files use the same
# values: -Xvfpumatrix=1 -Xvfpuscalar=8 (first auto-temp at vt=24/C120).
# Confirmed by scanning lv.q/sv.q register encodings across gcAll, eAll,
# mAll, and cAll obj files. nwAll and gMain have no VFPU (flags are harmless).
# See docs/decisions/007-vfpu-native-types.md.
# Flags are now in base CFLAGS/ECFLAGS above.

# ──────────────────────────────────────────
# Compile rules
# ──────────────────────────────────────────

# C → .o via SNC
$(BUILD_DIR)/src/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

# C++ → .o via SNC
$(BUILD_DIR)/src/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

# Assembly → .o (text, rodata, data, bss sections)
$(BUILD_DIR)/asm/%.s.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# Raw binary assets → .o (wraps bin in .data section)
$(BUILD_DIR)/assets/%.bin.o: assets/%.bin
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf32-tradlittlemips -B mips $< $@

# ──────────────────────────────────────────
# Convenience
# ──────────────────────────────────────────

# Build a single file for quick iteration:
#   make build/src/foo.c.o
#   make build/src/bar.cpp.o
#
# Compare with asm-differ:
#   python3 extern/asm-differ/diff.py -o -f build/src/bar.cpp.o SYMBOL_NAME

.PHONY: all clean verify
