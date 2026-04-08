extern "C" {

void *cHandlePairT_gcStateMachine_cSubHandleT_gcState____cHandlePairT_void__00233328(void *self) {
    ((int *)self)[0] = 0;
    ((int *)self)[1] = 0;
    return self;
}

void *cHandlePairT_gcUIDialog_cSubHandleT_gcUIWidget____cHandlePairT_void__0023C4A0(void *self) {
    ((int *)self)[0] = 0;
    ((int *)self)[1] = 0;
    return self;
}

void *cGUIDT_gcRegion___cGUIDT_void__00245578(void *self) {
    ((int *)self)[0] = 0;
    ((int *)self)[1] = 0;
    return self;
}

}

struct gcStreamedCinematic_data {
    char pad[0xD4];
    int groups[0x1B];
};

int *gcStreamedCinematic__GetGroups_intptr__0024436C(gcStreamedCinematic_data *self, int *count) {
    *count = 0x1B;
    return self->groups;
}

struct gcRegion_data {
    char pad[0x78];
    int groups[0xC];
};

int *gcRegion__GetGroups_intptr__00245824(gcRegion_data *self, int *count) {
    *count = 0xC;
    return self->groups;
}
