struct eCamera;
struct mOCS;
struct mVec2;

struct gcUIControl {
    void OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &, float, float, float, unsigned int);
};

void gcUIControl::OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &, float, float, float, unsigned int) {
}
