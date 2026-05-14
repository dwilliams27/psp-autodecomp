// eCollisionPolytope.cpp - destructor for eAll_psp.obj.
// ODR-WARNING: local declarations expose only the destructor and delete path.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

class eCollisionPolytope {
public:
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }

    ~eCollisionPolytope(void);
};

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oSeCollisionPolytopedtv, 0x260\n");

#pragma control sched=1

eCollisionPolytope::~eCollisionPolytope(void) {
    char *tree1930 = (char *)this + 0x1930;

    if ((void *)tree1930 != 0) {
        void *var_a0 = *(void **)((char *)this + 0x1930);
    tree1930_loop:
        if (var_a0 != 0) {
            void *var_a2 = *(void **)((char *)var_a0 + 0x18);
            if (var_a2 == 0) {
                var_a2 = *(void **)((char *)var_a0 + 0x1C);
                if (var_a2 != 0) {
                    var_a0 = *(void **)((char *)var_a0 + 0x1C);
                    goto tree1930_step;
                }
            } else {
                var_a0 = var_a2;
                goto tree1930_step;
            }

            int mask1930 = -2;
            var_a2 = var_a0;
            do {
                int temp_t0 = *(int *)((char *)var_a2 + 0x14) & mask1930;
                int var_a3 = 0;
                int var_a0_bool;
                if (temp_t0 != 0) {
                    var_a0_bool = var_a3 & 0xFF;
                    if (*(void **)((char *)temp_t0 + 0x18) == var_a0) {
                        var_a3 = 1;
                        goto tree1930_from_left;
                    }
                } else {
                tree1930_from_left:
                    var_a0_bool = var_a3 & 0xFF;
                }

                var_a0 = (void *)temp_t0;
                if (var_a0_bool != 0) {
                    var_a0 = (void *)temp_t0;
                    if (*(void **)((char *)temp_t0 + 0x1C) != 0) {
                        var_a0 = *(void **)((char *)temp_t0 + 0x1C);
                    tree1930_inner:
                        if (var_a0 != 0) {
                            void *var_a3_ptr = *(void **)((char *)var_a0 + 0x18);
                            if (var_a3_ptr == 0) {
                                var_a3_ptr =
                                    *(void **)((char *)var_a0 + 0x1C);
                                if (var_a3_ptr != 0) {
                                    var_a0 =
                                        *(void **)((char *)var_a0 + 0x1C);
                                    goto tree1930_inner_step;
                                }
                            } else {
                                var_a0 = var_a3_ptr;
                            tree1930_inner_step:
                                goto tree1930_inner;
                            }
                        }
                    }
                }

                if (var_a2 != 0) {
                    *(void **)var_a2 = *(void **)((char *)this + 0x193C);
                    *(void **)((char *)this + 0x193C) = var_a2;
                    *(int *)((char *)this + 0x1948) =
                        *(int *)((char *)this + 0x1948) - 1;
                }
                var_a2 = var_a0;
            } while (var_a0 != 0);
        }
    tree1930_step:
        if (var_a0 != 0) {
            goto tree1930_loop;
        }
        *(void **)((char *)this + 0x1930) = 0;
        *(void **)((char *)this + 0x1934) = 0;
    }

    if ((void *)this != 0) {
        void *var_a2 = *(void **)this;
    tree0_loop:
        if (var_a2 != 0) {
            void *temp_a0 = *(void **)((char *)var_a2 + 0x14);
            if (temp_a0 == 0) {
                if (*(void **)((char *)var_a2 + 0x18) != 0) {
                    var_a2 = *(void **)((char *)var_a2 + 0x18);
                    goto tree0_step;
                }
            } else {
                var_a2 = temp_a0;
                goto tree0_step;
            }

            int mask0 = -2;
            void *var_a3 = var_a2;
            do {
                int temp_t1 = *(int *)((char *)var_a3 + 0x10) & mask0;
                int var_t0 = 0;
                int var_a2_bool;
                if (temp_t1 != 0) {
                    var_a2_bool = var_t0 & 0xFF;
                    if (*(void **)((char *)temp_t1 + 0x14) == var_a2) {
                        var_t0 = 1;
                        goto tree0_from_left;
                    }
                } else {
                tree0_from_left:
                    var_a2_bool = var_t0 & 0xFF;
                }

                var_a2 = (void *)temp_t1;
                if (var_a2_bool != 0) {
                    var_a2 = (void *)temp_t1;
                    if (*(void **)((char *)temp_t1 + 0x18) != 0) {
                        var_a2 = *(void **)((char *)temp_t1 + 0x18);
                    tree0_inner:
                        if (var_a2 != 0) {
                            void *temp_t0 = *(void **)((char *)var_a2 + 0x14);
                            if (temp_t0 == 0) {
                                if (*(void **)((char *)var_a2 + 0x18) != 0) {
                                    var_a2 =
                                        *(void **)((char *)var_a2 + 0x18);
                                    goto tree0_inner_step;
                                }
                            } else {
                                var_a2 = temp_t0;
                            tree0_inner_step:
                                goto tree0_inner;
                            }
                        }
                    }
                }

                if (var_a3 != 0) {
                    *(void **)var_a3 = *(void **)((char *)this + 0x14);
                    *(void **)((char *)this + 0x14) = var_a3;
                    *(int *)((char *)this + 0x20) =
                        *(int *)((char *)this + 0x20) - 1;
                }
                var_a3 = var_a2;
            } while (var_a2 != 0);
        }
    tree0_step:
        if (var_a2 != 0) {
            goto tree0_loop;
        }
        *(void **)this = 0;
        *(void **)((char *)this + 4) = 0;
    }
}
