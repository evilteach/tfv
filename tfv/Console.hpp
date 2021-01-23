
/*                                                                  
    Console.cpp                                                     
    EvilTeach - MPL 1.1                                             
                                                                  */

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

/*                                                              
    Console.hpp                                                 
                                                                
    EvilTeach 2013 - MPL 1.1                                    
                                                                
    This header interfaces between curses and the way I want    
    to work with the console                                    
                                                              */

#include <vector>

#define NCURSES_MOUSE_VERSION 
#include <curses.h>

/*                                                              
    This is the physical number of rows available on the screen 
                                                              */
#define SCREEN_SIZE (gpConsole->rows())

/*                                                                  
    This is the physical number of columns available on the screen  
                                                                  */
#define SCREEN_WIDTH (gpConsole->cols())

/*                                                                      
    This is the physical number of rows that can be displayed on screen 
    due to the status bar and the ruler bar                             
                                                                      */
#define TFV_PAGE_SIZE   (SCREEN_SIZE - 1 - 1)

/*                                              
    This holds the number of lines in the file. 
    They are subscripted 0 .. TOTAL_LINES - 1   
    Note that this value is never zero.         
                                              */
#define TOTAL_LINES   (gTextLines.size())

/*                                                      
    This is the subscript of the last line of the file  
                                                      */
#define LAST_LINE (size_t)((TOTAL_LINES - 1))

/*                                   
    Buffer limit for keyboard input  
                                   */
#define KEYBOARD_LIMIT (1000)


/*                                                  
    We can deal with files with record lengths up   
    to and including this value.                    
                                                  */
#define MAX_LINE_SIZE 99999


void attention
(
    void
);

/*                                                                              
    This holds the value that the user typed in response to things like find    
                                                                              */
extern char gAskBuffer[KEYBOARD_LIMIT]; // BUG belongs in keyboard



/*                                                          
    This function prompts and gets a command from the user  
                                                          */
int ask
(
    const char *prompt
);


class Console
{
    public :

        Console
        (
            void
        );

        ~Console
        (
            void
        );

        int get_key();

        Console& operator<<
        (
            const char *iValue
        );

        Console& operator<<
        (
            int iValue
        );

        Console& operator<<
        (
            size_t iValue
        );

        Console& operator<<
        (
            const char iValue
        );

        Console& operator<<(Console& (*m)(Console&));

        void flush
        (
            void
        );

        void moveto
        (
            int row,
            int column
        );

        size_t rows
        (
            void
        );

        size_t cols
        (
            void
        );

        void home
        (
            void
        );

        void cls
        (
            void
        );

        void get_yx
        (
            int&   oY,
            int&   oX
        );

        void set_colors
        (
            void
        );

        void next_foreground
        (
            void
        );

        void next_background
        (
            void
        );

        /*
        */
        short fore_ground_color
        (
            void
        );

        /*
        */
        short back_ground_color
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
            const std::string  setting
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

        WINDOW *_window;

        size_t _rows;
        size_t _cols;

        int foreground;
        int background;

#ifdef _WIN64
        MEVENT mouseInput;
#endif
};


Console& flush(Console& that);

Console& endl(Console& that);

/*                                                      
    Once the console is set up, all of the actual usage 
    can get to it directly                              
                                                      */
extern Console *gpConsole;

#endif

