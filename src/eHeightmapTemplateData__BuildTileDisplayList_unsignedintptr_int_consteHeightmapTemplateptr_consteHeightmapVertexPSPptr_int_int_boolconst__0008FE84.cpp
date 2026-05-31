// eHeightmapTemplateData::BuildTileDisplayList(unsigned int *, int, const eHeightmapTemplate *,
//   const eHeightmapVertexPSP *, int, int, bool) const  @ 0x0008fe84
// Split-TU: classes declared locally (no eHeightmapTemplate header exists).

#pragma control sched=1

class eHeightmapVertexPSP {
public:
    char v30402[0x12C];
};

class eHeightmapTemplate {
public:
    char pad[0x6C];
    int  unk6C;
};

class eHeightmapTemplateData {
public:
    void BuildTileDisplayList(unsigned int *cmds, int unused, const eHeightmapTemplate *tmpl,
                              const eHeightmapVertexPSP *verts, int row, int col, bool flag) const;
};

void eHeightmapTemplateData::BuildTileDisplayList(unsigned int *cmds, int /*unused*/,
                                                  const eHeightmapTemplate *tmpl,
                                                  const eHeightmapVertexPSP *verts,
                                                  int row, int col, bool flag) const {
    __asm__ volatile("" ::: "memory");
    int idx = row + col * tmpl->unk6C;
    unsigned int idxAddr = (unsigned int)&verts[idx];

    if (flag) {
        *cmds++ = 0x12000916;
    }

    extern char gHeightmapTileVtx[];
    unsigned int vtxAddr = (unsigned int)&gHeightmapTileVtx;
    __asm__ volatile("" ::: "memory");
    *cmds++ = (((vtxAddr >> 0x18) & 0xF) << 0x10) | 0x10000000;
    int _tmp_616 = (vtxAddr & 0xFFFFFF) | 0x02000000;
    *cmds++ = _tmp_616;

    *cmds++ = (((idxAddr >> 0x18) & 0xF) << 0x10) | 0x10000000;
    *cmds++ = (idxAddr & 0xFFFFFF) | 0x01000000;

    int i = 0;
    do {
        *cmds++ = 0x0404000A;
        i++;
    } while (i < 4);
}
