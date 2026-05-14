// ODR-WARNING: this TU intentionally redeclares eMaterial with a minimal
// member list for a method absent from the canonical eMaterial.cpp class
// definition. Do not include this elsewhere unless all matched siblings are
// rechecked.

class cObject {
public:
    cObject &operator=(const cObject &);
};

class eMaterialData;

class eMaterial : public cObject {
public:
    char _padA[0x44];
    int mField44;
    int mTextures[2];
    int mField50;
    int mField54;
    int mField58;

    eMaterial &operator=(const eMaterial &);
    void CopyMaterialData(eMaterialData *);
};

// eMaterial::operator= @ 0x0002c154
// Base cObject assignment, copy mField44, two-word loop over mTextures,
// copy mField50/mField54, then CopyMaterialData(other.mField58).
eMaterial &eMaterial::operator=(const eMaterial &other) {
    ((cObject *)this)->operator=(*(const cObject *)&other);
    mField44 = other.mField44;

    register int i asm("$a2") = 0;
    register int *dst asm("$a1") = (int *)((char *)this + 0x48);
    register int *src asm("$a0") = (int *)((char *)&other + 0x48);
    do {
        int tmp = *src;
        i++;
        *dst = tmp;
        dst++;
        src++;
    } while (i < 2);

    mField50 = other.mField50;
    mField54 = other.mField54;
    CopyMaterialData((eMaterialData *)other.mField58);
    return *this;
}
