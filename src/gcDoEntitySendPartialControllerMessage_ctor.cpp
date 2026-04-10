#include "gcDoEntitySendMessage.h"
#include "cBase.h"

void gcAction_gcAction(gcDoEntitySendPartialControllerMessage *, cBase *);
void gcDesiredObject_gcDesiredObject(void *, void *);
void gcDesiredEntityHelper_gcDesiredEntityHelper(void *, int, int, int);
void cBaseArray_SetSize(void *, int);

extern char gcDoEntitySendPartialControllerMessagevirtualtable[];
extern char gcDoEntitySendPartialControllerMessage_desobj_vtable[];
extern char gcDoEntitySendPartialControllerMessage_vtable1[];
extern char gcDoEntitySendPartialControllerMessage_handle_vtable[];

gcDoEntitySendPartialControllerMessage::gcDoEntitySendPartialControllerMessage(cBase *parent) {
    gcAction_gcAction(this, parent);
    ((void **)this)[1] = gcDoEntitySendPartialControllerMessagevirtualtable;
    void *desobj = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desobj, this);
    ((void **)this)[4] = gcDoEntitySendPartialControllerMessage_desobj_vtable;
    gcDesiredEntityHelper_gcDesiredEntityHelper((char *)this + 0x18, 1, 0, 0);
    ((void **)this)[4] = gcDoEntitySendPartialControllerMessage_vtable1;
    ((void **)this)[8] = desobj;
    ((void **)this)[9] = gcDoEntitySendPartialControllerMessage_handle_vtable;
    ((char *)this)[0x28] = 1;
    ((char *)this)[0x29] = 0;
    ((int *)this)[0x0B] = 0;
    ((int *)this)[0x0C] = 0;
    int encoded1 = (int)desobj | 1;
    ((int *)this)[0x0D] = encoded1;
    ((int *)this)[0x0E] = (int)this;
    ((void **)this)[0x0F] = gcDoEntitySendPartialControllerMessage_handle_vtable;
    ((char *)this)[0x40] = 1;
    ((char *)this)[0x41] = 0;
    ((int *)this)[0x11] = 0;
    int encoded2 = (int)this | 1;
    ((int *)this)[0x12] = 0;
    ((int *)this)[0x13] = encoded2;
    ((int *)this)[0x14] = (int)this;
    ((void **)this)[0x15] = gcDoEntitySendPartialControllerMessage_handle_vtable;
    ((char *)this)[0x58] = 1;
    ((char *)this)[0x59] = 0;
    ((int *)this)[0x17] = 0;
    ((int *)this)[0x18] = 0;
    ((int *)this)[0x19] = encoded2;
    ((int *)this)[0x1A] = 0;
    ((int *)this)[0x1B] = (int)this;
    ((int *)this)[0x1C] = encoded2;
    cBaseArray_SetSize((char *)this + 0x68, 4);
}
