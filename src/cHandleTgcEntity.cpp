class gcEntity;
class eSprite;

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT();
};

template <class T>
cHandleT<T>::cHandleT() : mIndex(0) {
}

template cHandleT<gcEntity>::cHandleT();

class nwConnectionHandle {
public:
    int mHandle;
    nwConnectionHandle();
};

nwConnectionHandle::nwConnectionHandle() : mHandle(0) {
}

class gcUITextControl {
public:
    int GetCursor(cHandleT<eSprite> *) const;
};

int gcUITextControl::GetCursor(cHandleT<eSprite> *out) const {
    out->mIndex = 0;
    return -1;
}

struct cGUID {
    int a;
    int b;
};

class gcTableColumn {
public:
    void Get(int, cGUID *) const;
};

void gcTableColumn::Get(int, cGUID *guid) const {
    guid->a = 0;
    guid->b = 0;
}

class gcExpression {};

class gcAction : public gcExpression {
public:
    float Evaluate(void) const;
};

float gcAction::Evaluate(void) const {
    return 1.0f;
}
