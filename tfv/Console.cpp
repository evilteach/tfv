
/*                                                                  
    Console.cpp                                                     
    EvilTeach - MPL 1.1                                             
                                                                  */

#include "common.hpp"

/*
*/
void attention
(
    void
)
{
    beep();
}

/*                                                                              
    This holds the value that the user typed in response to things like find    
                                                                              */
char gAskBuffer[KEYBOARD_LIMIT] = {0};

/*                                                          
    This function prompts and gets a command from the user  
                                                          */
int ask
(
    const char *prompt
)
{
    attrset(gRulerColor);

    gpConsole->moveto((int) (gpConsole->rows() - 1), 0);
    *gpConsole << prompt << "> " << endl;

    char *p = gAskBuffer;

    for (;;)
    {
        int x = gpConsole->get_key();

        if (x == '\n')
        {
            *p = '\0';

            break;
        }
        else
        {
            if (x == 0x08)
            {
                if (p > gAskBuffer)
                {
                     p--;
                    *p = '\0';

                     gpConsole->moveto((int) (gpConsole->rows() - 1), 0);
                    *gpConsole << prompt << "> " << endl;

                    *gpConsole << gAskBuffer << flush;
                }
                else
                {
                    attention();
                }
            }
            else
            {
                *p++ = (char) x;
                *gpConsole << (char) x << flush;
            }
        }
    }

    attrset(gUserColorChoice);

    return FAILURE;
}



/*
*/
Console::Console
(
    void
) :
    _rows(0),
    _cols(0)
{
    _window = initscr();

#ifdef _WIN64
    mousemask (ALL_MOUSE_EVENTS, NULL);
#endif


    /* Do not buffer */
    cbreak();

    /* Accept keypad keys too */
    keypad(_window, true);

    /* Don't echo things that are typed */
    noecho();

#ifdef _WIN64
    /* Resize the terminal to something larger than the physical screen */
    resize_term(2000, 2000);

    /* Get the largest physical screen dimensions */
    {
        int rTmp;
        int cTmp;
        getmaxyx(_window, rTmp, cTmp);
        _rows = (size_t) rTmp;
        _cols = (size_t) cTmp;
    }

    /* Resize so it fits */
    resize_term((int)(_rows - 1), (int)(_cols - 1));
#endif

    /* Get the screen dimensions that fit */

    int rTmp;
    int cTmp;
    getmaxyx(_window, rTmp, cTmp);
    _rows = (size_t) rTmp;
    _cols = (size_t) cTmp;


    /* Don't display cursor */
    curs_set(0);

    /* Support colors */
    start_color();

    /* Default choice */
    foreground = COLOR_WHITE;
    background = COLOR_BLUE;

    set_colors();
}

Console::~Console
(
    void
)
{
    endwin();
}

int Console::get_key()
{
    int key = wgetch(_window);

#ifdef _WIN64
    if (key == KEY_MOUSE)
    {
        getmouse (&mouseInput);
        if (mouseInput.bstate == BUTTON1_PRESSED || mouseInput.bstate == BUTTON1_CLICKED)
        {
            key = CTL_LEFT;
        }
        else
        {
            if (mouseInput.bstate == BUTTON3_PRESSED || mouseInput.bstate == BUTTON3_CLICKED)
            {
                key = CTL_RIGHT;
            }
            else
            {
                if (mouseInput.bstate == BUTTON4_PRESSED)
                {
                        key = KEY_PPAGE;
                }
                else
                {
                    if (mouseInput.bstate == BUTTON5_PRESSED)
                    {
                        key = KEY_NPAGE;
                    }
                    else
                    {
                        key = get_key();
                    }
                }
            }
        }
    }
    else
    {
    }
#endif

    return key;
}

