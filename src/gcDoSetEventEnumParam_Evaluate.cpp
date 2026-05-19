// ODR-WARNING: split translation unit for one gcDoSetEventEnumParam method.
// Keep this local redeclaration minimal so matched siblings in the canonical TU
// do not pick up register-allocation drift.

class gcEnumeration;
class gcEnumerationEntry;

template <class T> class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U> class cHandlePairT {
public:
    int mHandle;
    int mSubHandle;
};

class gcDesiredEnumerationEntryHelper {
public:
    void Get(cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > *) const;
};

class gcEnumeration {
public:
    void GetSubObjectHandle(
        cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > *,
        cSubHandleT<gcEnumerationEntry> *, int) const;
};

class gcDoSetEventEnumParam {
public:
    float Evaluate(void) const;
};

struct EvalSlot {
    short offset;
    short pad;
    void *fn;
};

struct HandleLookup {
    char _pad[0x30];
    int handle;
};

extern void *D_00038890[];

float gcDoSetEventEnumParam::Evaluate(void) const {
    const gcDoSetEventEnumParam *self = this;
    char *root = *(char **)0x37D7B4;
    float one = 1.0f;
    char *events = 0;
    if (root != 0) {
        events = ((char **)root)[4];
    }
    if (events == 0) {
        return one;
    }

    cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > pair;
    cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > copy;
    cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > newPair;
    cSubHandleT<gcEnumerationEntry> sub;
    cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > zeroPair;
    volatile int unused;

    pair.mHandle = 0;
    int hasEntry = *(char *)((const char *)self + 0x14);
    pair.mSubHandle = 0;

    if (hasEntry != 0) {
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x14))
            ->Get(&pair);

        int value = *(int *)((const char *)self + 0x20);
        int ok = 0;
        int tagged = value & 1;
        if (tagged != 0) {
            ok = 1;
        }
        if (ok != 0) {
            ok = 0;
        } else {
            ok = (unsigned char)(value != 0);
            ok = (ok != 0);
        }

        if (ok != 0) {
            int taggedAgain = 0;
            if (tagged != 0) {
                taggedAgain = 1;
            }
            if (taggedAgain != 0) {
                value = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int index;
            int check = value;
            if (check != 0) {
                EvalSlot *slot = (EvalSlot *)(*(char **)(check + 4) + 0x70);
                short off = slot->offset;
                float (*fn)(void *) = (float (*)(void *))slot->fn;
                index = (int)fn((char *)value + off);
            } else {
                float zero = 0.0f;
                index = (int)zero;
            }

            HandleLookup *found;
            if (pair.mHandle == 0) {
                found = 0;
            } else {
                HandleLookup *cand =
                    (HandleLookup *)((void **)D_00038890)[pair.mHandle & 0xFFFF];
                found = 0;
                if (cand != 0) {
                    if (cand->handle == pair.mHandle) {
                        found = cand;
                    }
                }
            }

            cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >
                *selected;
            if (found != 0) {
                sub.mIndex = pair.mSubHandle;
                register cHandlePairT<gcEnumeration,
                                      cSubHandleT<gcEnumerationEntry> > *out
                    __asm__("$18") = &newPair;
                register int evalIndex __asm__("$8") = index;
                __asm__ volatile("" : "+r"(evalIndex));
                ((gcEnumeration *)found)->GetSubObjectHandle(out, &sub,
                                                             evalIndex);
                selected = out;
            } else {
                zeroPair.mHandle = 0;
                selected = &zeroPair;
                zeroPair.mSubHandle = 0;
            }

            __asm__ volatile("" : "+r"(selected));
            copy.mHandle = selected->mHandle;
            copy.mSubHandle = selected->mSubHandle;
            pair.mHandle = copy.mHandle;
            pair.mSubHandle = copy.mSubHandle;
        }
    } else {
        pair.mHandle = 0;
        unused = 0;
        pair.mSubHandle = 0;
    }

    char *slot = events + (*(int *)((const char *)self + 0x24) * 8) + 0x58;
    *(int *)slot = pair.mHandle;
    *(int *)(slot + 4) = pair.mSubHandle;
    return one;
}
