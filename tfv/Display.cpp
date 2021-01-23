
/*                                                                  
    Display.cpp                                                     
    EvilTeach - MPL 1.1                                             
                                                                  */

#include "common.hpp"

Display::Display
(
    void
) :
    _showHexCharacter(false),
    _currentLine(0)
{
}

Display::~Display
(
    void
)
{
}



/*                                                                                  
    This function displays 1 screens worth of information.                          
    Text before the file, and after the file displays as tildes.                    
                                                                                    
    The current line is displayed reverse of the other gUserColorChoice.            
                                                                                    
    if a find operation is in progress, the match string will be reversed instead   
                                                                                  */
void Display::display_the_screen
(
    void
)
{
    int lineNumberSize = (int) log10((double) gTextLines.size()) + 1;

    size_t offset = gpConsole->rows() / 2;
    char buffer1[500] = {0};
    assert(sizeof buffer1 > (size_t)(SCREEN_WIDTH));

    init_pair((short) 1, gpConsole->fore_ground_color(), gpConsole->back_ground_color());
    init_pair((short) 2, (short) COLOR_YELLOW, (short) COLOR_BLACK);

    gUserColorChoice = COLOR_PAIR(1);
    gUserColorBold   = COLOR_PAIR(1) | A_BOLD;
    gRulerColor      = COLOR_PAIR(2) | A_BOLD;

    /* Build a ruler that makes sense with respect to the column position */
    char ruler[MAX_LINE_SIZE + sizeof('\0')];

    /* Mark every column */
    memset(ruler, '.' , sizeof(ruler));

    /* Mark every 5 columns */
    for (size_t z = 4; z < sizeof(ruler); z += 5)
    {
        ruler[z] = '^';
    }

    /* Mark every 10 columns */
    for (size_t y = 0; y < (sizeof(ruler) - 4); y++)
    {
        if (((y + 1) % 10) == 0)
        {
            char tmp[60] = {0};
            int chars = sprintf(tmp, "%u", (unsigned int) (y + 1));
            (void) chars;

            assert(chars < (int) sizeof tmp);

            memcpy(ruler + y, tmp, strlen(tmp));
        }
        else
        {
        }
    }

    // BUG - FULL 999999 IS NOT VISIBLE

    /* And it is a string */
    ruler[sizeof(ruler) - 1] = '\0';

    int column = 0;

    /*                                                                      
        This loops across the console, displaying the desired column value  
                                                                          */
    for (size_t z = 0; z < TFV_PAGE_SIZE; z++)
    {
        /* Show it here */
        gpConsole->moveto(column, 0);

        size_t sub = gpDisplay->current_line() - offset + z;
        if (sub < TOTAL_LINES)
        {
            /* If we are at the middle line, we show it reversed */
            if (z == SCREEN_SIZE / 2)
            {
                /* Show line number */
                int chars = sprintf(buffer1, "%*zu ", lineNumberSize, sub + 1);
                (void) chars;

                assert(chars < (int) sizeof buffer1);

                *gpConsole << buffer1;

                /* Get information about the line to display_the_screen */
                size_t y = 0;
                size_t length = strlen(gTextLines[(size_t)(sub)]);

                if (gTextLines.carriageControlValue[0] == '\0' && gTextLines.show_carriage_control())
                {
                    length++;   /* For Binary files add the trailing NUL */
                }
                else
                {
                }

                size_t limit = gCurrentColumn + gpConsole->cols();
                if (length > limit)
                    length = limit;


                /* Iterate through the columns that need to be displayed */
                for (y = (size_t) gCurrentColumn; y < (size_t) length; y++)
                {
                    char c = gTextLines[(size_t) sub][y];

                    if ((unsigned char) c >= ' ')
                    {
                        /* This is a normal character but it may be part of the found pattern */

                        if (gpFinder->find_in_progress())
                        {
                            if (gpFinder->in_search_key(y))
                            {
                                chtype tmp = A_BLINK + A_REVERSE;
                                attrset(tmp);
                            }
                            else
                            {
                                attrset(gUserColorChoice);
                            }

                            *gpConsole << c;
                        }
                        else
                        {
                            attrset(gUserColorBold);
                            *gpConsole << c;
                        }
                    }
                    else
                    {
                        /* This is a control character of some sort */
                        gUserColorChoice |= A_REVERSE;
                        attrset(gUserColorChoice);

                        *gpConsole << (char) (c + '@');

                        gUserColorChoice &= ~A_REVERSE;
                        attrset(gUserColorChoice);
                    }
                }

                attrset(gUserColorChoice);

                *gpConsole << endl;

                column++;
                gpConsole->moveto(column, 0);

                ruler[gCurrentColumn + gpConsole->cols() - lineNumberSize] = '\0';

                attrset(gRulerColor);
                memset(buffer1, ' ', sizeof(buffer1));
                buffer1[lineNumberSize] = '\0';
                *gpConsole << ' ' 
                           << buffer1
                           << (ruler + gCurrentColumn)
                           << endl;
            }
            else
            {
                /*                                                                              
                    This  is a line other than the middle one, so we just display it            
                                                                                              */
                gUserColorChoice &= ~A_REVERSE;

                attrset(gUserColorChoice);

                /* Show line number */
                int chars = sprintf(buffer1, "%*zu ", lineNumberSize, sub + 1);
                (void) chars;

                assert(chars < (int) sizeof buffer1);

                *gpConsole << buffer1;

                size_t y = 0;
                size_t length = strlen(gTextLines[(size_t)(sub)]);

                if (gTextLines.carriageControlValue[0] == '\0' && gTextLines.show_carriage_control())
                {
                    length++;   /* For Binary files add the trailing NUL */
                }
                else
                {
                }

                size_t limit = gCurrentColumn + gpConsole->cols();
                if (length > limit) length = limit;

                for (y = gCurrentColumn; y < length; y++)
                {
                    char c = gTextLines[(size_t)(sub)][(size_t)(y)];
                    if ((unsigned char) c >= ' ')
                    {
                        *gpConsole << c;
                    }
                    else
                    {
                        gUserColorChoice |= A_REVERSE;
                        attrset(gUserColorChoice);

                        *gpConsole << (char) (c + '@');

                        gUserColorChoice &= ~A_REVERSE;
                        attrset(gUserColorChoice);
                    }
                }

                *gpConsole << endl;
            }
        }
        else
        {
            /*                                                                                  
                This line is before the first real line, or after the last, so we               
                display_the_screen a tilde.   If this is the very first painting of the file,   
                we show the license information.                                                
                                                                                              */

            static bool showLicense = true;

            if (showLicense)
            {
                if (sub == -9)
                {
                    *gpConsole << "~    Text File Viewer, " VERSION " "
                               << "MPL 1.1, "
                               << "by EvilTeach "
                               << endl;
                }
                else
                {
                    if (sub == -7)
                    {
                        *gpConsole << "~    Using https://pdcurses.sourceforge.io/ Version 3.4" << endl;
                    }
                    else
                    {
                        if (sub == -5)
                        {
                            if (gUserPreferenceFileExists)
                            {
                                *gpConsole << "~    User Preferences loaded from " << gUserPreferenceFileName << endl;
                            }
                            else
                            {
                                *gpConsole << "~    TFV_FILE environment variable not set.  No Preferences loaded." << endl;
                            }

                            showLicense = false;
                        }
                        else
                        {
                            *gpConsole << "~" << endl;
                        }
                    }
                }
            }
            else
            {
                *gpConsole << "~" << endl;
            }
        }

        gUserColorChoice &= ~A_REVERSE;
        attrset(gUserColorChoice);

        column++;
    }


    {
        //chtype gUserColorChoice = COLOR_PAIR(1);
        gUserColorChoice = COLOR_PAIR(1);
        //chtype gRulerColor      = COLOR_PAIR(2) | A_BOLD;
        gRulerColor = COLOR_PAIR(2) | A_BOLD;

        attrset(gRulerColor);

        //char buffer1[500] = {0};
        //assert(sizeof buffer1 > (size_t) (SCREEN_WIDTH));

        if (gTextLines.size() > 0)
        {
            size_t length = strlen(gTextLines[(size_t)(gpDisplay->current_line())]);

            if (gTextLines.carriageControlValue[0] == '\0' && gTextLines.show_carriage_control())
            {
                length++;
            }
            else
            {
            }

            if (length <= gCurrentColumn)
            {
                /* Don't reference past the end of the string */
            }
            else
            {
                unsigned char charName[3] = {0};

                if ((unsigned char) gTextLines[(size_t)(gpDisplay->current_line())][gCurrentColumn] >= ' ')
                {
                    charName[0] = (unsigned char) (gTextLines[(size_t)(gpDisplay->current_line())][gCurrentColumn]);
                }
                else
                {
                    charName[1] = (unsigned char) (gTextLines[(size_t)(gpDisplay->current_line())][gCurrentColumn] + '@');
                    charName[0] = '^';
                }

                if (gpDisplay->show_hex_character())
                {
                    int chars = sprintf(buffer1, "CharSet<0x%02X %s>",
                                      (unsigned char) gTextLines[(size_t)(gpDisplay->current_line())][(size_t)(gCurrentColumn)],
                                      charName);
                    (void) chars;
                    assert(chars < (int) sizeof buffer1);
                }
                else
                {
                    int chars = sprintf(buffer1, "CharSet<%02d %s>", 
                        (unsigned char) gTextLines[(size_t)(gpDisplay->current_line())][(size_t)(gCurrentColumn)],
                        charName);
                    (void) chars;
                    assert(chars < (int) sizeof buffer1);
                }
            }
        }
        else
        {
            /* Don't show a character for empty files */
        }

         gpConsole->moveto((int) gpConsole->rows() - 1, 0);  // BUG can underflow
        *gpConsole << "File "          << gFileNameOnly.c_str()          << ", "
                   << "Line "          << gpDisplay->current_line() + 1  << " of "
                                       << (int) gTextLines.size()        << ", "
                   << "Column "        << gCurrentColumn + 1             << ", "
                   << gpFinder->configuration();

        *gpConsole << ", Key";

        int x = 0;
        int y = 0;
        gpConsole->get_yx(y, x);

        *gpConsole << gKeyNames[(size_t)(gLastKeyPressed)].keyName;

        *gpConsole << " " << gpKeyboard->function_name((size_t)(gLastKeyPressed)) << ", "
                          << buffer1
                          ;
        *gpConsole << endl;

        gpConsole->moveto(y, x);

        attrset(gUserColorChoice);
    }
}

        bool Display::show_hex_character
        (
            void
        )
        {
            return _showHexCharacter;
        }

        void Display::change_show_hex_character
        (
            void
        )
        {
            _showHexCharacter = !_showHexCharacter;
        }

        /*                                                                  
            This function sets one of the preferences that are loaded       
            from the preference file.   It returns true of the preference   
            applies to this object.  Otherwise it returns false             
                                                                          */
        bool Display::set_preference
        (
            const std::string  preference
        )
        {

            if ((preference.length() > 6) && (preference.substr(0, 7) == "DISPLAY"))
            {
                if (preference.length() > 16 && preference.substr(0, 17) == "DISPLAY_HEX_CHAR:")
                {
                    _showHexCharacter = (preference.substr(17) == "1") ? true : false;
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
        void Display::save_preferences
        (
            std::ostream &udf
        )
        {
            udf << "DISPLAY_HEX_CHAR:" << _showHexCharacter << std::endl;
        }

Display *gpDisplay = NULL;
