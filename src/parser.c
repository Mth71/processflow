#include <string.h>
#include "parser.h"

int tokenize(char *line, char**tokens, int max_tokens){
    int cont =0;
    char*p=line;

    while(*p != '\0' && cont < max_tokens){
        //pula os espaços/tabs antes da proxima palavra
        while(*p == ' ' || *p == '\t'){
            p++;
        }


        if(*p == '\0'){
            break;
        }


        tokens[cont]= p;
        cont++;

        while(*p != '\0' && *p != ' ' && *p != '\t'){
            p++;
        }

        if(*p == '\0'){
            break;
        }

        *p = '\0';
        p++;

    }
    return cont;
}