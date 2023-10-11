#include <stdio.h>
#include "private_libedifact.h"

edi_parser_params_t* _detect_edi_params(char* data) {
    if (strncmp(data, "UNA", 3) != 0) {
        return NULL;
    }

    return edi_parser_params_create(data[8], data[4], data[3], data[5], data[6]);
}

edi_interchange_t* edi_parse(edi_parser_params_t* params, char* data, size_t data_length) {
    edi_parser_t* parser;
    unsigned char destroy_parser = 0;

    if(params == NULL) {
        destroy_parser = 1;
        if ((params = _detect_edi_params(data)) == NULL) {
            if ((params = edi_parser_params_default()) == NULL) {
                return NULL;
            }
        }
    }
    if((parser = edi_parser_create(params)) == NULL) {
        return NULL;
    }
    
    edi_parser_destroy(parser);
    if (destroy_parser)
        edi_parser_params_destroy(params);
    return 0;
}