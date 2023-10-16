#include <stdio.h>
#include "private_libedifact.h"

char* _append_to_buffer(edi_generator_t* generator, char* data, size_t data_length) {
    if (generator->private->buffer_length + data_length >= generator->private->buffer_allocated) {
        generator->private->buffer_allocated += EDI_GENERATOR_BUFFER_SIZE;
        generator->private->buffer = realloc(generator->private->buffer, generator->private->buffer_allocated);
    }

    memcpy(generator->private->buffer + generator->private->buffer_length, data, data_length);
    generator->private->buffer_length += data_length;

    return generator->private->buffer;
}

char* edi_generate(edi_parser_params_t* params, edi_interchange_t* interchange) {
    edi_generator_t* generator;
    char* edi_data;
    unsigned char destroy_params = 0;

    if(params == NULL) {
        destroy_params = 1;
        if ((params = edi_parser_params_default()) == NULL) {
            return NULL;
        }
    }
    if((generator = edi_generator_create(params)) == NULL) {
        edi_interchange_destroy(interchange);
        return NULL;
    }

    sprintf_s(generator->private->buffer, EDI_GENERATOR_BUFFER_SIZE, "UNA%c%c%c%c %c", params->subelement_sep, params->element_sep, params->decimal_notation, params->release_char, params->segment_terminator);
    generator->private->buffer_length = 9;

    for (int i = 0; i < interchange->segment_count; i++) {
        edi_segment_t *segment = interchange->segments + i;
        _append_to_buffer(generator, segment->tag, strlen(segment->tag));
        _append_to_buffer(generator, &params->element_sep, 1);

        for (int j = 0; j < segment->element_count; j++) {
            edi_element_t *element = segment->elements + j;
            for (int k = 0; k < element->subelement_count; k++) {
                _append_to_buffer(generator, element->subelements[k], strlen(element->subelements[k]));
                if (k < element->subelement_count - 1) {
                    _append_to_buffer(generator, &params->subelement_sep, 1);
                }
            }
            if (j < segment->element_count - 1) {
                _append_to_buffer(generator, &params->element_sep, 1);
            }
        }
        _append_to_buffer(generator, &params->segment_terminator, 1);
    }

    edi_data = create_dynamic_string(generator->private->buffer, generator->private->buffer_length);

    if(destroy_params != 0) {
        edi_parser_params_destroy(params);
    }
    edi_generator_destroy(generator);
    return edi_data;
}