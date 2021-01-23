#ifdef _UNICODE
    #error "Unicode may not be set for this library"
#else
    #ifdef _MBCS
        #error "Multi Byte may not be set for this libary"
    #else
    #endif
#endif

int dummy = 314;
