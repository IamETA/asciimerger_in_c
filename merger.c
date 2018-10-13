#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/functions.h"

//Set framewidth for ascii
static int FRAME_WIDTH = 30;

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
        const char * currentfile = files[i];
        //read positions
        pos[i].x = atoi(&currentfile[5]);
        pos[i].y = atoi(&currentfile[7]);
        
        //Confirm positions
        printf("%s positions -> x: %i y: %i \n",currentfile,pos[i].x,pos[i].y);
    }
    int my = max_y(pos,filecount);
    int mx = max_x(pos,filecount);
    
    int TOTAL_WIDTH = FRAME_WIDTH * (mx+1);

    //allocate the file contents
    char * outputfiledata[my+1];
    //outputfiledata = malloc(sizeof(char**)*filecount);

    //Load the files into outputfiledata, in the sequence we want
    load_in_order(mx,my,files,filecount,folder,outputfiledata,pos);

    //output data is now in correct order, start merge
    int row; 
    int page = -1; //Page multiplier
    int totalrows = strlen(outputfiledata[0])/(FRAME_WIDTH);
    for (int y = 0; y < totalrows*(my+1) ; y++)
    {
        int pager = y % FRAME_WIDTH;
        if (pager == 0)
         page++;
        //printf("%i:p%i\n",pager,page);
        row = y%FRAME_WIDTH * (TOTAL_WIDTH / (mx +1));
        //printf("Procesing row: %i\n",row);
        int cpos = (page*(mx+1));
        for (int m = cpos; m < cpos+(mx+1); m++)
        {
            for (int i = row; i < (row + FRAME_WIDTH); i++)
            {
                printf("%c", outputfiledata[m][i]);
            }
        }
        printf("\n");
    }
    printf("size per page: %zu\n", strlen(outputfiledata[0]));
    printf("Width: %i Height: %i\n",mx+1,my+1);
    printf("totalrows per sheet:%i\n",totalrows);
    //Free up memory
    free(files);
    
    return 0;
}