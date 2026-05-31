// eHeightmapShape::GetAABB(mBox *, const mOCS &) const — 0x000507dc
// Body is purely 3x (vpfxs prefix + vmov.t C120,C000), then jr ra / nop.
// No loads, no stores. The AABB stores were optimized out (heightmap has no
// finite bound), leaving only the basis-vector materialization into C120.
class mBox;
class mOCS;

class eHeightmapShape {
public:
    void GetAABB(mBox *, const mOCS &) const;
};

void eHeightmapShape::GetAABB(mBox *box, const mOCS &ocs) const {
    __asm__ volatile(
        ".word 0xdc0070c1\n"  // vpfxs 1, 0, 0, W
        "vmov.t C120, C000\n"
        ".word 0xdc0070c4\n"  // vpfxs 0, 1, 0, W
        "vmov.t C120, C000\n"
        ".word 0xdc0070d0\n"  // vpfxs 0, 0, 1, W
        "vmov.t C120, C000\n"
    );
}
