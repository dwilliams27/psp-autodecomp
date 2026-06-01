// cGetFileExt(const char *, cStr *) @ 0x000008fc, 196B, cAll_psp.obj
// symbol: __0FLcGetFileExtPCcP6EcStr

class cStr {
public:
    void Set(const char *, ...);
};

int cStrLength(const char *);

cStr *cGetFileExt(const char *path, cStr *out)
{
    if (path == 0 || *path == 0) {
        out->Set(0);
        goto done;
    }

    {
        int i = cStrLength(path);
        for (;;) {
            char c;
            if (i < 0)
                goto null_set;
            c = path[i];
            if (c == '/' || c == '\\') {
null_set:
                out->Set(0);
                return out;
            }
            if (c != '.') {
                --i;
                continue;
            }
            out->Set(&path[i] + 1);
            goto done;
        }
    }
done:
    return out;
}
