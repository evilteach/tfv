
/*                                                                  
    App.hpp                                                         
    EvilTeach - MPL 1.1                                             
                                                                  */

#include "common.hpp"


App::App
(
    void
)
{
    gpConsole           = &_console;
    gpKeyboard          = &_keyboard;
    gpFinder            = &_finder;
    gpDisplay           = &_display;
}

App::~App
(
    void
)
{
}

int App::run
(
    int   iArgsCount,
    char *iArgs[]
)
{
    int returnStatus = 0;

    try
    {
        if (iArgsCount == 2)
        {
            gpFileName = iArgs[1];

            returnStatus = gTextLines.load_file();
            if (returnStatus == 0)
            {
                load_user_preferences();

                do
                {
                    _display.display_the_screen();
                }
                while(_keyboard.process() == 0);
            }
            else
            {
                if (returnStatus == 2)
                {
                    *gpConsole << iArgs[1] << " is an empty file." << endl;
                }
                else
                {
                    *gpConsole << iArgs[1] << " Error " << returnStatus << endl;
                }

                gpKeyboard->press_a_key(gpConsole);
            }
        }
        else
        {
            *gpConsole << "Usage tfv file.txt" << endl;

            gpKeyboard->press_a_key(gpConsole);
        }
    }
    catch (std::exception &e)
    {
        _console << "Exception " << e.what() << endl;
        gpKeyboard->press_a_key(gpConsole);

        returnStatus = 3;
    }
    catch(...)
    {
        _console << "Unexpected exception " << endl;
        gpKeyboard->press_a_key(gpConsole);

        returnStatus = 3;
    }

    return returnStatus;
}



/*                                                                              
    This function loads the configuration values out of the environment file    
                                                                              */
void App::load_user_preferences
(
    void
)
{
    /* Have they set an environment variable to tell us where to get the values? */
    char *env = getenv("TFV_FILE");
    if (env != NULL)
    {
        gUserPreferenceFileExists = true;
        strcpy(gUserPreferenceFileName, env);

        std::ifstream upf (gUserPreferenceFileName);
        if (upf.is_open())
        {
            std::string preference;

            for (;;)
            {
                upf >> preference;
                if (upf.eof())
                {
                    break;
                }
                else
                {
                    _finder.set_preference(preference);
                    _console.set_preference(preference);
                    gTextLines.set_preference(preference);
                    _display.set_preference(preference);
                    _keyboard.set_preference(preference);
                }
            }
        }
        else
        {
            // BUG what to do
        }
    }
    else
    {
        /* There is no user preference file, so don't worry about it */
    }
}



/*                                                                              
    This function saves the environmental settings into the configuration file  
                                                                              */
int App::save_user_preferences
(
    void
)
{
    if (gUserPreferenceFileExists)
    {
        std::ofstream upf (gUserPreferenceFileName);

        _finder.save_preferences(upf);
        _console.save_preferences(upf);
        gTextLines.save_preferences(upf);
        _display.save_preferences(upf);
        _keyboard.save_preferences(upf);
    }
    else
    {
        attention();
    }

    return FAILURE;
}


/*                                                                  
    This holds the file name that is supplied via the command line  
                                                                  */
char *gpFileName = NULL;

/*
*/
App *gpApp = NULL;

