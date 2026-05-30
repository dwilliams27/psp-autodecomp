// eSavedContact::eSavedContact(void) @ 0x00208928
// Zeroes members at offsets 32 and 36, returns this.

class eSavedContact {
public:
    char pad[32];
    int m20;
    int m24;
    eSavedContact();
};

eSavedContact::eSavedContact() {
    m20 = 0;
    m24 = 0;
}
