#ifndef _PV_LIBEDIFACT_H
    #define _PV_LIBEDIFACT_H

    #include "libedifact.h"
    #include <stdlib.h>
    #include <stddef.h>

    struct edi_parser_params {
        char segment_terminator;
        char element_sep;
        char subelement_sep;
        char decimal_notation;
        char release_char;

        const char* header_tag;
        const char* trailer_tag;
    };

    struct edi_service_string_detector {
        char* service_string;
        size_t service_string_position;
        unsigned char service_string_found;
    };

    edi_parser_t* edi_parser_create();
#endif