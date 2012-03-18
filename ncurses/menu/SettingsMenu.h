
#ifndef PLA_SETTINGSMENU_H
#define PLA_SETTINGSMENU_H

#include "Menu.h"
#include "MainMenu.h"

class SettingsMenu: public Menu {
    private:
    public:
        SettingsMenu( GameClient* gc );
        void draw();

        Menu* execute();
};

#endif

