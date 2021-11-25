#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <filesystem>
     
#define DOMAIN AF_INET
#define TYPE SOCK_STREAM
#define PROTOCOL 0
#define BUF_SIZE 2048
#define MAX_NUM_OF_CLIENTS 10

using namespace std;

struct sockaddr_in destination_setup(const int& flag, char* ip_address, int& port){
    int addr_length;
    struct sockaddr_in address;

    address.sin_family = DOMAIN;
    address.sin_addr.s_addr = flag ? inet_addr(ip_address) : INADDR_ANY;
    address.sin_port = htons(port);
    addr_length = sizeof(address);

    return address;
}

int FD_setup(int& main_socket, int* child_socket, fd_set& readfds){
    int fd, fd_max = main_socket;

    FD_ZERO(&readfds);  
    FD_SET(main_socket, &readfds);  

    for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){
        fd = child_socket[i];
        if(fd > 0){
            FD_SET(fd, &readfds);
        }
        if(fd > fd_max){
            fd_max = fd;
        }  
    }

    return fd_max;
}

void check_new_connection(int& main_socket, int* child_socket, fd_set& readfds, struct sockaddr_in& address, int& addr_length){
    int new_socket;
    if(FD_ISSET(main_socket, &readfds)){  
        new_socket = accept(main_socket, (struct sockaddr*)&address, (socklen_t*)&addr_length);
        for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){  
            if(child_socket[i] == 0){  
                child_socket[i] = new_socket;  
                break;  
            }  
        }
    }
}

void handle_put(){
    
}

void receive_command(int* child_socket, fd_set readfds, char* buffer){
    int fd, length;
    string command;
    for(int i=0; i<MAX_NUM_OF_CLIENTS; i++){  
        fd = child_socket[i];
        if(FD_ISSET(fd, &readfds)){
            memset(buffer, '\0', BUF_SIZE);
            if(recv(fd, buffer, BUF_SIZE, 0) <= 0){  
                close(fd);
                child_socket[i] = 0;
            }
            else{
                command = buffer;
                cout << command.substr(0, command.find(' ')) << endl;
            }
        }  
    }
}

void handle_command(){

}