#include <string.h>
#include <stdio.h>
#include "prochttp.h"

#define GET "GET"
#define HTTPV11 "HTTP/1.1" 
#define WWWDIR "www"
#define DEFAULTFILE "index.html"

struct request {
    char *req_type;
    char *path;
    char *httpv;
};

// request comes in -> REQUEST_TYPE PATH/TO/FILE HTTPV
//  -> find file of path safely (only in www folder) 
// -> read file and return contents
void parseRequest(char *buffer, char *response){
    struct request req;

    int current_index = 0;
    char *result;

    result = strtok(buffer, " ");
    
    while( result != NULL){
        if(current_index == 0){
            req.req_type = result;
        }
        else if(current_index == 1){
            req.path = result;
        }
        else if(current_index == 2){
            req.httpv = result;
        }
        result = strtok(NULL, " ");
        current_index++;
    }

    if(strcmp(req.req_type, GET) == 0){
        // GET Request
        char file_content[16384];
        readFileFromPath(req.path, file_content);
        sprintf(response, "%s 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n%s", HTTPV11, file_content);
    }
}


void readFileFromPath(char *path, char *contents){
    if(strcmp(path, "/") == 0 || strcmp(path, "..") == 0){
        path = "/index.html";
    } 

    char final_path[512];
    snprintf(final_path, sizeof(final_path),"%s/%s", WWWDIR, path);

    FILE *fptr = fopen(final_path, "r");
    if(fptr == NULL){
        perror("File could not be opened");
        return;
    }

    int c;
    int index = 0;
    while((c = fgetc(fptr)) != EOF){
        contents[index] = c;
        index++;
    }

    fclose(fptr);
}