Console& Console::operator<<
(
    const char *iValue
)
{
    char buffer[MAX_LINE_SIZE + sizeof('\0')] = {0};
    if (strlen(iValue) < sizeof(buffer))
    {
        strcpy(buffer, iValue);
        printw(buffer);
    }
    else
    {
        printw("tfv:  line too long to display");
    }

    return *this;
}

Console& Console::operator<<
(
    int iValue
)
{
    char buffer[20] = {0};
    int chars = sprintf(buffer, "%d", iValue);
    (void) chars;
    assert(chars < (int) sizeof(buffer));
    printw(buffer);

    return *this;
}

Console& Console::operator<<
(
    size_t iValue
)
{
    char buffer[20] = {0};
    int chars = sprintf(buffer, "%zu", iValue);
    (void) chars;
    assert(chars < (int) sizeof(buffer));
    printw(buffer);

    return *this;
}

Console& Console::operator<<
(
    const char iValue
)
{
    char buffer[2] = {0};
    int chars = sprintf(buffer, "%c",iValue);
    (void) chars;

    assert(chars < (int) sizeof buffer);

    printw(buffer);

    return *this;
}


Console& Console::operator<<(Console& (*m)(Console&))
{
    return (*m)(*this);
}

void Console::flush
(
    void
)
{
    refresh();
}

void Console::moveto
(
    int row,
    int column
)
{
    move(row, column);
}

size_t Console::rows
(
    void
)
{
    return _rows;
}

size_t Console::cols
(
    void
)
{
    return _cols;
}

void Console::home
(
    void
)
{
    moveto(0,0);
}

void Console::cls
(
    void
)
{
    home();

    for (size_t row = 0; row < rows(); row++)
    {
        for (size_t col = 0; col < cols(); col++)
        {
            *this << ' ';
        }
    }

    home();
}

void Console::get_yx
(
    int&   oY,
    int&   oX
)
{
    getyx(_window, oY, oX);
}

void Console::set_colors
(
    void
)
{
#ifdef _WIN64
    assume_default_colors(foreground, background);
#endif
}

void Console::next_foreground
(
    void
)
{
    foreground++;
    foreground &= 7;
    set_colors();
}

void Console::next_background
(
    void
)
{
    background++;
    background &= 7;

    set_colors();
}

/*
*/
short Console::fore_ground_color
(
    void
)
{
    return (short) foreground;
}

/*
*/
short Console::back_ground_color
(
    void
)
{
    return (short) background;
}


/*                                                                  
    This function sets one of the preferences that are loaded       
    from the preference file.   It returns true of the preference   
    applies to this object.  Otherwise it returns false             
                                                                  */
bool Console::set_preference
(
    const std::string  preference
)
{

    if ((preference.length() > 6) && (preference.substr(0, 7) == "CONSOLE"))
    {
        if (preference.length() > 19 && preference.substr(0, 19) == "CONSOLE_FOREGROUND:")
        {
            foreground = atoi(preference.substr(19).c_str());
        }
        else
        {
            /* This preference does belong to this object, but is not recognized */
        }

        if (preference.length() > 19 && preference.substr(0, 19) == "CONSOLE_BACKGROUND:")
        {
            background = atoi(preference.substr(19).c_str());
        }
        else
        {
            /* This preference does belong to this object, but is not recognized */
        }
    }
    else
    {
        /* This preference does not belong to this object */
    }

    return false;
}

/*                                                                  
    This function saves all of the objects preferences into the     
    output stream                                                   
                                                                  */
void Console::save_preferences
(
    std::ostream &udf
)
{
    udf << "CONSOLE_FOREGROUND:" << fore_ground_color() << std::endl;
    udf << "CONSOLE_BACKGROUND:" << back_ground_color() << std::endl;
}

Console& flush(Console& that)
{
    that.flush();

    return that;
}

Console& endl(Console& that)
{
    that << "\n";
    clrtoeol();
    that << flush;

    return that;
}


/*                                                      
    Once the console is set up, all of the actual usage 
    can get to it directly                              
                                                      */
Console *gpConsole = NULL;

