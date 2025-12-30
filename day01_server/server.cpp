#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "log.h"


int main(const int argc, const char* argv[])
{
    // if(argc < 2)
    // {
    //     Logger::instance().error("argc < 2 error");
    //     return -1;
    // }

    int listen_fd = -1, conn_fd = -1;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    ssize_t read_bytes = 0;
    char buffer[1024] = {0};
    char prot[] = "8888";

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    Logger::instance().init("server.log");
    Logger::instance().info("Server starting");

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1) {
        Logger::instance().error("socket() failed");
        return -1;
    }
    Logger::instance().info("Socket created successfully");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(prot));
    if(-1 == bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
    {
        Logger::instance().error("bind() failed");
        return -1;
    }
    Logger::instance().info("Bind to port successfully");

    if(-1 == listen(listen_fd, 10))
    {
        Logger::instance().error("listen() failed");
        return -1;
    }
    Logger::instance().info("Listening for connections");

    conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if(conn_fd == -1)
    {
        Logger::instance().error("accept() failed");
        return -1;
    }
    Logger::instance().info("Connection accepted successfully");
    Logger::instance().info("Server start successfully");
    while(true){
        read_bytes = read(conn_fd, buffer, sizeof(buffer));
        if(read_bytes > 0)
        {
            Logger::instance().info(std::string("Received data: ") + std::string(buffer, read_bytes));
            
            write(conn_fd, buffer, read_bytes);
            memset(buffer, 0, sizeof(buffer));
        }
        else if(read_bytes == 0)
        {
            Logger::instance().info("Client disconnected");
            break;
        }
        else
        {
            Logger::instance().error("read() failed");
            return -1;
        }
    }
    
}