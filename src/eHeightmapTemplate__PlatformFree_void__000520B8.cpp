// eHeightmapTemplate::PlatformFree @ 0x000520b8

extern "C" void eHeightmapTemplateData___dtor_eHeightmapTemplateData_void(void *, int);

class cBase;
class cObject {
public:
    virtual ~cObject(void);
};

class eHeightmapTemplate : public cObject {
public:
    void PlatformFree(void);
};

#pragma control sched=1
void eHeightmapTemplate::PlatformFree(void) {
    void *data = *(void **)((char *)this + 0x80);
    if (data != 0) {
        eHeightmapTemplateData___dtor_eHeightmapTemplateData_void(data, 3);
        *(void **)((char *)this + 0x80) = 0;
    }
}
