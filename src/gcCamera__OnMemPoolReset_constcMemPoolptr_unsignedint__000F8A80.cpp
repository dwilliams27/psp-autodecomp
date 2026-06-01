// gcCamera::OnMemPoolReset(const cMemPool *, unsigned int)  @ 0x000f8a80
// Symbol: __0fIgcCameraOOnMemPoolResetPC6IcMemPoolUi

class cMemPool;
class cBase;

class cObject {
public:
    static bool WillBeDeleted(const cBase *, const cMemPool *, unsigned int);
};

class gcUI {
public:
    void OnMemPoolReset(const cMemPool *, unsigned int);
};

class gcCamera {
public:
    void OnMemPoolReset(const cMemPool *pool, unsigned int id);
};

// layout: cBase* at +4392, gcUI sub-object at +4580
struct gcCameraLayout {
    char pad0[4392];
    cBase *m_obj;       // +4392
    char pad1[4580 - 4392 - 4];
    gcUI m_ui;          // +4580
};

void gcCamera::OnMemPoolReset(const cMemPool *pool, unsigned int id) {
    gcCameraLayout *self = reinterpret_cast<gcCameraLayout *>(this);
    if (cObject::WillBeDeleted(self->m_obj, pool, id)) {
        self->m_obj = 0;
    }
    self->m_ui.OnMemPoolReset(pool, id);
}
