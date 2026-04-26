class eRigidBodyState {
public:
    void Update(void);
    void ReduceVolume(void);
};

void eRigidBodyState::ReduceVolume(void) {
    Update();
}
