// gcDesiredObjectT<...>::GetObject thunks + a couple of unrelated leaf
// runtime/utility functions that the orchestrator routed into this file.

class cBase;
class gcDesiredCustomAttack;
class gcDesiredCustomAttackHelper;
class gcEntityCustomAttack;
class gcDesiredPath;
class gcDesiredPathHelper;
class ePath;

template <class T1, class T2, class T3>
class gcDesiredObjectT {
public:
    T2 *Get(bool) const;
    cBase *GetObject(bool) const;
};

template <class T1, class T2, class T3>
cBase *gcDesiredObjectT<T1, T2, T3>::GetObject(bool b) const {
    return (cBase *)Get(b);
}

template class gcDesiredObjectT<gcDesiredCustomAttack,
                                gcDesiredCustomAttackHelper,
                                gcEntityCustomAttack>;
template class gcDesiredObjectT<gcDesiredPath,
                                gcDesiredPathHelper,
                                ePath>;

// ------------------------------------------------------------------
// cStr::Write(cWriteBlock &) const
// ------------------------------------------------------------------

class cWriteBlock {
public:
    int _data[2];
    void Write(int, const char *);
};

class cStr {
public:
    char _data[256];
    operator const char *() const { return _data; }
    void Write(cWriteBlock &) const;
};

void cStr::Write(cWriteBlock &wb) const {
    wb.Write(0x100, *this);
}

// ------------------------------------------------------------------
// C-runtime shims
// ------------------------------------------------------------------

extern "C" {
struct _reent;
int fclose(void *);
void _fwalk(struct _reent *, int (*)(void *));
void abort(void);

void _cleanup_r(struct _reent *r) {
    _fwalk(r, fclose);
}

void _pure_error_(void) {
    abort();
    while (1) {}
}
}
