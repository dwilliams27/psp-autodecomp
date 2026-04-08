#ifndef CBASE_H
#define CBASE_H

class cMemPool;
class cFile;

class cBase {
public:
    void SetDirty(void);
    int IsEditable(void) const;
    void Reset(cMemPool *, bool);
    void GetName(char *) const;
    int GetProperties(void) const;
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcExpression {
public:
    int GetMaxChildren(void) const;
};

#endif
