#ifndef EWORLD_H
#define EWORLD_H

class eWorld {
public:
    char pad[36];
    mutable int lockCount;

    void LockWorld(bool lock) const;
};

#endif
