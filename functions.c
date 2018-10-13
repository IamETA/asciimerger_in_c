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
    size_t len = 0;
    
    if (fp == NULL)
        exit(EXIT_FAILURE);

    //fill buffer until we have a line
    char chard = fgetc(fp);
    while (chard != EOF)
    {
        //Add to buffer;
        fcontent[len] = chard;
        len++;
        chard = fgetc(fp);
    }
    fcontent[len] = '\0';
    fclose(fp);
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
            *files = realloc(*files, sizeof(char*) * (fileCount+1)); //again, +1 for nullterminator
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
int max_x(const POSITION * a, int n) {
  int c, max;
 
  max = a[0].x;
  
  for (c = 1; c < n; c++) {
    if (a[c].x > max) {
       max = a[c].x;
    }
  }
 
  return max;
}

//Get the max Y value of POSITION
int max_y(const POSITION a[], int n) {
  int c, max;
  max = a[0].y; 
  for (c = 1; c < n; c++) {
    if (a[c].y > max) {
       max = a[c].y;
    }
  }
 
  return max;
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