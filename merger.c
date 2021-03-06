#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "include/functions.h"
int main(int argc, char const *argv[])
{
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    if (argc < 2)
    {
        printf("Syntax: ./merger <inputfolder> <outputfile=[output.txt]>\n");
        printf("ASCII files must be squared, and must follow nameconvention: part_x_y...\n");
        exit(EXIT_FAILURE);
    }
    char *folder;
    if (!(argv[1][strlen(argv[1])-1] == '/')) {
        folder = concat(argv[1],"/");
    }
    else {
        folder = concat(argv[1],"");
    }
    const char *outputfile = "output.txt";
    if (argc >= 3)
        outputfile = argv[2];

    char **files = NULL;
    int filecount = 0;
    printf("Enumerating files in folder: %s\n", folder);
    getfiles(&files, &filecount, folder);
    if (filecount == 0)
    {
        printf("Empty folder: %s", folder);
        exit(EXIT_FAILURE);
    }
    printf("Total files in %s: %i\n", folder, filecount);

    //map the filenames to positions
    POSITION pos[filecount];
    mapfilepositions(pos,filecount,files);

    //get the size of the FRAMES, 
    //use first file as size template
    char *templatefile = concat(folder, files[0]);
    const int filesize = getfilesize(templatefile);
    free(templatefile);

    //Calculate the framewidth based on
    //the square root, this way we'll have an even
    //matrix. add error handling later.
    const int FRAME_WIDTH = sqrt(filesize);

    //get the maximums of our linear matrix
    int my = max_y(pos, filecount);
    int mx = max_x(pos, filecount);

    //calculate the maximum size of our buffer for each frame.
    const int TOTAL_WIDTH = FRAME_WIDTH * (mx + 1);

    //allocate the array of pointers to match
    //the file sizes.
    char *outputfiledata[(mx + 1) * (my + 1)];

    //Load the files into outputfiledata, in the sequence we want
    load_in_order(mx, my, files, filecount, folder, outputfiledata, filesize, pos);

    //output data is now in correct order, start merge
    int totalrows = merge(outputfiledata,outputfile,TOTAL_WIDTH,FRAME_WIDTH,mx,my);
    printf("Completed: %s\n", outputfile);

    //Show running statistics
    printf("farme-size: %zu, frame-rows:%i, frame-width:%i\n", strlen(outputfiledata[0]),totalrows/(my+1),FRAME_WIDTH);
    printf("frame-x-count: %i, frame-y-count: %i, image-width: %i, image-height: %i\n", mx + 1, my + 1, TOTAL_WIDTH, totalrows);
    
    //end timer
    gettimeofday(&tv2, NULL);
    printf("Total time spent puzzling together %s: %f seconds\n", folder,
           (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 +
               (double)(tv2.tv_sec - tv1.tv_sec));
    
    //Free up memory
    for (int m = 0; m < filecount; m++)
    {
        free(outputfiledata[m]);
        free(files[m]);
    }
    free(folder);
    free(files);return 0;
}