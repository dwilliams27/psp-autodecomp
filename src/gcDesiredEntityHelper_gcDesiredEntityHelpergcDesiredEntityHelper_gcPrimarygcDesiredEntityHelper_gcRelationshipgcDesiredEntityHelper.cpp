// Combined source for several small leaf functions.
// - gcDesiredEntityHelper ctor / Validate (gcAll_psp.obj)
// - std::exception::what / std::bad_exception::what (gMain_psp.obj)
// - mem_block_descr_init (gMain_psp.obj)

class gcEntity;

class gcDesiredEntityHelper {
public:
    enum gcPrimary { gcPrimary_None };
    enum gcRelationship { gcRelationship_None };

    char primary;
    char rel1;
    char rel2;
    char pad;
    int field4;

    gcDesiredEntityHelper(gcPrimary p, gcRelationship r1, gcRelationship r2);
    gcEntity* Validate(gcEntity* e) const;
};

gcDesiredEntityHelper::gcDesiredEntityHelper(gcPrimary p, gcRelationship r1, gcRelationship r2) {
    primary = (char)p;
    rel1 = (char)r1;
    rel2 = (char)r2;
    field4 = 0;
}

gcEntity* gcDesiredEntityHelper::Validate(gcEntity* e) const {
    if (e == 0) return 0;
    return e;
}

// ---- std::exception::what / std::bad_exception::what ----
//
// Disassembly is `g = g; return "exception";` for some int at 0x37e54c.
// Compiler keeps the dead self-assignment so the global must be volatile-
// qualified (or accessed through a volatile-qualified expression).

extern volatile int __exception_ptr;
extern const char _exception_str[];

namespace std {
    class exception {
    public:
        virtual const char* what() const;
    };
    class bad_exception : public exception {
    public:
        virtual const char* what() const;
    };
}

const char* std::exception::what() const {
    __exception_ptr = __exception_ptr;
    return _exception_str;
}

const char* std::bad_exception::what() const {
    __exception_ptr = __exception_ptr;
    return _exception_str;
}

// ---- mem_block_descr_init ----

struct a_mem_block_descr {
    int field0;
    int field4;
    int field8;
    int fieldC;
    char field10;
};

void mem_block_descr_init(struct a_mem_block_descr* d) {
    d->field0 = 0;
    d->field4 = 0;
    d->field8 = 0;
    d->fieldC = 0;
    d->field10 = 0;
}
