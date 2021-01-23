
/*                                                                  
    App.hpp                                                         
    EvilTeach - MPL 1.1                                             
                                                                  */

#ifndef APP_HPP
#define APP_HPP

#include "Keyboard.hpp"
#include "Finder.hpp"
#include "Display.hpp"

class App
{
    public :

        App
        (
            void
        );

        ~App
        (
            void
        );

        int run
        (
            int   iArgsCount,
            char *iArgs[]
        );

        /*                                                                              
            This function loads the configuration values out of the environment file    
                                                                                      */
        void load_user_preferences
        (
            void
        );

        /*                                                                              
            This function saves the environmental settings into the configuration file  
                                                                                      */
        int save_user_preferences
        (
            void
        );

    protected :

        Console           _console;
        Keyboard          _keyboard;
        Finder            _finder;
        Display           _display;

              App           (const App &);
        const App &operator=(const App &);
};


extern char *gpFileName;

extern App *gpApp;

#endif
