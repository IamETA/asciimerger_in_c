#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "include/functions.h"

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Syntax: ./merger <inputfolder> <outputfile=[output.txt]>\n");
        exit(EXIT_FAILURE);
    }
    const char* folder = argv[1];
    const char* outputfile = "output.txt";
    if (argc >= 3) outputfile = argv[2];
    //else outputfile = "output.txt";

    char** files = NULL;
    int filecount = 0; 
    printf("Enumerating files in folder: %s\n",folder);
    getfiles(&files,&filecount,folder);
    if (filecount <= 0) {
        printf("Empty folder: %s",folder);
        exit(EXIT_FAILURE);
    }
    printf("Total files in %s: %i\n",folder,filecount);

    //map the filenames to positions
    POSITION pos[filecount];
    for (int i=0;i<filecount;i++) {
        const char * currentfile = files[i];
        //read positions
        //Future version: find first integer in string
        pos[i].x = atoi(&currentfile[5]);
        //Future version: find second integer in string
        pos[i].y = atoi(&currentfile[7]);
        
        //Confirm positions
        printf("%s positions -> x: %i y: %i \n",currentfile,pos[i].x,pos[i].y);
    }
    //set the size of FRAMES
    char * templatefile = concat(folder,files[0]);
    int filesize = getfilesize(templatefile);
    free(templatefile);
    //Calculate the framewidth;
    int FRAME_WIDTH = sqrt(filesize);

    int my = max_y(pos,filecount);
    int mx = max_x(pos,filecount);
    
    int TOTAL_WIDTH = FRAME_WIDTH * (mx+1);

    //allocate the array of pointers to file contents
    char * outputfiledata[(mx+1)*(my+1)];
    
    //Load the files into outputfiledata, in the sequence we want
    load_in_order(mx,my,files,filecount,folder,outputfiledata,filesize,pos);

    //output data is now in correct order, start merge
    int row; 
    int page = -1; //Page multiplier
    int totalrows = strlen(outputfiledata[0])/(FRAME_WIDTH);
    FILE *fp;
    fp = fopen(outputfile,"w+");
    if(fp==NULL)
        exit(EXIT_FAILURE);
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
                fputc(outputfiledata[m][i],fp);
            }
        }
        printf("\n");
        fputc('\n',fp);
    }
    fclose(fp);
    printf("size per page: %zu\n", strlen(outputfiledata[0]));
    printf("Width: %i Height: %i\n",mx+1,my+1);
    printf("totalrows per sheet:%i\n",totalrows);
    //Free up memory
    printf("Freeing up memory\n");
    for (int m = 0;m<filecount;m++){
        free(outputfiledata[m]);
        free(files[m]);
    }
    free(files);
    printf("Completed: %s", outputfile);
    return 0;
}