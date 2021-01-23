
/*                                                                                                      
    tfv.cpp                                                                                             
                                                                                                        
    EvilTeach 2013.  MPL 1.1                                                                            
                                                                                                      */

#include "common.hpp"

/*                                                                          
    This is the entry point.  It expects the name of the file to view on    
    the command line as argument 1.  On anything like success it returns 0. 
    Any nonzero status implies an error.                                    
                                                                          */
int main
(
    int   iArgsCount,
    char *iArgs[]
)
{
    int returnStatus = 1;

    try
    {
        App app;
        gpApp = &app;

        returnStatus = app.run(iArgsCount, iArgs);
    }
    catch (std::exception &e)
    {
        std::cout << "Exception Caught " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unexpected Exception Caught " << std::endl;
    }

    return returnStatus;
}

