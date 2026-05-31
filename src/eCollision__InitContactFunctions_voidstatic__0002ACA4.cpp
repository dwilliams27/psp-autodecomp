// eCollision::InitContactFunctions(void) static
// Address: 0x0002aca4, Size: 240 bytes
// Obj: eAll_psp.obj
//
// memset a 0x64-byte global contact-function table, then populate a set of
// function-pointer slots at specific offsets. The pointer targets are
// relocations (masked by compare_func), so what matters is the
// lui/addiu/sw instruction sequence and the destination offsets.

extern "C" void *memset(void *dst, int c, unsigned int n);

typedef void (*ContactFn)();

// External contact-handler symbols referenced by the table.
extern "C" {
    void cf_00();  void cf_04();  void cf_08();  void cf_0C();  void cf_10();
    void cf_18();  void cf_1C();  void cf_20();  void cf_24();
    void cf_30();  void cf_34();  void cf_38();
    void cf_48();  void cf_4C();
    void cf_60();
}

struct eCollisionContactTable {
    ContactFn f00;   // 0x00
    ContactFn f04;   // 0x04
    ContactFn f08;   // 0x08
    ContactFn f0C;   // 0x0C
    ContactFn f10;   // 0x10
    ContactFn f14;   // 0x14
    ContactFn f18;   // 0x18
    ContactFn f1C;   // 0x1C
    ContactFn f20;   // 0x20
    ContactFn f24;   // 0x24
    ContactFn f28;   // 0x28
    ContactFn f2C;   // 0x2C
    ContactFn f30;   // 0x30
    ContactFn f34;   // 0x34
    ContactFn f38;   // 0x38
    ContactFn f3C;   // 0x3C
    ContactFn f40;   // 0x40
    ContactFn f44;   // 0x44
    ContactFn f48;   // 0x48
    ContactFn f4C;   // 0x4C
    ContactFn f50;   // 0x50
    ContactFn f54;   // 0x54
    ContactFn f58;   // 0x58
    ContactFn f5C;   // 0x5C
    ContactFn f60;   // 0x60
};

static eCollisionContactTable g_contactTable;

class eCollision {
public:
    static void InitContactFunctions();
};

void eCollision::InitContactFunctions()
{
    memset(&g_contactTable, 0, 0x64);
    g_contactTable.f00 = cf_00;
    g_contactTable.f04 = cf_04;
    g_contactTable.f08 = cf_08;
    g_contactTable.f0C = cf_0C;
    g_contactTable.f10 = cf_10;
    g_contactTable.f18 = cf_18;
    g_contactTable.f1C = cf_1C;
    g_contactTable.f20 = cf_20;
    g_contactTable.f24 = cf_24;
    g_contactTable.f30 = cf_30;
    g_contactTable.f34 = cf_34;
    g_contactTable.f38 = cf_38;
    g_contactTable.f48 = cf_48;
    g_contactTable.f4C = cf_4C;
    g_contactTable.f60 = cf_60;
}
