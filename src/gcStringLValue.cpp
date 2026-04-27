class cFile;
class cMemPool;
class gcStringValue;
void cFile_SetCurrentPos(void *, unsigned int);

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

class gcStringValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcStringLValue : public gcStringValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void Set(const gcStringValue *) const;
};

struct DispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, unsigned short *, int);
};

// ── gcStringLValue::Write(cFile &) const @ 0x00128D0C ──
void gcStringLValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcStringValue::Write(file);
    wb.End();
}

// ── gcStringLValue::Read(cFile &, cMemPool *) @ 0x00128D58 ──
int gcStringLValue::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcStringValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcStringLValue::Set(const gcStringValue *) const @ 0x0026A100 ──
void gcStringLValue::Set(const gcStringValue *src) const {
    unsigned short buf[4096];
    char *src_classdesc = *(char **)((char *)src + 4);
    buf[0] = 0;
    DispatchEntry *get_entry = (DispatchEntry *)(src_classdesc + 0x70);
    get_entry->fn((char *)src + get_entry->offset, buf, 0x1000);

    char *this_classdesc = *(char **)((char *)this + 4);
    DispatchEntry *set_entry = (DispatchEntry *)(this_classdesc + 0x80);
    set_entry->fn((char *)this + set_entry->offset, buf, set_entry->offset);
}
