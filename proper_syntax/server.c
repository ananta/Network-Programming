//: Blocking wala ho 

#include <sys/types.h> // for all types with _t
#include <sys/socket.h> // for function in TCP/UDP flow diagram
#include <netinet/in.h> //for sockaddr_in numbers
#include <arpa/inet.h> // for sockaddr_in and inet_ntoa
#include <stdio.h> // print f
#include <stdlib.h> // atoi, itoa, exit 
#include <string.h>// memset 
#include <unistd.h> // close

int main(){


                            //family, type, protocol
    int server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(server_fd == -1){
        perror("Cannot create a socket!\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.!\n");

    struct sockaddr_in sa;

    //intitializing garbage value huna sakhcha so
    memset(&sa, 0, sizeof sa );

    sa.sin_family = AF_INET;
    sa.sin_port = htons(5678);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_fd, (struct sockaddr *) &sa, sizeof sa) == -1){
        perror("Bind failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Bind success\n");

    //listening 
    if(listen(server_fd, 10) == -1){ // back log bhaneko queue i.e kati ota listen garna sakcha server le
                                    //hit and trial 
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Listen success\n");

    for(;;){
        int c_fd = accept(server_fd, NULL, NULL); //client ko scoket_fd paucha
        if(c_fd < 0){
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        printf(" Connection accepted from Client:%d \n", c_fd);

        close(c_fd);
    }
  
    close(server_fd);

    return EXIT_SUCCESS;
}