// gcDesiredObjectT<...>::GetObject thunks for two template instantiations.

class cBase;
class gcDesiredTimer;
class gcDesiredTimerHelper;
class gcTimer;
class gcDesiredSkin;
class gcDesiredSkinHelper;
class eSkin;

template <class T1, class T2, class T3>
class gcDesiredObjectT {
public:
    T2 *Get(bool) const;
    cBase *GetObject(bool) const;
};

template <class T1, class T2, class T3>
cBase *gcDesiredObjectT<T1, T2, T3>::GetObject(bool b) const {
    return (cBase *)Get(b);
}

template class gcDesiredObjectT<gcDesiredTimer,
                                gcDesiredTimerHelper,
                                gcTimer>;
template class gcDesiredObjectT<gcDesiredSkin,
                                gcDesiredSkinHelper,
                                eSkin>;
