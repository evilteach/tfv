
/*                                                                  
    common.cpp                                                      
    EvilTeach - MPL 1.1                                             
                                                                  */

#include "common.hpp"

/*                                                                                  
    This holds a chunk of memory large enough to hold the entire file in memory.    
    Hence compiling as a 64 bit app make sense to allow large files to be examined. 
                                                                                  */
char *gpAllTextInMemory = NULL;



/*                                                                      
    This holds a the file name portion, so that it can be displayed     
    on the status line.                                                 
                                                                      */
std::string gFileNameOnly;



/*                                                                      
    This is set to true, when the TFV_FILE environment variable is set, 
    which tells the program where the configuration file is located.    
                                                                      */
bool gUserPreferenceFileExists = false;



/*                                                                              
    This holds the name of the file that contains configuration information.    
                                                                              */
char gUserPreferenceFileName[1000] = {0};


/*                                                                          
    This holds the actual offset that the user has picked.  It is used to   
    decide which screen should currently be viewed.                         
                                                                          */
size_t gCurrentColumn = 0;

/*                                                                          
    When this is true, the status line character is hex instead of decimal. 
                                                                          */
bool gUseHexRadix = false;

/*                                                  
    This holds the color choice that the user set.  
                                                  */
chtype gUserColorChoice;

/*                                      
    This holds the color of the ruler   
                                      */
chtype gRulerColor;

/*                                                              
    This holds the bolded version of the users color choice.    
                                                              */
chtype gUserColorBold;

TextLines gTextLines;


/*                                                                  
    This function sets one of the preferences that are loaded       
    from the preference file.   It returns true of the preference   
    applies to this object.  Otherwise it returns false             
                                                                  */
bool TextLines::set_preference
(
    const std::string  preference
)
{

    if ((preference.length() > 9) && (preference.substr(0, 9) == "TEXTLINES"))
    {
        if (preference.length() > 26 && preference.substr(0, 27) == "TEXTLINES_CARRIAGE_CONTROL:")
        {
            gTextLines.show_carriage_control(atoi(preference.substr(27).c_str()));
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
void TextLines::save_preferences
(
    std::ostream &udf
)
{
    udf << "TEXTLINES_CARRIAGE_CONTROL:" << show_carriage_control() << std::endl;
}
