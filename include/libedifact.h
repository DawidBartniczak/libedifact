#ifndef _LIBEDIFACT_H
    #define _LIBEDIFACT_H

    # undef EXTERNC_
    # if defined(__cplusplus)
        # define EXTERNC_ extern "C"
    # else
        # define EXTERNC_
    # endif

    # undef PUBLISHED
    #if defined(_WIN32) || defined(_WIN64)
        #define PUBLISHED __declspec(dllexport)
    #endif

    #define EDI_ELEMENT_SIMPLE 1
    #define EDI_ELEMENT_COMPOSITE 2

    #define EDI_ERROR_NONE 0
    #define EDI_ERROR_EMPTY 1
    #define EDI_ERROR_NTERMINATED 2
    #define EDI_ERROR_GENERAL 3

    typedef struct edi_interchange edi_interchange_t;
    typedef struct edi_segment edi_segment_t;
    typedef struct edi_element edi_element_t;
    typedef struct edi_parser_params edi_parser_params_t;

    struct edi_parser {
        int error;
        edi_parser_params_t* params;
    };

    struct edi_interchange {
        edi_segment_t* segments;
        size_t segment_count;
    };

    struct edi_segment {
        char* tag;
        edi_element_t* elements;
        size_t element_count;
    };

    struct edi_element {
        char** subelements;
        size_t subelement_count;
    };

    PUBLISHED edi_interchange_t* edi_parse(edi_parser_params_t*, char*, size_t);
#endif