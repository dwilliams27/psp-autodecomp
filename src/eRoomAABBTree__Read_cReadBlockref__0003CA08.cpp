// eRoomAABBTree::Read(cReadBlock &) (0x0003ca08, 220B) — eAll_psp.obj
// Split-TU isolated build: classes declared locally; symbol emitted is
// __0fNeRoomAABBTreeEReadR6KcReadBlock.

class cReadBlock;
class eRoom;

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
};

// eRoomTreeNode is 0x30 bytes; SetSize takes a const-ref default element.
struct eRoomTreeNode {
    int _w[12];
    void Read(cReadBlock &);
};

template <class T>
class cArrayBase {
public:
    T *mData;
    bool SetSize(int, const T &);   // default element passed by const ref
};

template <class T>
class cArray : public cArrayBase<T> {
public:
    void Read(cReadBlock &);
};

template <class T>
class cHandleT {
public:
    T *mPtr;
};

class eRoomAABBTree {
public:
    cArrayBase<eRoomTreeNode> mNodes;        // at +0
    cArray<cHandleT<eRoom> > mRooms;         // at +4
    void Read(cReadBlock &);
};

void eRoomAABBTree::Read(cReadBlock &rb) {
    eRoomTreeNode defNode;
    int count;

    cFileSystem::Read(*(void **)rb._data[0], &count, 4);
    int c = count;
    defNode._w[0] = 0; defNode._w[1] = 0; defNode._w[2] = 0;
    defNode._w[3] = 0; defNode._w[4] = 0; defNode._w[5] = 0;
    defNode._w[6] = 0; defNode._w[7] = 0; defNode._w[8] = 0;
    defNode._w[9] = 0; defNode._w[10] = 0; defNode._w[11] = 0;
    mNodes.SetSize(c, defNode);

    int n = count;
    eRoomTreeNode *base = mNodes.mData;
    for (int i = 0; i < n; i++) {
        base[i].Read(rb);
    }

    mRooms.Read(rb);
}
