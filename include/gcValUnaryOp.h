#ifndef GCVALUNARYOP_H
#define GCVALUNARYOP_H

class cBase;
class cMemPool;
class cType;
class gcExpression;

struct gcValUnaryOp_PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct gcValUnaryOp_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValUnaryOp_cMemPoolNS {
public:
    static gcValUnaryOp_cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcValUnaryOp {
public:
    gcValUnaryOp(cBase *parent);
    ~gcValUnaryOp(void);
    static void operator delete(void *p) {
        gcValUnaryOp_cMemPoolNS *pool =
            gcValUnaryOp_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcValUnaryOp_DeleteRecord *rec =
            (gcValUnaryOp_DeleteRecord *)(((gcValUnaryOp_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    int GetExprFlags(void) const;
    void GetText(char *) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
