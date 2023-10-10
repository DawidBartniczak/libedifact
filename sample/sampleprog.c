#include <stdio.h>

#include "libedifact.h"

char* edi_data =    "UNA:+.? '"
                    "UNB+UNOC:3+30-051-2308:ZZ+yourDUNS:14+230417:1443+536'"
                    "UNH+1+ORDRSP:D:96A:UN'"
                    "BGM+231+2001710702'"
                    "DTM+137:20230411:102'"
                    "RFF+ON:4501838443'"
                    "DTM+4:20230411:102'"
                    "NAD+DP+yourDUNS::91'"
                    "NAD+SU+30-051-2308::91'"
                    "NAD+BY+yourDUNS::91'"
                    "CUX+2:EUR:9'"
                    "LIN+10++MXQ8-10-X2480:SA'"
                    "PIA+1+03328262:BP'"
                    "QTY+12:2:PCE'"
                    "DTM+55:20230522:102'"
                    "PRI+AAA:999.99:::1:PCE'"
                    "PRI+AAB:1234.56:::1:PCE'"
                    "RFF+ON:4501838443:000010'"
                    "LIN+20++CDQSB16-5S:SA'"
                    "PIA+1+01195682:BP'"
                    "QTY+12:1:PCE'"
                    "DTM+55:20230522:102'"
                    "PRI+AAA:999.99:::1:PCE'"
                    "PRI+AAB:1234.56:::1:PCE'"
                    "RFF+ON:4501838443:000020'"
                    "UNS+S'"
                    "UNT+54+1'"
                    "UNZ+1+536'";

int main(int argc, char** argv){
    edi_parser_t* parser = edi_parser_create(200);

    printf("Parser created\n%d", parser->auto_detect_params);

    edi_parser_destroy(parser);
    return 0;
}