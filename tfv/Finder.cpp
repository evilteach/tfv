
/*                                                                  
    Finder.cpp                                                      
    EvilTeach - MPL 1.1                                             
                                                                  */

#include <sstream>

#include "Console.hpp"

#include "Finder.hpp"

#include "common.hpp"

Finder::Finder
(
    void
) :
    _caseSensitiveCompare(false),
    _findInProgress(false)
{
}


/*                                                                          
    Get the state variables, so they can be displayed on the status line    
                                                                          */
const char * Finder::configuration
(
    void
)
{
    if (case_sensitive())
    {
        return "Find Case Sensitive";
    }
    else
    {
        return "Find Case Insensitive";
    }
}


/*                                                              
    API function to allow the user to toggle the sensitivity    
                                                              */
void Finder::change_find_case_sensitivity
(
    void
)
{
    _caseSensitiveCompare = ! _caseSensitiveCompare;
}


/*
*/
bool Finder::case_sensitive
(
    void
)
{
    return _caseSensitiveCompare;
}



/*                                                   
    Ask for a string, then find it                   
                                                   */
bool Finder::find_first_string  /* False if found          */
(                       /* True if no more matches */
    void
)
{
    ask("Enter the String to find ");

    /* The ask buffer contains the string to search for */

    strcpy(_searchKey, gAskBuffer);

    _findInProgress = true;
    _matchColumn = 0;
    return find_next();
}

/*                                                   
    Ask for a Byte string then find it               
                                                   */
bool Finder::find_first_byte_string  /* False if found          */
(                            /* True if no more matches */
    void
)
{
    ask("Enter Byte String to find ");

    /*                      
        Decimal             
                            
            d,65,66,127,255 
                            
        Hex                 
                            
            x,41,42,f3      
                          */

    if (strlen(gAskBuffer) > 0)
    {
        std::string pattern = "";

        std::stringstream ss(gAskBuffer + 2);

        if (toupper(gAskBuffer[0]) == 'D')
        {
            int i;
            while (ss >> i)
            {
                pattern += (char) i;

                if (ss.peek() == ',')
                    ss.ignore();
            }
        }
        else
        {
            if (toupper(gAskBuffer[0]) == 'X')
            {
                int i;
                while (ss >> std::hex >> i)
                {
                    pattern += (char) i;

                    if (ss.peek() == ',')
                        ss.ignore();
                }
            }
            else
            {
                attention();
                return true;
            }
        }

        strcpy(_searchKey, pattern.c_str());

        _findInProgress = true;
        _matchColumn = 0;
        return find_next();
    }
    else
    {
        return true;
    }
}



/*                                          
    Find the next match                     
                                          */
bool Finder::find_next /* False if found          */
(              /* true if no more matches */
    void
)
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
    for (;gpDisplay->current_line() < (int) totalLines; gpDisplay->current_line(gpDisplay->current_line() + 1))
    {
        find_patterns_on_line();
        for (size_t z = 0 ; z < _matchColumns.size(); z++)
        {
            if (_matchColumns[z] > _matchColumn)
            {
                /* Move to that new match */
                _matchColumn = _matchColumns[z] + 1;

                gCurrentColumn = _matchColumns[z];
                gCurrentColumn -= gCurrentColumn % SCREEN_WIDTH;

                return false;
            }
            else
            {
            }
        }

        _matchColumn = 0;
        gCurrentColumn = 0;
    }

    gpDisplay->current_line(saveCurrentLine);
    gCurrentColumn = saveCurrentOffset;

    _findInProgress = false;
    attention();

    return true;
}



/*                                                              
    This function tells the caller if the current column offset 
    is part of the current match key                            
                                                              */
bool Finder::in_search_key  /* true if it is, false if it is not */
(
    size_t position
)
{
    bool inKey = false;

    if (_matchColumns.size() > 0)
    {
        if (position >= _matchColumn - 2)
        {
            if (position < _matchColumn - 2 + strlen(_searchKey))
            {
                inKey = true;
            }
            else
            {
                /* If there is a match, we are after it */
            }
        }
        else
        {
            /* If there is a match, we are before it */
        }
    }
    else
    {
        /* There are no matches on this line */
    }

    return inKey;
}

/*                                                          
    returns true when we are currently performing a fine,   
    otherwise false                                         
                                                          */
bool Finder::find_in_progress
(
    void
)
{
    return _findInProgress;
}

/*                                                                  
    This function sets one of the preferences that are loaded       
    from the preference file.   It returns true of the preference   
    applies to this object.  Otherwise it returns false             
                                                                  */
bool Finder::set_preference
(
    const std::string  preference
)
{
    if ((preference.length() > 5) && (preference.substr(0, 6) == "FINDER"))
    {
        if (preference.length() > 24 && preference.substr(0, 24) == "FINDER_CASE_SENSITIVITY:")
        {
            _caseSensitiveCompare = (preference.substr(24, 1) == "1") ? true : false;
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
void Finder::save_preferences
(
    std::ostream &udf
)
{
    udf << "FINDER_CASE_SENSITIVITY:" << case_sensitive() << std::endl;
}



/*                                                          
    This function hides the issue of whether we need case   
    sensitivity or not.                                     
                                                          */
const char *Finder::find_string
(
    const char *lookInHere,
    const char *forThisMatch
)
{
    if (_caseSensitiveCompare)
    {
        return strstr(lookInHere, forThisMatch);
    }
    else
    {
        /* Iterate through the searched string */
        const char *cp =  lookInHere;

        while (*cp != '\0')
        {
            const char *inHere = cp;
            const char *forThis = forThisMatch;

            while
            (
                *inHere  != '\0' &&
                *forThis != '\0' &&
                toupper(*inHere) == toupper(*forThis)
            )
            {
                /* Well this character matched, keep looking */
                inHere++;
                forThis++;
            }

            if (*forThis == '\0')
            {
                /* All of the substring matched */
                /* so return this position      */
                return(cp);
            }
            else
            {
                /* Substring mismatch, so look */
                /* for it at the next position */
                cp++;
            }
        }

        /* When we don't find the substring */
        /* return NULL                      */
        return(NULL);
    }
}


/*                                                      
    This function finds all of the search keys on the   
    current line, and saves them for find processing    
                                                      */
void Finder::find_patterns_on_line
(
    void
)
{
    _matchColumns.clear();

    const char *t = gTextLines[(size_t)(gpDisplay->current_line())];
    const char *p = t;

    while ((p = find_string(p, _searchKey)) != NULL)
    {
        _matchColumns.push_back((size_t) (p - t + 1));
        p++;
    }
}


/*                                      
    This thing knows how to do finding. 
                                      */
Finder *gpFinder = NULL;

