
#ifndef PLA_GAMEMENU_H
#define PLA_GAMEMENU_H

#include "Menu.h"
#include "MainMenu.h"

class GameMenu: public Menu {
    private:
    public:
        GameMenu( GameClient* gc );
        void draw();

        Menu* execute();
};

#endif

