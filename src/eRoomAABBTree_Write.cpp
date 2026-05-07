// ODR-WARNING: split-TU local redeclaration for one eRoomAABBTree method.

class cWriteBlock {
public:
    void Write(int);
};

class cHandle {
public:
    int mKey;
    void Write(cWriteBlock &) const;
};

class eRoomTreeNode {
public:
    char _pad[0x30];
    void Write(cWriteBlock &) const;
};

class eRoomAABBTree {
public:
    void Write(cWriteBlock &) const;
};

void eRoomAABBTree::Write(cWriteBlock &wb) const {
    eRoomTreeNode *nodes = *(eRoomTreeNode **)((const char *)this + 0);
    int nodeCount = 0;
    if (nodes != 0) {
        nodeCount = ((int *)nodes)[-1] & 0x3FFFFFFF;
    }
    wb.Write(nodeCount);

    nodes = *(eRoomTreeNode **)((const char *)this + 0);
    nodeCount = 0;
    if (nodes != 0) {
        nodeCount = ((int *)nodes)[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < nodeCount) {
        int offset = 0;
        eRoomTreeNode *node = nodes + offset;
        do {
            node->Write(wb);
            i++;
            node++;
        } while (i < nodeCount);
    }

    cHandle *handles = *(cHandle **)((const char *)this + 4);
    int handleCount0 = 0;
    if (handles != 0) {
        handleCount0 = ((int *)handles)[-1] & 0x3FFFFFFF;
    }
    wb.Write(handleCount0);

    handles = *(cHandle **)((const char *)this + 4);
    int handleCount = 0;
    if (handles != 0) {
        handleCount = ((int *)handles)[-1] & 0x3FFFFFFF;
    }

    int handleIndex = 0;
    if (handleIndex < handleCount) {
        int handleOffset = 0;
        cHandle *handle = handles + handleOffset;
        do {
            handle->Write(wb);
            handleIndex++;
            handle++;
        } while (handleIndex < handleCount);
    }
}
