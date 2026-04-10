class eHeightmapTemplate {
public:
    char pad[0x58];
    short mField58;
    int GetNumExternalDependencies(void) const;
};

int eHeightmapTemplate::GetNumExternalDependencies(void) const {
    return ((mField58 == 0) & 0xFF) == 0;
}
