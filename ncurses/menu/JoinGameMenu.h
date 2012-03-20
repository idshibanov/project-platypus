
#ifndef PLA_JOINGAMEMENU_H
#define PLA_JOINGAMEMENU_H

#include "Menu.h"

class JoinGameMenu: public Menu {
    private:

    public:
        JoinGameMenu( GameClient* gc ): Menu( gc, 3 )
        {

        };
        void draw()
        {
             box( stdscr, '3', '4' );
        };
};

#endif

