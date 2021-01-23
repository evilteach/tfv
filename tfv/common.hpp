
/*                                                                  
    common.cpp                                                      
    EvilTeach - MPL 1.1                                             
                                                                  */

#ifndef COMMON_HPP
#define COMMON_HPP

#define VERSION "2.21"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>

enum
{
    FAILURE      = 0,
    SUCCESS      = 1,
    QUIT         = 2,
    HELP         = 3,
    OPEN_FAILURE = 4,
    SEEK_FAILURE = 5,
    READ_FAILURE = 8
};

#ifdef _WIN64
    #define FOFFSET __int64
    #define FSEEK  _fseeki64
    #define FTELL  _ftelli64
    #define FOPEN   fopen
    #define FCLOSE  fclose
    #define FREAD   fread
    #define FWRITE  fwrite
    #define FOFFSET_ZERO 0I64
#else
    #define FOFFSET long long
    #define FSEEK   fseek
    #define FTELL   ftell
    #define FOPEN   fopen
    #define FCLOSE  fclose
    #define FREAD   fread
    #define FWRITE  fwrite
    #define FOFFSET_ZERO 0
#endif


#include "Console.hpp"

#include "App.hpp"



/*                                                                          
    This holds the actual offset that the user has picked.  It is used to   
    decide which screen should currently be viewed.                         
                                                                          */
extern size_t gCurrentColumn;  // BUG belongs in display


/*                                                                                  
    This holds a chunk of memory large enough to hold the entire file in memory.    
    Hence compiling as a 64 bit app make sense to allow large files to be examined. 
                                                                                  */
extern char *gpAllTextInMemory; // BUG belongs in text lines



/*                                                                      
    This holds a the file name portion, so that it can be displayed     
    on the status line.                                                 
                                                                      */
extern std::string gFileNameOnly;   // BUG should be in app or display



/*                                                                      
    This is set to true, when the TFV_FILE environment variable is set, 
    which tells the program where the configuration file is located.    
                                                                      */
extern bool gUserPreferenceFileExists;  // BUG should be in app.



/*                                                                              
    This holds the name of the file that contains configuration information.    
                                                                              */
extern char gUserPreferenceFileName[1000];  // BUG should be string in app



/*                                                                              
    This holds the integer value of last key that was pressed for status line   
    and debugging purposes.                                                     
                                                                              */
extern int gLastKeyPressed; // BUG should be in keyboard class



/*                                                                      
    This class is an interface to a vector which contains pointers to   
    the text lines which were loaded from the input file.               
                                                                      */
class TextLines
{
    public :

        ~TextLines
        (
            void
        )
        {
            delete[] gpAllTextInMemory;
        }

        /*                                                                                      
            When this is true, the user wants to see the carriage control as part of the line   
            When this is false, the carriage control should be hidden                           
                                                                                              */
        bool showCarriageControl;

        /*                                                              
            This vector contains pointers to the start of each line.    
                                                                      */
        std::vector<const char *> pStartOfLines;

        /*                                                                  
            The text representation of the line is accumulated into here.   
                                                                          */
        char buffer[MAX_LINE_SIZE + sizeof('\0')];

        /*                                                                                  
            This holds either '\r\n', '\n', or '\0' Which is the carriage control sequence  
            that the program decided applied to this file.                                  
                                                                                          */
        char *carriageControlValue;

        /*                                                                                      
            To address a performance problem, a  tiny cache subsystem was added to operator[].  
            There are cases where it is necessary to flush that cache.  This flag can be set    
            to cause that flush to occur.                                                       
                                                                                              */
        bool theScreenNeedsRedrawn;

        /*                                                                          
            This function fetches the desired line into the buffer in a reasonably  
            fast fashion.                                                           
                                                                                  */
        char *operator[]
        (
            size_t sub
        )
        {
            static size_t previousSub = (size_t) -1;
            if ((! theScreenNeedsRedrawn) && (previousSub == sub))
            {
                /* The buffer already contains what we need */
            }
            else
            {
                size_t ccHideSize;

                if (showCarriageControl)
                {
                    /* Let's not hide the carriage control */
                    ccHideSize = 0U;
                }
                else
                {
                    /* Hide the carriage control */
                    ccHideSize = strlen(carriageControlValue);
                }

                size_t lineLength;
                if (sub < (size() - 1))
                {
                    lineLength = (size_t)
                                 (
                                     pStartOfLines[sub + 1] -    /* Start of the next line */
                                     pStartOfLines[sub]          /* Start of the current line */
                                 );
                }
                else
                {
                    lineLength = (size_t)(strlen(pStartOfLines[sub])); /* Start of Last Line */
                }

                /*                                                          
                    We may be showing the carriage control or not.          
                    the very last line may have carriage control or not.    
                                                                          */

                if (lineLength >= ccHideSize)
                {
                    const char *tmp = pStartOfLines[sub];
                                tmp += lineLength;
                                tmp -= ccHideSize;
                    if (memcmp(carriageControlValue, tmp, ccHideSize) == 0)
                    {
                        lineLength -= ccHideSize;
                    }
                    else
                    {
                    }
                }
                else
                {
                }

                if (lineLength > MAX_LINE_SIZE)
                {
                    lineLength = MAX_LINE_SIZE;
                }
                else
                {
                }

                memcpy(buffer, pStartOfLines[sub], lineLength);
                buffer[lineLength] = '\0';

                previousSub = sub;
            }

            return buffer;
        }



