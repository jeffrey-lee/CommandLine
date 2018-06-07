#include <termios.h> // termios

using namespace std;

string CommandLine::ReadCommand(){
	PrintPrompt();
    _history_cur = 0;
    std::string out_put;
    bool new_line_flag = false;
	while (1) {
		Key key = ParseChar( GetChar() );
		if (key == INPUT_END_KEY) break;
		switch (key) {
			case LINE_BEGIN_KEY :
			case HOME_KEY       :
            {
                MoveBufPtr(0);
                break;
            }
			case LINE_END_KEY   :
			case END_KEY        : 
            {
                MoveBufPtr(_buf.size());
                break;
            }
			case BACK_SPACE_KEY : 
            {
                if(MoveBufPtr(_buf_cur - 1)){
                    DeleteChar(); 
                }
                break;
            }
			case DELETE_KEY     : 
            {
                DeleteChar();
                break;
            }
			case NEWLINE_KEY    :
            {
                out_put = _buf;
                AddHistory(); 
                printf("\n"); 
                new_line_flag = true;
                break;
            }
			case ARROW_UP_KEY   :
            { 	
                MoveToHistory(1);
                break;
            }
			case ARROW_DOWN_KEY :
            { 	
                MoveToHistory(-1);
                break;
            }
			case ARROW_RIGHT_KEY:
            { 	
                MoveBufPtr(_buf_cur + 1);
                break;
            }
			case ARROW_LEFT_KEY :
            { 	
                MoveBufPtr(_buf_cur - 1); 
                break;
            }
			case PG_UP_KEY      :
            { 	
                MoveToHistory(PG_OFFSET);
                break;
            }
			case PG_DOWN_KEY    :
            { 	
                MoveToHistory(-PG_OFFSET);
                break;
            }
			case TAB_KEY        :
            { 	
                InsertChar(' ');
                InsertChar(' ');
                InsertChar(' ');
                InsertChar(' ');
                break;
            }
			case INSERT_KEY     : 	// not yet supported; fall through to UNDEFINE
			case UNDEFINED_KEY  :
            {
                Beep(); 
                break;
            }
			default		        :  	
            {
                InsertChar((char)key); 
                break;
            }
		}

        if(new_line_flag){
            break;
        }
	}

    return out_put;
}

CommandLine::Key CommandLine::ParseChar(char c){
    char c2;
    if(c==EOF) return INPUT_END_KEY;
    switch(c){
        case ESC_KEY:
            c2 = GetChar();
            if(c2 == ARROW_KEY_INT) return static_cast<Key>(GetChar() + ARROW_KEY_FLAG);
            else if(c2 == MOD_KEY_INT) return static_cast<Key>(GetChar() + MOD_KEY_FLAG);
            else return INPUT_END_KEY;
    }
    return static_cast<Key>(c);
}

void CommandLine::PrintPrompt(){
    std::cout << (_prompt.empty() ? ">> " : _prompt);
}

bool CommandLine::MoveBufPtr(ssize_t pos){
    // check safty
    if(pos < 0 || pos > _buf.size()){
         Beep(); 
         return false; 
    } 

    for(ssize_t cur = pos; cur < _buf_cur; ++cur){
        printf("\b");
    }
    for(ssize_t cur = pos; cur > _buf_cur; --cur){
        printf("%c", _buf[cur-1]);
    }

    _buf_cur = pos;
    return true;
}

void CommandLine::DeleteChar(){
    for(size_t cur = _buf_cur; cur < _buf.size() - 1; ++cur ){
        printf("%c", _buf[cur + 1]);
    }
    printf(" ");
    // move cursor
    for(int i=0; i < ( _buf.size() - _buf_cur ) ; i++) printf("\b");
    _buf.erase(_buf_cur, 1);
}

void CommandLine::InsertChar(char ch){
    printf(" ");
    for(size_t cur = _buf_cur; cur < _buf.size(); ++cur){
        printf("%c", _buf[cur]);
    }
    for(size_t cur = _buf_cur; cur < _buf.size(); ++cur){
        printf("\b");
    }
    printf("\b");
    printf("%c", ch);
    _buf.insert(_buf_cur, 1, ch);
    _buf_cur++;
}

void CommandLine::AddHistory(){
    if(_buf.empty() || (0 != _history.size() && _buf == _history.back())){
        return;
    }
    _history.push_back(_buf);
    if(_history.size() > MAX_HISTORY){
        _history.pop_front();
    }
    _buf.clear();
    _buf_cur = 0;
}

void CommandLine::MoveToHistory(ssize_t offset){
    if((-1 == offset && _history_cur == 0)
        || (1 == offset && _history_cur == _history.size())){
        Beep();
        return;
    }

    if(offset < -1 && _history_cur + offset < 0){
        _history_cur = 0;
    }else if(offset > 1 && _history_cur + offset > _history.size() - 1){
        _history_cur = _history.size() - 1;
    }else{
        _history_cur = _history_cur + offset;
    }

    MoveBufPtr(0);
    for(size_t i = 0; i < _buf.size(); ++i){
        printf(" ");
    }
    _buf_cur = _buf.size();
    MoveBufPtr(0);
    if(0 != _history_cur){
        std::list<std::string>::reverse_iterator riter = _history.rbegin();
        for(ssize_t i = 1; i < _history_cur; ++i){
            riter++;
        }
        _buf = *riter;
    }else{
        _buf.clear();
    }
    printf("%s", _buf.c_str());
    _buf_cur = _buf.size();
}

void CommandLine::Beep(){
    printf("\a");
}

static int file_des = 0;
static struct termios stored_settings;

static void reset_keypress(void)
{
	tcsetattr(file_des, TCSANOW, &stored_settings);
}

static void set_keypress(void)
{
	struct termios new_settings;
	tcgetattr(file_des, &stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= (~ECHO);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(file_des, &stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(file_des, TCSANOW, &new_settings);
}

char CommandLine::GetChar()
{
	char ch;
	set_keypress();
	//istr.unsetf(ios_base::skipws);
	ch = getc(stdin);
	//istr.setf(ios_base::skipws);
	reset_keypress();
	//#ifdef TEST_ASC
	//cout << left << setw(6) << int(ch);
	//#endif // TEST_ASC
	return ch;
}
