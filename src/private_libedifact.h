#ifndef _PV_LIBEDIFACT_H
    #define _PV_LIBEDIFACT_H

    #include <stdlib.h>
    #include <string.h>
    #include <stddef.h>
    #include "libedifact.h"

    typedef struct edi_parser edi_parser_t;

    struct edi_parser_params {
        char segment_terminator;
        char element_sep;
        char subelement_sep;
        char decimal_notation;
        char release_char;

        const char* interchange_header_tag;
        const char* interchange_trailer_tag;
    };

    edi_parser_params_t* _detect_edi_params(char*);

    edi_parser_params_t* edi_parser_params_default();
    edi_parser_params_t* edi_parser_params_create(char, char, char, char, char);
    void edi_parser_params_destroy(edi_parser_params_t*);

    edi_parser_t* edi_parser_create(edi_parser_params_t*);
    void edi_parser_destroy(edi_parser_t* parser);
#endif