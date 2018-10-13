#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "include/functions.h"

extern char *strdup(const char *src);

int getfilesize(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    fseek(fp, 0, SEEK_END); // seek to end of file
    int size = ftell(fp);   // get current file pointer
    fclose(fp);
    return size;
}
void readfromfile(const char *fileName, char *fcontent)
{
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
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(directory);
    if (dpdf == NULL)
        exit(EXIT_FAILURE);

    //Folder has files, enumerate
    int fileCount = 0;
    while ((epdf = readdir(dpdf)))
    {
        //Dropping two bytes in 2 blocks somewhere in this code, don't know where.
        char *directoryName = malloc(strlen(epdf->d_name) + 1); //= epdf->d_name; //tried pointing to this, but that didnt work well
        strcpy(directoryName, epdf->d_name);
        if ((directoryName[0] != '.'))
        {
            *files = realloc(*files, sizeof(char *) * (fileCount + 1)); //again, +1 for nullterminator
            (*files)[fileCount++] = directoryName;
            printf("Found: %s\n", (*files)[fileCount - 1]);
        }
        else
        {
            //Free the unwanted directories
            free(directoryName);
        }
    }
    (*count) = fileCount;
    closedir(dpdf);
}

//Merger
void load_in_order(int mx, int my, char **files, int filecount, const char *folder, char *outputfiledata[], int filesize, POSITION pos[])
{
    //load the files in the order we want
    //we want to load the vertical files after the horizontal
    int order = 0;
    for (int y = 0; y <= my; y++)
    {
        for (int x = 0; x <= mx; x++)
        {
            for (int i = 0; i < filecount; i++)
            {
                if (pos[i].y == y && pos[i].x == x)
                {
                    //get the full filepath
                    char *fullfilepath = concat(folder, files[i]);
                    outputfiledata[order] = malloc(sizeof(char) * filesize + 1);
                    printf("Reading file %s\n", fullfilepath);
                    readfromfile(fullfilepath, outputfiledata[order]);
                    free(fullfilepath);
                    order++;
                }
            }
        }
    }
}

//Math operations
//Get the max x value of POSITION
int max_x(const POSITION *a, int n)
{
    int c, max;

    max = a[0].x;

    for (c = 1; c < n; c++)
    {
        if (a[c].x > max)
        {
            max = a[c].x;
        }
    }
    return max;
}

//Get the max Y value of POSITION
int max_y(const POSITION a[], int n)
{
    int c, max;
    max = a[0].y;
    for (c = 1; c < n; c++)
    {
        if (a[c].y > max)
        {
            max = a[c].y;
        }
    }
    return max;
}

//String operations
//Combine
char *concat(const char *s1, const char *s2)
{
    size_t stringsize = strlen(s1) + strlen(s2) + 1;
    char *result = malloc(stringsize); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}