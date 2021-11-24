#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

#define DOMAIN AF_INET
#define TYPE SOCK_STREAM
#define PROTOCOL 0
#define BUF_SIZE 512

using namespace std;

int main(int argc, char** argv){
    char buffer[BUF_SIZE];
    int client_fd = socket(DOMAIN, TYPE, PROTOCOL);
    int port = atoi(argv[2]);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = DOMAIN;
    inet_pton(DOMAIN, argv[1], &addr.sin_addr);
    addr.sin_port = htons(port);
    connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));
    while(cin.getline(buffer,sizeof(buffer))){
        send(client_fd, buffer, sizeof(buffer), 0);
    }
    close(client_fd);
}