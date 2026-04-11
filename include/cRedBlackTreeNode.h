#ifndef CREDBLACKTREENODE_H
#define CREDBLACKTREENODE_H

template <class T>
class cRedBlackTreeNode : public T {
public:
    int mLeft;
    int mRight;
    int mParent;
    cRedBlackTreeNode();
};

template <class T>
cRedBlackTreeNode<T>::cRedBlackTreeNode() : mLeft(0), mRight(0), mParent(0) {
}

#endif
