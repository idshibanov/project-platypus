
#ifndef PLA_SCORESMENU_H
#define PLA_SCORESMENU_H

#include "Menu.h"
#include "MainMenu.h"

class ScoresMenu: public Menu {
    private:
    public:
        ScoresMenu( GameClient* gc );
        void draw();

        Menu* execute();
};

#endif

