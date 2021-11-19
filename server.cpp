#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define DOMAIN AF_LOCAL
#define TYPE SOCK_STREAM
#define PROTOCOL 0

using namespace std;

int main(int argc, char** argv){
    int server_fd = socket(DOMAIN, TYPE, PROTOCOL);
    int port = *argv[1];
    char buffer[512];
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = DOMAIN;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);
    while(true){
        int child_fd = accept(server_fd, (struct sockaddr*)NULL, (socklen_t*)NULL);
        recv(child_fd, buffer, sizeof(buffer), 0);
        cout << buffer << endl;
        close(child_fd);
    }
    close(server_fd);
}