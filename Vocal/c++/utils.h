#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fatal(char* msg) {
    char error_msg[100];
    strcpy(error_msg, "** Fatal error ");
    strncat(error_msg, msg, 83);
    perror(error_msg);
    exit(-1);
}

void debug(char* msg) {
    char debug_msg[100];
    strcpy(debug_msg, "[DEBUG] ");
    strncat(debug_msg, msg, 83);
    printf("%s\n", debug_msg);
    exit(-1);
}
