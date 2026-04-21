#ifndef GCVALCAMERAFOLLOWENTITY3RDVARIABLE_H
#define GCVALCAMERAFOLLOWENTITY3RDVARIABLE_H

class cBase;
class cFile;
class cMemPool;

class gcValCameraFollowEntity3rdVariable {
public:
    ~gcValCameraFollowEntity3rdVariable(void);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void GetText(char *) const;
};

#endif
