float infinityf(void) {
    union { unsigned int i; float f; } u;
    u.i = 0x7F800000;
    return u.f;
}
