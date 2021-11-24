#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>

#define DOMAIN AF_INET
#define TYPE SOCK_STREAM
#define PROTOCOL 0
#define BUF_SIZE 512

using namespace std;

int main(int argc, char** argv){
    char buffer[BUF_SIZE], ch;
    const char* delimiter = ":";
    char *ip_address = strtok((char *)argv[1], delimiter);
    int port = atoi(strtok(NULL, delimiter));
    cout << port << " " << ip_address << endl;
    ifstream file;
    string file_name;
    int client_socket = socket(DOMAIN, TYPE, PROTOCOL);
    int addr_length, i = 0;
    struct sockaddr_in address;
    // memset(&addr, 0, addr_length);
    address.sin_family = DOMAIN;
    address.sin_addr.s_addr = inet_addr(ip_address);
    address.sin_port = htons(port);
    addr_length = sizeof(address);
    connect(client_socket, (struct sockaddr*)&address, addr_length);
    while(true){
        cin >> file_name;
        file.open(file_name);
        while(file.get(ch) && i<BUF_SIZE) {
	    	buffer[i] = ch;
	    	i++;
	    }
	    // cout << buffer << endl;
        send(client_socket, buffer, BUF_SIZE, 0);
        file.close();
    }
    close(client_socket);
    return 0;
}