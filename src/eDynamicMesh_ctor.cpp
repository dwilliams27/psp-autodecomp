#include "eDynamicMesh.h"

extern char eDynamicMeshclassdesc[];

eDynamicMesh::eDynamicMesh(cBase *parent) : cObject(parent) {
    ((int *)this)[0x44 / 4] = 0;
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x48 / 4] = -1;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x4C) = 0.0f;
    ((unsigned char *)this)[0x58] = 0;
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
