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

edi_parser_t* edi_parser_create(edi_parser_params_t* params, size_t data_length) {
    edi_parser_t* parser;
    edi_parser_private_t* private;
    char* buffer;

    if(params == NULL) {
        return NULL;
    }

    if((parser = (edi_parser_t *) calloc(1, sizeof(edi_parser_t))) == NULL) {
        return NULL;
    }
    if((private = (edi_parser_private_t *) calloc(1, sizeof(edi_parser_private_t))) == NULL) {
        free(parser);
        return NULL;
    }
    if((buffer = (char *) calloc(data_length + 1, sizeof(char))) == NULL) {
        free(parser);
        free(private);
        return NULL;
    }
    parser->private = private;
    parser->private->buffer = buffer;

    parser->params = params;

    return parser;
}

edi_segment_t* edi_parser_next_segment(edi_parser_t* parser) {
    if (parser->private->current_interchange->segment_count >= parser->private->allocated_segments) {
        parser->private->allocated_segments += EDI_SEGMENT_CLUSTER_SIZE;
        if ((parser->private->current_interchange->segments = realloc(parser->private->current_interchange->segments, sizeof(edi_segment_t) * parser->private->allocated_segments)) == NULL) {
            return NULL;
        }
    }
    parser->private->current_segment = parser->private->current_interchange->segments + parser->private->current_interchange->segment_count;
    parser->private->current_interchange->segment_count++;
    parser->private->allocated_elements = 0;
    memset(parser->private->current_segment, 0, sizeof(edi_segment_t));

    if (edi_parser_next_element(parser) == NULL) {
        free(parser->private->current_segment);
        return NULL;
    }

    return parser->private->current_segment;
}

edi_element_t* edi_parser_next_element(edi_parser_t* parser) {
    if (parser->private->current_segment->element_count >= parser->private->allocated_elements) {
        parser->private->allocated_elements += EDI_ELEMENT_CLUSTER_SIZE;
        if ((parser->private->current_segment->elements = realloc(parser->private->current_segment->elements, sizeof(edi_element_t) * parser->private->allocated_elements)) == NULL) {
            return NULL;
        }
    }
    parser->private->current_element = parser->private->current_segment->elements + parser->private->current_segment->element_count;
    parser->private->current_segment->element_count++;
    parser->private->allocated_subelements = 0;
    memset(parser->private->current_element, 0, sizeof(edi_element_t));

    return parser->private->current_element;
}

char* edi_parser_next_subelement(edi_parser_t* parser) {
    parser->private->allocated_subelements++;
    if ((parser->private->current_element->subelements = realloc(parser->private->current_element->subelements, sizeof(char*) * parser->private->allocated_subelements)) == NULL) {
        return NULL;
    }
    parser->private->current_subelement = (char*)(parser->private->current_element->subelements + parser->private->current_element->subelement_count);
    parser->private->current_element->subelement_count++;
    memset(parser->private->current_subelement, 0, sizeof(char *));

    return parser->private->current_subelement;
}

void edi_parser_destroy(edi_parser_t* parser) {
    free(parser->private->buffer);
    free(parser->private);
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

void edi_element_destroy(edi_element_t* element) {
    for (size_t i = 0; i < element->subelement_count; i++) {
        free(element->subelements[i]);
    }
    free(element->subelements);
}

void edi_segment_destroy(edi_segment_t* segment) {
    for (size_t i = 0; i < segment->element_count; i++) {
        edi_element_destroy(&segment->elements[i]);
    }
    free(segment->elements);
}

void edi_interchange_destroy(edi_interchange_t* interchange) {
    edi_segment_t* segment = interchange->segments;
    for (size_t i = 0; i < interchange->segment_count; i++) {
        edi_segment_destroy(&interchange->segments[i]);
    }
    free(interchange->segments);
    free(interchange);
}