        /*                                                                                  
            Having made a reasonable guess as to the number of lines that are in the file,  
            we attempt to reserve enough space in the vector to hold it.                    
                                                                                          */
        void reserve
        (
            size_t size
        )
        {
            pStartOfLines.reserve(size);
        }



        /*                                                                  
            This is used to add lines to the vector, as they are loaded.    
                                                                          */
        void push_back
        (
            const char *s
        )
        {
            pStartOfLines.push_back(s);
        }



        /*                                                              
            This tells you how many lines are referenced by the vector. 
                                                                      */
        size_t size
        (
            void
        )
        {
            return pStartOfLines.size();
        }

        size_t count
        (
            size_t bytesInChunk,
            char   *pChunk,
            size_t &countCRLF,
            size_t &countLF,
            size_t &countCR,
            size_t &countNUL
        )
        {
            size_t bytes[256] = {0};

            countCRLF = 0;

            size_t z = 0;
            for (; z < bytesInChunk - 1; z++)
            {
                bytes[pChunk[z]]++;

                // we loop to -1 as this test needs to look at 2 bytes
                countCRLF += (pChunk[z] == '\r' && pChunk[z + 1] == '\n');
            }

            // This takes care of the last byte
            bytes[pChunk[z]]++;

            countLF  = bytes['\n'];
            countCR  = bytes['\r'];
            countNUL = bytes['\0'];

            return 0;
        }


