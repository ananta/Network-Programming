//fork use garda socket ko refeerence counter or diagram
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h> //perror ko lagi
#include <string.h> //memset ko lagi
#include <unistd.h>

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

        struct sockaddr_in localAdd_sa;
        socklen_t len_local;
        struct sockaddr_in peerAdd_sa;
        socklen_t len_peer;

        int localSockRes = getsockname(client_fd, (struct sockaddr *)&localAdd_sa, &len_local);

        printf("Socket name: %s:%d\n", inet_ntoa(localAdd_sa.sin_addr), ntohs(localAdd_sa.sin_port));

        int peerSockRes = getpeername(client_fd, (struct sockaddr *)&peerAdd_sa, &len_peer);

        printf("Peername: %s:%d\n", inet_ntoa(peerAdd_sa.sin_addr), ntohs(peerAdd_sa.sin_port));

        //int getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);//

        if (0 > client_fd)
        {
            perror("Connect failed.\n");
            close(server_fd);   // socket close gareko since no garbage collector [preventing memory leak]
            exit(EXIT_FAILURE); //continue gare hucha exit garna pardaina
        }
        printf("Connection accepted\n");
        printf("Creating new process to handle client");
        

        int pid = fork();

        if (pid == 0)
        {
            //child ma parent lai close

            //frees resource
            close(server_fd); //---

            //child process

            handle_Client(client_fd);

            //read write yaha
            printf("Client handeling complete");
            close(client_fd);
            printf("Socket Closed\n");
            exit(0);
        }
        else
        {
            // parent ma client_fd lai close
            close(client_fd); //---//NEVER ENDING SERVER
        }
    }
}

void handle_Client(int client_fd)
{
    //messages ko lai buffer
    int size = 100;
    char buff[size];

    read(client_fd, buff, size);
    printf("Message from client: %s\n", buff);

    //resends same message to client[Echo server]
    write(client_fd, buff, strlen(buff));

    if (shutdown(client_fd, SHUT_RDWR) == -1)
    {
        perror("Shutdown failed");
        close(client_fd);
    }
}
