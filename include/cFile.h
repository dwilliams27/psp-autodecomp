#ifndef CFILE_H
#define CFILE_H

class cFile {
public:
    void OnCreated(void);
    void OnClosed(void);
};

class cListSubscriber {
public:
    char pad[0x20];
    int mNumVisible;

    void SetNumVisible(int numVisible);
    bool IsModifiable(void) const;
};

class cLanguage {
public:
    static int PlatformInitialize(void);
};

class cBufferedFile {
public:
    char pad[0x128];
    int mSize;

    int GetSize(void) const;
};

#endif
