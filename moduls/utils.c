#include "utils.h"

char* UTILS_cleanLabel(char cadena[11]){
    char *token;
    char s[2] = " ";
    token = strtok(cadena, s);
   return token;
}

char* UTILS_fileInDirectory(char* directory, char* fileName){
    char *fullPath = malloc(sizeof(char)*(strlen(directory) + strlen(fileName) + 3));;
    strcpy(fullPath, directory);
    strcat(fullPath, "/");
    strcat(fullPath, fileName);
   return fullPath;
}