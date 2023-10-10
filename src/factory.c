#include "private_libedifact.h"

edi_parser_t* edi_parser_create(int auto_detect_params) {
    edi_parser_t* parser;

    if((parser = (edi_parser_t *) calloc(1, sizeof(edi_parser_t))) == NULL) {
        return NULL;
    }

    parser->auto_detect_params = auto_detect_params;

    return parser;
}

int edi_parser_destroy(edi_parser_t* parser) {
    free(parser->params);
    return 0;
}