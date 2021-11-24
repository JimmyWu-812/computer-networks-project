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
#define BUF_SIZE 2048

using namespace std;

int main(int argc, char** argv){
    char buffer[BUF_SIZE], ch;
    const char* delimiter = ":";
    int addr_length, i;
    char *ip_address = strtok((char *)argv[1], delimiter);
    int port = atoi(strtok(NULL, delimiter));
    ifstream file;
    string file_name;
    int client_socket = socket(DOMAIN, TYPE, PROTOCOL);
    struct sockaddr_in address;
    address.sin_family = DOMAIN;
    address.sin_addr.s_addr = inet_addr(ip_address);
    address.sin_port = htons(port);
    addr_length = sizeof(address);
    connect(client_socket, (struct sockaddr*)&address, addr_length);
    while(true){
        i = 0;
        memset(buffer, '\0', BUF_SIZE);
        cin >> file_name;
        file.open(file_name);
        while(file.get(ch) && i < BUF_SIZE){
	    	buffer[i] = ch;
	    	i++;
	    }
        cout << i << endl;
        cout << buffer << endl;
        send(client_socket, buffer, i, 0);
        file.close();
    }
    close(client_socket);
    return 0;
}