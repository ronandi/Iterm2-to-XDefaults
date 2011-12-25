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

void parseColor(char * prefix, FILE * inputfile) {
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
    printf("%s: rgbi:%1.17Lf/%1.17Lf/%1.17Lf\n", prefix, r, g, b);
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
                char prefix[15];
                if(strcmp(s, "Ansi") == 0) {     //We have found one of the numbered ANSI colors
                    char * colornum;
                    colornum = strtok(NULL, delim);
                    sprintf(prefix, "*color%s", colornum);
                }
                else if(strcmp(s, "Background") == 0) {
                    sprintf(prefix, "*background");
                }
                else if(strcmp(s, "Foreground") == 0) {
                    sprintf(prefix,"*foreground");
                }
                else if (strcmp(s, "Bold") == 0) {
                    sprintf(prefix,"*colorBD");
                }
                else if(strcmp(s, "Cursor") == 0) {
                    s = strtok(NULL, delim);
                    if (strcmp(s, "Color") == 0)
                        sprintf(prefix, "*cursorColor");
                    else if (strcmp(s, "Text") == 0)
                        sprintf(prefix, "*cursorColor2");
                }
                else {
                    continue;
                }
                parseColor(prefix, inputfile);
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
    
    printf("Paste the following in your XDefaults/Xresources file after commenting out old values.\n\n");
    parseFile(inputfile);
    printf("\n");
    fclose(inputfile);
    return 0;
}

