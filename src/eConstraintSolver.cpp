class eRigidBodyState;

class eSolverRow {
public:
    typedef void (*ApplyFn)(eRigidBodyState *, eRigidBodyState *, const eSolverRow *, float);

    char _pad0[0x98];
    ApplyFn apply;
    char _pad9c[4];

    void WarmStart(void);
    static void ApplyUnembedImpulse(eRigidBodyState *, eRigidBodyState *, const eSolverRow *, float);
    unsigned int Solve(void);
};

class eConstraintSolver {
public:
    int rowCount;
    char _pad4[0x0c];
    eSolverRow rows[1];

    void PreSolve(void);
    void Solve(int);
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

#pragma control sched=1
void eConstraintSolver::Solve(int numIterations) {
    int iter = 0;

    if (iter < numIterations) {
        eSolverRow *start = rows;
        eSolverRow *row;
        int solved;
        int i;
        int cond;
        int count;

        count = rowCount;
        row = start;
loop:
        solved = 0;
        i = 0;
        cond = i < count;

        if (cond != 0) {
            do {
                solved = (solved | row->Solve()) != 0;
                i++;
                __asm__ volatile("" ::: "memory");
                count = rowCount;
                row++;
                cond = i < count;
            } while (cond != 0);
        }
        iter++;
        if (solved != 0) {
            row = start;
            if (iter < numIterations) {
                goto loop;
            }
        }
    }
}
#pragma control sched=2
