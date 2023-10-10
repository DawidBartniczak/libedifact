#ifndef _LIBEDIFACT_H
    #define _LIBEDIFACT_H

    #if defined(_WIN32) || defined(_WIN64)
        #define PUBLISHED __declspec(dllexport)
    #endif

    PUBLISHED size_t parse_edi(char*);
#endif