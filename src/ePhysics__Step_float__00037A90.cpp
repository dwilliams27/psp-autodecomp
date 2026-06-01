// ePhysics::Step(float) @ 0x00037a90 (eAll_psp.obj)
// Split-TU local declarations.

class eConstraintSolver {
public:
    int m_first;
    char m_pad[240000];
    void PreSolve(void);
    int Solve(int);
};

class ePhysics {
public:
    int m_unk0;
    int m_iterations;       // offset 4
    char m_pad[0x48 - 8];
    eConstraintSolver *m_solver; // offset 0x48
    void Step(float dt);
    void CheckForCollision(eConstraintSolver *solver);
    void ApplyExternalForcesAndIntegrateVelocity(float dt);
    void ClearExternalForcesAndIntegrateLocalToWorld(float dt);
    void CheckForSleeping(void);
};

void ePhysics::Step(float dt)
{
    eConstraintSolver solver;
    solver.m_first = 0;
    m_solver = &solver;
    CheckForCollision(&solver);
    ApplyExternalForcesAndIntegrateVelocity(dt);
    solver.PreSolve();
    solver.Solve(m_iterations);
    ClearExternalForcesAndIntegrateLocalToWorld(dt);
    CheckForSleeping();
    m_solver = 0;
}
