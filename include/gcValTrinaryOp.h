#ifndef GCVALTRINARYOP_H
#define GCVALTRINARYOP_H

class cBase;
class cFile;
class cMemPool;
class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};
class cType;
class gcExpression;

struct gcValTrinaryOpDeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct gcValTrinaryOpData {
    char _pad[0x08];
    int mField08;
    int mOp;
    gcExpression **mChildren;
};

class gcValTrinaryOp {
public:
    gcValTrinaryOp(cBase *);
    ~gcValTrinaryOp(void);
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcValTrinaryOpDeleteRec *rec =
            (gcValTrinaryOpDeleteRec *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    int GetExprFlags(void) const;
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
