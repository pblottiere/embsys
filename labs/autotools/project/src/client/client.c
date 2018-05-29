#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = 0;
    char *send_buff;
    struct sockaddr_in serv_addr;

    // read arguments
    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <msg to send>\n",argv[0]);
        return 1;
    }

    send_buff = argv[2];

    // init socket to get a file descriptor
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    // socket configuration to talk on port 5000
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // connect to server
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    int bytes = write(sockfd, send_buff, sizeof(send_buff));

    if(bytes < 0)
    {
        printf("\n Send error \n");
    }

    // close socket
    close(sockfd);

    return 0;
}
