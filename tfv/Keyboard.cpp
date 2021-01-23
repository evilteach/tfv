
/*                                                                  
    Keyboard.cpp                                                    
    EvilTeach - MPL 1.1                                             
                                                                  */


#include "common.hpp"


/*                                                              
    This function is the standard interface to the keyboard.    
                                                              */
int getkey
(
    void
)
{
    gLastKeyPressed = gpConsole->get_key();

    return gLastKeyPressed;
}

/*                                                                      
    This function sets one of the preferences that are loaded           
    from the preference file.   It returns true of the preference       
    applies to this object.  Otherwise it returns false                 
                                                                      */
bool Keyboard::set_preference
(
    const std::string  preference
)
{

    if ((preference.length() > 6) && (preference.substr(0, 6) == "KEYDEF"))
    {
        std::string keyValue = preference.substr(7,50);
        size_t x = keyValue.find("=");
        if (x != keyValue.npos)
        {
            std::string functionName = keyValue.substr(x + 1);
            keyValue = keyValue.substr(0, x);

            std::cout << x << std::endl;
        }
        else
        {
            std::cout << x << std::endl;
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
void Keyboard::save_preferences
(
    std::ostream &udf
)
{
//    udf << "CONSOLE_FOREGROUND:" << fore_ground_color() << std::endl;
    udf << "KEYDEF:0x41=move_screen_to_prev_page" << std::endl;
}

Keyboard::Keyboard
(
    void
)
{
    /*                                                  
        There are a lot of possible key functions.      
        Each one of them should do nothing by default,  
        except maybe beep.                              
                                                      */
    int z;
    for (z = 0; z < KEY_MAX; z++)
    {
        associate_function_with_keystroke(invalid_function, z);
    }

    /* Other than that, we are free to map the keys however we want */

    /* Default values for all platforms */

    associate_function_with_keystroke(change_background_color,         'E', 'e');
    associate_function_with_keystroke(change_forground_color,          'R', 'r');
    associate_function_with_keystroke(change_character_radix,          'T', 't');
    associate_function_with_keystroke(change_visible_carriage_control, 'Y', 'y');

    associate_function_with_keystroke(quit_program,                    'Q', 'q');

    associate_function_with_keystroke(save_user_preferences,                        0x01);  /* CTRL_A */

    associate_function_with_keystroke(change_find_case_sensitivity,    'C', 'c');


    associate_function_with_keystroke(find_first_string,                            0x06);  /* CTRL_F */
    associate_function_with_keystroke(find_first_byte_string,                       0x02);  /* CTRL_B */
    associate_function_with_keystroke(find_next_non_ascii,                          0x0E);  /* CTRL_N */
    associate_function_with_keystroke(find_next,                       'F', 'f');

    associate_function_with_keystroke(move_screen_to_top_of_file,                   0x15);  /* CTRL_U */
    associate_function_with_keystroke(move_screen_to_end_of_file,                   0x10);  /* CTRL_P */

    associate_function_with_keystroke(move_screen_to_start_of_line,    'U', 'u');
    associate_function_with_keystroke(move_screen_to_end_of_line,      'P', 'p');

    associate_function_with_keystroke(move_screen_to_next_line,        'J', 'j');
    associate_function_with_keystroke(move_screen_to_prev_line,        'K', 'k');


    associate_function_with_keystroke(move_screen_to_next_column,      'L', 'l');
    associate_function_with_keystroke(move_screen_to_prev_column,      'H', 'h');
 
    associate_function_with_keystroke(move_screen_to_prev_half_page,                0x09);  /* CTRL_I */
    associate_function_with_keystroke(move_screen_to_next_half_page,                0x0F);  /* CTRL_O */


    //associate_function_with_keystroke(move_screen_to_prev_page,      'I', 'i');

    associate_function_with_keystroke(move_screen_to_specific_line,                 0x07);  /* CTRL_G */

    associate_function_with_keystroke(move_screen_to_next_line,        KEY_DOWN  );
    associate_function_with_keystroke(move_screen_to_next_page,        KEY_NPAGE );
    associate_function_with_keystroke(move_screen_to_prev_column,      KEY_LEFT  );
    associate_function_with_keystroke(move_screen_to_next_column,      KEY_RIGHT );
    associate_function_with_keystroke(move_screen_to_end_of_line,      KEY_END   );
    associate_function_with_keystroke(move_screen_to_start_of_line,    KEY_HOME  );
    associate_function_with_keystroke(move_screen_to_prev_line,        KEY_UP    );
    associate_function_with_keystroke(move_screen_to_prev_page,        KEY_PPAGE );


#ifdef _WIN64

    associate_function_with_keystroke(find_next,                       KEY_F(3)  );
    associate_function_with_keystroke(move_screen_to_prev_half_page,   CTL_LEFT  );
    associate_function_with_keystroke(move_screen_to_next_half_page,   CTL_RIGHT );
    associate_function_with_keystroke(move_screen_to_end_of_file,      CTL_END   );
    associate_function_with_keystroke(move_screen_to_top_of_file,      CTL_HOME  );
#else
    associate_function_with_keystroke(find_next,                       KEY_F0  );
#endif
}


/*                                                              
    This function attaches a function to one or two keystrokes. 
                                                              */
void Keyboard::associate_function_with_keystroke
(
    ControlFunction funct,
    int             key1,
    int             key2
)
{
    if (key1 < KEY_MAX + 1)
    {
        controlFunctions[key1] = funct;
    }
    else
    {
    }

    if (key2 < KEY_MAX + 1 && key2 != -1)
    {
        controlFunctions[key2] = funct;
    }
    else
    {
    }
}


/*                                                                       
    This function gets a key, and dispatches it to the correct function. 
                                                                        */
int Keyboard::process /* Returns 0 on success, 1 on no function associated with key, 2 on quit_program */
(
    void
)
{
    int returnStatus = 0;

    int key = getkey();
    if (key < KEY_MAX + 1)
    {
        const char *functionName = NULL;
        const char *helpText     = NULL;

        returnStatus = controlFunctions[key](&functionName, &helpText, 1);
        if (returnStatus == 1)
        {
            /* The key was processed correctly */
        }
        else
        {
            if (returnStatus == HELP)
            {
                /* We showed the help screen, let's try to process again */
                returnStatus = process();
            }
            else
            {
            }
        }
    }
    else
    {
        /* This is not a recognized key that we can deal with */
    }

    return returnStatus;
}


/*                                                                  
    This function prompts and waits for a key.  It is generally     
    used after a message of some sort is displayed                  
                                                                  */
void Keyboard::press_a_key
(
    Console *_console
)
{
    *_console << "\nPress a key to continue\n";

    getkey();
}


/*                                                              
    This function gets the name of the function being invoked.  
                                                              */
const char *Keyboard::function_name
(
    size_t sub
)
{
    static const char *functionName = NULL;
    static const char *helpText     = NULL;

    controlFunctions[sub](&functionName, &helpText, 0);

    return functionName;
}

/*                                                                      
    This function gets the help text for the function being invoked.    
                                                                      */
const char *Keyboard::help_text
(
    size_t sub
)
{
    static const char *functionName = NULL;
    static const char *helpText     = NULL;

    if (controlFunctions[sub] != NULL)
    {
        controlFunctions[sub](&functionName, &helpText, 0);
    }
    else
    {
        functionName = NULL;
        helpText     = NULL;
    }

    return helpText;
}

KeyNames::KeyNames
(
    void
)
{
    size_t z;

    /* Pick a default representation for all possible keys */
    for (z = 0; z < KEY_MAX; z++)
    {
        int chars = sprintf(keyNames[z].keyName, "<%u>", (unsigned int) z);
        (void) chars;
        assert(chars < (int) sizeof keyNames[0].keyName);
    }

    /* Control Keys get a ^ */
    for (z = 0; z < 32; z++)
    {
        int chars = sprintf(keyNames[z].keyName, "<^%c>", (char)(z + '@'));
        (void) chars;
        assert(chars < (int) sizeof keyNames[0].keyName);
    }

    /* Each printable ascii is displayed as itself */
    for (z = 32; z < 127; z++)
    {
        int chars = sprintf(keyNames[z].keyName, "<%c>", (char)(z));
        (void) chars;
        assert(chars < (int) sizeof keyNames[0].keyName);
    }

    /* The delete character is not printable */
    strcpy(keyNames[127].keyName, "<DEL>");

    /* Non ascii has a display representation*/
    for (z = 128; z < 256; z++)
    {
        int chars = sprintf(keyNames[z].keyName, "<%c>", (char)(z));
        (void) chars;
        assert(chars < (int) sizeof keyNames[0].keyName);
    }

    /* Function Keys and miscellaneous */
    for (z = 1; z < 13; z++)
    {
        int chars = sprintf(keyNames[264 + z].keyName, "<F%u>",(unsigned int) z);
        (void) chars;
        assert(chars < (int) sizeof keyNames[0].keyName);
    }

    strcpy(keyNames[262].keyName, "<HOME>");    // BUG is this were the bad names are coming from?
    strcpy(keyNames[358].keyName, "<END>");
    strcpy(keyNames[447].keyName, "<^HOME>");
    strcpy(keyNames[448].keyName, "<^END>");

    strcpy(keyNames[339].keyName, "<PAGE_UP>");
    strcpy(keyNames[338].keyName, "<PAGE_DOWN>");
    strcpy(keyNames[480].keyName, "<^PAGE_UP>");
    strcpy(keyNames[481].keyName, "<^PAGE_DOWN>");

    strcpy(keyNames[260].keyName, "<LEFT_ARROW>");
    strcpy(keyNames[261].keyName, "<RIGHT_ARROW>");
    strcpy(keyNames[443].keyName, "<^LEFT_ARROW>");
    strcpy(keyNames[444].keyName, "<^RIGHT_ARROW>");

    strcpy(keyNames[258].keyName, "<DOWN_ARROW>");
    strcpy(keyNames[259].keyName, "<UP_ARROW>");
}

/*                                                                              
    This holds the integer value of last key that was pressed for status line   
    and debugging purposes.                                                     
                                                                              */
int gLastKeyPressed = 0;


/*                                                          
    This holds all of the key names, so we don't have to    
    screw around in the caller to get the name              
                                                          */
KeyNames gKeyNames;


/*                                                  
    This is the instance of the keyboard processor  
                                                  */
Keyboard *gpKeyboard = NULL;

