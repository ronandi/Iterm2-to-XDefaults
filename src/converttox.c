/*
 * converttox.c
 * A part of Iterm-to-Xdefaults
 * To convert Iterm2 themes to Xdefaults compatible
 * Author: Rohit Kumar
 * Contact: rohit.kumar@rutgers.edu
 * Last Updated: Dec. 24th 2011
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Print usage message */
void printUsage(char * name) {
    printf("Usage: %s <iterm file to convert>\n", name);
}

void parseNumColor(int colornum, FILE * inputfile) {
    char linebuf[128];
    long double r, g, b;
    fgets(linebuf, sizeof(linebuf), inputfile);     //should be <dict> line. throw out
    while(fgets(linebuf, sizeof(linebuf), inputfile), strcmp("\t</dict>\n", linebuf) != 0) {
        char * s, * delim;
        delim = "\t<>/ ";
        strtok(linebuf, delim);     //throw out first
        s = strtok(NULL, delim);
        if (strcmp(s, "Blue") == 0) {
            fgets(linebuf, sizeof(linebuf), inputfile);
            strtok(linebuf, delim);     //throw out first
            s = strtok(NULL, delim);
            b = strtold(s, NULL);
        } else if (strcmp(s, "Green") == 0) {
            fgets(linebuf, sizeof(linebuf), inputfile);
            strtok(linebuf, delim);     //throw out first
            s = strtok(NULL, delim);
            g = strtold(s, NULL);
        } else if (strcmp(s, "Red") == 0) {
            fgets(linebuf, sizeof(linebuf), inputfile);
            strtok(linebuf, delim);     //throw out first
            s = strtok(NULL, delim);
            r = strtold(s, NULL);
        }
    }
    printf("*color%d: rgbi:%1.17Lf/%1.17Lf/%1.17Lf\n", colornum, r, g, b);
}

/* Process input itermcolors file */
void parseFile(FILE * inputfile) {
    char linebuf[128];
    while (fgets(linebuf, sizeof(linebuf), inputfile) != NULL) {    //Read file line by line
        if (strncmp("\t<key>", linebuf, 6) == 0) {
            char * s, * delim;
            delim = "\t<>/ ";
            strtok(linebuf, delim);             //First strok should be the tag name. We can skip
            while(s = strtok(NULL, delim), s != NULL) {
                if(strcmp(s, "Ansi") == 0) {     //We have found one of the numbered ANSI colors
                    int colornum;
                    colornum = atoi(strtok(NULL, delim));
                    parseNumColor(colornum, inputfile);
                }
                //TODO: Handle Background, Foreground, Cursor...
            }
        }
    }
}

int main(int argc, char ** argv) {
    if (argc != 2) {            //Check for correct # of args
        printUsage(argv[0]);
        exit(-1);
    }

    FILE * inputfile = fopen(argv[1], "r");
    if (inputfile == NULL) {    //Check if file exists / readable
        perror(NULL);
        exit(-1);
    }

    parseFile(inputfile);
    fclose(inputfile);
    return 0;
}

