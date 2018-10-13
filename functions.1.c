#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

typedef struct POSITION {
    int x;
    int y;
} POSITION;

extern char *strdup(const char *src);
void readFile(const char *fileName, char * fcontent)
{
    //char** fcontent = malloc(sizeof(char*));
    FILE *fp = fopen(fileName, "r");
    //    char * line = NULL;
    size_t len = 0;
    size_t lines = 0;
    size_t LINE_SIZE = 30;
    //    size_t read;
    if (fp == NULL)
        exit(EXIT_FAILURE);

    //fill buffer until we have a line
    char chard = fgetc(fp);
    size_t maximumLineLength = 30;
    //char *buffer = (char *)malloc(sizeof(char) * maximumLineLength + 1);
    char buffer[LINE_SIZE+1];
    while (chard != EOF)
    {
        if (len >= maximumLineLength)
        {
            //new line detected
            buffer[LINE_SIZE + 1] = '\0';
            fcontent[lines] = malloc(sizeof(char) * maximumLineLength +1);
            strcpy(fcontent[lines],buffer);
            printf("dat: %s\n",fcontent[lines]);
            lines++;
            len = 0;
        }
        else
        {
            //Add to buffer;
            buffer[len] = chard;
            len++;
        }
        chard = fgetc(fp);
    }

    printf("Data size: %i\n", (int)sizeof(fcontent));
    fclose(fp);
    //return fcontent;
}
//Get files in directory
void getfiles(char ***files, int *count, const char *directory)
{
    *files = malloc(sizeof(char*) +1);
    //int fileSize = 50;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(directory);
    if (dpdf == NULL)
        exit(EXIT_FAILURE);

    //Folder has files, enumerate
    int fileCount = 0;
    while ((epdf = readdir(dpdf)))
    {
        char *directoryName = malloc(sizeof(char*)); //= epdf->d_name; //tried pointing to this, but that didnt work well
        strcpy(directoryName,epdf->d_name);
        if ((directoryName[0] != '.'))
        {
            printf("attemting to resize to: %zu\n",sizeof(char*) * (fileCount+1));
            *files = realloc(*files, sizeof(char*) * (fileCount+1)); //again, +1 for nullterminator
            printf("sizeoffileobj: %zu\n",sizeof(files));
            //Check for alloc errors
            (*files)[fileCount++] = directoryName;
            printf("Found: %s\n", (*files)[fileCount-1]);
        }
    }
    (*count) = fileCount;
    closedir(dpdf);
    free(epdf);
}

//Math :)

//Get the max x value of POSITION
int max_x(POSITION a[], int n) {
  int c, max, index;
 
  max = a[0].x;
  index = 0;
 
  for (c = 1; c < n; c++) {
    if (a[c].x > max) {
       index = c;
       max = a[c].x;
    }
  }
 
  return index;
}

//Get the max Y value of POSITION
int max_y(POSITION a[], int n) {
  int c, max, index;
 
  max = a[0].y;
  index = 0;
 
  for (c = 1; c < n; c++) {
    if (a[c].y > max) {
       index = c;
       max = a[c].y;
    }
  }
 
  return index;
}
//String operations
//Combine
char* concat(const char *s1, const char *s2)
{
    size_t stringsize = strlen(s1) + strlen(s2) + 1;
    char *result = malloc(stringsize); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}