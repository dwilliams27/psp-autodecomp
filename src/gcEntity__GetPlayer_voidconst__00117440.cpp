// gcEntity::GetPlayer(void) const
// Address: 0x00117440, Size: 88B
// Obj: gcAll_psp.obj
// mangled: __0fIgcEntityJGetPlayervK

class gcPlayer;

class gcEntity {
public:
    gcPlayer *GetPlayer() const;
};

extern char *g_playerTable;

gcPlayer *gcEntity::GetPlayer() const {
    const gcEntity *p = this;
    if (p == 0) {
        return 0;
    }
    signed char idx;
top:
    idx = *((const signed char *)p + 103);
    if (idx >= 0) {
        gcPlayer *result = 0;
        if (idx >= 0 && idx < 8) {
            result = (gcPlayer *)(g_playerTable + idx * 0x44);
        }
        return result;
    }
    p = *(const gcEntity *const *)((const char *)p + 536);
    if (p != 0) {
        goto top;
    }
    return 0;
}
