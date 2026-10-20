// PARTIAL: added for Form/Menu/JMemoFlag
#ifndef FORM_MENU_JIMINYMEMO_H
#define FORM_MENU_JIMINYMEMO_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Task.h"

/* Placeholder for the original header's inline function asserting inId (CursorDataParam): its strings
 * ("System/../Form/Menu/JiminyMemo.h", "inId < PARAM_NUM") are in .rodata of every includer. The line number
 * is not known. */
#line 1 "System/../Form/Menu/JiminyMemo.h"
inline void _jiminyMemoCheckId(INT inId) {
    if (!(inId < 8)) AssertApp(__FILE__, __LINE__, "inId < PARAM_NUM", 0);
}

#ifndef FORM_MENU_JIMINYMEMO_CPP /* full class in Form/Menu/JiminyMemo.cpp */
class CJiminyMemo : public CTask {
public:
    CJiminyMemo(CTask* inParent, INT inMode);

private:
    BYTE _pad[0x570 - sizeof(CTask)]; // members not known yet
};
#endif

#endif // FORM_MENU_JIMINYMEMO_H
