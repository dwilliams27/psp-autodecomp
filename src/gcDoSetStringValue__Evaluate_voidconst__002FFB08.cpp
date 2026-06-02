class cBase;
class cType;

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;
};

class gcDoSetStringValue : public gcAction {
public:
    int mValue;
    int mString;

    float Evaluate(void) const;
};

float gcDoSetStringValue::Evaluate(void) const {
    int valV = ((const int *)this)[3];
    int tagV = valV & 1;

    int noChildV = 0;
    if (tagV != 0) {
        noChildV = 1;
    }
    int validV;
    if (noChildV != 0) {
        validV = 1;
    } else {
        validV = (valV == 0) & 0xFF;
        validV = (validV != 0);
    }
    if (validV != 0) {
        return 0.0f;
    }

    int valS = ((const int *)this)[4];
    int tagS = valS & 1;

    int noChildS = 0;
    if (tagS != 0) {
        noChildS = 1;
    }
    int nullS;
    if (noChildS != 0) {
        nullS = 1;
    } else {
        nullS = (valS == 0) & 0xFF;
        nullS = (nullS != 0);
    }

    if (nullS != 0) {
        int flagV = 0;
        if (tagV != 0) {
            flagV = 1;
        }
        const void *childV;
        if (flagV != 0) {
            childV = 0;
        } else {
            childV = (const void *)valV;
        }
        const short *vtV = (const short *)((char *)((void **)childV)[1] + 0x80);
        short adjV = vtV[0];
        float (*fnV)(const void *, const char *) =
            (float (*)(const void *, const char *))((void **)vtV)[1];
        fnV((const char *)childV + adjV, (const char *)0x36DAF4);
    } else {
        int flagV = 0;
        if (tagV != 0) {
            flagV = 1;
        }
        const void *childV;
        if (flagV != 0) {
            childV = 0;
        } else {
            childV = (const void *)valV;
        }
        const short *vtV = (const short *)((char *)((void **)childV)[1] + 0x88);
        short adjV = vtV[0];
        const void *arg0 = (const char *)childV + adjV;

        int flagS = 0;
        if (tagS != 0) {
            flagS = 1;
        }
        const void *childS;
        if (flagS != 0) {
            childS = 0;
        } else {
            childS = (const void *)valS;
        }

        float (*fnV)(const void *, const void *) =
            (float (*)(const void *, const void *))((void **)vtV)[1];
        fnV(arg0, childS);
    }
    return 1.0f;
}
