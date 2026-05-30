// eMovie::Open(const char *) @ 0x00056e44
// Tail-forwards to the embedded eMoviePlatform member (at offset 8).
// Symbol: __0fGeMovieEOpenPCc

class eMoviePlatform {
public:
    void Open(const char *path);
};

class eMovie {
public:
    void Open(const char *path);
private:
    int m_pad0;                // offset 0..3
    int m_pad1;                // offset 4..7
    eMoviePlatform m_platform; // offset 8
};

void eMovie::Open(const char *path) {
    m_platform.Open(path);
}
