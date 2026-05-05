// std::type_info::~type_info(void)  @ 0x001bb2e8, 52B (gMain_psp.obj)
//
// Canonical C++ virtual destructor. SNC's ABI auto-emits:
//   - the (this != NULL) guard
//   - the vtable reset (*this = type_info-vtable) at offset 0
//   - the (in_chrg & 1) deleting-tail dispatch via operator delete
// Mangled symbol: __0o5DstdJtype_infodtv

namespace std {
    class type_info {
    public:
        virtual ~type_info();
    };
}

std::type_info::~type_info() {
}
