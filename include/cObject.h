#ifndef COBJECT_H
#define COBJECT_H

class cBase;
class cFilename;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    void SetDirty(void);
    void GetExternalDependency(int, cFilename *) const;
    int GetNumExternalDependencies(void) const;
    int GetUserData(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class cFactory {
public:
    int AllowParentCreate(void) const;
    int GetContainedFactories(int *count);
};

#endif
