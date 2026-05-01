class eRigidBodyState;

class eSolverRow {
public:
    typedef void (*ApplyFn)(eRigidBodyState *, eRigidBodyState *, const eSolverRow *, float);

    char _pad0[0x98];
    ApplyFn apply;
    char _pad9c[4];

    void WarmStart(void);
    static void ApplyUnembedImpulse(eRigidBodyState *, eRigidBodyState *, const eSolverRow *, float);
};

class eConstraintSolver {
public:
    int rowCount;
    char _pad4[0x0c];
    eSolverRow rows[1];

    void PreSolve(void);
};

#pragma control sched=1
void eConstraintSolver::PreSolve(void) {
    eSolverRow *row = rows;
    int i = 0;
    __asm__ volatile("" ::: "memory");

    if (i < rowCount) {
        do {
            if (row->apply != eSolverRow::ApplyUnembedImpulse) {
                row->WarmStart();
            }
            i++;
            row++;
        } while (i < rowCount);
    }
}
#pragma control sched=2
