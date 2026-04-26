class cFile;
class eVolume;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End();
};

class eRoom {
public:
    void RemoveVolume(eVolume *);
    void ClearRoomVolumeList(eVolume *);
};

class eVolumeBody {
public:
    char _pad[0x24];
    int mField24;
};

class gcUI {
public:
    void Write(cFile &) const;
};

class gcViewport {
public:
    void Write(cFile &) const;
};

void eRoom::RemoveVolume(eVolume *volume) {
    ClearRoomVolumeList(volume);
    ((eVolumeBody *)volume)->mField24 = 0;
}

void gcUI::Write(cFile &f) const {
    cWriteBlock wb(f, 1);
    wb.End();
}

void gcViewport::Write(cFile &f) const {
    cWriteBlock wb(f, 1);
    wb.End();
}

extern "C" {

extern void *_impure_ptr;
void *_fopen_r(void *, const char *, const char *);
double _strtod_r(void *, const char *, char **);

void *fopen(const char *file, const char *mode) {
    return _fopen_r(_impure_ptr, file, mode);
}

double strtod(const char *s, char **endptr) {
    return _strtod_r(_impure_ptr, s, endptr);
}

}
