#ifndef GCVALCAMERAVALUE_H
#define GCVALCAMERAVALUE_H

class cBase;
class cType;
class cFile;
class cMemPool;

struct gcValCameraValue_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcValCameraValue_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValCameraValue_cMemPoolNS {
public:
    static gcValCameraValue_cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcValCameraValue {
public:
    ~gcValCameraValue(void);
    static cBase *New(cMemPool *, cBase *);
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    void Write(cFile &) const;
    float Evaluate(void) const;
    const cType *GetType(void) const;

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcValCameraValue_cMemPoolNS *pool = gcValCameraValue_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcValCameraValue_DeleteRecord *rec = (gcValCameraValue_DeleteRecord *)
            (((gcValCameraValue_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

#endif
