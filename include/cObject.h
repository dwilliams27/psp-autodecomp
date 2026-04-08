#ifndef COBJECT_H
#define COBJECT_H

class cFilename;

class cObject {
public:
    void SetDirty(void);
    void GetExternalDependency(int, cFilename *) const;
    int GetNumExternalDependencies(void) const;
    int GetUserData(void) const;
};

class cFactory {
public:
    int AllowParentCreate(void) const;
};

#endif
