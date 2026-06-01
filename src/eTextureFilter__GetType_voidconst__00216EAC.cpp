// ODR-WARNING: split-TU. eTextureFilter::GetType(void) const @ 0x00216EAC.
// Emits __0fOeTextureFilterHGetTypevK. The canonical class declaration in
// eTextureMap.h / src/eTextureFilter.cpp does not list GetType; redeclare the
// class locally here with only what this TU references. Do not add GetType to
// shared headers (perturbs sibling TUs).

class cBase;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern const char eTextureFilter_base_name[];
extern const char eTextureFilter_base_desc[];

static cType *type_base;
static cType *type_eTextureFilter;

class eTextureFilter {
public:
    const cType *GetType(void) const;
};

#pragma control sched=1
const cType *eTextureFilter::GetType(void) const {
    if (!type_eTextureFilter) {
        if (!type_base) {
            const char *name = eTextureFilter_base_name;
            const char *desc = eTextureFilter_base_desc;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            type_base = cType::InitializeType(name, desc,
                                              1, 0, 0, 0, 0, 0);
        }
        type_eTextureFilter = cType::InitializeType(0, 0, 0x13B, type_base,
                                                    0, 0, 0, 0);
    }
    return type_eTextureFilter;
}
