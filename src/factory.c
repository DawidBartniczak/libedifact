#include "private_libedifact.h"

// PARSER PARAMS FACTORY

edi_parser_params_t* _edi_parser_params_alloc() {
    edi_parser_params_t* params;

    if((params = (edi_parser_params_t *) calloc(1, sizeof(edi_parser_params_t))) == NULL) {
        return NULL;
    }

    return params;
}

edi_parser_params_t* edi_parser_params_default() {
    return edi_parser_params_create('\'', '+', ':', '.', '?');
}

edi_parser_params_t* edi_parser_params_create(char segment_t, char element_s, char selement_t, char decimal, char release) {
    edi_parser_params_t *params;

    if((params = _edi_parser_params_alloc()) == NULL) {
        return NULL;
    }

    params->segment_terminator = segment_t;
    params->element_sep = element_s;
    params->subelement_sep = selement_t;
    params->decimal_notation = decimal;
    params->release_char = release;
    params->interchange_header_tag = "UNB";
    params->interchange_trailer_tag = "UNZ";

    return params;
}

void edi_parser_params_destroy(edi_parser_params_t* params) {
    free(params);
}

// PARSER FACTORY

edi_parser_t* edi_parser_create(edi_parser_params_t* params) {
    edi_parser_t* parser;

    if((parser = (edi_parser_t *) calloc(1, sizeof(edi_parser_t))) == NULL) {
        return NULL;
    }

    if(params == NULL) {
        return NULL;
    }
    parser->params = params;

    return parser;
}

void edi_parser_destroy(edi_parser_t* parser) {
    free(parser);
}

// INTERCHANGE FACTORY

edi_interchange_t* edi_interchange_create() {
    edi_interchange_t* interchange;

    if((interchange = (edi_interchange_t *) calloc(1, sizeof(edi_interchange_t))) == NULL) {
        return NULL;
    }

    return interchange;
}

void edi_interchange_destroy(edi_interchange_t* interchange) {
    free(interchange);
}