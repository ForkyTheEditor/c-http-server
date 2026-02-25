#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>


int main(){
    // PF_INET = IPv4 protocol
    // The socket function can be used for creating a socket (endpoint) for any IPC, returning a file descriptor 
    // i.e. Bluetooth, TCP, UDP etc. Thus both the domain and the type must be specified. 0 refers to the default protocol given the domain and type.

    int socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd == -1){
        perror("An error has occured while creating the socket");
        return -1;
    }
    
    // bind() expects a sockaddr which is the general specification. sockaddr_in is for internet protocol
    // The function associates the socket with an address.
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(80)
    };
    int res = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if(res == 0 ){
        perror("inet_pton: No valid network string for binding");
    }

    if(bind(socketfd, (struct sockaddr *)&addr, sizeof addr) == -1){
        perror("An error has occured while binding the socket");
        close(socketfd);
        return -1;
    };

    // Now we mark the socket as accepting connections. Only necessary for stream-oriented modes (SOCK_STREAM, SOCK_SEQPACKET)
    // Second argument represents the number of connections that can be queued up.
    if(listen(socketfd, 10) == -1){
        perror("Failed to listen on socket");
    }

    while (true){
        // The 2nd and 3rd argument can be a pointer to an addr struct of the connected process
        int connfd = accept(socketfd, NULL, NULL);
        if(connfd == -1){
            perror("Failed to accept connection");
            close(socketfd);
            return -1;
        }

        
        close(connfd);
    }

    close(socketfd);
    return 0;
}   

