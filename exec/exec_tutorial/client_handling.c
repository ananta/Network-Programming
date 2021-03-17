

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




int main()
{
    printf("This is child program.\n");
    return 0;
}
//TODO:  perform this function here

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