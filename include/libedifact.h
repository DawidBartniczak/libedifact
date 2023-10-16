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
    #define EDI_ERROR_SYS 2

    typedef struct edi_interchange edi_interchange_t;
    typedef struct edi_segment edi_segment_t;
    typedef struct edi_element edi_element_t;
    typedef struct edi_parser_params edi_parser_params_t;

    struct edi_parser_params {
        char segment_terminator;
        char element_sep;
        char subelement_sep;
        char decimal_notation;
        char release_char;

        const char* message_header_tag;
        const char* interchange_trailer_tag;
    };

    struct edi_interchange {
        int error;
        char message_type[7];
        edi_segment_t* segments;
        size_t segment_count;
    };

    struct edi_segment {
        char tag[4];
        edi_element_t* elements;
        size_t element_count;
    };

    struct edi_element {
        char** subelements;
        size_t subelement_count;
    };

    PUBLISHED edi_interchange_t* edi_parse(edi_parser_params_t*, char*);

    PUBLISHED char* edi_generate(edi_parser_params_t*, edi_interchange_t*);

    PUBLISHED edi_interchange_t* edi_interchange_create();
    PUBLISHED void edi_interchange_destroy(edi_interchange_t*);

    PUBLISHED edi_segment_t* edi_segment_create();
    PUBLISHED void edi_segment_destroy(edi_segment_t*);

    PUBLISHED edi_element_t* edi_element_create();
    PUBLISHED void edi_element_destroy(edi_element_t*);

    PUBLISHED edi_interchange_t* edi_interchange_segment_add(edi_interchange_t*, edi_segment_t*);
    PUBLISHED edi_segment_t* edi_segment_element_add(edi_segment_t*, edi_element_t*);
    PUBLISHED edi_element_t* edi_element_subelement_add(edi_element_t*, char*);

    PUBLISHED edi_parser_params_t* edi_parser_params_create(char, char, char, char, char);
#endif