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
    params->message_header_tag = "UNH";
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

void edi_parser_destroy(edi_parser_t* parser) {
    free(parser->private->buffer);
    free(parser->private);
    free(parser);
}

// GENERATOR FACTORY

edi_generator_t* edi_generator_create(edi_parser_params_t* params) {
    edi_generator_t* generator;
    edi_generator_private_t* private;
    char* buffer;

    if(params == NULL) {
        return NULL;
    }

    if((generator = (edi_generator_t *) calloc(1, sizeof(edi_generator_t))) == NULL) {
        return NULL;
    }
    if((private = (edi_generator_private_t *) calloc(1, sizeof(edi_generator_private_t))) == NULL) {
        free(generator);
        return NULL;
    }
    if((buffer = (char *) calloc(EDI_GENERATOR_BUFFER_SIZE, sizeof(char))) == NULL) {
        free(generator);
        free(private);
        return NULL;
    }
    generator->private = private;
    generator->private->buffer = buffer;
    generator->private->buffer_allocated = EDI_GENERATOR_BUFFER_SIZE;

    generator->params = params;

    return generator;
}

void edi_generator_destroy(edi_generator_t* generator) {
    free(generator->private->buffer);
    free(generator->private);
    free(generator);
}

// INTERCHANGE FACTORY

edi_interchange_t* edi_interchange_create() {
    edi_interchange_t* interchange;

    if((interchange = (edi_interchange_t *) calloc(1, sizeof(edi_interchange_t))) == NULL) {
        return NULL;
    }

    return interchange;
}

edi_interchange_t* edi_interchange_segment_add(edi_interchange_t* interchange, edi_segment_t* segment) {
    if((interchange->segments = realloc(interchange->segments, sizeof(edi_segment_t) * (interchange->segment_count + 1))) == NULL) {
        return NULL;
    }
    interchange->segments[interchange->segment_count] = *segment;
    interchange->segment_count++;

    return interchange;
}

void edi_interchange_destroy(edi_interchange_t* interchange) {
    edi_segment_t* segment = interchange->segments;
    for (size_t i = 0; i < interchange->segment_count; i++) {
        edi_segment_destroy(&interchange->segments[i]);
    }
    free(interchange->segments);
    free(interchange);
}

// SEGMENT FACTORY

edi_segment_t* edi_segment_create() {
    edi_segment_t* segment;

    if((segment = (edi_segment_t *) calloc(1, sizeof(edi_segment_t))) == NULL) {
        return NULL;
    }

    return segment;
}

edi_segment_t* edi_segment_element_add(edi_segment_t* segment, edi_element_t* element) {
    if((segment->elements = realloc(segment->elements, sizeof(edi_element_t) * (segment->element_count + 1))) == NULL) {
        return NULL;
    }
    segment->elements[segment->element_count] = *element;
    segment->element_count++;

    return segment;
}

void edi_segment_destroy(edi_segment_t* segment) {
    for (size_t i = 0; i < segment->element_count; i++) {
        edi_element_destroy(&segment->elements[i]);
    }
    free(segment->elements);
}

// ELEMENT FACTORY

edi_element_t* edi_element_create() {
    edi_element_t* element;

    if((element = (edi_element_t *) calloc(1, sizeof(edi_element_t))) == NULL) {
        return NULL;
    }

    return element;
}

edi_element_t* edi_element_subelement_add(edi_element_t* element, char* subelement) {
    if((element->subelements = realloc(element->subelements, sizeof(char*) * (element->subelement_count + 1))) == NULL) {
        return NULL;
    }
    element->subelements[element->subelement_count] = subelement;
    element->subelement_count++;

    return element;
}

edi_element_t* edi_element_subelement_add_copy(edi_element_t* element, char* subelement, size_t subelement_length) {
    if((element->subelements = realloc(element->subelements, sizeof(char*) * (element->subelement_count + 1))) == NULL) {
        return NULL;
    }
    element->subelements[element->subelement_count] = create_dynamic_string(subelement, subelement_length);
    element->subelement_count++;

    return element;
}

void edi_element_destroy(edi_element_t* element) {
    for (size_t i = 0; i < element->subelement_count; i++) {
        free(element->subelements[i]);
    }
    free(element->subelements);
}