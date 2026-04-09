struct gcAction {
    char pad[8];
    unsigned int mNext;

    gcAction *GetNext(void) const;
};

gcAction *gcAction::GetNext(void) const {
    return (gcAction *)(mNext & ~3u);
}
