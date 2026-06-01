// gcDesiredObjectHelper::GetDesiredObject(gcPrimary, cHandle, const cType *, bool) static
// Address: 0x0012512c, Size: 156B
// Obj: gcAll_psp.obj
//
// Structure (from disasm):
//   obj = GetDesiredObject(primary, handle);   // 2-arg static
//   if (obj) {
//       e = obj->m4;                            // type descriptor
//       list = e[8].fn((char*)obj + e[8].offset);
//       found = false;
//       if (type) {
//           for (n = list; n; n = n->m1C) if (n == type) { found = true; break; }
//       }
//       if (!found) obj = 0;
//   }
//   return obj;

class cType;

class cHandle {
public:
    int mIndex;
};

class gcDesiredObjectHelper {
public:
    enum gcPrimary {};
    static void *GetDesiredObject(gcPrimary, cHandle);
    static void *GetDesiredObject(gcPrimary, cHandle, const cType *, bool);
};

struct TypeEntry {
    short offset;
    short _pad;
    void *(*fn)(void *);
};

void *gcDesiredObjectHelper::GetDesiredObject(gcPrimary primary, cHandle handle,
                                             const cType *type, bool flag) {
    void *obj = GetDesiredObject(primary, handle);
    if (obj) {
        TypeEntry *e = (TypeEntry *)(*(char **)((char *)obj + 4) + 8);
        void *node = e->fn((char *)obj + e->offset);
        int found;
        if (!type) {
            found = 0;
        } else {
            while (node) {
                if (node == (void *)type) {
                    found = 1;
                    goto checked;
                }
                node = *(void **)((char *)node + 0x1c);
            }
            found = 0;
        checked:;
        }
        if (!found) {
            obj = 0;
        }
    }
    return obj;
}
