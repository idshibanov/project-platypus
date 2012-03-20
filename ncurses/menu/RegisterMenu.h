
#ifndef PLA_REGISTERMENU_H
#define PLA_REGISTERMENU_H

#include "Menu.h"
#include "MainMenu.h"

class RegisterMenu: public Menu {
    private:
    public:
        RegisterMenu( GameClient* gc );
        void draw();

        Menu* execute();
};

#endif

