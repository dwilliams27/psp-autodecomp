class cWriteBlock {
public:
    void Write(int);
    void Write(int, const float *);
};

class eStaticMeshVisNode {
public:
    void Write(cWriteBlock &) const;
};

class eStaticMeshVisLOD {
public:
    void Write(cWriteBlock &) const;
};

void eStaticMeshVisLOD::Write(cWriteBlock &wb) const {
    wb.Write(*(const int *)((const char *)this + 0));
    wb.Write(*(const int *)((const char *)this + 4));
    wb.Write(*(const int *)((const char *)this + 8));

    const char *nodes0 = *(const char *const *)((const char *)this + 0x0C);
    int nodeCount0 = 0;
    if (nodes0 != 0) {
        nodeCount0 = ((const int *)nodes0)[-1] & 0x3FFFFFFF;
    }
    wb.Write(nodeCount0);

    const char *nodes = *(const char *const *)((const char *)this + 0x0C);
    int nodeCount = 0;
    if (nodes != 0) {
        nodeCount = ((const int *)nodes)[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < nodeCount) {
        int offset = 0;
        const char *node = nodes + offset;
        do {
            ((const eStaticMeshVisNode *)node)->Write(wb);
            i++;
            node += 4;
        } while (i < nodeCount);
    }

    const char *records0 = *(const char *const *)((const char *)this + 0x10);
    int recordCount0 = 0;
    if (records0 != 0) {
        recordCount0 = ((const int *)records0)[-1] & 0x3FFFFFFF;
    }
    wb.Write(recordCount0);

    const char *records = *(const char *const *)((const char *)this + 0x10);
    int recordCount = 0;
    if (records != 0) {
        recordCount = ((const int *)records)[-1] & 0x3FFFFFFF;
    }

    int j = 0;
    if (j < recordCount) {
        int offset = 0;
        const char *record;
        __asm__ volatile("addu %0,%1,%2"
                         : "=r"(record)
                         : "r"(records), "r"(offset));
        do {
            wb.Write(3, (const float *)record);
            wb.Write(3, (const float *)(record + 0x10));
            j++;
            record += 0x20;
        } while (j < recordCount);
    }
}
