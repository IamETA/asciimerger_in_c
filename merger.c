#include <stdio.h>
#include <stdlib.h>
#include "../include/functions.h"

//Set framewidth for ascii
static int FRAME_WIDTH = 90;

//extern POSITION;
int max_x(const POSITION a[], int n);
int max_y(const POSITION a[], int n);


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Syntax: ./merger <inputfolder>\n");
        exit(EXIT_FAILURE);
    }
    const char* folder = argv[1];

    char** files = NULL;
    int filecount = 0; 
    printf("Enumerating files in folder: %s\n",folder);
    getfiles(&files,&filecount,folder);
    if (filecount <= 0) {
        printf("Empty folder: %s",folder);
        exit(EXIT_FAILURE);
    }
    printf("Total files in %s: %i\n",folder,filecount);

    //Initialize POSITION part -> remember to free
    POSITION pos[filecount];// = malloc(sizeof(POSITION) * filecount);
    for (int i=0;i<filecount;i++) {
        char * currentfile = files[i];
        //read positions
        pos[i].x = atoi(&currentfile[5]);
        pos[i].y = atoi(&currentfile[7]);
        
        //Confirm positions
        printf("%s positions -> x: %i y: %i \n",currentfile,pos[i].x,pos[i].y);
    }
    int my = max_y(pos,filecount);
    int mx = max_x(pos,filecount);
    //get the width per page based on FRAME_WIDTH
    int ppagewidth = FRAME_WIDTH/(mx+1);
    //char outputbuffer[my][my* mx * FRAME_WIDTH + 1];
    char outputfiledata[filecount][(FRAME_WIDTH * 10) + 1];

    //load the files in order
    int order = 0;
    for (int y=0;y<=my;y++) {
        for (int x=0;x<=mx;x++) {
            for (int i=0;i<filecount;i++) {
                if (pos[i].y == y && pos[i].x == x) {
                    char * fullfilepath = concat(folder,files[i]);
                    printf("Reading file %s\n",fullfilepath);
                    free(files[i]);
                    readFile(fullfilepath,outputfiledata[order]);
                    order++;
                }
            }
        }
    }
    //output data is now in correct order, start merge
    int row;
    int page = -1;
    int totalrows = (sizeof(outputfiledata[0])-1)/(ppagewidth);
    for (int y = 0; y < totalrows*(my+1) ; y++)
    {
        int pager = y % ppagewidth;
        if (pager == 0)
         page++;
        //printf("%i:p%i\n",pager,page);
        row = y%ppagewidth * (FRAME_WIDTH / (mx +1));
        //printf("Procesing row: %i\n",row);
        int cpos = (page*(mx+1));
        for (int m = cpos; m < cpos+(mx+1); m++)
        {
            for (int i = row; i < (row + ppagewidth); i++)
            {
                printf("%c", outputfiledata[m][i]);
            }
        }
        printf("\n");
    }
    printf("size per page: %zu\n", sizeof(outputfiledata[0]));
    printf("Width: %i Height: %i\n",mx+1,my+1);
    printf("totalrows per sheet:%i\n",totalrows);
    //Free up memory
    free(files);
    
    return 0;
}
