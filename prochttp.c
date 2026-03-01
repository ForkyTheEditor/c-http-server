#include <string.h>
#include <stdio.h>

#define GET "GET"
#define HTTPV11 "HTTP/1.1" 


void parseRequest(char *buffer, char *response){
    char *req_type;
    char *path;
    char *httpv;

    int current_index = 0;
    char *result;

    result = strtok(buffer, " ");
    
    while( result != NULL){
        if(current_index == 0){
            req_type = result;
        }
        else if(current_index == 1){
            path = result;
        }
        else if(current_index == 2){
            httpv = result;
        }
        result = strtok(NULL, " ");
        current_index++;
    }

    if(strcmp(req_type, GET) == 0){
        // GET Request
        sprintf(response, "%s 200 OK\r\n\r\nRequested path: %s \r\n", HTTPV11, path);
    }
}