        /*                                              
            This loads the file, into the line object.  
                                                        
            It returns 0 on success or 4 - 8 on failure 
                                                      */
        int load_file
        (
            void
        )
        {
            extract_filename();

            int returnStatus = 0;

            carriageControlValue = NULL;

            /*                                                                  
                We look at this as being a binary file, so no carriage control  
                translation occurs on the windows platform                      
                                                                              */
            FILE *fpi = FOPEN(gpFileName, "rb");
            if (fpi != NULL)
            {
                /* Get about 2% faster loads with a larger buffer */
                setvbuf (fpi, NULL, _IOFBF, 65536);  // BUG make much larger

                /* How big is this file? */
                if (FSEEK(fpi, FOFFSET_ZERO, 2) == 0)
                {
                    FOFFSET tmp = FTELL(fpi);
                    if (tmp == -1)
                    {
                        std::cout << strerror(errno) << std::endl;
                    }
                    else
                    {
                    }

                    size_t eof = (size_t)(tmp);
                    if (tmp == 0)
                    {
                        returnStatus = 2;
                    }
                    else
                    {
                        if (FSEEK(fpi, FOFFSET_ZERO, 0) == 0)
                        {
                            /* Ok, it is eof bytes long */
                            /* Let's load it into memory */

                            /* We need some memory to hold it */
                            gpAllTextInMemory = new char[eof + 1];

                            gpConsole->moveto(0, 0);
                            *gpConsole << "Loading File into memory "
                                       << "(" << eof << " bytes)"
                                       << endl;
                            *gpConsole << "";

                            if (FREAD(gpAllTextInMemory, 1, eof, fpi) == eof)
                            {
                                gpAllTextInMemory[eof] = '\0';

                                /*                                                                      
                                    There are zero or more lines in the file.                           
                                    The actual carriage control is not known at this point.             
                                    The last line in the file may or may not have carriage control.     
                                                                                                        
                                    Expect:                                                             
                                        CRLF for dos                                                    
                                        LF   for unix                                                   
                                        CR   for oldmac                                                 
                                        NUL  for binary file                                            
                                                                                                      */
//                                size_t actualLines = 0;

                                size_t countCRLF = 0;
                                size_t countLF   = 0;
                                size_t countCR   = 0;
                                size_t countNUL  = 0;
                                count(eof, gpAllTextInMemory, countCRLF, countLF, countCR, countNUL);

                                size_t extraLineForMissingCC = 1;

                                size_t estimatedLines  = std::max(countCRLF, countLF);
                                       estimatedLines  = std::max(estimatedLines, countCR);
                                       estimatedLines  = std::max(estimatedLines, countNUL);
                                       estimatedLines += extraLineForMissingCC;


                                reserve(estimatedLines);

                                if (countNUL > 0)
                                {
                                    // Embedded NULs imply binary file format.
                                    // Treat NUL as carriage control
                                    carriageControlValue = "";  /* '\0' */
                                }
                                else
                                {
                                    if (countCRLF >= countLF && countCRLF >= countCR)
                                    {
                                        /* This is a dos text file */
                                        carriageControlValue = "\r\n";
                                    }
                                    else
                                    {
                                        if (countLF > countCR)
                                        {
                                            /* This is a unix file */
                                            carriageControlValue = "\n";
                                        }
                                        else
                                        {
                                            if (countCR > 0)
                                            {
                                                /* This is an old mac file */
                                                carriageControlValue = "\r";
                                            }
                                            else
                                            {
                                                /* I don't know what kind of file this is */
                                                carriageControlValue = "";  /* '\0' */
                                            }
                                        }
                                    }
                                }

                                /* Now we divvy up the text lines, and make the vector reference them */

                                char *p = gpAllTextInMemory;

                                if (carriageControlValue[0] == '\0')
                                {
                                    /* So this is is probably a binary file            */
                                    /* We treat '\0' as the carriage control character */
                                    char *end = gpAllTextInMemory + eof;

                                    do
                                    {
                                        push_back(p);

                                        if ((size() % 500000) == 0)
                                        {
                                            size_t percent = size() * 100 / estimatedLines;

                                            gpConsole->moveto(0, 0);
                                            *gpConsole << "Indexing Text Lines "
                                                       << size()
                                                       << " "
                                                       << percent
                                                       << " percent complete"
                                                       << endl;

                                            *gpConsole << "";
                                        }

                                        p += strlen(p) + 1;
                                    }
                                    while (p < end);
                                }
                                else
                                {
                                    p -= strlen(carriageControlValue);

                                    for (;;)
                                    {
                                        p += strlen(carriageControlValue);
                                        push_back(p);
                                        p = strstr(p, carriageControlValue);
                                        if (p == NULL)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            size_t xxx = estimatedLines / 100;
                                            if (xxx == 0)
                                            {
                                                xxx = estimatedLines;
                                            }
                                            else
                                            {
                                            }

                                            if ((size() % (estimatedLines / xxx)) == 0)
                                            {
                                                size_t percent = (size_t) (((p - gpAllTextInMemory) * 100U) / eof);

                                                gpConsole->moveto(0, 0);
                                                *gpConsole << "Indexing Text Lines "
                                                           << (size_t) size()
                                                           << "  "
                                                           << (size_t) percent
                                                           << " percent complete"
                                                           << endl;

                                                *gpConsole << "";
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                *gpConsole << "Can't read the enter file into memory" << endl;
                                returnStatus = READ_FAILURE;
                            }
                        }
                        else
                        {
                            *gpConsole << "Failed to seek to eof" << endl;
                            returnStatus = SEEK_FAILURE;
                        }
                    }
                }
                else
                {
                    *gpConsole << "Failed to seek to eof" << endl;
                    returnStatus = SEEK_FAILURE;
                }

                FCLOSE(fpi);
            }
            else
            {
                *gpConsole << "Can't open <" << gpFileName << ">" << endl;
                returnStatus = OPEN_FAILURE;
            }

            return returnStatus;
        }


        /*                                                                      
            This function skips past the directory part of the file name,       
            to pick out the file name for the status line display_the_screen.   
                                                                              */
        void extract_filename   // BUG probably belongs in APP.
        (
            void
        )
        {
            char *p = gpFileName;
            char *q = NULL;

            while ((q = strchr(p, '\\')) != NULL)
            {
                p = q;
                q = p++;
            }

            gFileNameOnly = p;
        }



        /*                                                                                          
            This function toggles the flag that decides if the carriage control should be shown     
                                                                                                  */
        void change_visible_carriage_control    // BUG belongs in bindable functions
        (
            void
        )
        {
            showCarriageControl = !showCarriageControl;
            theScreenNeedsRedrawn = true;
        }



        /*                                                  
            This function gets the carriage control status  
                                                          */
        bool show_carriage_control   // BUG belongs in bindable functions
        (
            void
        )
        {
            return showCarriageControl;
        }



        /*                                                  
            This function sets the carriage control status  
                                                          */
        void show_carriage_control   // BUG belongs in bindable functions
        (
            int value
        )
        {
            showCarriageControl = value != 0;
        }



        /*
        */
        TextLines
        (
            void
        ) :
            theScreenNeedsRedrawn(false)
        {
        }

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
};



/*                                                              
    This container holds the pointers to the start of the lines 
                                                              */
extern TextLines gTextLines;



/*                                                                          
    This holds the actual offset that the user has picked.  It is used to   
    decide which screen should currently be viewed.                         
                                                                          */
extern size_t gCurrentColumn;      // BUG belongs in display

/*                                                                          
    When this is true, the status line character is hex instead of decimal. 
                                                                          */
extern bool gUseHexRadix;       // BUG belongs in display

/*                                                  
    This holds the color choice that the user set.  
                                                  */
extern chtype gUserColorChoice; // BUG belongs in display

/*                                      
    This holds the color of the ruler   
                                      */
extern chtype gRulerColor;      // BUG belongs in display

/*                                                              
    This holds the bolded version of the users color choice.    
                                                              */
extern chtype gUserColorBold;   // BUG belongs in display

#endif



