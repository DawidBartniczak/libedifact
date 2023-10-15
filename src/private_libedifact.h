#ifndef _PV_LIBEDIFACT_H
    #define _PV_LIBEDIFACT_H

    #define EDI_SEGMENT_CLUSTER_SIZE 4
    #define EDI_ELEMENT_CLUSTER_SIZE 2

    #define EDI_GENERATOR_BUFFER_SIZE 128

    #include <stdlib.h>
    #include <string.h>
    #include <stddef.h>
    #include "libedifact.h"

    typedef struct edi_parser edi_parser_t;
    typedef struct edi_parser_private edi_parser_private_t;
    typedef struct edi_generator edi_generator_t;
    typedef struct edi_generator_private edi_generator_private_t;

    struct edi_parser {
        int error;
        edi_parser_private_t* private;
        edi_parser_params_t* params;
    };

    struct edi_parser_private {
        char* buffer;
        size_t buffer_length;

        edi_interchange_t* current_interchange;
        edi_segment_t* current_segment;
        edi_element_t* current_element;
        char* current_subelement;

        size_t allocated_segments;
        size_t allocated_elements;
        size_t allocated_subelements;
    };

    struct edi_generator {
        int error;
        edi_generator_private_t* private;
        edi_parser_params_t* params;
    };

    struct edi_generator_private {
        char* buffer;
        size_t buffer_length;
        size_t buffer_allocated;
    };

    edi_parser_params_t* edi_parser_params_default();
    void edi_parser_params_destroy(edi_parser_params_t*);

    edi_parser_t* edi_parser_create(edi_parser_params_t*, size_t);
    void edi_parser_destroy(edi_parser_t*);

    edi_generator_t* edi_generator_create(edi_parser_params_t*);
    void edi_generator_destroy(edi_generator_t*);

    edi_segment_t* edi_segment_create();
    void edi_segment_destroy(edi_segment_t*);

    edi_element_t* edi_element_create();
    void edi_element_destroy(edi_element_t*);

    char* create_dynamic_string(char*, size_t);
#endif