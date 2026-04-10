extern "C" {

void gcUIDialog__OnFinalOpen_void(void *self) {
    *(int *)((char *)self + 0x50) &= ~0x10;
}

}
