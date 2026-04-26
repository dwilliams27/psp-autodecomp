class cBase;

extern char eSimulatedMotorclassdesc[];

extern "C" void eSimulatedMotor__eSimulatedMotor_cBaseptr__0006B694(void *, cBase *);

class eSimpleMotor {
public:
    eSimpleMotor(cBase *);
};

eSimpleMotor::eSimpleMotor(cBase *parent) {
    eSimulatedMotor__eSimulatedMotor_cBaseptr__0006B694(this, parent);
    *(void **)((char *)this + 4) = eSimulatedMotorclassdesc;
    *(int *)((char *)this + 0x1C) = 0;
}

struct gcExternalVariableGroup {
    char pad0[8];
    unsigned char unk8;
    char pad9[3];
    int unkC;
};

struct gcRoomInstanceGroup {
    char pad0[8];
    unsigned char unk8;
    char pad9[3];
    int unkC;
};

struct gcValExternalVariableValue {
    int unk8;
};

struct gcValExternalVariable {
    char pad0[8];
    gcValExternalVariableValue value;
};

struct gcValMemCardStatus {
    char pad0[8];
    int unk8;
    int unkC;
};

extern "C" {

gcExternalVariableGroup *dcastdcast_gcExternalVariableGroupptr__constcBaseptr__0021F6D0(const cBase *);
gcRoomInstanceGroup *dcastdcast_gcRoomInstanceGroupptr__constcBaseptr__0021FC20(const cBase *);
gcValExternalVariable *dcastdcast_gcValExternalVariableptr__constcBaseptr__0022EE5C(const cBase *);
gcValMemCardStatus *dcastdcast_gcValMemCardStatusptr__constcBaseptr__002306BC(const cBase *);

void gcExternalVariableGroup__AssignCopy_constcBaseptr__002379C4(gcExternalVariableGroup *self, const cBase *other) {
    gcExternalVariableGroup *src = dcastdcast_gcExternalVariableGroupptr__constcBaseptr__0021F6D0(other);
    self->unk8 = src->unk8;
    self->unkC = src->unkC;
}

void gcRoomInstanceGroup__AssignCopy_constcBaseptr__00238364(gcRoomInstanceGroup *self, const cBase *other) {
    gcRoomInstanceGroup *src = dcastdcast_gcRoomInstanceGroupptr__constcBaseptr__0021FC20(other);
    self->unk8 = src->unk8;
    self->unkC = src->unkC;
}

void gcValExternalVariable__AssignCopy_constcBaseptr__00342E98(gcValExternalVariable *self, const cBase *other) {
    gcValExternalVariable *src = dcastdcast_gcValExternalVariableptr__constcBaseptr__0022EE5C(other);
    self->value = src->value;
}

void gcValMemCardStatus__AssignCopy_constcBaseptr__0034FFD0(gcValMemCardStatus *self, const cBase *other) {
    gcValMemCardStatus *src = dcastdcast_gcValMemCardStatusptr__constcBaseptr__002306BC(other);
    self->unk8 = src->unk8;
    self->unkC = src->unkC;
}

}
