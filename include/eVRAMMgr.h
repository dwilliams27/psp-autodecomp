#ifndef EVRAMMGR_H
#define EVRAMMGR_H

class eVRAMMgr {
public:
    struct eAllocTag {
        char _pad0[0xC];      // 0x00
        eAllocTag *prev;      // 0x0C
        eAllocTag *next;      // 0x10
    };

    static void EndRender(void);
    static void InvalidateAll(void);
    static void InsertAllocBefore(eAllocTag *node, eAllocTag *pos);
    static void InsertAllocAfter(eAllocTag *node, eAllocTag *pos);
};

class eHeightmapTemplateData {
public:
    eHeightmapTemplateData(void);
};

#endif
