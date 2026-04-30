#ifndef GCVALCONSOLEINFO_H
#define GCVALCONSOLEINFO_H

class cBase;
class cFile;
class cMemPool;
class cType;

class gcValConsoleInfo {
public:
    gcValConsoleInfo(cBase *);
    int Evaluate(void) const;
    int EvaluateCommon(void) const;
    gcValConsoleInfo &operator=(const gcValConsoleInfo &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;

    int pad0;
    int pad4;
    int pad8;
    int padC;
    int pad10;
    int pad14;
};

#endif
