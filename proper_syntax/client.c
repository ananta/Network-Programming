//: Blocking wala ho

#include <sys/types.h>  // for all types with _t
#include <sys/socket.h> // for function in TCP/UDP flow diagram
#include <netinet/in.h> //for sockaddr_in numbers
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa
#include <stdio.h>      // print f
#include <stdlib.h>     // atoi, itoa, exit
#include <string.h>     // memset
#include <unistd.h>     // close

int main(){
    int client_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client_fd == -1){
        perror("Cannot create a socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successifully\n");

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(5678);
    //sa.sin_addr.s_addr =  yo kaam hamile tala ko function bata assign gareko 

    int res = inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);
    if (res <= 0)
    {
        perror("IP address conversion failed.");
        exit(EXIT_FAILURE);
    }
    printf("Address conversion success\n");

    res = connect(client_fd, (struct sockaddr *) &sa, sizeof sa);

    if(res <= -1){
        perror("Cannot connect to the server");
        exit(EXIT_FAILURE);
    }
    printf("Connection successful \n");
    close(client_fd);
    return EXIT_SUCCESS;
}