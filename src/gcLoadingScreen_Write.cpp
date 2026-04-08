class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cFactory {
public:
    void Write(cFile &) const;
    void WriteGroups(cWriteBlock &) const;
};

class gcLoadingScreen : public cFactory {
public:
    void Write(cFile &) const;
};

void gcLoadingScreen::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cFactory::Write(file);
    cFactory::WriteGroups(wb);
    ((cHandle *)((char *)this + 0x4C))->Write(wb);
    wb.End();
}
