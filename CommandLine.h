#ifndef APPLESEED_COMMAND_LINE_H
#define APPLESEED_COMMAND_LINE_H

#include <list>
#include <string>
#include <iostream>
#include <stdio.h>

class CommandLine{
public:
    typedef enum
    {
    	LINE_BEGIN_KEY   = 1,
    	LINE_END_KEY     = 5,
    	INPUT_END_KEY    = 4,
    	TAB_KEY          = (int)'\t',
    	NEWLINE_KEY      = (int)'\n',
    	ESC_KEY          = 27,
    
    	BACK_SPACE_KEY   = 127,
    
    	ARROW_KEY_FLAG   = 1 << 8,
    	ARROW_KEY_INT    = 91,
    	ARROW_UP_KEY     = 65 + ARROW_KEY_FLAG,
    	ARROW_DOWN_KEY   = 66 + ARROW_KEY_FLAG,
    	ARROW_RIGHT_KEY  = 67 + ARROW_KEY_FLAG,
    	ARROW_LEFT_KEY   = 68 + ARROW_KEY_FLAG,
    	ARROW_KEY_BEGIN  = ARROW_UP_KEY,
    	ARROW_KEY_END    = ARROW_LEFT_KEY,
    
    	MOD_KEY_FLAG     = 1 << 9,
    	MOD_KEY_INT      = 91,
    	HOME_KEY         = 49 + MOD_KEY_FLAG,
    	INSERT_KEY       = 50 + MOD_KEY_FLAG,
    	DELETE_KEY       = 51 + MOD_KEY_FLAG,
    	END_KEY          = 52 + MOD_KEY_FLAG,
    	PG_UP_KEY        = 53 + MOD_KEY_FLAG,
    	PG_DOWN_KEY      = 54 + MOD_KEY_FLAG,
    	MOD_KEY_BEGIN    = HOME_KEY,
    	MOD_KEY_END      = PG_DOWN_KEY,
    	MOD_KEY_DUMMY    = 126,
    
    	UNDEFINED_KEY    = 99999999,
    
    	KEY_END
    } Key;

    enum STATIC_NUM{
        MAX_HISTORY  = 100,
        PG_OFFSET  = 4,
    };

public:
    CommandLine():_history_cur(0), _buf_cur(0){}
    std::string ReadCommand();
    inline void SetPrompt(const std::string prompt){
        _prompt = prompt;
    }

private:
    char    GetChar();
    Key     ParseChar(char c);
    void    PrintPrompt();
    bool    MoveBufPtr(ssize_t pos);
    void    DeleteChar();
    void    InsertChar(char ch);
    void    AddHistory();
    void    MoveToHistory(ssize_t offset);
    void    Beep();
private:
    std::list<std::string>  _history;
    size_t                  _history_cur;
    std::string             _buf;
    size_t                  _buf_cur;

    std::string             _prompt;
};

#include "CommandLine.hpp"

#endif // APPLESEED_COMMAND_LINE_H
