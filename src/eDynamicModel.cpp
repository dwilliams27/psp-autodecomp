#include "eDynamicModel.h"
#include "eDynamicMesh.h"

bool eDynamicModel::NeedsSkinning(const eDynamicMesh *mesh, int count, int *indices) const {
    for (int i = 0; i < count; i++) {
        int idx = indices[i];
        idx <<= 2;
        char *nodes = *(char **)((char *)mesh + 0x7C);
        int n = 0;
        char *ptr = *(char **)((unsigned int)nodes + idx + 0x88);
        if (ptr != 0) {
            n = *(int *)(ptr - 4) & 0x3FFFFFFF;
        }
        if (n > 0) {
            char *table = *(char **)((char *)this + 0x148);
            unsigned int val = *(unsigned int *)((unsigned int)table + idx);
            unsigned int gval = *(unsigned int *)0x0037D124;
            if ((unsigned char)((val >> 16) == (gval & 0xFFFF))) {
                return false;
            }
            return true;
        }
    }
    return true;
}

extern char eDynamicMeshclassdesc[];

eDynamicMesh::eDynamicMesh(cBase *parent) : cObject(parent) {
    ((int *)this)[0x44 / 4] = 0;
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x48 / 4] = -1;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x4C) = 0.0f;
    __asm__ volatile("" ::: "memory");
    ((unsigned char *)this)[0x58] = 0;
    __asm__ volatile("" ::: "memory");
    ((void **)this)[1] = eDynamicMeshclassdesc;
    *(short *)((char *)this + 0x5C) = 0;
    *(short *)((char *)this + 0x5E) = 0;
    *(short *)((char *)this + 0x60) = 0;
    *(short *)((char *)this + 0x62) = 0;
    *(short *)((char *)this + 0x64) = 0;
    *(short *)((char *)this + 0x66) = 0;
    ((int *)this)[0x68 / 4] = 0;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((int *)this)[0x74 / 4] = 0;
    *(void **)((char *)this + 0x78) = (void *)this;
    ((int *)this)[0x7C / 4] = 0;
    ((int *)this)[0x80 / 4] = 0;
    *(float *)((char *)this + 0x50) = 1024.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x54) = 0.0009765625f;
}
