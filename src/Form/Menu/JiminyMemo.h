// PARTIAL: added for Form/Menu/JMemoFlag
#ifndef FORM_MENU_JIMINYMEMO_H
#define FORM_MENU_JIMINYMEMO_H

#include "khtypes.h"
#include "System/MainApp.h"
#include "System/Task.h"

/* Placeholder for the original header's inline function asserting inId (CursorDataParam): its strings
 * ("System/../Form/Menu/JiminyMemo.h", "inId < PARAM_NUM") are in .rodata of every includer. The line number
 * is not known. */
#ifndef FORM_MENU_JIMINYMEMO_CPP /* MATCH: FORM_MENU_JIMINYMEMO_CPP gate (tested 2026-11-02): its own unit includes it as "JiminyMemo.h" (the full path) */
#line 1 "System/../Form/Menu/JiminyMemo.h"
#endif
inline void _jiminyMemoCheckId(INT inId) {
    if (!(inId < 8)) AssertApp(__FILE__, __LINE__, "inId < PARAM_NUM", 0);
}

#ifdef FORM_MENU_JIMINYMEMO_CPP
/* the class inlines setContentParam/getContentParam (header lines 1185/1193) assert their index with the
 * header's file name; the class itself is in Form/Menu/JiminyMemo.cpp */
inline void _jiminyMemoAssertParamId(INT inId, INT inLine) {
    if (!(inId < 2)) AssertApp(__FILE__, inLine, "inId < PARAM_NUM", 0);
}
#endif

#ifndef FORM_MENU_JIMINYMEMO_CPP /* MATCH: FORM_MENU_JIMINYMEMO_CPP gate (tested 2026-11-02): full class in Form/Menu/JiminyMemo.cpp */
class CJiminyMemo : public CTask {
public:
    CJiminyMemo(CTask* inParent, INT inMode);

private:
    BYTE _pad[0x570 - sizeof(CTask)]; // members not known yet
};
#endif

#endif // FORM_MENU_JIMINYMEMO_H
