
/*                                                                  
    Finder.hpp                                                      
    EvilTeach - MPL 1.1                                             
                                                                  */

#ifndef FINDER_HPP
#define FINDER_HPP

class Finder
{
    public :

        Finder
        (
            void
        );


        /*                                                                          
            Get the state variables, so they can be displayed on the status line    
                                                                                  */
        const char * configuration
        (
            void
        );


        /*                                                              
            API function to allow the user to toggle the sensitivity    
                                                                      */
        void change_find_case_sensitivity
        (
            void
        );


        /*
        */
        bool case_sensitive
        (
            void
        );


        /*                                                   
            Ask for a string, then find it                   
                                                           */
        bool find_first_string  /* False if found          */
        (                       /* True if no more matches */
            void
        );


        /*                                                   
            Ask for a Byte string then find it               
                                                           */
        bool find_first_byte_string  /* False if found          */
        (                            /* True if no more matches */
            void
        );


        /*                                          
            Find the next match                     
                                                  */
        bool find_next /* False if found          */
        (              /* true if no more matches */
            void
        );


        /*                                                              
            This function tells the caller if the current column offset 
            is part of the current match key                            
                                                                      */
        bool in_search_key  /* true if it is, false if it is not */
        (
            size_t position
        );

        /*                                                          
            returns true when we are currently performing a fine,   
            otherwise false                                         
                                                                  */
        bool find_in_progress
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

        bool   _caseSensitiveCompare;
        char   _searchKey[KEYBOARD_LIMIT];

        /*                                                          
            This holds the column offsets to the match positions    
            on the gCurrentLine.  There can be zero or more of them 
                                                                  */
        std::vector<size_t> _matchColumns;

        /*                                                          
            This holds the column offset of the next place to look  
            for a match                                             
                                                                  */
        size_t _matchColumn;

        /*                                                          
            This function hides the issue of whether we need case   
            sensitivity or not.                                     
                                                                  */
        const char *find_string
        (
            const char *lookInHere,
            const char *forThisMatch
        );


        /*                                                      
            This function finds all of the search keys on the   
            current line, and saves them for find processing    
                                                              */
        void find_patterns_on_line
        (
            void
        );

        bool _findInProgress;
};


/*                                      
    This thing knows how to do finding. 
                                      */
extern Finder *gpFinder;

#endif
