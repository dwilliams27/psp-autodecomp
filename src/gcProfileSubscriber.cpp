// Batch of unrelated functions grouped by the orchestrator into one TU.
// Sections are independent.

class cBase;

template <class T> T *dcast(const cBase *);

class cListSubscriber {
public:
    void Attach(void);
    void Detach(void);
};

class gcGameSettings {
public:
    enum gcSubscription {};
    static gcGameSettings *Get(void);
    void Subscribe(gcSubscription, cListSubscriber *);
    void Unsubscribe(gcSubscription, cListSubscriber *);
};

// ── gcProfileSubscriber::Attach(void) @ 0x00287B78 ──
class gcProfileSubscriber {
public:
    void Attach(void);
};

void gcProfileSubscriber::Attach(void) {
    ((cListSubscriber *)this)->Attach();
    gcGameSettings::Get()->Subscribe(
        (gcGameSettings::gcSubscription)1, (cListSubscriber *)this);
}

// ── gcSaveGameSubscriber::Detach(void) @ 0x00288640 ──
class gcSaveGameSubscriber {
public:
    void Detach(void);
};

void gcSaveGameSubscriber::Detach(void) {
    gcGameSettings::Get()->Unsubscribe(
        (gcGameSettings::gcSubscription)0, (cListSubscriber *)this);
    ((cListSubscriber *)this)->Detach();
}

// ── gcValEntityVelocity::AssignCopy(const cBase *) @ 0x00341018 ──
class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityVelocity {
public:
    void AssignCopy(const cBase *);
};

void gcValEntityVelocity::AssignCopy(const cBase *base) {
    gcValEntityVelocity *other = dcast<gcValEntityVelocity>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 12))->operator=(
        *(gcDesiredEntity *)((char *)other + 12));
}

// ── eRoomSet::RemoveFromWorld(eWorld *) @ 0x000400E4 ──
class eRoomSet;
class eWorld;

class eRoomAABBTree {
public:
    void RemoveFromWorld(eWorld *);
};

class eWorld {
public:
    void RemoveRoomSet(eRoomSet *);
};

class eRoomSet {
public:
    void RemoveFromWorld(eWorld *);
};

void eRoomSet::RemoveFromWorld(eWorld *world) {
    ((eRoomAABBTree *)((char *)this + 0x44))->RemoveFromWorld(world);
    world->RemoveRoomSet(this);
}

// ── ePath::ePath(cBase *) @ 0x0005B5A0 ──
class cObject {
public:
    cObject(cBase *);
};

class ePath : public cObject {
public:
    ePath(cBase *);
};

ePath::ePath(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x381F70;
    *(int *)((char *)this + 0x44) = 0;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x48) = 0.0f;
    __asm__ volatile("" : : "r"(this) : "memory");
}
