#include <sys/types.h> // for all types with _t
#include <sys/socket.h> // for function in TCP/UDP flow diagram
#include <netinet/in.h> //for sockaddr_in numbers
#include <arpa/inet.h> // for sockaddr_in and inet_ntoa
#include <stdio.h> // print f
#include <stdlib.h> // atoi, itoa, exit 
#include <string.h>// memset 
#include <unistd.h> // close
#include<sys/select.h>
#include<sys/time.h>

void handleClient(int client_fd);

int main(){

    int s_fd = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP);
    if(s_fd == -1){
        perror("Cannot create a socket.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.!\n");
    struct sockaddr_in sa;


    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(5678);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);


    if(bind(s_fd, (struct sockaddr *) &sa, sizeof sa) == -1){
        perror("Bind unsuccessful.\n");
        exit(EXIT_FAILURE);
    }
    printf("Bind successful.\n");

    if(listen(s_fd, 10) == -1){
        perror("Listening failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("Listen successful.\n");

    fd_set all_set, read_set;
    FD_ZERO(&all_set);
    FD_SET(s_fd, &all_set);//(source, destination)

    struct timeval interval;
    memset(&interval, 0, sizeof interval);
    interval.tv_sec = 5;

    int max = s_fd;

    while (1)
    {
        
        read_set = all_set;
        //FD_SETSIZE

        if (select(FD_SETSIZE, &read_set, NULL, NULL, &interval) == -1)
        {
            perror("select failed\n");
            close(s_fd);
            exit(EXIT_FAILURE);
        }
        //FD_SETSIZE
        for (int i = s_fd; i <= FD_SETSIZE; i++)
        {
            printf("Checking socket: %d \n", i);
            int issetRes = FD_ISSET(i, &read_set);
            printf("issetRes: %d\n", issetRes);

            if (FD_ISSET(i, &read_set))
            {
                if( i == s_fd){

                    //accept mechanism
                    printf("Server is waiting for a new connection\n");
                    int client_fd = accept(s_fd, NULL, NULL);
                    if(client_fd == -1){
                        perror("Accept failed.\n");
                        exit(EXIT_FAILURE);
                    }
                    
                        printf("Accept success.\n");
                        FD_SET(client_fd, &all_set);

                        
                    }
                    else
                {
                    //handle client
                    handleClient(i);
                    close(i);
                    FD_CLR(i, &all_set);
                }
                    
                }
                
            }
        }
        return EXIT_SUCCESS;
        }
    
    

void handleClient(int client_fd)
{

    int size = 1000;
    char buffer[size];
    memset(buffer, 0, size);

    //read is a blocking statement as accept
    int r_len = read(client_fd, buffer, size); //array pass garda sadhai size ni pathaune
                                               //golden rule
    int s_len = write(client_fd, buffer, r_len);
}
