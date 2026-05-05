class cTimeValue {
public:
    int value;
};

class gcEntity;

template <class T>
class cHandleT {
public:
    int handle;
};

class gcCamera {
public:
    class gcState {
    public:
        char pad[0xA0];
        cHandleT<gcEntity> followEntity;
    };

    void Follow(cTimeValue, cHandleT<gcEntity>);
};

class gcCameraModes {
public:
    static void OnEntitySnappedFollow(gcCamera &, gcCamera::gcState &);
};

void gcCameraModes::OnEntitySnappedFollow(gcCamera &camera, gcCamera::gcState &state) {
    camera.Follow(cTimeValue(), state.followEntity);
}
