typedef unsigned char u8;

u8 eShape__IsSubShape_voidconst(const void *self) {
    return *(const u8 *)((const char *)self + 0x50);
}
