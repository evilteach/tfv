
/*                                                                  
    bindable_functions.hpp                                          
    EvilTeach - MPL 1.1                                             
                                                                  */


#ifndef BINDABLE_FUNCTIONS_HPP
#define BINDABLE_FUNCTIONS_HPP

/*                                                                          
    This is the function signature of the functions that are hooked to the  
    various keystrokes                                                      
                                                                          */
typedef int (*ControlFunction)  /* 1 on success, 0 on failure, 2 on quit_program */
        (
              const char **functionName,
              const char **helpText,
              int action        /* 0 = do function, 1 = return help */
        );

extern ControlFunction allControlFunction[];

/*                                      
    Cause the program to exit           
                                      */
int quit_program
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                      
    Move 1 screen down  
                      */
int move_screen_to_next_page
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                      
    Move 1 screen up    
                      */
int move_screen_to_prev_page
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                              
    Move the screen 1 line down 
                              */
int move_screen_to_next_line
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                              
    Move the screen 1 line up   
                              */
int move_screen_to_prev_line
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                          
    Move the screen to the end of the file  
                                          */
int move_screen_to_end_of_file
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                          
    Move the screen to the top of the file  
                                          */
int move_screen_to_top_of_file
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                                                      
    move the screen so that the user specified line is above the ruler  
                                                                      */
int move_screen_to_specific_line
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                              
    Change the foreground color 
                              */
int change_forground_color
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                              
    Change the background color 
                              */
int change_background_color
(
    const char **functionName,
    const char **helpText,
    int action
);;



/*                                          
    Move the screen 1 column to the right   
                                          */
int move_screen_to_next_column
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                          
    Move the screen 1 column to the left    
                                          */
int move_screen_to_prev_column
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                                  
    Move the screen left about 1/2 a screen width   
                                                  */
int move_screen_to_prev_half_page
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                                      
    Move the screen right about 1/2 of a screen width   
                                                      */
int move_screen_to_next_half_page
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                      
    Move the screen to the first column 
                                      */
int move_screen_to_start_of_line
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                                              
    Move the screen so that the end of the current line appears 
                                                              */
int move_screen_to_end_of_line
(
    const char **functionName,
    const char **helpText,
    int action
);


/*                                          
    flip the show carriage control boolean  
                                          */
int change_visible_carriage_control
(
    const char **functionName,
    const char **helpText,
    int action
);;



/*                                  
    flip the radix control boolean  
                                  */
int change_character_radix
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                      
    This starts a find  
                      */
int find_first_string
(
    const char **functionName,
    const char **helpText,
    int action
);



int find_first_byte_string
(
    const char **functionName,
    const char **helpText,
    int action
);


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
    int action
);


/*                                              
    This takes you to the next found pattern    
                                              */
int find_next
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                       
    This toggles the sensitivity flag    
                                       */
int change_find_case_sensitivity
(
    const char **functionName,
    const char **helpText,
    int action
);


/*                                                                              
    This function saves the environmental settings into the configuration file  
                                                                              */
int save_user_preferences
(
    const char **functionName,
    const char **helpText,
    int action
);



/*                                              
    This gets called for undefined keystrokes.  
                                              */
int invalid_function
(
    const char **functionName,
    const char **helpText,
    int action
);

#endif


