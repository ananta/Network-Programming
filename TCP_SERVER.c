/*
Dilip Kumar Shrestha7 : 48 AM
        heartbeat or
    are you alive(aya)
    ? request-- are you alive
    ? response-- i am alive.

      echo server
      client can send anything server should reply same message to server

      time server
      client ask for time-- message can be anything
          server reply with time
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

      int main(void){
    int size = 100;
    char buff[size];


    struct sockaddr_in sa;

    //allocate resource
    //system call 
    int socketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*
    AF = Address Family
    PF = Protocol Family
    */
    if(socketFD == -1){
        perror("Cannot create socket.");    //like print error
        exit(EXIT_FAILURE);

    }
    memset(&sa, 0, sizeof sa);
    //type of sockaddr_in
    
    //memset(&sa,0, sizedof(struct sockaddr_in)sa);

    // bzero(&sa, sizedof a);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(5678); //direct assign garna painna suru ma host order ma hucha teslai netw byte order ma change garnu parcha
    //hton = host to network
    sa.sin_addr.s_addr = htonl(INADDR_ANY);


    //requires generic type so tyecasting sa required
    //sockaddr --> sockaddr_in
    //tyecasting since it is a structure so struct pani lekhna parcha
    //pointer bhako le poiner casting ho bhanna parcha
    int res = bind(socketFD, (struct sockaddr *) &sa, sizeof sa);// length kina pathako bhanda ipv4 or ipv6 ma length kati bela aaucha thaha hunna 1st ki last tei bhayera

    if(res == -1){
        perror("Cannot bind socketFD and structure"); 
        close(socketFD);// socket close gareko since no garbage collector [preventing memory leak] 
        exit(EXIT_FAILURE);//like print error
    }
    printf("Bind Success\n");

    if(listen(socketFD, 10) == -1){
         perror("Listen failed"); 
        close(socketFD);// socket close gareko since no garbage collector [preventing memory leak] 
        exit(EXIT_FAILURE);//like print error
    }
    printf("Listen Success\n");

    for(;;){ //infinite loop


    printf("Before accept\n");

    socklen_t len;
    struct sockaddr_in client_sa;
    int ConnectFD = accept(socketFD, (struct sockaddr *)&client_sa, &len);

    // int client_fd = accept(server_fd, (struct sockaddr *)&client_sa, &len);
    //printf("Client address: %s:%d \n", inet_ntoa(client_sa.sin_addr), ntohs(client_sa.sin_port));

    // int getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);
    // int getpeername(int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);

    struct sockaddr_in localAdd_sa;
    socklen_t len_local;
    struct sockaddr_in peerAdd_sa;
    socklen_t len_peer;

    int localSockRes = getsockname(ConnectFD, (struct sockaddr *)&localAdd_sa, &len_local);

    printf("Socket name: %s:%d\n", inet_ntoa(localAdd_sa.sin_addr), ntohs(localAdd_sa.sin_port));

    int peerSockRes = getpeername(ConnectFD, (struct sockaddr *)&peerAdd_sa, &len_peer);

    printf("Perrname: %s:%d\n", inet_ntoa(peerAdd_sa.sin_addr), ntohs(peerAdd_sa.sin_port));

    //fork

    printf("After accept\n");

    if (0 > ConnectFD)
    {
        perror("Connect failed failed"); 
        close(socketFD);// socket close gareko since no garbage collector [preventing memory leak] 
        exit(EXIT_FAILURE);//continue gare hucha exit garna pardaina
        }
        printf("Connection accepted\n");






        //read write yaha
        read(ConnectFD, buff, size);
        printf("Message from client: %s\n", buff);


////////////Creatting an echo server that sends the same message to client
        write(ConnectFD, buff, strlen(buff));





        if(shutdown(ConnectFD, SHUT_RDWR) == -1){
            perror("Shutdown failed"); 
            close(ConnectFD);
            close(socketFD);// socket close gareko since no garbage collector [preventing memory leak] 
            exit(EXIT_FAILURE);
        }
        close(ConnectFD);
        printf("Socket Closed\n");
    }
    close(socketFD);
    return EXIT_SUCCESS;


}