
#include <sys/types.h>  // for all types with _t
#include <sys/socket.h> // for function in TCP/UDP flow diagram
#include <netinet/in.h> //for sockaddr_in numbers
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa
#include <stdio.h>      // print f
#include <stdlib.h>     // atoi, itoa, exit
#include <string.h>     // memset
#include <unistd.h>     // close
#include <fcntl.h>

//fcntrl( targetFile, command, value)



//TODO: Explain fork is called once but returned twice?
//concurrent server bhaneko k ho and kasari kaam garcha
//difference between close and shutdown
//child le close garda ni kina server chalcha ? (reference count ko kura)

void handleClient(int client_fd);

int main(){
    int s_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(s_fd == -1){
        perror("Cannot create a socket.");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.!\n");

    struct sockaddr_in sa;

    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(5678);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(s_fd,(struct sockaddr *)&sa, sizeof sa) == -1){
        perror("Build failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Bind success.\n");

    if(listen(s_fd, 10 ) == -1){
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Listen success.\n");

    fcntl(s_fd, F_SETFL, O_NONBLOCK);
    printf("Trying to handle multiple connection once.\n");
    printf("Server_fd: %d\n", s_fd);
    int i = 0;
    int last_fd = s_fd;
    printf("Last fd: %d\n", last_fd);

    int c_fd;

        while (1)
    {

        for(i= s_fd; i<= last_fd; i++){
            if(i == s_fd){
                printf("Tying to accept client\n");
                c_fd = accept(s_fd, NULL, NULL );
                if(c_fd < 0){
                    
                    printf("EWOULDBLOCK_ERROR\n");
                     
                }else{
                    printf("Accepted from client: %d\n", c_fd);
                    fcntl(c_fd, F_SETFL, O_NONBLOCK);
                    last_fd = c_fd;
                }
               
            }else{
                handleClient(i);
                
            }
            //sleep(2);
        }
    }
    close(s_fd);
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
}// void handleClient(int client_fd)


// int main()
// {

//     //family, type, protocol
//     int server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

//     if (server_fd == -1)
//     {
//         perror("Cannot create a socket!\n");
//         exit(EXIT_FAILURE);
//     }
//     printf("Socket created successfully.!\n");

//     struct sockaddr_in sa;

//     //intitializing garbage value huna sakhcha so
//     memset(&sa, 0, sizeof sa);

//     sa.sin_family = AF_INET;
//     sa.sin_port = htons(5678);
//     sa.sin_addr.s_addr = htonl(INADDR_ANY);

//     if (bind(server_fd, (struct sockaddr *)&sa, sizeof sa) == -1)
//     {
//         perror("Bind failed\n");
//         exit(EXIT_FAILURE);
//     }
//     printf("Bind success\n");

//     //listening
//     if (listen(server_fd, 10) == -1)
//     {   // back log bhaneko queue i.e kati ota listen garna sakcha server le
//         //hit and trial
//         perror("listen failed");
//         exit(EXIT_FAILURE);
//     }
//     printf("Listen success\n");

//      fcntl(server_fd, F_SETFL, O_NONBLOCK);

//     for (;;)
//     {
//         int c_fd = accept(server_fd, NULL, NULL); //client ko scoket_fd paucha
//         if (c_fd < 0)
//         {
//             close(server_fd);
//             exit(EXIT_FAILURE);
//         }

//         //TODO: associativity eg. '=' operator ko which side paila execute garne
//         //TODO: precedence : kun operator paila garne  bhanne

//         // '=' le sadhai right paila execute garcha

//         //fork le child ko process id parent lai dincha
//         int pid = fork();

//         //pid = 0 then it is a child

//         if (pid == 0)
//         {
//             close(server_fd);
//             printf("Child process\n");
//             //handeling client
//             handleClient(c_fd);
//             close(c_fd);
//         }
//         else
//         {
//             printf("Parent process\n");
//             close(c_fd);
//         }
//         printf(" Connection accepted from Client:%d \n", c_fd);
//         // close(c_fd);
//     }

//     close(server_fd);

//     //difference between colse and shutdown :
//     /*
//     - close re reference count gataucha ani ref count 0 bhayo bhane shutdown call garcha
//     - shutdown le TCP LAYER MAI gayera socket close garcha
//     */

//     return EXIT_SUCCESS;
// }
// void handleClient(int client_fd)
// {
//     int size = 1000;
//     char buffer[size];
//     memset(buffer, 0, size);

//     //read is a blocking statement as accept
//     int r_len = read(client_fd, buffer, size); //array pass garda sadhai size ni pathaune
//                                                //golden rule
//     int s_len = write(client_fd, buffer, r_len);
// }