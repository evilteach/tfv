
/*                                                                  
    Keyboard.hpp                                                    
    EvilTeach - MPL 1.1                                             
                                                                  */

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "bindable_functions.hpp"

/*                                                              
    This function is the standard interface to the keyboard.    
                                                              */
int getkey
(
    void
);


/*                                                                                  
    Curses appears to have some problems for cross platform keystrokes.             
    This class exists, as a first step in isolating out the cross platform issues.  
                                                                                  */
class Keyboard
{
    public :

        Keyboard
        (
            void
        );


        /*                                                              
            This function attaches a function to one or two keystrokes. 
                                                                      */
        void associate_function_with_keystroke
        (
            ControlFunction funct,
            int             key1,
            int             key2 = -1
        );


        /*                                                                       
            This function gets a key, and dispatches it to the correct function. 
                                                                               */
        int process /* Returns 0 on success, 1 on no function associated with key, 2 on quit_program */
        (
            void
        );


        /*                                                                  
            This function prompts and waits for a key.  It is generally     
            used after a message of some sort is displayed                  
                                                                          */
        void press_a_key
        (
            Console *_console
        );


        /*                                                              
            This function gets the name of the function being invoked.  
                                                                      */
        const char *function_name
        (
            size_t sub
        );

        /*                                                                      
            This function gets the help text for the function being invoked.    
                                                                              */
        const char *help_text
        (
            size_t sub
        );

        /*                                                                  
        This function sets one of the preferences that are loaded       
        from the preference file.   It returns true of the preference   
        applies to this object.  Otherwise it returns false             
        */
        bool set_preference
        (
            const std::string  preference
        );

        /*                                                                      
        This function saves all of the objects preferences into the             
        output stream                                                           
                                                                              */
        void save_preferences
        (
            std::ostream &udf
        );

    protected :

        /*                                                         
            This holds the mappings between all of the keystrokes  
            and their corresponding functions.                     
                                                                 */
        ControlFunction controlFunctions[KEY_MAX];
};




/*                                                              
    This class holds a textual representation of a keystroke.   
                                                              */
class KeyName
{
    public :

        KeyName
        (
            void
        )
        {
            memset(keyName, '\0', sizeof(keyName));
            helpOrder = 0;  /* Don't display help for this keystroke */
        }

        char keyName[20];
        int  helpOrder;
};


/*                                                          
    This class holds the textual representation for all     
    keystrokes that are available on this platform.         
                                                          */
class KeyNames
{
    public :

        KeyNames
        (
            void
        );

        /*                                              
            Return the name associated with the key     
                                                      */
        KeyName &operator[]
        (
            size_t sub
        )
        {
            if (sub < KEY_MAX + 1)
            {
            }
            else
            {
                /* This can't happen, so we display and fail       */
                std::cout << "operator[] on " << sub << std::endl;
                exit(1);
            }

            return keyNames[sub];
        }

    protected :

        KeyName keyNames[KEY_MAX];
};

/*                                                                              
    This holds the integer value of last key that was pressed for status line   
    and debugging purposes.                                                     
                                                                              */
extern int gLastKeyPressed;

/*                                                          
    This holds all of the key names, so we don't have to    
    screw around in the caller to get the name              
                                                          */
extern KeyNames gKeyNames;

extern Keyboard *gpKeyboard;

#endif
