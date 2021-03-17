//not as efficient as concurrent
/*
single server socket can be made concurrent as well using for loop
*/
//fork use garda socket ko refeerence counter or diagram
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h> //perror ko lagi
#include <string.h> //memset ko lagi
#include <unistd.h>

#include <errno.h>

//libraries used for select function
#include<sys/select.h>
#include<sys/time.h>

void handle_Client(int client_fd);
int main(void)
{

    int server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //protocolfamily,type, protocol
    
    if (server_fd == -1)
    {
        perror("Cannot create socket\n");
        exit(EXIT_FAILURE);
    }
    printf("Server_fd: %d\n", server_fd);

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);

    //initializing
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    //logical socket initialized

    if (bind(server_fd, (struct sockaddr *)&sa, sizeof sa) == -1)
    {
        perror("Cannot bind socket\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //now listen
    if (listen(server_fd, 10) == -1)
    {
        perror("Listen failed\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("listen success\n");

    fd_set all_set, read_set;

    FD_ZERO(&all_set);
    FD_SET(server_fd, &all_set);

    int max = server_fd;

    struct timeval interval;
    memset(&interval, 0, sizeof interval);
    interval.tv_sec = 2;


    int i;
    while (1)
    {
       
        read_set = all_set;
        printf("Calling select function\n");
        if(select(max + 1, &read_set, NULL, NULL, &interval) == -1){
            perror("select failed\n");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        /*run through the existing connections looking for data to be read*/
        for (i = server_fd; i <= max; i++)
        {

            printf("checking socket-fd : %d\n", i);
            int issetRes = FD_ISSET(i, &read_set);
            printf("issetRes: %d\n", issetRes);
            if (FD_ISSET(i, &read_set))
            {
                /* we got one... */

                if(i == server_fd){
                    printf("Server is waiting for a new connection\n");
                    int client_fd = accept(server_fd, NULL, NULL);
                    if(0 > client_fd){
                        perror("Accept failed");
                        close(server_fd);
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_fd, &all_set); //add to all set
                    if(client_fd > max)
                        max = client_fd;
                    max = client_fd;
                    printf("new connection accepted\n");
                }
                else{
                    printf("handeling client\n");
                    handle_Client(i);
                    

                }
           }
       }
    }
    
}
void handle_Client(int client_fd){
    int size = 1000;
    char buff[size];
    memset(buff, 0,size);

    int r_len = read(client_fd, buff, size);
    int s_len = write(client_fd, buff, r_len);

    printf("Client message: %s\n", buff);
}



