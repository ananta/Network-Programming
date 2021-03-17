#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char msg[100];

    int res;
    int SocketFD;

    SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (SocketFD == -1)
    {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(5000);

    //yo kaam function le garakeo
    //sa.sin_addr.s_addr = htonl(23232323232);

    /*
    192 168 1 5
    C0  A8  1 5
    */
    res = inet_pton(AF_INET, "192.168.100.3", &sa.sin_addr); //ifconfig hanera IP nikalne
    if (res <= 0)
    {
        perror("IP address conversion failed.");
        exit(EXIT_FAILURE);
    }

    // if (connect(SocketFD, (struct sockaddr *)&sa, sizeof sa) == -1)
    // {
    //     perror("Connection failed");
    //     exit(EXIT_FAILURE);
    // }
    int client = connect(SocketFD, (struct sockaddr *)&sa, sizeof sa);
    if (client == -1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // struct sockaddr_in localAdd_sa;
    // memset(&localAdd_sa, 0, sizeof localAdd_sa);
    // socklen_t len_local;
    // struct sockaddr_in peerAdd_sa;
    // memset(&peerAdd_sa, 0, sizeof peerAdd_sa);
    // socklen_t len_peer;

    // int localSockRes = getsockname(SocketFD, (struct sockaddr *)&localAdd_sa, &len_local);

    // printf("Socket name: %s:%d\n", inet_ntoa(localAdd_sa.sin_addr), ntohs(localAdd_sa.sin_port));

    // int peerSockRes = getpeername(SocketFD, (struct sockaddr *)&peerAdd_sa, &len_peer);

    // printf("Peername: %s:%d\n", inet_ntoa(peerAdd_sa.sin_addr), ntohs(peerAdd_sa.sin_port));

    // int pid = fork();
    // if (pid == 0)
    // {
    //     printf("Message from child.:\n");
    // }

    // if (pid > 0)
    // {
    //     printf("Message from parent.:\n");
    // }

    //READ AND WRITE HERE -->
    sprintf(msg, "This is the first program by roll number: %d\n", 43);

    //sleep(10); //wait for 10 sec and send message
    write(SocketFD, msg, strlen(msg));
    printf("Message sent to server and client closed.\n");

    //char msgFromServer[100];

    //read(SocketFD, msgFromServer, 100);
    //printf("Message from server: %s\n", msgFromServer);

    //read(ConnectFD, buff, size)

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
    return EXIT_SUCCESS;
}
