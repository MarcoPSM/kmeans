
#include "general.h"
#include "datasetFile.h"

int getNumberOfEntities(char *filename) {
    int nLines = 0;
    char c, last = SPACE;

    FILE *fp=NULL;
    fp=fopen(filename, "r");
    // Check if file exists 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", filename); 
        return 0; 
    } 

    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) {

        if (c == '\n') {
            // Increment nLines if this character is newline 
            nLines++;
            
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
        }
        last = c;
    }

    printf("The file %s has %d lines\n", filename, nLines); 

    fclose(fp);
    return nLines;
}

int getNumberOfDimensions(char *filename) {
    int nLines = 0, nAttributes = 0, nAttributesToWork = 0;
    char c, last = SPACE, lastNonSpace;
    int inAttr = FALSE;

    FILE *fp=NULL;
    fp=fopen(filename, "r");
    // Check if file exists 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", filename); 
        return 0; 
    } 

    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) {

        if (c == '\n') {
            // Increment nLines if this character is newline 
            nAttributes++;
            break;
        } 
        else {
            if (c == '#') {
                break;
            }
            else {
                if (  c == SPACE ) {
                    inAttr = FALSE;
                    nAttributes++;
                }
                else {
                    inAttr = TRUE; 
                }
            }
        }
    }
    printf("The file %s has %d nAttributesToWork\n", filename, nAttributes);

    fclose(fp);

    return nAttributes;
}
