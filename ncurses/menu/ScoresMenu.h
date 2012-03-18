
#ifndef PLA_SCORESMENU_H
#define PLA_SCORESMENU_H

#include "Menu.h"

class ScoresMenu: public Menu {
    private:

    public:
        ScoresMenu( GameClient* gc ): Menu( gc )
        {

        };
        void draw()
        {
             box( stdscr, 'X', 'X' );
        };
};

#endif

