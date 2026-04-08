class cFactory {
public:
};

class gcLoadingScreen : public cFactory {
public:
    void *GetGroups(int *);
};

void *gcLoadingScreen::GetGroups(int *count) {
    *count = 0x14;
    return (char *)this + 0x50;
}
