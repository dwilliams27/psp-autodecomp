extern "C" void *memset(void *, int, unsigned int);

class gcReplicationGroup {
public:
    gcReplicationGroup(void);
};

// gcReplicationGroup::gcReplicationGroup(void) @ 0x0024400c
gcReplicationGroup::gcReplicationGroup(void) {
    *(char *)this = 0;
    memset((char *)this + 4, 0, 2);
    memset((char *)this + 1, 0, 1);
    memset((char *)this + 2, 0, 1);
}
