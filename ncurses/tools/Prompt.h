
#ifndef _PLA_PROMPT_H_
#define _PLA_PROMPT_H_

#include <string>
#include <curses.h>

using namespace::std;

// Set what characters are allowed for an input session
enum PROMPTCHARSET {
    ALPHANUMERIC, // 1-9 and A -> z
    ALPHANUMERIC_SYMBOLS, // char values 33 -126
    PASSWORD // Hidden Entry, but same as ALPHANUMERIC
};

class Prompt {
    private:
        int _colX;
        int _lineY;
        int _maxLen;
        int _startPos;
        string _inputBuff;
        PROMPTCHARSET _charset;

        void advanceCursor();
        void retreatCursor();
        void deleteCharAt( int );
        void addChar( char );
    public:
        Prompt( PROMPTCHARSET );
        string getMessage( int, int, int );
};

#endif

