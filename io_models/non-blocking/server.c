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
#include <fcntl.h>
#include <errno.h>

void handle_Client(int client_fd);
int main(void)
{

    int server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //protocolfamily,type, protocol
    if (server_fd == -1)
    {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);

    //initializing
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    //logical socket initialized

    if (bind(server_fd, (struct sockaddr *)&sa, sizeof sa) == -1)
    {
        perror("Cannot bind socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //now listen
    if (listen(server_fd, 10) == -1)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        socklen_t len;
        struct sockaddr_in client_sa;
        int client_fd = accept(server_fd, (struct sockaddr *)&client_sa, &len);

        //printf("Client address: %s:%d \n", inet_ntoa(client_sa.sin_addr), ntohs(client_sa.sin_port));

        // int getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);
        // int getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);

        if (0 > client_fd)
        {
            perror("Connect failed.\n");
            close(server_fd);   // socket close gareko since no garbage collector [preventing memory leak]
            exit(EXIT_FAILURE); //continue gare hucha exit garna pardaina
        }

        printf("Server fd: %d \n", server_fd);
        printf("Client fd: %d \n", client_fd);

        struct sockaddr_in localAdd_sa;
        socklen_t len_local;
        struct sockaddr_in peerAdd_sa;
        socklen_t len_peer;

        int localSockRes = getsockname(client_fd, (struct sockaddr *)&localAdd_sa, &len_local);

        printf("Socket name: %s:%d\n", inet_ntoa(localAdd_sa.sin_addr), ntohs(localAdd_sa.sin_port));

        int peerSockRes = getpeername(client_fd, (struct sockaddr *)&peerAdd_sa, &len_peer);

        printf("Peername: %s:%d\n", inet_ntoa(peerAdd_sa.sin_addr), ntohs(peerAdd_sa.sin_port));

        //int getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);//

        printf("Connection accepted\n");
        printf("Creating new process to handle client\n");

        int size = 100;
        char buff[size];

        fcntl(client_fd, F_SETFL, O_NONBLOCK); /* Making it non blocking*/

        int i;
        for (i = 0; i < 15; i++)
        {
            int a = read(client_fd, buff, size);
            if (a < 0)
            {
                printf("a = %d\n", a);
                printf("No data to read. Trying again in 1 sec.\n");
                sleep(1);
            }
            else
            {
                printf("Message from client: %s\n", buff);
                memset(buff, 0, size);
            }
        }

        //resends same message to client[Echo server]
        write(client_fd, buff, strlen(buff));

        if (shutdown(client_fd, SHUT_RDWR) == -1)
        {
            perror("Shutdown failed");
            close(client_fd);
        }
    }
}
