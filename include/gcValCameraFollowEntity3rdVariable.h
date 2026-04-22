#ifndef GCVALCAMERAFOLLOWENTITY3RDVARIABLE_H
#define GCVALCAMERAFOLLOWENTITY3RDVARIABLE_H

class cBase;
class cFile;
class cMemPool;

class cType;

class gcValCameraFollowEntity3rdVariable {
public:
    ~gcValCameraFollowEntity3rdVariable(void);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    float Evaluate(void) const;
    void Set(float);
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

#endif
