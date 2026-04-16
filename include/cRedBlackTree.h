#ifndef CREDBLACKTREE_H
#define CREDBLACKTREE_H

#include "cRedBlackTreeNode.h"

template <class T>
class cFixedSizeAllocatorBase {
};

template <class T>
class cFixedSizeAllocator {
};

class cRedBlackTreeCompare {
};

template <class ValueType, class AllocType, class KeyType, class Compare>
class cRedBlackTree {
public:
    cRedBlackTreeNode<ValueType> *mRoot;

    cRedBlackTreeNode<ValueType> *FindNode(const AllocType &key) const;
};

#endif
