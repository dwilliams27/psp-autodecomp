// gcDoEntityFindAttached::Iterate(gcEvent::gcLoop &, gcEntity *, gcEntity *) const
// 0x002b0c28, 84B, gcAll_psp.obj  — leaf
//
// Reads this->[0xC] (gating flag). If zero, store the current entity into the
// loop's indexed slot and return 0. Otherwise, if the two entities differ do
// the same store/return-0; if they are equal return 1 (stop).
//
// gcLoop& first member (offset 0) is an array base; this->[0x64] indexes it
// (element size 4 -> the array holds gcEntity* / 4-byte slots). Element field
// at +0x40 receives the entity pointer.

class gcEntity;

namespace gcEvent {
    class gcLoop {
    public:
        gcEntity **mEntries;   // offset 0
    };
}

class gcDoEntityFindAttached {
public:
    char _padToC[0xC];
    int mFieldC;               // 0x0C
    char _padCto64[0x54];
    int mIndex;                // 0x64
    int Iterate(gcEvent::gcLoop &, gcEntity *, gcEntity *) const;
};

int gcDoEntityFindAttached::Iterate(gcEvent::gcLoop &loop, gcEntity *entity,
                                    gcEntity *other) const {
    if (mFieldC == 0) {
        *(gcEntity **)((char *)(loop.mEntries + mIndex) + 0x40) = entity;
        return 0;
    }
    if (entity != other) {
        *(gcEntity **)((char *)(loop.mEntries + mIndex) + 0x40) = entity;
        return 0;
    }
    return 1;
}
