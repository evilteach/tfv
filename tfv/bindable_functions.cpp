
/*                                                                  
    bindable_functions.cpp                                          
    EvilTeach - MPL 1.1                                             
                                                                  */
#include "common.hpp"



/*                                      
    Cause the program to quit_program   
                                      */
int quit_program
(   
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "quit_program";
    *helpText     = "00Exit out of the program";

    if (action)
    {
        return QUIT;   /* Cause the program to quit_program out */
    }
    else
    {
        return FAILURE;
    }
}



/*                      
    Move 1 screen down  
                      */
int move_screen_to_next_page
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_next_page";
    *helpText     = "10Page Down";

    if (action)
    {
        if (gpDisplay->current_line() + TFV_PAGE_SIZE < (int)(TOTAL_LINES))
        {
            gpDisplay->current_line(gpDisplay->current_line() + TFV_PAGE_SIZE);
        }
        else
        {
            gpDisplay->current_line(LAST_LINE);
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                      
    Move 1 screen up    
                      */
int move_screen_to_prev_page
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_prev_page";
    *helpText     = "11Page Up";

    if (action)
    {
        if (gpDisplay->current_line() > TFV_PAGE_SIZE)
        {
            gpDisplay->current_line(gpDisplay->current_line() - TFV_PAGE_SIZE);
        }
        else
        {
            gpDisplay->current_line(0);
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                              
    Move the screen 1 line down 
                              */
int move_screen_to_next_line
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_next_line";
    *helpText     = "12Line Down";

    if (action)
    {
        if (gpDisplay->current_line() < LAST_LINE)
        {
            gpDisplay->current_line(gpDisplay->current_line() + 1);
        }
        else
        {
            attention();
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                              
    Move the screen 1 line up   
                              */
int move_screen_to_prev_line
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_prev_line";
    *helpText     = "13Line Up";

    if (action)
    {
        if (gpDisplay->current_line() > 0)
        {
            gpDisplay->current_line(gpDisplay->current_line() - 1);
        }
        else
        {
            attention();
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                                          
    Move the screen to the end of the file  
                                          */
int move_screen_to_end_of_file
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_end_of_file";
    *helpText     = "14End of File";

    if (action)
    {
        gpDisplay->current_line(LAST_LINE);
    }
    else
    {
    }

    return FAILURE;
}



/*                                          
    Move the screen to the top of the file  
                                          */
int move_screen_to_top_of_file
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_top_of_file";
    *helpText     = "15Top of File";

    if (action)
    {
        gpDisplay->current_line(0);
    }
    else
    {
    }

    return FAILURE;
}



/*                                                                      
    move the screen so that the user specified line is above the ruler  
                                                                      */
int move_screen_to_specific_line
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_specific_line";
    *helpText     = "16Prompt and Go To Specific Line";

    if (action == 1)
    {
        ask("Go to Which line?");

        size_t newLine = (size_t) (atoi(gAskBuffer) - 1);
//        if (newLine >= 0)
//        {
            if (newLine < LAST_LINE)
            {
                gpDisplay->current_line(newLine);
            }
            else
            {
                attention();
                ask("The go to line must be less than the last line.  Press (ENTER)");
            }
        }
//        else
//        {
//            attention();
//            ask("The go to line number must be positive.  Press (ENTER)");
//        }
//    }
//    else
//    {
//    }

    return FAILURE;
}



/*                              
    Change the foreground color 
                              */
int change_forground_color
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "change_forground_color";
    *helpText     = "02Change Foreground Color";

    if (action)
    {
        gpConsole->next_foreground();

        if (gpConsole->fore_ground_color() == gpConsole->back_ground_color())
        {
            change_forground_color(functionName, helpText);
        }
        else
        {
            /* the fore and back colors are different so the text ought to be readable */
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                              
    Change the background color 
                              */
int change_background_color
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "change_background_color";
    *helpText     = "03Change Background Color";

    if (action)
    {
        gpConsole->next_background();

        if (gpConsole->fore_ground_color() == gpConsole->back_ground_color())
        {
            change_background_color(functionName, helpText);
        }
        else
        {
            /* the fore and back colors are different so the text ought to be readable */
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                                          
    Move the screen 1 column to the right   
                                          */
int move_screen_to_next_column
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_next_column";
    *helpText     = "17Right 1 Column";

    if (action)
    {
        if (gCurrentColumn < MAX_LINE_SIZE)
        {
            gCurrentColumn++;
        }
        else
        {
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                                          
    Move the screen 1 column to the left    
                                          */
int move_screen_to_prev_column
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_prev_column";
    *helpText     = "18Left 1 Column";

    if (action)
    {
        if (gCurrentColumn != 0)
        {
            gCurrentColumn--;
        }
        else
        {
            attention();
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                                                  
    Move the screen left about 1/2 a screen width   
                                                  */
int move_screen_to_prev_half_page
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_prev_half_page";
    *helpText     = "1:Page Left";

    if (action)
    {
        if (gCurrentColumn > gpConsole->cols() / 2)
        {
            gCurrentColumn -= gpConsole->cols() / 2;
        }
        else
        {
            gCurrentColumn = 0;
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                                                      
    Move the screen right about 1/2 of a screen width   
                                                      */
int move_screen_to_next_half_page
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_next_half_page";
    *helpText     = "1;Page Right";

    if (action)
    {
        if (gCurrentColumn + gpConsole->cols() / 2 < MAX_LINE_SIZE)
        {
            gCurrentColumn += gpConsole->cols() / 2;
        }
        else
        {
            attention();
        }
    }
    else
    {
    }

    return FAILURE;
}



/*                                      
    Move the screen to the first column 
                                      */
int move_screen_to_start_of_line
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_start_of_line";
    *helpText     = "1<Beginning of Line";

    if (action)
    {
        gCurrentColumn = 0;
    }
    else
    {
    }

    return FAILURE;
}



/*                                                              
    Move the screen so that the end of the current line appears 
                                                              */
int move_screen_to_end_of_line
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "move_screen_to_end_of_line";
    *helpText     = "1=End Of Line";

    if (action)
    {
        size_t length = strlen(gTextLines[(size_t)(gpDisplay->current_line())]);

        size_t size = gpConsole->cols();

        gCurrentColumn = (length + 1) - (length % size);
    }
    else
    {
    }

    return FAILURE;
}



/*                                          
    flip the show carriage control boolean  
                                          */
int change_visible_carriage_control
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "change_visible_carriage_control";
    if (gTextLines.showCarriageControl)
    {
        *helpText     = "04Hide Carriage Control";
    }
    else
    {
        *helpText     = "04Show Carriage Control";
    }

    if (action)
    {
        gTextLines.change_visible_carriage_control();
    }
    else
    {
    }

    return FAILURE;
}



/*                                  
    flip the radix control boolean  
                                  */
int change_character_radix
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "change_character_radix";

    if (gpDisplay->show_hex_character())
    {
        *helpText     = "05Show Decimal Character";
    }
    else
    {
        *helpText     = "05Show Hex Character";
    }

    if (action)
    {
        gpDisplay->change_show_hex_character();
    }
    else
    {
    }

    return FAILURE;
}


/*                      
    This starts a find  
                      */
int find_first_string
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "find_first_string";
    *helpText     = "20Prompt for String then Find";

    if (action)
    {
        gpFinder->find_first_string();
    }
    else
    {
    }

    return FAILURE;
}



int find_first_byte_string
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "find_first_byte_string";
    *helpText     = "21Prompt for Byte String then Find";

    if (action)
    {
        gpFinder->find_first_byte_string();
    }
    else
    {
    }

    return FAILURE;
}



/*                                                              
    This function positions the screen so that the next         
    non-ascii character is positioned on the current line,      
    with the column containing it at the left hand column of    
    the screen.  This makes it appear in the charSet field      
    of the status bar.                                          
                                                              */
int find_next_non_ascii
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "find_next_non_ascii";
    *helpText     = "22Find Next Non-ASCII";

    if (action)
    {
        /*                                                          
            If we don't find another match, we want to be able to   
            restore the cursor to the last found value              
                                                                  */
        size_t saveCurrentLine   = gpDisplay->current_line();
        size_t saveCurrentOffset = gCurrentColumn;

        /*                                                              
            Let's continue to look at the current line, and possibly    
            additional lines in the file.                               
                                                                      */
        size_t totalLines = gTextLines.size();
        for (;gpDisplay->current_line() < totalLines; gpDisplay->current_line(gpDisplay->current_line() + 1))
        {
            size_t lineLength = strlen(gTextLines[(size_t) gpDisplay->current_line()]);

            for (gCurrentColumn++; gCurrentColumn < lineLength; gCurrentColumn++)
            {
                char c = gTextLines[(size_t) gpDisplay->current_line()][(size_t) gCurrentColumn];
                if (isascii(c))
                {
                }
                else
                {
                    return FAILURE;
                }
            }

            gCurrentColumn = 0;  // BUG ? was initted to -1 which no longer applies
        }

        attention();
        gpDisplay->current_line(saveCurrentLine);
        gCurrentColumn = saveCurrentOffset;
    }
    else
    {
    }

    return FAILURE;
}



/*                                              
    This takes you to the next found pattern    
                                              */
int find_next
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "find_next";
    *helpText     = "24Find Next";

    if (action)
    {
        gpFinder->find_next();
    }
    else
    {
    }

    return FAILURE;
}



/*                                       
    This toggles the sensitivity flag    
                                       */
int change_find_case_sensitivity
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "change_find_case_sensitivity";

    if (gpFinder->case_sensitive())
    {
        *helpText     = "26Set Case Insensitive Search";
    }
    else
    {
        *helpText     = "26Set Case Sensitive Search";
    }

    if (action)
    {
        gpFinder->change_find_case_sensitivity();
    }
    else
    {
    }

    return FAILURE;
}



/*                                                                              
    This function saves the environmental settings into the configuration file  
                                                                              */
int save_user_preferences
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "save_user_preferences";
    *helpText     = "07Save Settings to File";

    if (gUserPreferenceFileExists)
    {
        if (action)
        {
            gpApp->save_user_preferences();
        }
        else
        {
        }
    }
    else
    {
        attention();
    }

    return FAILURE;
}


/*                                                  
    This gets called for undefined keystrokes.      
    It basically puts up a 1/2 screen showing       
    the supported keystrokes along with the         
    help text.                                      
                                                  */
int invalid_function
(
    const char **functionName,
    const char **helpText,
    int action = 1
)
{
    *functionName = "invalid_function";
    *helpText     = NULL;

    if (action)
    {
        /* Something didn't work as expected */
        attention();

        /* Clear the half screen */
        attrset(gRulerColor);
        for (size_t z = 0; z < SCREEN_SIZE / 2; z++)
        {
             gpConsole->moveto( (int) z, 0);
             for (size_t y = 0; y < SCREEN_WIDTH; y++)
             {
                 *gpConsole << " ";
             }
             *gpConsole << endl;
        }

        /*                                                          
            This gets the help text associated with each defined    
            keystroke, and displays the associated help text        
            at the correct location.                                
                                                                  */
        for (size_t key = 0; key < KEY_MAX; key++)
        {
            const char *help = gpKeyboard->help_text(key);

            if (help != NULL)
            {
                std::ostringstream oss;

                int column = help[0] - '0';
                int row    = help[1] - '0';
                gpConsole->moveto(row, column * 40);

                oss << std::setw(15) << std::right << gKeyNames[key].keyName << " "
                    << std::setw(20) << std::left  << (const char *)(help + 2) << '\0';
                *gpConsole << oss.str().c_str();
            }
            else
            {
            }
        }

#ifdef _WIN64
        gpConsole->moveto((int) SCREEN_SIZE / 2 - 12, 10);
        *gpConsole << "Mouse Wheel Up,   Page Up"     << '\0';

        gpConsole->moveto((int) SCREEN_SIZE / 2 - 11, 10);
        *gpConsole << "Mouse Wheel Down, Page Down"   << '\0';

        gpConsole->moveto((int) SCREEN_SIZE / 2 - 10, 10);
        *gpConsole << "Left  Click,      Page Left"   << '\0';

        gpConsole->moveto((int) SCREEN_SIZE / 2 -  9, 10);
        *gpConsole << "Right Click,      Page Right"  << '\0';
#endif

        /* Show the key that was pressed to make it easy to identify the key */
        gpConsole->moveto((int) SCREEN_SIZE / 2 - 6, 0);
        *gpConsole << "Last Key Pressed " << gKeyNames[(size_t) gLastKeyPressed].keyName << '\0';

        gpConsole->moveto((int) SCREEN_SIZE / 2 - 4, 0);
        *gpConsole << "The list above is all valid keystrokes with the associated behavior." << '\0';


        /*                                                  
            Literally.  Don't press a key to continue.      
            Press a key to do your next action              
                                                          */
        gpConsole->moveto((int) SCREEN_SIZE / 2 - 2, 0);
        *gpConsole << "Press any valid keystroke to continue." << '\0';

        gpConsole->flush();

        return HELP;
    }
    else
    {
        return FAILURE;
    }
}

ControlFunction allControlFunction[] = 
{
    quit_program,
    move_screen_to_next_page,
    move_screen_to_prev_page,
    move_screen_to_next_line,
    move_screen_to_prev_line,
    move_screen_to_end_of_file,
    move_screen_to_top_of_file,
    move_screen_to_specific_line,
    change_forground_color,
    change_background_color,
    move_screen_to_next_column,
    move_screen_to_prev_column,
    move_screen_to_prev_half_page,
    move_screen_to_next_half_page,
    move_screen_to_start_of_line,
    move_screen_to_end_of_line,
    change_visible_carriage_control,
    change_character_radix,
    find_first_string,
    find_first_byte_string,
    find_next_non_ascii,
    find_next,
    change_find_case_sensitivity,
    save_user_preferences,
    invalid_function
};


