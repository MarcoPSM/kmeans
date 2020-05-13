#include <stdio.h>
#include <stdlib.h>

#define TRUE 1 
#define FALSE 0
#define SPACE ' '

int main(int argc, char *argv[]) {
    int nLines = 0, nAttributes = 0, nAttributesToWork = 0;
    char c, last = SPACE, lastNonSpace;
    int inAttr = TRUE;

    if (argc < 2) {
        printf("Sintaxe: \n%s <nome_ficheiro>\n", argv[0]);
        exit(1);
    }

    FILE *fp=NULL;
    fp=fopen(argv[1], "a+");
    // Check if file exists 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", argv[1]); 
        return 0; 
    } 

    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) {

        if (c == '\n') {
            // Increment nLines if this character is newline 
            nLines++;
            if ( nAttributes > 0 ) {
                //printf("Attr: %d\n", nAttributes);
                nAttributesToWork = (nAttributes < nAttributesToWork || nAttributesToWork == 0) ?
                                    nAttributes : nAttributesToWork;
            } 

            nAttributes = 0;
            if (inAttr) { 
                inAttr = 0; 
            }

            if (c == last) {
                //Decrement nLines, is an empty line
                nLines--;
            }
        } 
        else {
            if (c == '#') {
                if (last == '\n') {
                    //Decrement nLines, is a comment line
                    nLines--;
                }
            }
            else {
                if ( (last == ' ' || last == '\n') && c != last ) {
                    //printf("%c\n", c);
                    inAttr = 1; 
                    nAttributes++; 
                }
                else {
                    inAttr = 0; 
                }
            }
        }

        lastNonSpace = (c==' ') ? last : c;
        last = c;
    }

        

    printf("The file %s has %d lines\n ", argv[1], nLines); 
    printf("The file %s has %d nAttributesToWork\n ", argv[1], nAttributesToWork);
/*
    rewind(fp);
    fprintf(fp, "#START HEADER\n");
    fprintf(fp, "#N=%d\n", nLines);
    fprintf(fp, "#M=%d\n", nAttributesToWork);
    fprintf(fp, "#END HEADER\n");
*/
    fclose(fp);

    return 0;
}


