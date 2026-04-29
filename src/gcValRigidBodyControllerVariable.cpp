class cBase;

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValRigidBodyControllerVariable {
public:
    void AssignCopy(const cBase *);
};

struct SixInts {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
};

template <class T>
T dcast(const cBase *);

void gcValRigidBodyControllerVariable::AssignCopy(const cBase *base) {
    gcValRigidBodyControllerVariable *other = dcast<gcValRigidBodyControllerVariable *>(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 0x34) = *(const int *)((const char *)other + 0x34);
    *(SixInts *)((char *)this + 0x38) = *(const SixInts *)((const char *)other + 0x38);
}
