// Split-TU: gcDoSwitchCase::GetText is absent from the canonical header, so
// declare a minimal class locally. Do not include the canonical header here.
class gcDoSwitchCase {
public:
    void GetText(char *) const;
};

void cStrCat(char *, const char *);

void gcDoSwitchCase::GetText(char *buf) const {
    cStrCat(buf, (const char *)0x36EF74);
}
