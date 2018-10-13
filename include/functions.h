#ifndef FUNCTIONS_H_   /* Include guard */
#define FUNCTIONS_H_
typedef struct POSITION {
    int x;
    int y;
} POSITION;

//File operations
void readFile(const char* fileName, char* fcontent);
void getfiles(char*** files,int* count, const char * directory);

//Math operations
int max_x(const POSITION a[], int n);
int max_y(const POSITION a[], int n);

//String operations
char* concat(const char *s1, const char *s2);

#endif // FUNCTIONS_H_
