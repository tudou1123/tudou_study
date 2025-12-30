#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>   
#include <arpa/inet.h>
#include <string.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    //bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); 客户端不进行bind操作

    int ret = connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); 
    
    if(ret == -1) {
        perror("connect failed");
        return -1;
    }
    char buffer[1024] = {0};
    while (true)
    {
        fgets(buffer, 1024, stdin);
        buffer[strlen(buffer)] = '\0';
        int tmp = send(sockfd, buffer, strlen(buffer), 0);
        if (tmp <= 0) {
            perror("send failed");
            break;
        }
        int tre = read(sockfd, buffer, sizeof(buffer));
        if (tre > 0) {
            buffer[tre] = '\0';
            printf("Received from server: %s\n", buffer);
        } else if (tre == 0) {
            printf("Server closed the connection\n");
            break;
        } else {
            perror("read failed");
            break;
        }
    }
    
    
    
    return 0;
}