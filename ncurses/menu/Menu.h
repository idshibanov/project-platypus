
#ifndef PLA_MENU_H
#define PLA_MENU_H

#include <curses.h>

class GameClient;

class Menu {
    protected:
        GameClient* _gc;
        unsigned short int _selected,
                           _max;
    public:
        Menu( GameClient* gc, unsigned short int max ):
            _gc( gc ),
            _selected( 0 ),
            _max( max )
        {};
        void goUp() {
            if ( _selected > 0 )
                _selected--;
            else
                _selected = _max;
        };
        void goDown() {
            if ( _selected < _max )
                _selected++;
            else
                _selected = 0;
        };
        virtual void draw(){ };
        virtual Menu* execute(){};
        void drawTitle() {
            mvaddstr( 3, ( COLS/2 )-13, "_______  ________    _______");
            mvaddstr( 4, ( COLS/2 )-14, "/  _____||       /   /  _____|");
            mvaddstr( 5, ( COLS/2 )-14, "|  |  __  `---/  /   |  |  __");
            mvaddstr( 6, ( COLS/2 )-14, "|  | |_ |    /  /    |  | |_ |");
            mvaddstr( 7, ( COLS/2 )-14, "|  |__| |   /  /----.|  |__| |");
            mvaddstr( 8, ( COLS/2 )-13, "\\______|  /________| \\______|");
        };
};

#endif
