
#ifndef PLA_MAINMENU_H
#define PLA_MAINMENU_H

#include "Menu.h"

#include "LoginMenu.h"
#include "RegisterMenu.h"
#include "SettingsMenu.h"
#include "../nclient.h"

class MainMenu: public Menu {
    private:
    public:
        MainMenu( GameClient* gc );
        void draw();

        Menu* execute();
};

#endif
