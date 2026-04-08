#ifndef CFASTMEMALLOCATOR_H
#define CFASTMEMALLOCATOR_H

class cFastMemAllocator {
public:
    float GetSizeScale(void) const;
    void Free(void *);
};

#endif
