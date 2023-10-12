#include "private_libedifact.h"

edi_parser_params_t* _detect_edi_params(char* data) {
    if (strncmp(data, "UNA", 3) != 0) {
        return NULL;
    }

    return edi_parser_params_create(data[8], data[4], data[3], data[5], data[6]);
}

edi_interchange_t* _parse_edi_interchange(edi_parser_t* parser, char* data) {
    if (edi_parser_next_segment(parser) == NULL) {
        edi_interchange_destroy(parser->private->current_interchange);
        edi_parser_destroy(parser);
        return NULL;
    }
    strncpy_s(parser->private->current_segment->tag, 4, data, 3);
    while(data && *data) {
        if (*data == parser->params->release_char) {
            data++;
        } else {
            if (*data == parser->params->segment_terminator) {
                if (strncmp(parser->private->current_segment->tag, parser->params->interchange_trailer_tag, 3) == 0) {
                    break;
                }

                if (edi_parser_next_segment(parser) == NULL) {
                    edi_interchange_destroy(parser->private->current_interchange);
                    edi_parser_destroy(parser);
                    return NULL;
                }
                
                data++;
                strncpy_s(parser->private->current_segment->tag, 4, data, 3);
            } else if (*data == parser->params->element_sep) {
                if (edi_parser_next_element(parser) == NULL) {
                    edi_interchange_destroy(parser->private->current_interchange);
                    edi_parser_destroy(parser);
                    return NULL;
                }

                data++;
            } else if (*data == parser->params->subelement_sep) {
                data++;
            }
        }

        data++;
    }

    return parser->private->current_interchange;
}

edi_interchange_t* edi_parse(edi_parser_params_t* params, char* data) {
    edi_interchange_t* interchange;
    edi_parser_t* parser;
    unsigned char destroy_parser = 0;

    if((interchange = edi_interchange_create()) == NULL) {
        return NULL;
    }
    if(!data || !*data) {
        interchange->error = EDI_ERROR_EMPTY;
        return interchange;
    }

    if(params == NULL) {
        destroy_parser = 1;
        if ((params = _detect_edi_params(data)) != NULL) {
            data += 9;
        } else if ((params = edi_parser_params_default()) == NULL) {
            return NULL;
        }
    }
    if((parser = edi_parser_create(params, strlen(data))) == NULL) {
        edi_interchange_destroy(interchange);
        return NULL;
    }

    parser->private->current_interchange = interchange;
    if (_parse_edi_interchange(parser, data) == NULL) {
        interchange->error = EDI_ERROR_SYS;
    }
    
    edi_parser_destroy(parser);
    if(destroy_parser != 0) {
        edi_parser_params_destroy(params);
    }
    return interchange;
}