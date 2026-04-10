#ifndef ERENDERTARGET_H
#define ERENDERTARGET_H

class cBase;
class cFile;
class cMemPool;
class eCamera;
class eWorld;

class eTexture {
public:
    char _pad[0x4C];
    eTexture(cBase *);
    void Write(cFile &) const;
};

class eRenderTarget : public eTexture {
public:
    bool field_4C;
    bool field_4D;
    bool field_4E;
    bool field_4F;
    void *field_50;
    int field_54;
    int field_58;

    eRenderTarget(cBase *);
    void Write(cFile &) const;
    void Apply(int, const eCamera *, const eWorld *) const;

    static int Initialize(void);
    static void Uninitialize(void);
    static void Flip(void);
    static eRenderTarget *New(cMemPool *, cBase *);
};

#endif
