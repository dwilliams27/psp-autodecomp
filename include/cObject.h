#ifndef COBJECT_H
#define COBJECT_H

class cBase;
class cFile;
class cFilename;
class cGUID;
class cMemPool;
class cName;
class cType;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    const cType *GetType(void) const;
    void SetDirty(void);
    void GetExternalDependency(int, cFilename *) const;
    int GetNumExternalDependencies(void) const;
    int GetUserData(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    cObject &operator=(const cObject &);
    void Copy(cMemPool *, cBase *) const;
    void Write(cFile &) const;
    int Load(void);
    static int GetNameFromGUID(const cGUID &, const cType *, bool, bool, cName *);
};

class cFactory {
public:
    char _pad0[72];         // 0x00
    unsigned int mField48;  // 0x48

    int AllowParentCreate(void) const;
    int GetContainedFactories(int *count);
    void CopyObject(const cObject *, const cGUID &);
    void Write(cFile &) const;
    void Clean(bool flag);
    void LoadLocalized(void);
    void LoadLocalized(const char *);
    int Load(void);
    ~cFactory(void);
    void ClearVisitedReferences(unsigned int);
    void MarkForClean(unsigned int);
    void DeleteMarkedForClean(unsigned int, bool);
};

#endif
