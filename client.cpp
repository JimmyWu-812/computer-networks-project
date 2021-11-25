#include "util.cpp"

int main(int argc, char** argv){
    char buffer[BUF_SIZE], ch;
    char *ip_address;
    const char* delimiter = ":";
    int addr_length, i, port, client_socket;
    ifstream file;
    string operation;
    
    client_socket = socket(DOMAIN, TYPE, PROTOCOL);

    ip_address = strtok(argv[1], delimiter);
    port = atoi(strtok(NULL, delimiter));

    struct sockaddr_in address = destination_setup(1, ip_address, port);
    addr_length = sizeof(address);

    connect(client_socket, (struct sockaddr*)&address, addr_length);

    while(true){
        getline(cin, operation);
        send(client_socket, operation.c_str(), operation.size(), 0);
    }

    close(client_socket);

    return 0;
}