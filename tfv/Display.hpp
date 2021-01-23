
/*                                                                  
    Display.hpp                                                     
    EvilTeach - MPL 1.1                                             
                                                                  */

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

/*
*/
class Display
{
    public :

        Display
        (
            void
        );

        ~Display
        (
            void
        );

        /*                                                                                  
            This function displays 1 screens worth of information.                          
            Text before the file, and after the file displays as tildes.                    
                                                                                            
            The current line is displayed reverse of the other gUserColorChoice.            
                                                                                            
            if a find operation is in progress, the match string will be reversed instead   
                                                                                          */
        void display_the_screen
        (
            void
        );


        bool show_hex_character
        (
            void
        );

        void change_show_hex_character
        (
            void
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

        size_t current_line
        (
            void
        )
        {
            return _currentLine;
        }

        void current_line
        (
            size_t newLine
        )
        {
            _currentLine = newLine;
        }

    protected :

        bool _showHexCharacter;
        size_t  _currentLine;

    private :

              Display           (const Display &);
        const Display &operator=(const Display &);
};

extern Display *gpDisplay;

#endif

