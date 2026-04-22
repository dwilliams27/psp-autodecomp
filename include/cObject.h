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
    void SetDirty(void);
    void GetExternalDependency(int, cFilename *) const;
    int GetNumExternalDependencies(void) const;
    int GetUserData(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    cObject &operator=(const cObject &);
    void Copy(cMemPool *, cBase *) const;
    void Write(cFile &) const;
    static int GetNameFromGUID(const cGUID &, const cType *, bool, bool, cName *);
};

class cFactory {
public:
    int AllowParentCreate(void) const;
    int GetContainedFactories(int *count);
    void CopyObject(const cObject *, const cGUID &);
};

#endif
