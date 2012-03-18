
#ifndef PLA_LOGINMENU_H
#define PLA_LOGINMENU_H

#include "Menu.h"
#include "MainMenu.h"

class LoginMenu: public Menu {
    private:
    public:
        LoginMenu( GameClient* gc );
        void draw();

        Menu* execute();
};

#endif

