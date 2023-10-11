#ifndef _PV_LIBEDIFACT_H
    #define _PV_LIBEDIFACT_H

    #define EDI_SEGMENT_CLUSTER_SIZE 4

    #include <stdlib.h>
    #include <string.h>
    #include <stddef.h>
    #include "libedifact.h"

    typedef struct edi_parser edi_parser_t;

    struct edi_parser {
        int error;
        edi_parser_params_t* params;
    };

    edi_parser_params_t* edi_parser_params_default();
    edi_parser_params_t* edi_parser_params_create(char, char, char, char, char);
    void edi_parser_params_destroy(edi_parser_params_t*);

    edi_parser_t* edi_parser_create(edi_parser_params_t*);
    void edi_parser_destroy(edi_parser_t* parser);
#endif