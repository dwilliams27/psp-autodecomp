class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cListSubscriber {
public:
    void Write(cFile &) const;
};

void cListSubscriber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}
