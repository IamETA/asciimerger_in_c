#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "include/functions.h"

int main(int argc, char const *argv[])
{

    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    if (argc < 2) {
        printf("Syntax: ./merger <inputfolder> <outputfile=[output.txt]>\n");
        exit(EXIT_FAILURE);
    }
    const char* folder = argv[1];
    const char* outputfile = "output.txt";
    if (argc >= 3) outputfile = argv[2];

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
    //get the size of the FRAMES
    char * templatefile = concat(folder,files[0]);
    const int filesize = getfilesize(templatefile);
    free(templatefile);
    //Calculate the framewidth based on
    //the square root, this way we'll have an even
    //matrix. add error handling later.
    const int FRAME_WIDTH = sqrt(filesize);

    //get the maximums of our linear matrix 
    int my = max_y(pos,filecount);
    int mx = max_x(pos,filecount);
    
    //calculate the maximum size of our buffer for each frame.
    const int TOTAL_WIDTH = FRAME_WIDTH * (mx+1);

    //allocate the array of pointers to match
    //the file sizes.
    char * outputfiledata[(mx+1)*(my+1)];
    
    //Load the files into outputfiledata, in the sequence we want
    load_in_order(mx,my,files,filecount,folder,outputfiledata,filesize,pos);

    //output data is now in correct order, start merge

    //Calculate the rows we need to loop for writing
    int totalrows = strlen(outputfiledata[0])/(FRAME_WIDTH);
    int row; //the byte position of row
    int page = -1; //Page multiplier

    FILE *fp;
    fp = fopen(outputfile,"w+");
    if(fp==NULL)
        exit(EXIT_FAILURE);
    
    for (int y = 0; y < totalrows*(my+1) ; y++)
    {
        int pager = y % FRAME_WIDTH;
        if (pager == 0)
         page++;
         
        
        row = y%FRAME_WIDTH * (TOTAL_WIDTH / (mx +1));
        int cpos = (page*(mx+1));
        for (int fileindex = cpos; fileindex < cpos+(mx+1); fileindex++)
        {
            for (int i = row; i < (row + FRAME_WIDTH); i++)
            {
                printf("%c", outputfiledata[fileindex][i]);
                fputc(outputfiledata[fileindex][i],fp);
            }
        }
        printf("\n");
        fputc('\n',fp);
    }
    fclose(fp);    
    printf("Completed: %s\n", outputfile);

    //Show running statistics
    printf("size per frame: %zu\n", strlen(outputfiledata[0]));
    printf("Frame width: %i, Frame Height: %i, width: %i, height: %i\n",mx+1,my+1,TOTAL_WIDTH,totalrows * (my+1));
    printf("totalrows per sheet:%i\n",totalrows);    /* stuff to do! */
    
    //Free up memory
    for (int m = 0;m<filecount;m++){
        free(outputfiledata[m]);
        free(files[m]);
    }
    free(files);
    //end timer
    gettimeofday(&tv2, NULL);
    printf ("Total time spent puzzling together %s: %f seconds\n",folder,
    (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
    (double) (tv2.tv_sec - tv1.tv_sec));
    return 0;
}