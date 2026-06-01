// gcEntityController::OnDeactivated (0x001119cc) — split-TU.
// Mirrors the matched OnActivated/OnDeselected siblings in
// src/gcEntityController.cpp; class declared locally here for isolation.

extern "C" void *__vec_new(void *, int, int, void (*)(void *));

class gcEventParams {
public:
    char _data[0x58];
};

class gcEventStackData {
public:
    char _data[0x1C];
};

class gcEntity {
public:
    enum gcDesiredStateMachine {
        DESIRED_STATE_MACHINE_3 = 3
    };
};

extern "C" void gcEntity_Send_SystemMessage(
    gcEntity *, bool, int, int, const gcEventStackData &, const gcEventParams &)
    __asm__("__0fIgcEntityESendb65IgcEntityi__5IgcEntityVgcDesiredStateMachine65PgcStateMessagesj__5PgcStateMessagesPgcSystemMessageRC6NgcEventParamsRC6QgcEventStackData");

struct EntityActivatedScratch {
    int words0[4];
    int zero10;
    int zero14;
    unsigned char flag18;
    char pad19[3];
    int word1C;
    float floats20[10];
    int word48;
    int word4C;
    int word50;
    int word54;
    int word58;
    int word5C;
    int word60;
    int word64;
    int word68;
    int word6C;
    int word70;
    int vec74[4];
};

class gcEntityController {
public:
    gcEntity *m_entity;   // offset 0
    void ClearCurrentNavMeshPosition(void);
    void OnDeactivated(void);
};

void gcEntityController::OnDeactivated(void) {
    gcEntity *entity = *(gcEntity **)this;
    EntityActivatedScratch scratch;

    scratch.zero10 = 0;
    scratch.zero14 = 0;
    scratch.flag18 = 1;
    scratch.words0[0] = 0;
    scratch.words0[1] = 0;
    scratch.words0[2] = 0;
    scratch.words0[3] = 0;
    scratch.word1C = 0;
    scratch.floats20[0] = 0.0f;
    scratch.word58 = 0;
    __vec_new(scratch.vec74, 2, 8, (void (*)(void *))0x2275F0);
    scratch.floats20[1] = 0.0f;
    scratch.floats20[2] = 0.0f;
    scratch.floats20[3] = 0.0f;
    scratch.floats20[4] = 0.0f;
    scratch.floats20[5] = 0.0f;
    scratch.floats20[6] = 0.0f;
    scratch.floats20[7] = 0.0f;
    scratch.floats20[8] = 0.0f;
    scratch.floats20[9] = 0.0f;
    scratch.word5C = 0;
    scratch.word60 = 0;
    scratch.word64 = 0;
    scratch.word68 = 0;
    scratch.word6C = 0;
    scratch.word70 = 0;
    gcEntity_Send_SystemMessage(entity, true, gcEntity::DESIRED_STATE_MACHINE_3, 0xB,
                                *(gcEventStackData *)&scratch.words0[0],
                                *(gcEventParams *)&scratch.word1C);
    ClearCurrentNavMeshPosition();
}
