// gcEntity::AttachToParent(gcEntity *, const cName &, bool)
// Address: 0x00117ae0, Size: 180B
// Obj: gcAll_psp.obj
// mangled: __0fIgcEntityOAttachToParentP6IgcEntityRC6FcNameb
//
// Resolves a cName to a sub-object index, then forwards to the int-index
// overload AttachToParent(gcEntity*, int, bool).
//
// If the supplied cName has an empty token (short at +0x14 == 0), the entity's
// own enumeration key (this->[0x44]) is chased through the global enum table
// (D_00038890) to obtain a fallback name object at node+0x98. That name is then
// passed to eDynamicModel::GetSubObjectIndex on the parent's model subobject
// (parent+0x80) to get the index. An empty resolved name yields index -1.

class cName {
public:
    char _pad14[0x14];
    short mToken;  // 0x14
    bool IsNull() const { return mToken == 0; }
};

struct eDynamicModel {
    int GetSubObjectIndex(const cName &, int) const;
};

// global enum-node pointer table at 0x38890
extern void *D_00038890[];

class gcEntity {
public:
    char _padTo44[0x44];
    int mEnumKey;  // 0x44

    void AttachToParent(gcEntity *, const cName &, bool);
    void AttachToParent(gcEntity *, int, bool);
};

void gcEntity::AttachToParent(gcEntity *parent, const cName &name, bool flag) {
    short index = -1;
    const cName *resolved = &name;

    if (name.IsNull()) {
        char *node = 0;
        int key = mEnumKey;
        if (key != 0) {
            node = (char *)D_00038890[key & 0xFFFF];
        }
        resolved = (const cName *)(node + 0x98);
    }

    if (!resolved->IsNull()) {
        index = (short)((eDynamicModel *)((char *)parent + 0x80))->GetSubObjectIndex(*resolved, 0);
    }

    AttachToParent(parent, index, flag);
}
