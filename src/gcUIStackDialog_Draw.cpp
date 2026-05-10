// ODR-WARNING: this TU intentionally redeclares gcUIStackDialog with a minimal
// member list (just mDialog at +0) so SNC's codegen for Draw doesn't see the
// destructor's full member set or operator-delete from gcUIStackDialog.cpp.
// DO NOT add `#include` for any gcUIStackDialog header. Add new methods to
// the local class only if their bodies are emitted in this TU.

class gcUIDialog {
public:
    void Draw(unsigned int);
};

class eViewport;

class eVideo {
public:
    static void PreFlip(void);
    static void BeginFrame(void);
    static void SetViewport(const eViewport *);
    static void Clear(bool, bool, unsigned int, float, const eViewport *);
    static void EndFrame(void);
    static void Flip(void);
};

class gcUIStackDialog {
public:
    gcUIDialog *mDialog;
    void Draw(void);
};

void gcUIStackDialog::Draw(void) {
    eVideo::PreFlip();
    eVideo::BeginFrame();
    eVideo::SetViewport(0);
    eVideo::Clear(true, true, 0, 1.0f, 0);
    if (mDialog != 0) {
        mDialog->Draw(0);
    }
    eVideo::EndFrame();
    eVideo::Flip();
}
