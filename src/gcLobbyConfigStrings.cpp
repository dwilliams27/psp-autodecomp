class cBase;

template <class T> T *dcast(const cBase *);

class gcLobbyConfigStrings {
public:
    char _pad0[8];
    int mText;
    void AssignCopy(const cBase *);
};

class gcTableTemplateGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcDesiredTableTemplate;
class gcDesiredTableTemplateHelper;
class gcTableTemplate;
class gcDesiredTrigger;
class gcDesiredTriggerHelper;
class gcTrigger;
class gcDesiredPortal;
class gcDesiredPortalHelper;
class ePortal;

template <class T, class H, class O>
class gcDesiredObjectT {
public:
    H *Get(bool) const;
    cBase *GetObject(bool) const;
};

void gcLobbyConfigStrings::AssignCopy(const cBase *base) {
    gcLobbyConfigStrings *other = dcast<gcLobbyConfigStrings>(base);
    mText = other->mText;
}

bool gcTableTemplateGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

typedef gcDesiredObjectT<gcDesiredTableTemplate,
                         gcDesiredTableTemplateHelper,
                         gcTableTemplate> gcDesiredTableTemplateObject;
typedef gcDesiredObjectT<gcDesiredTrigger,
                         gcDesiredTriggerHelper,
                         gcTrigger> gcDesiredTriggerObject;
typedef gcDesiredObjectT<gcDesiredPortal,
                         gcDesiredPortalHelper,
                         ePortal> gcDesiredPortalObject;

extern "C" cBase *
__0fQgcDesiredObjectT76WgcDesiredTableTemplate6cgcDesiredTableTemplateHelper6PgcTableTemplate_JGetObjectb_P6FcBaseK(
    gcDesiredTableTemplateObject *self, bool b) {
    return (cBase *)self->Get(b);
}

extern "C" cBase *
__0fQgcDesiredObjectT76QgcDesiredTrigger6WgcDesiredTriggerHelper6JgcTrigger_JGetObjectb_P6FcBaseK(
    gcDesiredTriggerObject *self, bool b) {
    return (cBase *)self->Get(b);
}

extern "C" cBase *
__0fQgcDesiredObjectT76PgcDesiredPortal6VgcDesiredPortalHelper6HePortal_JGetObjectb_P6FcBaseK(
    gcDesiredPortalObject *self, bool b) {
    return (cBase *)self->Get(b);
}
