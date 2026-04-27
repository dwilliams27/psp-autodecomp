class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcExpression {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValue : public gcExpression {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcLValue : public gcValue {
public:
    void Write(cFile &) const;
};

// ── gcValue::Write(cFile &) const @ 0x0011b244 ──
void gcValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcExpression::Write(file);
    wb.End();
}

// ── gcValue::Read(cFile &, cMemPool *) @ 0x0011b290 ──
int gcValue::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcExpression::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcLValue::Write(cFile &) const @ 0x0011b34c ──
void gcLValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcValue::Write(file);
    wb.End();
}
