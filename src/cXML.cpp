// cXML small accessor methods.
// Each calls cXML::Find(name, false) and inspects the returned element.

class cXML {
public:
    class cElement {
    public:
        int     mType;       // +0x00: 0=element, 1=attribute, 2=text, ...
        int     mPad04;      // +0x04
        const char *mText;   // +0x08
        int     mPad0C;      // +0x0C
        int     mPad10;      // +0x10
        int     mPad14;      // +0x14
        int     mPad18;      // +0x18
        int     mPad1C;      // +0x1C
        cElement **mChildren;// +0x20: pointer to dynamic array; size word at [-4]
    };

    cElement *Find(const char *name, bool createIfMissing);
    const char *GetText(const char *name) const;
    int         NumElements(const char *name) const;
    bool        HasAttribute(const char *name) const;
};

// -----------------------------------------------------------------------------
// cXML::GetText(const char *) const
// -----------------------------------------------------------------------------
const char *cXML::GetText(const char *name) const {
    cElement *elem = const_cast<cXML *>(this)->Find(name, false);
    if (elem == 0) return 0;
    return elem->mText ? elem->mText : "";
}

// -----------------------------------------------------------------------------
// cXML::NumElements(const char *) const
// -----------------------------------------------------------------------------
int cXML::NumElements(const char *name) const {
    cElement *elem = const_cast<cXML *>(this)->Find(name, false);
    if (elem == 0 || elem->mType != 0) {
        return 0;
    }
    cElement **arr = elem->mChildren;
    int count;
    if (arr == 0) {
        count = 1;
    } else {
        count = (((int *)arr)[-1] & 0x3FFFFFFF) + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// cXML::HasAttribute(const char *) const
// -----------------------------------------------------------------------------
bool cXML::HasAttribute(const char *name) const {
    cElement *elem = const_cast<cXML *>(this)->Find(name, false);
    if (elem != 0 && elem->mType == 1) {
        return true;
    }
    return false;
}
