#ifndef GCREGION_H
#define GCREGION_H

class gcEnumeration;
class gcEnumerationEntry;
class cObject;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    T mHandle;
    U mSubHandle;
};

class cBase;
class cFile;
class cMemPool;
class gcReplicationVisitor;

class gcRegion {
public:
    ~gcRegion(void);
    int Read(cFile &, cMemPool *);
    int RemoveFromWorld(void);
    const void *GetType(void) const;
    int *GetGroups(int *);
    int AllowParentCreate(void) const;
    void MemCardReplicate(gcReplicationVisitor &);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcMap {
public:
    int GetUserData(void) const;

    char pad[0x13C];
    int mUserData;
};

class gcDesiredObject {
public:
    int HasRelationship(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &, bool) const;
    int HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const;
    int GetRelationships(cHandleT<gcEnumeration>, int, cObject **) const;
};

#endif
