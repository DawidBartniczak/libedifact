#include "private_libedifact.h"

edi_parser_params_t* _detect_edi_params(char* data) {
    if (strncmp(data, "UNA", 3) != 0) {
        return NULL;
    }

    return edi_parser_params_create(data[8], data[4], data[3], data[5], data[6]);
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
    if((parser = edi_parser_create(params)) == NULL) {
        return NULL;
    }

    size_t allocated_segments = 0, allocated_elements = 0;
    edi_segment_t* current_segment;
    edi_element_t* current_element;
    unsigned char new_segment = 0, new_element = 0, new_subelement = 0;
    while (data && *data)
    {
        if (interchange->segment_count >= allocated_segments) {
            allocated_segments += EDI_SEGMENT_CLUSTER_SIZE;
            if ((interchange->segments = realloc(interchange->segments, allocated_segments * sizeof(edi_segment_t))) == NULL) {
                return NULL;
            }
        }
        current_segment = interchange->segments + interchange->segment_count;
        interchange->segment_count++;
        memset(current_segment, 0, sizeof(edi_segment_t));
        new_segment = 1;
        allocated_elements = 0;

        if (*data == params->segment_terminator)
            data++;
        while (*data && *data != params->segment_terminator) {
            if (new_segment == 1) {
                strncpy_s(current_segment->tag, 4, data, 3);
                data += 3;
                new_segment = 0;
            }

            if (current_segment->element_count >= allocated_elements) {
                allocated_elements += EDI_ELEMENT_CLUSTER_SIZE;
                if ((current_segment->elements = realloc(current_segment->elements, allocated_elements * sizeof(edi_element_t))) == NULL) {
                    return NULL;
                }
            }
            current_element = current_segment->elements + current_segment->element_count;
            memset(current_element, 0, sizeof(edi_element_t));
            current_segment->element_count += 1;

            if (*data == params->element_sep)
                data++;
            while (*data && *data != params->element_sep && *data != params->segment_terminator) {
                current_element->subelement_count++;

                if (*data == params->subelement_sep)
                    data++;
                while (*data && *data != params->subelement_sep && *data != params->segment_terminator && *data != params->element_sep) {
                    data++;
                }
            }
        }

        if (strncmp(current_segment->tag, params->interchange_trailer_tag, 3) == 0) {
            break;
        }
    }
    
    
    edi_parser_destroy(parser);
    if(destroy_parser != 0) {
        edi_parser_params_destroy(params);
    }
    return interchange;
}