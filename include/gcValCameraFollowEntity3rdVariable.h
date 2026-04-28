#ifndef GCVALCAMERAFOLLOWENTITY3RDVARIABLE_H
#define GCVALCAMERAFOLLOWENTITY3RDVARIABLE_H

class cBase;
class cFile;
class cMemPool;

class cType;

struct gcValCFE3V_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcValCFE3V_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValCFE3V_cMemPoolNS {
public:
    static gcValCFE3V_cMemPoolNS *GetPoolFromPtr(const void *);
};

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

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcValCFE3V_cMemPoolNS *pool = gcValCFE3V_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcValCFE3V_DeleteRecord *rec = (gcValCFE3V_DeleteRecord *)
            (((gcValCFE3V_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

#endif
