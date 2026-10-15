#ifndef FORM_MENU_DECKNAMEEDIT_H
#define FORM_MENU_DECKNAMEEDIT_H

#include "khtypes.h"

class CTask;

class CDecknameEditWnd {
public:
    static HANDLE startEdit(CTask* inpParent, INT inDeckNo);
    static BOOL isFinish();
    static INT getLastEditDeckNo();

private:
    CDecknameEditWnd();
    ~CDecknameEditWnd();
};

#endif /* FORM_MENU_DECKNAMEEDIT_H */
